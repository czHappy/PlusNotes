echo "GOTO DRIVE docker at $(date)" > LOGX
source /opt/plusai/setup.bash
source /opt/ros/melodic/setup.bash
bash /opt/plusai/tools/event_recorder/start_ros_master.sh
export ROS_MASTER_URI=http://127.0.0.1:11311
cd /opt/plusai/bin
./dispatcher_server
