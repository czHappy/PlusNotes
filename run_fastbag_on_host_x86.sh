echo "GOTO bj4 at 2023年 07月 24日 星期一 11:58:50 CST" > LOGX
docker cp /home/chengzhen/tmp/run_fastbag.sh yonghui_drive:/home/yonghui
docker exec yonghui_drive /bin/bash -xe run_fastbag.sh
