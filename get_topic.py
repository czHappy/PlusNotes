# 打开要处理的文件，这里假设文件名为input.txt
with open('/home/plusai/Documents/PlusNotes/PlusNotes/topics_select.md', 'r') as file:
    # 逐行读取文件内容
    lines = [line.strip() for line in file.readlines()]

# 使用逗号连接所有行
result = ','.join(lines)

# 打印拼接后的结果
print(result)

# 如果你想将结果写入另一个文件，可以使用以下代码
with open('output_topics.txt', 'w') as output_file:
    output_file.write(result)


fastbag_play --paths 20230911T003149_pdb-l4e-b0007_15_100to220.db -loop -skip_topics /gmsl_cameras/status_report,/front_center_camera/image_color/compressed,/front_right_camera/image_color/compressed,/front_right_camera/image_color/quarter/compressed,/front_left_camera/image_color/compressed,/front_left_camera/image_color/quarter/compressed,/rear_right_camera/image_color/compressed,/side_left_camera/image_color/compressed,/side_left_camera/image_color/quarter/compressed,/rear_left_camera/image_color/compressed,/side_right_camera/image_color/compressed,/side_right_camera/image_color/quarter/compressed,/gmsl_cam/runtime,/perception/obstacles,/vehicle/trailer_angle,/vehicle/truck_state,/perception/traffic_lights,/perception/lane_path,/perception/lane_benchmark_info,/perception/lane_intermediates,/perception/fusion_tracker_report,/perception/obstacle_intermediates,/perception/calibrations,/lane/status_report,/perception/ot_calibrations,/obstacle/status_report,/lane_detector/runtime,/fusion_object_tracker/runtime,/localization/status_report,/localization/visual_geometry,/localization/remained_gnss,/localization/odometry,/localization/state,/navsat/odom_null,/localization/fusion_debug_info,/localization/runtime,/control/status_report,/vehicle/control_cmd,/vehicle/status,/vehicle/location,/control_v2/runtime,/prediction/obstacles,/prediction/runtime,/planning/trajectory,/planning/fuel_report_input,/planning/debug_info,/vehicle/turn_signal_cmd,/planning/lead_info,/planning/jerk_info,/planning/status_report,/plus_scenario_planner/runtime,/imu/data,/localization/gnss,/vehicle/dbw_reports,fastbag_player/log,/plus/odom,/watchdog/current_state,/ublox/status_report,/novatel_data/inspvax,/rslidar_points,/bumper_radar/radar_tracks,/rear_left_radar/radar_tracks,/rear_right_radar/radar_tracks,/side_left_radar/radar_tracks,/side_right_radar/radar_tracks,/front_center_camera/image_color/fake,/front_right_camera/image_color/fake,/front_left_camera/image_color/fake,/rear_right_camera/image_color/fake,/side_left_camera/image_color/fake,/rear_left_camera/image_color/fake,/side_right_camera/image_color/fake,/novatel_data/inspva,/vehicle/misc_1_report,/prediction/status_report,/app_watchdog/latency_report,/app_watchdog/runtime,/auto_calibration/status_report,/bbox/audio,/bosch_radar_can_node/runtime,/bumper_radar/radar_can_node/latency_report,/bumper_radar/status_report,/controller_ros_node/latency_report,/dispatcher_client/latency_report,/dispatcher_client/runtime,/dispatcher_server/runtime,/dispatcher_server/status_report,/event_recorder/latency_report,/event_recorder/status_report,/fastbag_player/runtime,/fastbag_player_1697534221904182579/shm_bus_dispatcher_report,/fusion_object_tracker/latency_report,/gmsl_cameras/latency_report,/hdi_map_lane_geometry_data,/lane_detector/latency_report,/localization/latency_report,/navinfo_map_lane_geometry_data,/navsat/odom,/plusai_record/runtime,/plusai_record_1694363436986282757/latency_report,/prediction/latency_report,/radar_can_node/runtime,/rear_radar/radar_can_node/latency_report,/rear_radars/status_report,/recorder/runtime,/robo_sense_lidar_node/runtime,/rs_lidar/latency_report,/rs_lidar/status_report,/scenario_planner/latency_report,/sense_dms_camera/image_color/compressed,/sense_dms_camera/image_color/fake,/sense_dms_camera/latency_report,/sense_dms_camera/status_report,/side_left_radar/radar_parsed,/side_radar/bosch_radar_can_node/latency_report,/side_radars/status_report,/side_right_radar/radar_parsed,/system_metrics,/system_metrics/latency_report,/system_metrics/runtime,/ublox/esfalg_debug,/ublox/esfins,/ublox/esfstatus_debug,/ublox/latency_report,/ublox/navatt,/ublox/navpvt,/ublox/navsat,/uds_server_node/latency_report,/vehicle/dbw_enabled,/vehicle/info,/vehicle/status_report,/vehicle_can_node/latency_report



fastbag_play -clock -envelope_timestamp --paths 20230911T003149_pdb-l4e-b0007_15_100to220.db --hz=600 skip_topics /gmsl_cameras/status_report,/gmsl_cam/runtime,/perception/obstacles,/vehicle/trailer_angle,/perception/traffic_lights,/perception/lane_benchmark_info,/perception/fusion_tracker_report,/perception/obstacle_intermediates,/perception/calibrations,/lane/status_report,/perception/ot_calibrations,/obstacle/status_report,/lane_detector/runtime,/fusion_object_tracker/runtime,/localization/visual_geometry,/localization/remained_gnss,/localization/odometry,/navsat/odom_null,/localization/fusion_debug_info,/localization/runtime,/control/status_report,/vehicle/control_cmd,/vehicle/status,/vehicle/location,/control_v2/runtime,/prediction/obstacles,/prediction/runtime,/planning/trajectory,/planning/fuel_report_input,/planning/debug_info,/vehicle/turn_signal_cmd,/planning/lead_info,/planning/jerk_info,/planning/status_report,/plus_scenario_planner/runtime,/imu/data,fastbag_player/log,/ublox/status_report,/prediction/status_report,/app_watchdog/latency_report,/app_watchdog/runtime,/auto_calibration/status_report,/bbox/audio,/bosch_radar_can_node/runtime,/bumper_radar/radar_can_node/latency_report,/bumper_radar/status_report,/controller_ros_node/latency_report,/dispatcher_client/latency_report,/dispatcher_client/runtime,/dispatcher_server/runtime,/dispatcher_server/status_report,/event_recorder/latency_report,/event_recorder/status_report,/fastbag_player/runtime,/fastbag_player_1697534221904182579/shm_bus_dispatcher_report,/fusion_object_tracker/latency_report,/gmsl_cameras/latency_report,/hdi_map_lane_geometry_data,/lane_detector/latency_report,/localization/latency_report,/navinfo_map_lane_geometry_data,/navsat/odom,/plusai_record/runtime,/plusai_record_1694363436986282757/latency_report,/prediction/latency_report,/radar_can_node/runtime,/rear_radar/radar_can_node/latency_report,/rear_radars/status_report,/recorder/runtime,/robo_sense_lidar_node/runtime,/rs_lidar/latency_report,/rs_lidar/status_report,/scenario_planner/latency_report,/sense_dms_camera/image_color/compressed,/sense_dms_camera/latency_report,/sense_dms_camera/status_report,/side_left_radar/radar_parsed,/side_radar/bosch_radar_can_node/latency_report,/side_radars/status_report,/side_right_radar/radar_parsed,/system_metrics,/system_metrics/latency_report,/system_metrics/runtime,/ublox/esfalg_debug,/ublox/esfins,/ublox/esfstatus_debug,/ublox/latency_report,/ublox/navatt,/ublox/navpvt,/ublox/navsat,/uds_server_node/latency_report,/vehicle/dbw_enabled,/vehicle/info,/vehicle/status_report,/vehicle_can_node/latency_report -rate=1 -terminate_on_buffer_exhausted=false --shm_bus_enable_simulated_clock_management=false



./dispatcher_client --dispatcher_decompress --dispatcher_client_stable_fq_publish --dispatcher_rewrite_decompressed_topic
./dispatcher_server --dispatcher_decompress --dispatcher_rewrite_decompressed_topic
bash /opt/plusai/tools/event_recorder/start_ros_master.sh > /dev/null 2>&1 &




#只需要
/rear_left_camera/image_color
/rear_right_camera/image_color
/front_left_camera/image_color
/front_right_camera/image_color
/front_center_camera/image_color
/side_left_camera/image_color
/side_right_camera/image_color


/rear_left_radar/radar_tracks
/rslidar_points
/bumper_radar/radar_tracks
/rear_right_radar/radar_tracks
/side_left_radar/radar_tracks
/side_right_radar/radar_tracks

/plus/odom
/imu/data
/novatel_data/inspvax
/novatel_data/inspva

/vehicle/dbw_reports
/vehicle/truck_state
/vehicle/misc_1_report
/ublox/status_report
/watchdog/current_state

fastbag_play -clock -envelope_timestamp --paths 20230911T003149_pdb-l4e-b0007_15_100to220.db --hz=600 skip_topics /gmsl_cameras/status_report,/gmsl_cam/runtime,/perception/obstacles,/vehicle/trailer_angle,/perception/traffic_lights,/perception/lane_path,/perception/lane_benchmark_info,/perception/lane_intermediates,/perception/fusion_tracker_report,/perception/obstacle_intermediates,/perception/calibrations,/lane/status_report,/perception/ot_calibrations,/obstacle/status_report,/lane_detector/runtime,/fusion_object_tracker/runtime,/localization/status_report,/localization/visual_geometry,/localization/remained_gnss,/localization/odometry,/localization/state,/navsat/odom_null,/localization/fusion_debug_info,/localization/runtime,/control/status_report,/vehicle/control_cmd,/vehicle/status,/vehicle/location,/control_v2/runtime,/prediction/obstacles,/prediction/runtime,/planning/trajectory,/planning/fuel_report_input,/planning/debug_info,/vehicle/turn_signal_cmd,/planning/lead_info,/planning/jerk_info,/planning/status_report,/plus_scenario_planner/runtime,/localization/gnss,fastbag_player/log,/watchdog/current_state,/ublox/status_report,/prediction/status_report,/app_watchdog/latency_report,/app_watchdog/runtime,/auto_calibration/status_report,/bbox/audio,/bosch_radar_can_node/runtime,/bumper_radar/radar_can_node/latency_report,/bumper_radar/status_report,/controller_ros_node/latency_report,/dispatcher_client/latency_report,/dispatcher_client/runtime,/dispatcher_server/runtime,/dispatcher_server/status_report,/event_recorder/latency_report,/event_recorder/status_report,/fastbag_player/runtime,/fastbag_player_1697534221904182579/shm_bus_dispatcher_report,/fusion_object_tracker/latency_report,/gmsl_cameras/latency_report,/hdi_map_lane_geometry_data,/lane_detector/latency_report,/localization/latency_report,/navinfo_map_lane_geometry_data,/navsat/odom,/plusai_record/runtime,/plusai_record_1694363436986282757/latency_report,/prediction/latency_report,/radar_can_node/runtime,/rear_radar/radar_can_node/latency_report,/rear_radars/status_report,/recorder/runtime,/robo_sense_lidar_node/runtime,/rs_lidar/latency_report,/rs_lidar/status_report,/scenario_planner/latency_report,/sense_dms_camera/latency_report,/sense_dms_camera/status_report,/side_left_radar/radar_parsed,/side_radar/bosch_radar_can_node/latency_report,/side_radars/status_report,/side_right_radar/radar_parsed,/system_metrics,/system_metrics/latency_report,/system_metrics/runtime,/ublox/esfalg_debug,/ublox/esfins,/ublox/esfstatus_debug,/ublox/latency_report,/ublox/navatt,/ublox/navpvt,/ublox/navsat,/uds_server_node/latency_report,/vehicle/dbw_enabled,/vehicle/info,/vehicle/status_report,/vehicle_can_node/latency_report -rate=1 -terminate_on_buffer_exhausted=false --shm_bus_enable_simulated_clock_management=false



plusai@tegra-ubuntu:~$ rosnode info /fusion_object_tracker
--------------------------------------------------------------------------------
Node [/fusion_object_tracker]
Publications: 
 * /fusion_object_tracker/latency_report [std_msgs/String]
 * /fusion_object_tracker/runtime [std_msgs/String]
 * /obstacle/status_report [std_msgs/String]
 * /perception/fusion_tracker_report [std_msgs/String]
 * /perception/lane_benchmark_info [plusai_msgs/LaneResult]
 * /perception/lane_intermediates [std_msgs/String]
 * /perception/lane_path [std_msgs/String]
 * /perception/obstacle_intermediates [std_msgs/String]
 * /perception/obstacles [std_msgs/String]
 * /perception/ot_calibrations [std_msgs/String]
 * /perception/traffic_lights [std_msgs/String]
 * /vehicle/trailer_angle [std_msgs/Float32MultiArray]
 * /vehicle/truck_state [std_msgs/String]
Sub:
/perception/lane_path
/rear_left_camera/image_color
/rear_right_camera/image_color
/front_left_camera/image_color
/front_right_camera/image_color
/localization/status_report
/localization/state
/rslidar_points
/bumper_radar/radar_tracks
/rear_left_radar/radar_tracks
/rear_right_radar/radar_tracks
/side_left_radar/radar_tracks
/side_right_radar/radar_tracks
/vehicle/dbw_reports


plusai@tegra-ubuntu:~$ rosnode info /lane_detector 
--------------------------------------------------------------------------------
Node [/lane_detector]
Publications: 
 * /lane/status_report [std_msgs/String]
 * /lane_detector/latency_report [std_msgs/String]
 * /lane_detector/runtime [std_msgs/String]
 * /perception/calibrations [std_msgs/String]
 * /perception/fusion_tracker_report [std_msgs/String]
 * /perception/lane_benchmark_info [plusai_msgs/LaneResult]
 * /perception/lane_intermediates [std_msgs/String]
 * /perception/lane_path [std_msgs/String]
 * /perception/obstacle_intermediates [std_msgs/String]
 * /perception/obstacles [std_msgs/String]
 * /perception/traffic_lights [std_msgs/String]
 * /vehicle/trailer_angle [std_msgs/Float32MultiArray]
 * /vehicle/truck_state [std_msgs/String]

Sub
/front_left_camera/image_color
/front_right_camera/image_color
/side_left_camera/image_color
/side_right_camera/image_color
/front_center_camera/image_color
/vehicle/truck_state
/localization/state
/vehicle/misc_1_report
/vehicle/dbw_reports


plusai@tegra-ubuntu:~$ rosnode info /localization
--------------------------------------------------------------------------------
Node [/localization]
Publications: 
 * /localization/fusion_debug_info [std_msgs/String]
 * /localization/gnss [std_msgs/String]
 * /localization/latency_report [std_msgs/String]
 * /localization/odometry [std_msgs/String]
 * /localization/runtime [std_msgs/String]
 * /localization/state [std_msgs/String]
 * /localization/status_report [std_msgs/String]
 * /localization/visual_geometry [std_msgs/String]
 * /navsat/odom_null [nav_msgs/Odometry]

Sub
/perception/lane_intermediates
/perception/lane_path
/imu/data
/novatel_data/inspvax
/novatel_data/inspva
/vehicle/dbw_reports
/plus/odom
/watchdog/current_state

plusai@tegra-ubuntu:~$ rosnode info /prediction
--------------------------------------------------------------------------------
Node [/prediction]
Publications: 
 * /prediction/latency_report [std_msgs/String]
 * /prediction/obstacles [std_msgs/String]
 * /prediction/runtime [std_msgs/String]
Sub:
/perception/obstacles
/localization/state
/localization/status_report
/vehicle/status
/perception/lane_path
/vehicle/dbw_reports
/watchdog/current_state


plusai@tegra-ubuntu:~$ rosnode info /scenario_planner
--------------------------------------------------------------------------------
Node [/scenario_planner]
Publications: 
 * /planning/debug_info [geometry_msgs/Pose2D]
 * /planning/fuel_report_input [std_msgs/String]
 * /planning/jerk_info [std_msgs/String]
 * /planning/lead_info [std_msgs/String]
 * /planning/status_report [std_msgs/String]
 * /planning/trajectory [std_msgs/String]
 * /plus_scenario_planner/runtime [std_msgs/String]
 * /scenario_planner/latency_report [std_msgs/String]
 * /vehicle/turn_signal_cmd [dbw_mkz_msgs/TurnSignalCmd]

Sub:
/localization/state
/localization/status_report
/perception/traffic_lights
/perception/lane_path
/vehicle/trailer_angle
/vehicle/truck_state
/vehicle/status
/prediction/obstacles
/vehicle/dbw_reports
/plus/odom

plusai@tegra-ubuntu:~$ rosnode info /controller_ros_node
--------------------------------------------------------------------------------
Node [/controller_ros_node]
Publications: 
 * /control/status_report [std_msgs/String]
 * /control_v2/runtime [std_msgs/String]
 * /controller_ros_node/latency_report [std_msgs/String]
 * /vehicle/control_cmd [std_msgs/String]
 * /vehicle/location [std_msgs/String]
 * /vehicle/status [std_msgs/String]
Sub
/watchdog/current_state
/localization/state
/planning/trajectory
/planning/lead_info
/vehicle/dbw_reports
/novatel_data/inspvax
/ublox/status_report



ps aux | grep "launch" | awk '{print $2}' | xargs kill -2












fastbag_play -clock -envelope_timestamp --paths 20230911T003149_pdb-l4e-b0007_15_100to220.db --hz=600 -skip_topics /gmsl_cameras/status_report,/gmsl_cam/runtime,/perception/obstacles,/vehicle/trailer_angle,/perception/traffic_lights,/perception/lane_path,/perception/lane_benchmark_info,/perception/lane_intermediates,/perception/fusion_tracker_report,/perception/obstacle_intermediates,/perception/calibrations,/lane/status_report,/perception/ot_calibrations,/obstacle/status_report,/lane_detector/runtime,/fusion_object_tracker/runtime,/localization/status_report,/localization/visual_geometry,/localization/remained_gnss,/localization/odometry,/localization/state,/navsat/odom_null,/localization/fusion_debug_info,/localization/runtime,/control/status_report,/vehicle/control_cmd,/vehicle/status,/vehicle/location,/control_v2/runtime,/prediction/obstacles,/prediction/runtime,/planning/trajectory,/planning/fuel_report_input,/planning/debug_info,/vehicle/turn_signal_cmd,/planning/lead_info,/planning/jerk_info,/planning/status_report,/plus_scenario_planner/runtime,/localization/gnss,fastbag_player/log,/prediction/status_report,/app_watchdog/latency_report,/app_watchdog/runtime,/auto_calibration/status_report,/bbox/audio,/bosch_radar_can_node/runtime,/bumper_radar/radar_can_node/latency_report,/bumper_radar/status_report,/controller_ros_node/latency_report,/dispatcher_client/latency_report,/dispatcher_client/runtime,/dispatcher_server/runtime,/dispatcher_server/status_report,/event_recorder/latency_report,/event_recorder/status_report,/fastbag_player/runtime,/fastbag_player_1697534221904182579/shm_bus_dispatcher_report,/fusion_object_tracker/latency_report,/gmsl_cameras/latency_report,/hdi_map_lane_geometry_data,/lane_detector/latency_report,/localization/latency_report,/navinfo_map_lane_geometry_data,/navsat/odom,/plusai_record/runtime,/plusai_record_1694363436986282757/latency_report,/prediction/latency_report,/radar_can_node/runtime,/rear_radar/radar_can_node/latency_report,/rear_radars/status_report,/recorder/runtime,/robo_sense_lidar_node/runtime,/rs_lidar/latency_report,/rs_lidar/status_report,/scenario_planner/latency_report,/sense_dms_camera/latency_report,/sense_dms_camera/status_report,/side_left_radar/radar_parsed,/side_radar/bosch_radar_can_node/latency_report,/side_radars/status_report,/side_right_radar/radar_parsed,/system_metrics,/system_metrics/latency_report,/system_metrics/runtime,/ublox/esfalg_debug,/ublox/esfins,/ublox/esfstatus_debug,/ublox/latency_report,/ublox/navatt,/ublox/navpvt,/ublox/navsat,/uds_server_node/latency_report,/vehicle/dbw_enabled,/vehicle/info,/vehicle/status_report,/vehicle_can_node/latency_report -rate=1 -terminate_on_buffer_exhausted=false --shm_bus_enable_simulated_clock_management=false




python aggregate_latency_reports_from_bags.py --topics /fusion_object_tracker/latency_report --abnormal-threshold=-1 --bags 20231024T143225_pdb-l4e-b0005_8_100to300.db > origin-obstacle.txt