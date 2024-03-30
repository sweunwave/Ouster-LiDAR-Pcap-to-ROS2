# Ouster-LiDAR-Pcap-to-ROS2
Convert ouster pcap data to ros2 topic

**Author : sweunwave**

<img src="readme/pcap_to_ros.gif">

## Environment
Ubuntu 22.04  
ROS Humble  

## Start Guide
### 1. Install repository<br/>
```
git clone --recursive https://github.com/sweunwave/Ouster-LiDAR-Pcap-to-ROS2.git
```

### 2. Follow ouster-sdk install [document](https://static.ouster.dev/sdk-docs/cpp/building.html)<br/>
```
cd Ouster-LiDAR-Pcap-to-ROS2/ouster_example/
```
#### Building on Linux / macOS
#### To install build dependencies on Ubuntu:20.04+, run:
```
sudo apt install build-essential cmake libjsoncpp-dev libeigen3-dev libcurl4-openssl-dev \
               libtins-dev libpcap-dev libglfw3-dev libglew-dev libspdlog-dev
```
#### To build run the following commands:
```
mkdir build
cd build
cmake  <path to ouster_example> -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=ON
cmake --build .
```

### 3. Colcon build
```
cd ~/your_ws
colcon build --symlink-install --cmake-args -DCMAKE_BUILD_TYPE=Release --packages-select lidar_dev
```
### 4. Modify the pcap/metadata path of the launch.py file
[Ouster Sample Lidar Data](https://ouster.com/downloads/sample-lidar-data)
```python
package=package_name,
executable='os_pcap_to_ros',
name='pcap_to_publish',
output="screen",
parameters=[
    {"pcap_path" : "/home/sweun/pcaps/OS-1-32-U0_v3.0.1_1024x10_20230510_100110-000.pcap"},
    {"metadata_path" : "/home/sweun/pcaps/OS-1-32-U0_v3.0.1_1024x10_20230510_100110.json"}
]),
```

### 5. Run
```
ros2 launch lidar_dev os_pcap_to_ros_launch.py
```

## To-Do-List
- [ ] Source Optimization
