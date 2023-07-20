#!/ bin/sh
echo "start..."
user_name_x86="chengzhen"
password_x86="chengzhen"
address_x86="192.168.2.14"
ros_master_uri=http://192.168.2.118:11311
echo "copy script that docker container will execute to x86 host"
run_record="run_record_on_host_x86.sh"
cat > $run_record << EOF
echo "RUN record \$(date)" > LOGX
source /opt/plusai/setup.bash
source /opt/ros/melodic/setup.bash
bash /opt/plusai/tools/event_recorder/start_ros_master.sh
export ROS_MASTER_URI=$ros_master_uri
cd /opt/plusai/bin
fastbag_record --path store.record
EOF
sshpass -p $password_x86 scp $run_record "$user_name_x86@$address_x86:/home/chengzhen/tmp"
echo "scp file success!"

echo "start drive container and run dispatcher_server"
run_record_on_host_x86="run_record_on_host_x86.sh"
cat > $run_record_on_host_x86 << EOF
echo "GOTO bj4 at $(date)" > LOGX
docker cp /home/chengzhen/tmp/$run_record_on_host_x86 chengzhen2:/home/yonghui
docker exec -itd chengzhen2 /bin/bash -xe ${run_record}
EOF

cat ${run_record_on_host_x86}
sshpass -p \"${password_x86}\" ssh -o StrictHostKeyChecking=no ${user_name_x86}@${address_x86} bash -s < ${run_record_on_host_x86}