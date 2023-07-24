echo "GOTO bj4 at 2023年 07月 24日 星期一 11:40:00 CST" > LOGX
sudo rm -rf /dev/shm/*
docker cp /home/chengzhen/tmp/run_on_docker_dispatcher_server.sh yonghui_drive:/home/yonghui
docker exec -itd yonghui_drive /bin/bash -xe run_on_docker_dispatcher_server.sh

# 这里新开container 需要对网络做些配置
# docker stop run_dispatcher_server
# docker rm run_dispatcher_server
# ls -la run_on_docker_dispatcher_server.sh
# docker run --name run_dispatcher_server -w /home/chengzhen/tmp -v /home/chengzhen/tmp:/home/chengzhen/tmp -itd docker.plusai.co:5050/plusai/drive:latest /bin/bash -xe run_on_docker_dispatcher_server.sh
# echo "Complete!"
