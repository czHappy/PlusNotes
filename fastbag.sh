user_name_x86="chengzhen"
password_x86="chengzhen"
address_x86="192.168.2.14"
ros_master_uri=http://127.0.0.1:11311
echo "copy script that docker container will execute to x86 host"
run_fastbag="run_fastbag.sh"
bag_path="/home/yonghui/bags_for_test/20230703T145816_j7-00010_0_103to283.db"
cat > $run_fastbag << EOF
echo "GOTO DRIVE docker to run_fastbag at \$(date)" > LOGX
source /opt/plusai/setup.bash
source /opt/ros/melodic/setup.bash
export ROS_MASTER_URI=$ros_master_uri
fastbag_play --paths $bag_path
EOF
sshpass -p $password_x86 scp $run_fastbag "$user_name_x86@$address_x86:/home/chengzhen/tmp"
echo "scp file success!"

echo "start drive container and run dispatcher_server"
run_fastbag_on_host_x86="run_fastbag_on_host_x86.sh"
cat > $run_fastbag_on_host_x86 << EOF
echo "GOTO bj4 at $(date)" > LOGX
docker cp /home/chengzhen/tmp/$run_fastbag yonghui_drive:/home/yonghui
docker exec yonghui_drive /bin/bash -xe ${run_fastbag}
EOF
cat ${run_fastbag_on_host_x86}
sshpass -p \"${password_x86}\" ssh -o StrictHostKeyChecking=no ${user_name_x86}@${address_x86} 'bash -x' < ${run_fastbag_on_host_x86}



