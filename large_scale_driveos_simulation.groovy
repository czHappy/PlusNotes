import groovy.json.JsonOutput
import groovy.json.JsonSlurper
import groovy.time.*
import java.util.Random;
import hudson.EnvVars
import hudson.Util

def whoAmI() {
    def userId = sh(returnStdout: true, script: 'id -u').trim()
    def groupId = sh(returnStdout: true, script: 'id -g').trim()
    return String.format("%s:%s", userId, groupId)
}

def runOnBench(Inet4Address target, String local_send_dir, String local_scratch_dir,
                String script, int timeout_mins, Boolean with_lock = true) {
    // TODO(yonghui): add a credential for linux bench, hardcode for now
    def user_name = "plusai"
    def password = "plusai"
    def address = target.getHostAddress()
    def script_short_md5 = generateMD5(script).substring(0, 6)
    def script_name = "script_${script_short_md5}.sh"
    def tmp_script_name = "$local_scratch_dir/$script_name"
    def lock_name = "${address}_bench_remote".replace(".", "_")
    writeFile(file: tmp_script_name, text: script)
    def my_script="sshpass -p \"${password}\" ssh -o StrictHostKeyChecking=no ${user_name}@${address}" +
                    " 'bash -s' < ${tmp_script_name}"
    def output = ""
    if (with_lock) {
        lock(lock_name) {
            timeout(time: timeout_mins, unit: 'MINUTES') {
                // upload folder
                if (!local_send_dir.isEmpty()) {
                    sendDir(remote, local_send_dir, local_scratch_dir, remote_workspace)
                }
                // run script via ssh
                output = sh(script: my_script,
                            returnStdout: true)
            }
        }
    } else {
        timeout(time: timeout_mins, unit: 'MINUTES') {
            // upload folder
            if (!local_send_dir.isEmpty()) {
                sendDir(remote, local_send_dir, local_scratch_dir, remote_workspace)
            }
            // run script via ssh
            output = sh(script: my_script,
                        returnStdout: true)
        }
    }
    println "run script on bench finished:${output}"
    return output
}

def start_container(String drive_img_name, String container_name){
    def commandStr = """
        /usr/bin/docker stop ${container_name} 2>/dev/null || true
        /usr/bin/docker rm ${container_name} 2>/dev/null || true
        /usr/bin/docker run -itd --init --rm \
        --ipc private --network host \
        --add-host dist-cn:10.50.10.133 \
        --hostname `hostname` \
        -w  ${env.WORKSPACE}  \
        -v  ${env.WORKSPACE}:${env.WORKSPACE}:rw,z \
        -v /home/jenkins/.ssh:/home/jenkins/.ssh:ro \
        -v /etc/resolv.conf:/etc/resolv.conf:ro \
        -v /etc/hosts:/etc/hosts:ro --runtime=runc \
        -v /run/udev:/run/udev:ro \
        -v /etc/localtime:/etc/localtime:ro \
        -v /etc/timezone:/etc/timezone:ro \
        -v /opt/plusai/log:/opt/plusai/log:rw \
        --group-add audio \
        --runtime=runc \
        --name ${container_name} \
        --privileged \
        --user root \
        --shm-size=4gb \
        ${drive_img_name} /bin/bash
    """
    print("Running: " + commandStr)
    return sh(script: commandStr, returnStdout: true)
}

populateEnv()

// how to get a x86 mechine?
node("l4e-perf-slaves") {
    def dregistry = getDockerRegistry()
    def repo_prefix = "${dregistry.short_name}:${dregistry.port}/"
    // def drive_img_name = ensurePrefixed(repo_prefix, "${env.DRIVE_IMG_NAME}")
    def drive_img_name = ensurePrefixed(repo_prefix, "plusai/drive:latest")
    // def drive_img_name = "145b94e29bb3"
    // def package_name = "${env.package_name}" http://192.168.10.18/gtx5/dists/l4e_v3na_linux_packages/
    def package_name = "1.6.1998.tar.gz"
    // how to get current host ip
    def slave_node =  InetAddress.getByName(env.NODE_NAME)
    def host_ip = slave_node.getHostAddress()
    // def ros_master_uri = "${env.ROS_MASTER_URI}"
    def ros_master_uri = "http://localhost:11311"
    def ros_master_uri_recorder = "http://localhost:11312"
    // here we just test 192.168.10.184
    def adu_ip = "192.168.10.184"
    def bag_addr = "https://bagdb.pluscn.cn:28443/raw/mnt/vault6/drives/2023-07-03/20230703T233017_pdb-l4e-b0008_0.db"
    def package_addr = "http://192.168.10.18/gtx5/dists/l4e_v3na_linux_packages/1.6.2169.tar.gz"
    def play_time = 2
    def dispatcher_server_container_name = "driveos-simulation-dispatcher_server"
    def recorder_container_name = "driveos-simulation-recorder"
    def vehicle_name="ADU_${adu_ip}".replace(".", "_")
    stage('Test and init') {
        timeout(time: 1, unit: 'HOURS') {
            timestamps {
                echo 'start...'
                echo "dregistry.url = ${dregistry.url}"
                echo "dregistry.credential = ${dregistry.credential}"
                def command = """
                    ls -la
                    pwd
                    echo "start"
                    echo begin
                    echo pwd = \$PWD
                    echo repo_prefix = ${repo_prefix}
                    echo drive_img_name = ${drive_img_name}
                    echo bag_addr = ${bag_addr}
                    echo package_name = ${package_name}
                    echo host_ip = ${host_ip}
                    echo ros_master_uri = ${ros_master_uri}
                    echo "WORKSPACE = ${env.WORKSPACE}"
                    ls -la /dev/shm/
                    echo "clean shm..."
                    rm -rf /dev/shm/*
                """
                sh(script: command, returnStdout: true)
                echo 'Print Vars OK...'
                
            }
        }
    }

    stage("Pull image"){
        docker.withRegistry("${dregistry.url}", "${dregistry.credential}") {
            echo "Start Pulling ${drive_img_name} from ${dregistry.short_name} registry..."
            def drive_image = docker.image(drive_img_name)
            drive_image.pull()
            echo "Complete Pulling ${drive_img_name} from ${dregistry.short_name} registry"
        }
    }

    stage("Download Bag"){
        def command = """
            wget ${bag_addr}
        """
        sh(script: command, returnStdout: true)
    }

    stage("Start Software Stack"){
        def remote = InetAddress.getByName(adu_ip)
        def local_send_dir = ""
        def local_scratch_dir = "/tmp/plusai_bench_perf"
        def script_timeout = 3
        def bench_lock_name = "${adu_ip}_bench_perf".replace(".", "_")
        lock(bench_lock_name) {
            def command = """
                #cd /data
                #rf -rf opt.tar.gz || true
                #curl ${package_addr} --output opt.tar.gz
                #echo "download opt finished"
                #rm -rf opt
                #rm -rf plusai
                #rm -rf ros
                #tar -xzvf opt.tar.gz >/dev/null
                #echo "uncompress opt finished"
                #sudo rm -rf /opt/plusai
                #sudo rm -rf /opt/ros
                #sudo ln -sf /data/plusai /opt/plusai
                #sudo ln -sf /data/ros /opt/ros
                #echo "link opt finished"
                echo "Start Software Stack at \$(date) " > LOGSSS
                vehicle=\$(cat < /data/VIN)
                echo \$vehicle
                source /opt/plusai/launch/pdb-l4e-\$vehicle/setup.sh
                hamlaunch stop
                echo "To start full stack..."
                echo 4 | hamlaunch start --modules system_metrics,vehicle_can,app_watchdog,prediction,uds_server_node,dispatcher_server
                hamlaunch stop --modules dispatcher_server
                echo 4 | hamlaunch start --modules dispatcher_server
            """
            runOnBench(remote, local_send_dir, local_scratch_dir, command, script_timeout)
            echo "Start Software Stack Success!"
        }
    }

    stage("Start Dispatcher Server") {
        docker.withRegistry("${dregistry.url}", "${dregistry.credential}") {
            // start a drive container in x86 host
            start_container(drive_img_name, dispatcher_server_container_name)
            def commandFile = "start_dispatcher.sh"
            def docker_exec_command  = """
                echo "Start Dispatcher Server docker at \$(date)" > LOGX
                rm -rf /dev/shm/*
                source /opt/plusai/setup.bash
                source /opt/ros/melodic/setup.bash
                export ROS_MASTER_URI=${ros_master_uri}
                bash /opt/plusai/tools/event_recorder/start_ros_master.sh
                cd /opt/plusai/bin
                ./dispatcher_server > dispatcher_server.log 2>&1
                """
            writeFile(file: commandFile, text: docker_exec_command)
            def start_script = "sudo docker exec -itd ${dispatcher_server_container_name} bash -xe ${commandFile}"
            echo "Start Dispatcher Server Success!"
            sh(script: start_script, returnStdout: true)
        }
    }

    stage("Start Dispatcher Client") {
        def remote = InetAddress.getByName(adu_ip)
        def local_send_dir = ""
        def local_scratch_dir = "/tmp/plusai_bench_perf"
        def script_timeout = 3
        def bench_lock_name = "${adu_ip}_bench_perf".replace(".", "_")
        lock(bench_lock_name) {
            def command = """
                echo "Start Dispatcher Client docker at \$(date) " > LOGSDC
                # start full stack, here just test
                vehicle=\$(cat < /data/VIN)
                echo \$vehicle
                source /opt/plusai/launch/pdb-l4e-\$vehicle/setup.sh
                # start dispatcher_client
                kill -9 \$(pgrep -f "/usr/bin/python /opt/ros/kinetic/bin/rosmaster --core")
                
                bash /opt/plusai/tools/event_recorder/start_ros_master.sh > /dev/null 2>&1 &
                export ROS_MASTER_URI=${ros_master_uri}
                export PLUSAI_DISPATCHER_SERVER_IP_ADDRESS=${host_ip}
                rm -rf /dev/shm/*
                cd /opt/plusai/bin
                kill -9 \$(pgrep -f "./dispatcher_client --dispatcher_decompress")
                ./dispatcher_client --dispatcher_decompress > dispatcher_client.log 2>&1 &
            """
            runOnBench(remote, local_send_dir, local_scratch_dir, command, script_timeout)
            echo "Start Dispatcher Client Success!"
        }
    }

    stage("Record Bag"){
        docker.withRegistry("${dregistry.url}", "${dregistry.credential}") {
            def container_name=recorder_container_name
            
            def user_name = shellEscape(whoAmI())
            echo user_name
            def docker_run_command = """
                /usr/bin/docker stop ${container_name} 2>/dev/null || true
                /usr/bin/docker rm ${container_name} 2>/dev/null || true
                /usr/bin/docker run -itd --rm \
                --log-driver syslog \
                --log-opt tag=${container_name} \
                --ipc private \
                --network bridge \
                --hostname `hostname` \
                -w ${env.WORKSPACE}  \
                -v ${env.WORKSPACE}:${env.WORKSPACE}:rw  \
                -v /media:/media:slave \
                -v /run/udev:/run/udev:ro \
                -v /etc/localtime:/etc/localtime:ro \
                -v /etc/timezone:/etc/timezone:ro \
                -v /opt/plusai/log:/opt/plusai/log:rw \
                --group-add audio \
                --runtime=runc \
                --name ${container_name} \
                --privileged \
                --user root \
                --env VEHICLE_NAME=${vehicle_name} \
                --env PLUSAI_IPC_PUBSUB_SUBSCRIBER_MODES=shm_bus \
                --env PLUSAI_SHM_BUS_DISPATCHER_SOCKET_NODELAY=false \
                --env PLUSAI_SHM_BUS_DISPATCHER_BDFL_IPV4S=${host_ip} \
                --env PLUSAI_SHM_BUS_DISPATCHER_ENABLE=true \
                --env IPC_PUBSUB_PUBLISHER_USE_SHM_ALLOCATORS=true \
                --env IPC_PUBSUB_PUBLISHER_USE_SHM_TAPE_ALLOCATORS=false \
                --env SHM_ENABLE_TOPIC_SPECIFIC_RING_BUFFER=false \
                --env DISPATCHER_SERVER_IP_ADDRESS=${adu_ip} \
                --env DISPATCHER_MAX_TRY_TIMES=36000 \
                --env USE_NEW_DISPATCHER=true \
                --shm-size=1gb \
                ${drive_img_name} /bin/bash -xe run-recorder.sh
            """
            def commandFile = "run-recorder-container.sh"
            writeFile(file: commandFile, text: docker_run_command)
            def docker_exec_command = """
                echo "I'M comming! `date`" > LOGX
                source /opt/ros/melodic/setup.bash
                source /opt/plusai/setup.bash
                printf "\$BGreen%s\$Color_Off: %s\n" "USE_NEW_DISPATCHER" \${USE_NEW_DISPATCHER}
                if [ "\${USE_NEW_DISPATCHER}" = true ]; then
                    export ROS_IP=localhost
                    export ROS_MASTER_URI=${ros_master_uri_recorder}
                    printf " \$BGreen%s\$Color_Off: %s\n" "ROS_IP" \${ROS_IP}
                    printf " \$BGreen%s\$Color_Off: %s\n" "ROS_MASTER_URI" \${ROS_MASTER_URI}
                    nohup rosmaster -p 11312 --core 2>&1 &
                    printf "===== running dispatcher client\n"
                    /opt/plusai/bin/dispatcher_client > dispatcher_client.log 2>&1 &
                fi
                sed -i "s/64M/8M/g" /opt/plusai/conf/recorder/recorder_qnx.json
                sed -i "s/4G/16G/g" /opt/plusai/conf/recorder/recorder_qnx.json
                sed -i 's/"count": 4/"count": 1/g' /opt/plusai/conf/recorder/recorder_qnx.json
                old_path="/opt/plusai/run/recording"
                new_path="${env.WORKSPACE}"
                sed -i "s|\$old_path|\$new_path|g" /opt/plusai/conf/recorder/recorder_qnx.json
                cat /opt/plusai/conf/recorder/recorder_qnx.json
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
                --ipc_pubsub_subscriber_modes=shm_bus > recorder.log 2>&1
            """
            commandFile = "run-recorder.sh"
            writeFile(file: commandFile, text: docker_exec_command)
            sh(script: "bash run-recorder-container.sh", returnStdout: true)
            echo "start fastbag_play success..."
        }
    }

    stage("Play Bag"){
        docker.withRegistry("${dregistry.url}", "${dregistry.credential}") {
            def commandFile = "play_bag.sh"
            def docker_exec_command = """
                echo "Now let's play bag..." > LOGBAG
                source /opt/plusai/setup.bash
                source /opt/ros/melodic/setup.bash
                export ROS_MASTER_URI=${ros_master_uri}
                bag_filename=\$(basename $bag_addr)
                timeout ${play_time}m fastbag_play --paths \$bag_filename -loop > fastbag_play.log 2>&1 || true
                echo "Play complete..."
                """
            writeFile(file: commandFile, text: docker_exec_command)
            // here we reuse dispather_server container to start fastbag_play
            def start_script = "sudo docker exec ${dispatcher_server_container_name} bash -xe ${commandFile}"
            sh(script: start_script, returnStdout: true)
            echo "start fastbag_play success..."
        }
    }

    stage("Publish record result"){
        def command = """
            rosbag reindex ${vehicle_name}_fallback_0.bag

        """
        sh(script: command, returnStdout: false)
        archiveArtifacts("${vehicle_name}_fallback_0.bag")
        sh(script:"echo record ok", returnStdout: true)
    }

    stage("Cleanup"){
        def command = """
            /usr/bin/docker stop ${recorder_container_name} 2>/dev/null || true
            /usr/bin/docker stop ${dispatcher_server_container_name} 2>/dev/null || true
            cd ${env.WORKSPACE}
            sudo rm -rf *
        """
        sh(script: command, returnStdout: true)
    }

}
