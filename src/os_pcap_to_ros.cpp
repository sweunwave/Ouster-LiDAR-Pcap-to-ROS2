#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "sensor_msgs/msg/point_cloud.hpp"

#include "ouster/pcap.h"
#include "ouster/os_pcap.h"
#include "ouster/client.h"
#include "ouster/lidar_scan.h"
#include "ouster/types.h"
#include "ouster/impl/build.h"
#include "ouster/types.h"

constexpr std::size_t BUF_SIZE = 65536;

using namespace std;
using namespace ouster;

std::vector<ouster::LidarScan> get_pcap_scans(
    std::shared_ptr<ouster::sensor_utils::playback_handle> handle,
    ouster::LidarScan& scan, sensor::sensor_info& info) {
    
    // Make sure we start at beginning
    ouster::sensor_utils::replay_reset(*handle);

    std::vector<ouster::LidarScan> scans;

    auto pf = get_format(info);
    ouster::ScanBatcher batch_to_scan(info.format.columns_per_frame, pf);

    // Buffer to store raw packet data
    auto packet_buf = std::make_unique<uint8_t[]>(BUF_SIZE);

    ouster::sensor_utils::packet_info packet_info;

    int idx = 0;
    while (ouster::sensor_utils::next_packet_info(*handle, packet_info)) {
        auto packet_size = ouster::sensor_utils::read_packet(
            *handle, packet_buf.get(), BUF_SIZE);

        if (packet_size == pf.lidar_packet_size &&
            packet_info.dst_port == info.udp_port_lidar) {

            if (batch_to_scan(packet_buf.get(), scan)) {
                idx += 1;
                scans.push_back(scan);
            }
        }
    }
    return scans;
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = rclcpp::Node::make_shared("pcap_to_pointcloud");

    node->declare_parameter<string>("pcap_path");
    node->declare_parameter<string>("metadata_path");

    cout << "pcap publisher start!" << endl;

    string pcap_path;
    string metadata_path;
    
    node->get_parameter("pcap_path", pcap_path);
    node->get_parameter("metadata_path", metadata_path);

    auto info = sensor::metadata_from_json(metadata_path);
    XYZLut xyzlut = ouster::make_xyz_lut(info);

    size_t w = info.format.columns_per_frame;
    size_t h = info.format.pixels_per_column;

    ouster::sensor::ColumnWindow column_window = info.format.column_window;

    std::cerr << "  Firmware version:  " << info.fw_rev
              << "\n  Serial number:     " << info.sn
              << "\n  Product line:      " << info.prod_line
              << "\n  Scan dimensions:   " << w << " x " << h
              << "\n  Column window:     [" << column_window.first << ", "
              << column_window.second << "]" << std::endl;

    auto handle = sensor_utils::replay_initialize(pcap_path);

    auto profile_scan = ouster::LidarScan(w, h, info.format.udp_profile_lidar);

    std::cerr << "Creating scans from pcap ... " << std::endl;
    auto scans = get_pcap_scans(handle, profile_scan, info);
    std::cerr << "Scans created!" << std::endl;

    ouster::sensor_utils::replay_uninitialize(*handle);

    auto print_el = [](ouster::LidarScan& scan, std::string label) {
        std::cerr << "Available fields in " << label << "...\n";
        for (auto it = scan.begin(); it != scan.end(); it++) {
            auto field = it->first;
            std::cerr << "\t" << to_string(field) << "\n ";
        }
        std::cerr << std::endl;
    };

    // Eigen::Array<uint32_t, -1, -1, Eigen::RowMajor> reflectivity;
    auto lidar_pub = node->create_publisher<sensor_msgs::msg::PointCloud>("os_sensor/point_cloud", 10);

    rclcpp::WallRate loop_rate(5);

    sensor_msgs::msg::PointCloud::SharedPtr msg = std::make_shared<sensor_msgs::msg::PointCloud>();;
    msg->header.frame_id = "os_sensor";
    
    for (auto& scan : scans) {
        msg->channels.clear();
        msg->points.clear();

        msg->header.stamp = rclcpp::Clock().now();

        auto frame_id = scan.frame_id;
        auto ts = scan.timestamp();
        auto status = scan.status();
        auto measurement_id = scan.measurement_id();

        auto range = scan.field(sensor::ChanField::RANGE);
        auto xyz = cartesian(scan, xyzlut);
        auto signal = scan.field<uint16_t>(sensor::ChanField::SIGNAL).cast<uint32_t>();
        // reflectivity = scan.field<uint16_t>(sensor::ChanField::REFLECTIVITY).cast<uint32_t>();

        geometry_msgs::msg::Point32::SharedPtr point32 = std::make_shared<geometry_msgs::msg::Point32>();
        sensor_msgs::msg::ChannelFloat32::SharedPtr channel = std::make_shared<sensor_msgs::msg::ChannelFloat32>();
        channel->name = "intensity";

        for (int i = 0; i < xyz.rows(); ++i) {
            for (int j = 0; j < xyz.cols(); ++j) {
                if (j == 0) point32->x = xyz(i, j);
                if (j == 1) point32->y = xyz(i, j);
                if (j == 2) point32->z = xyz(i, j);
            }
            channel->values.push_back(signal(0, i));
            msg->points.push_back(*point32);
        }
        msg->channels.push_back(*channel);
        lidar_pub->publish(*msg);
        loop_rate.sleep();
    }

    return 0;
}