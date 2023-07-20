echo "GOTO ADU at $(date)" > LOGX
# vehicle=$(cat < /data/VIN)
# echo $vehicle
# source /opt/plusai/launch/pdb-l4e-$vehicle/setup.sh
# hamlaunch list
echo "Env setup successful!"
echo "To start full stack..."
# echo "1" | hamlaunch start
echo "Setup ROS..."
source /opt/plusai/setup.bash
source /opt/ros/kinetic/setup.bash
kill -9 $(pgrep -f "/usr/bin/python /opt/ros/melodic/bin/rosmaster --core")
bash /opt/plusai/tools/event_recorder/start_ros_master.sh
ps -aux | grep ros
echo "Start dispatcher_client..."
export ROS_MASTER_URI=http://127.0.0.1:11311
export PLUSAI_DISPATCHER_SERVER_IP_ADDRESS=192.168.2.14
echo $PLUSAI_DISPATCHER_SERVER_IP_ADDRESS
cd /opt/plusai/bin
./dispatcher_client
