user_name_adu="plusai"
password_adu="plusai"
address_adu="192.168.2.118"

echo "start dispather_client in ADU"
dispatcher_server_address="192.168.2.14"
script_run_on_adu="run_on_adu.sh"
ros_master_uri=http://127.0.0.1:11311
cat > $script_run_on_adu << EOF
echo "GOTO ADU at \$(date)" > LOGX
# vehicle=\$(cat < /data/VIN)
# echo \$vehicle
# source /opt/plusai/launch/pdb-l4e-\$vehicle/setup.sh
# hamlaunch list
echo "Env setup successful!"
echo "To start full stack..."
# echo "1" | hamlaunch start
echo "Setup ROS..."
source /opt/plusai/setup.bash
source /opt/ros/kinetic/setup.bash
kill -9 \$(pgrep -f "/usr/bin/python /opt/ros/melodic/bin/rosmaster --core")
bash /opt/plusai/tools/event_recorder/start_ros_master.sh
ps -aux | grep ros
echo "Start dispatcher_client..."
export ROS_MASTER_URI=$ros_master_uri
export PLUSAI_DISPATCHER_SERVER_IP_ADDRESS=$dispatcher_server_address
echo \$PLUSAI_DISPATCHER_SERVER_IP_ADDRESS
cd /opt/plusai/bin
./dispatcher_client
EOF

echo "scp script"
# sshpass -p $password_adu scp $script_run_on_adu "$user_name_adu@$address_adu:/home/plusai"

echo "ssh & execute"
sshpass -p "${password_adu}" ssh ${user_name_adu}@${address_adu} 'bash -x' < ${script_run_on_adu}