echo "GOTO DRIVE docker to run_fastbag at $(date)" > LOGX
source /opt/plusai/setup.bash
source /opt/ros/melodic/setup.bash
export ROS_MASTER_URI=http://127.0.0.1:11311
fastbag_play --paths /home/yonghui/bags_for_test/20230703T145816_j7-00010_0_103to283.db
