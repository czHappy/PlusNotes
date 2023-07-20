echo "GOTO bj4 at 2023年 07月 20日 星期四 19:07:25 CST" > LOGX
docker cp /home/chengzhen/tmp/run_fastbag.sh yonghui_drive:/home/yonghui
docker exec yonghui_drive /bin/bash -xe run_fastbag.sh
