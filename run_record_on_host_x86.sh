echo "GOTO bj4 at 2023年 07月 24日 星期一 11:59:17 CST" > LOGX-record
docker cp /home/chengzhen/tmp/run_record_on_host_x86.sh yonghui_drive:/home/yonghui
docker exec -itd yonghui_drive /bin/bash -x run_record_on_host_x86.sh
