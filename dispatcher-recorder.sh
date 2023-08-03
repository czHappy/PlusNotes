
# run.sh
CONTAINER_NAME="driveos-simulation-dispatcher-recorder"  # 自己设定的
CONTAINER_NAME_SUFIX="driveos-simulation"  # 自己设定的
VEHICLE_NAME="ADU-118"  # 这个名字无所谓吧
HOST_IP="192.168.2.14"  # 北京4 IP
ADU_IP="192.168.2.118"  # 118 ADU
IMAGE="docker.plusai.co:5050/plusai/drive:latest"  # 这里选用什么镜像呢？ 是drive:latest镜像吗？
/usr/bin/docker run -it --rm \
--log-driver syslog \
--log-opt tag=${CONTAINER_NAME} \
--ipc private \
--network bridge \
--hostname `hostname` \
-v /media:/media:slave \
-v /run/udev:/run/udev:ro \
-v /etc/localtime:/etc/localtime:ro \
-v /etc/timezone:/etc/timezone:ro \
-v /opt/plusai/log:/opt/plusai/log:rw \
-v `pwd`:/bbox  \
--group-add audio \
--runtime=runc \
--name ${CONTAINER_NAME} \
--privileged \
--user root \
--env VEHICLE_NAME=${VEHICLE_NAME} \
--env PLUSAI_IPC_PUBSUB_SUBSCRIBER_MODES=shm_bus \
--env PLUSAI_SHM_BUS_DISPATCHER_SOCKET_NODELAY=false \
--env PLUSAI_SHM_BUS_DISPATCHER_BDFL_IPV4S=${HOST_IP} \
--env PLUSAI_SHM_BUS_DISPATCHER_ENABLE=true \
--env IPC_PUBSUB_PUBLISHER_USE_SHM_ALLOCATORS=true \
--env IPC_PUBSUB_PUBLISHER_USE_SHM_TAPE_ALLOCATORS=false \
--env SHM_ENABLE_TOPIC_SPECIFIC_RING_BUFFER=false \
--env DISPATCHER_SERVER_IP_ADDRESS=${ADU_IP} \
--env DISPATCHER_MAX_TRY_TIMES=36000 \
--env USE_NEW_DISPATCHER=true \
--env CONTAINER_NAME_SUFIX=${CONTAINER_NAME_SUFIX} \
--env ROS_MASTER_URI=http://192.168.2.14:11312  \
--shm-size=1gb \
${IMAGE} /bin/bash -e /bbox/recorder.sh

# recorder.sh
#! /bin/bash


source /opt/ros/melodic/setup.bash
source /opt/plusai/setup.bash

printf " $BGreen%s$Color_Off: %s\n" "USE_NEW_DISPATCHER" ${USE_NEW_DISPATCHER}

if [ "${USE_NEW_DISPATCHER}" = true ]; then
  export ROS_IP=localhost
  export ROS_MASTER_URI="http://localhost:11312"
  printf " $BGreen%s$Color_Off: %s\n" "ROS_IP" ${ROS_IP}
  printf " $BGreen%s$Color_Off: %s\n" "ROS_MASTER_URI" ${ROS_MASTER_URI}
  nohup rosmaster -p 11312 --core 2>&1 &
  printf "===== running dispatcher client\n"
  /opt/plusai/bin/dispatcher_client &
fi
sed -i "s/64M/1M/g" /opt/plusai/conf/recorder/recorder_qnx.json
printf "===== running recorder\n"
/opt/plusai/bin/recorder \
--logtostderr \
--minloglevel=0 \
--logbuflevel=-1 \
--stderrthreshold=0 \
--config=/opt/plusai/conf/recorder/recorder_qnx.json \
--logbufsecs=0 \
--v=0 \
--shm_bus_dispatcher_forwarding_strategy=all \
--shm_bus_dispatcher_enable=true \
--ipc_pubsub_subscriber_modes=shm_bus