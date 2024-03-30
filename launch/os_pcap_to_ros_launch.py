import launch
from launch_ros.actions import Node

import ament_index_python.packages as pk
import ament_index_python.resources as pr
import os

package_name = 'lidar_dev'

def generate_launch_description():
    package_dir = pk.get_package_share_directory(package_name)
    return launch.LaunchDescription([
        Node(
            package=package_name,
            executable='os_pcap_to_ros',
            name='pcap_to_publish',
            output="screen",
            parameters=[
                {"pcap_path" : "/home/sweun/pcaps/OS-1-32-U0_v3.0.1_1024x10_20230510_100110-000.pcap"},
                {"metadata_path" : "/home/sweun/pcaps/OS-1-32-U0_v3.0.1_1024x10_20230510_100110.json"}
            ]),

        Node(
            package='rviz2',
            namespace='',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', os.path.join(package_dir, 'rviz', 'pcap_to_ros.rviz')]),
  ])