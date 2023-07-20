#!/ bin/sh
echo "start..."
user_name_x86="chengzhen"
password_x86="chengzhen"
address_x86="192.168.2.14"
ros_master_uri=http://127.0.0.1:11311
echo "copy script that docker container will execute to x86 host"
script_run_on_docker_dispatcher_server="run_on_docker_dispatcher_server.sh"
cat > $script_run_on_docker_dispatcher_server << EOF
echo "GOTO DRIVE docker at \$(date)" > LOGX
source /opt/plusai/setup.bash
source /opt/ros/melodic/setup.bash
bash /opt/plusai/tools/event_recorder/start_ros_master.sh
export ROS_MASTER_URI=$ros_master_uri
cd /opt/plusai/bin
./dispatcher_server
EOF
sshpass -p $password_x86 scp $script_run_on_docker_dispatcher_server "$user_name_x86@$address_x86:/home/chengzhen/tmp"
echo "scp file success!"

echo "start drive container and run dispatcher_server"
script_run_on_host_x86="run_on_host_x86.sh"
cat > $script_run_on_host_x86 << EOF
echo "GOTO bj4 at $(date)" > LOGX
docker cp /home/chengzhen/tmp/$script_run_on_docker_dispatcher_server yonghui_drive:/home/yonghui
docker exec -itd yonghui_drive /bin/bash -xe ${script_run_on_docker_dispatcher_server}

# 这里新开container 需要对网络做些配置
# docker stop run_dispatcher_server
# docker rm run_dispatcher_server
# ls -la run_on_docker_dispatcher_server.sh
# docker run --name run_dispatcher_server -w /home/chengzhen/tmp -v /home/chengzhen/tmp:/home/chengzhen/tmp -itd docker.plusai.co:5050/plusai/drive:latest /bin/bash -xe ${script_run_on_docker_dispatcher_server}
# echo "Complete!"
EOF
cat ${script_run_on_host_x86}
sshpass -p \"${password_x86}\" ssh -o StrictHostKeyChecking=no ${user_name_x86}@${address_x86} bash -s < ${script_run_on_host_x86}


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