#!/usr/bin/env groovy
// See: https://github.com/jenkinsci/pipeline-examples/blob/master/docs/BEST_PRACTICES.md
// See: https://jenkins.io/doc/book/pipeline/
import groovy.json.JsonOutput
import groovy.json.JsonSlurper
import groovy.time.*
import java.util.Random;
import com.cloudbees.groovy.cps.NonCPS

populateEnv()

env.ADU_SERVER = "192.168.10.229"

class Globals {
    static def node_name_mapping = [
        "obstacle": "fusion_object_tracker",
        "lane": "lane_detector",
        "perception": "perception",
        "prediction": "prediction",
        "planning": "scenario_planner",
        "control": "controller_ros_node",
        "localization": "localization",
    ]

    static def compare_timer_names = [
        "obstacle": ["scoped_timer.fusion_tracker_step_impl"],
        "lane": ["scoped_timer.lane_detector_by_async_image"],
        "perception": ["scoped_timer.fusion_tracker_step_impl", "scoped_timer.lane_detector_by_async_image"],
        "prediction": ["Prediction::runOnce"],
        "planning": ["scoped_timer.planning_run_once_by_behavior"],
        "localization": ["LocalizationManager::CheckAndPublishLocalizationState",
            "LocalizationManager::onSimpleFusionResult", "VisualGeometryLocalizer::localize"],
        "control": ["scoped_timer.control_module_latency"],
    ]

    static def baseline_url_prefix = "http://192.168.10.18/gtx5/dists/single_node_perf_test/v3na_linux_baseline/"

}

class Bucket {
    //From    0.0 ms to    0.1 ms:    751 hits, avg    0.011 ms (percentile: 100.00%)
    static def bucket_pattern = /\s*From\s+(?<lowbound>\d+.\d+)\s+ms\s+to\s+(?<upbound>\d+.\d+)\s+ms:\s+(?<hitnum>\d+)\s+hits,\s+avg\s+(?<avgtime>\d+.\d+)\s+ms.*$/
    Double low_bound
    Double up_bound
    Integer hit
    Double avg
    @NonCPS
    String toString() {
        return "Bucket[${low_bound} to ${up_bound}, hit: ${hit}, avg: ${avg}]"
    }
}


class LatencyItem {
    // Timer stats for 'AEBThreatRegion::getAEBThreatSTRegion':    751 hits, avg:    0.011
    static def timer_pattern = /\s*Timer\s+stats\s+for\s+'(?<timername>.+)':\s+(?<hitnum>\d+)\s+hits,\s+avg:\s+(?<avgtime>\d+.\d+)/
    String name
    ArrayList buckets
    Integer total_hits
    Double avg
    @NonCPS
    String toString() {
        return "LatencyItem[name: ${name}, hit: ${total_hits}, avg: ${avg}]"
    }
}


class CheckResult {
    String node_name
    String timer_name
    boolean passed
    Double baseline
    Double current_result
    @NonCPS
    String toString() {
        String s = passed ? "PASSED " : "FAILED!!! "
        return s + "[node_name: ${node_name}, timer_name: ${timer_name}, baseline: ${baseline}," + " current_result: ${current_result}]"
    }
}


def string_contains(String orig, String checkStr) {
    return orig.indexOf(checkStr) > -1;
}


def get_latency_items_from_text(def text) {
    String begin_mark = "Latency report over the last"
    boolean start_read = false
    String current_item_name
    def latency_items = [:]
    for (line in text.split("\n")) {
        if (string_contains(line, begin_mark)) {
            if (start_read) {
                break
            }
            start_read = true
            continue
        }
        if (!start_read) {
            continue
        }
        def bucket_matcher = line =~ Bucket.bucket_pattern
        if (bucket_matcher.matches()) {
            def bucket = new Bucket()
            bucket.low_bound = new Double(bucket_matcher.group("lowbound"))
            bucket.up_bound = new Double(bucket_matcher.group("upbound"))
            bucket.hit = new Integer(bucket_matcher.group("hitnum"))
            bucket.avg = new Double(bucket_matcher.group("avgtime"))
            latency_items[current_item_name].buckets.add(bucket)
            continue
        }
        def timer_matcher = line =~ LatencyItem.timer_pattern
        if (timer_matcher.matches()) {
            def latency_item = new LatencyItem()
            latency_item.name = timer_matcher.group("timername")
            latency_item.buckets = []
            latency_item.total_hits = new Integer(timer_matcher.group("hitnum"))
            latency_item.avg = new Double(timer_matcher.group("avgtime"))
            latency_items[latency_item.name] = latency_item
            current_item_name = latency_item.name
        }
    }
    return latency_items
}


def should_pass(LatencyItem baseline, LatencyItem output) {
    // latency increase more than 0.5ms or more than 20%(for tiny scoped timer) is considered to be
    // failed
    def diff = output.avg - baseline.avg
    return diff < 0.5 && diff / baseline.avg < 0.2
}

def get_latency_file_name_by_node(String node) {
    def module = Globals.node_name_mapping[node]
    return "latency_report_${module}.txt"
}

def get_latency_baseline_file_name_by_node(String node) {
    def module = Globals.node_name_mapping[node]
    return "latency_report_${module}_baseline.txt"
}

def get_comparing_pairs(String directory) {
    def comparison_pairs = [:]
    nodes = env.nodes.split(",")
    nodes.each { node ->
        def latency_file_name = get_latency_file_name_by_node(node)
        def local_file = "${directory}/${latency_file_name}"
        def latency_baseline_file_name = get_latency_baseline_file_name_by_node(node)
        def base_line_url = Globals.baseline_url_prefix + "/${latency_baseline_file_name}"
        comparison_pairs[node] = [local_file, base_line_url]
    }
    return comparison_pairs
}


def CheckNodesLatency(String directory) {
    def check_results = []
    def comparison_pairs = get_comparing_pairs(directory)
    boolean ret_value = true
    comparison_pairs.each { node, pair ->
        def local_file_text = readFile(pair[0])
        def local_items = get_latency_items_from_text(local_file_text)
        def baseline_latency_text = new URL(pair[1]).openConnection().getInputStream().getText()
        def baseline_items = get_latency_items_from_text(baseline_latency_text)

        Globals.compare_timer_names[node].each { timer_name ->
            def check_result = new CheckResult()
            check_result.node_name = node
            check_result.timer_name = timer_name
            check_result.passed = true
            check_result.baseline = baseline_items[timer_name].avg
            check_result.current_result = local_items[timer_name].avg
            check_result.passed = should_pass(baseline_items[timer_name], local_items[timer_name])
            check_results.add(check_result)
        }
    }
    check_results.each { result ->
        println result
        if (!result.passed) {
            ret_value = false
        }
    }
    return ret_value
}


def PrepareBaseline(String job_data_dir, String baseline_dir) {
    Globals.node_name_mapping.each { node_name, bin_name ->
        def origin_name = job_data_dir + "/" + get_latency_file_name_by_node(node_name)
        if (fileExists(origin_name)) {
            def content = readFile(origin_name)
            def baseline_file_name = baseline_dir + "/" + get_latency_baseline_file_name_by_node(node_name)
            println "COPY file: ${origin_name} AS baseline: ${baseline_file_name}"
            writeFile(file: baseline_file_name, text: content)
        }
    }
}


def pick_v3na_linux_server() {
    return env.ADU_SERVER
}


def runOnV3NALinux(Inet4Address target, String local_send_dir, String local_scratch_dir,
             String script, int timeout_mins, Boolean with_lock = true) {
    def user_name = "plusai"
    def password = "plusai"
    def address = target.getHostAddress()
    def script_short_md5 = generateMD5(script).substring(0, 6)
    def script_name = "script_${script_short_md5}.sh"
    def tmp_script_name = "$local_scratch_dir/$script_name"
    def lock_name = "${address}_v3na_linux_remote".replace(".", "_")
    writeFile(file: tmp_script_name, text: script)
    def my_script="sshpass -p \"${password}\" ssh -o StrictHostKeyChecking=no ${user_name}@${address}" +
                    " 'bash -s' < ${tmp_script_name}"
    def output = ""
    if (with_lock) {
        lock(lock_name) {
            timeout(time: timeout_mins, unit: 'MINUTES') {
                // run script via ssh
                output = sh(script: my_script,
                            returnStdout: true)
            }
        }
    } else {
        timeout(time: timeout_mins, unit: 'MINUTES') {
            // run script via ssh
            output = sh(script: my_script,
                        returnStdout: true)
        }
    }
    println "run script on v3na_linux finished:${output}"
    return output
}


def start_container(String drive_img_name, String container_name){
    def commandStr = """
        /usr/bin/docker stop ${container_name} 2>/dev/null || true
        /usr/bin/docker rm ${container_name} 2>/dev/null || true
        /usr/bin/docker run -itd --init --rm \
        --ipc private --network bridge \
        --add-host dist-cn:10.50.10.10 \
        --hostname `hostname` \
        -w  ${env.WORKSPACE}  \
        -v  ${env.WORKSPACE}:${env.WORKSPACE}:rw,z \
        -v  /mnt/nfs2/offline_bags:/mnt/nfs2/offline_bags:rw,z  \
        -v /home/jenkins/.ssh:/home/jenkins/.ssh:ro \
        -v /etc/resolv.conf:/etc/resolv.conf:ro \
        -v /etc/hosts:/etc/hosts:ro  \
        -v /run/udev:/run/udev:ro \
        -v /etc/localtime:/etc/localtime:ro \
        -v /etc/timezone:/etc/timezone:ro \
        -v /opt/plusai/log:/opt/plusai/log:rw \
        -p 13009:13009  \
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

def start_recorder_container(String drive_img_name, String recorder_container_name, String record_bag_name, String host_ip){
    def container_name = recorder_container_name
    def docker_run_command = """
        /usr/bin/docker stop ${container_name} 2>/dev/null || true
        /usr/bin/docker rm ${container_name} 2>/dev/null || true
        sleep 2s
        /usr/bin/docker run -itd --rm \
        --log-driver syslog \
        --log-opt tag=${container_name} \
        --ipc private \
        --network bridge \
        --add-host dist-cn:10.50.10.10 \
        --hostname `hostname` \
        -w ${env.WORKSPACE}  \
        -v ${env.WORKSPACE}:${env.WORKSPACE}:rw  \
        -v /etc/resolv.conf:/etc/resolv.conf:ro \
        -v /etc/hosts:/etc/hosts:ro  \
        -v /media:/media:slave \
        -v /run/udev:/run/udev:ro \
        -v /etc/localtime:/etc/localtime:ro \
        -v /etc/timezone:/etc/timezone:ro \
        -v /opt/plusai/log:/opt/plusai/log:rw \
        -p 13010:13010  \
        --group-add audio \
        --runtime=runc \
        --name ${container_name} \
        --privileged \
        --user root \
        --env VEHICLE_NAME=${record_bag_name} \
        --env PLUSAI_IPC_PUBSUB_SUBSCRIBER_MODES=shm_bus \
        --env PLUSAI_SHM_BUS_DISPATCHER_SOCKET_NODELAY=false \
        --env PLUSAI_SHM_BUS_DISPATCHER_BDFL_IPV4S=${host_ip} \
        --env PLUSAI_SHM_BUS_DISPATCHER_ENABLE=false \
        --env IPC_PUBSUB_PUBLISHER_USE_SHM_ALLOCATORS=true \
        --env IPC_PUBSUB_PUBLISHER_USE_SHM_TAPE_ALLOCATORS=false \
        --env SHM_ENABLE_TOPIC_SPECIFIC_RING_BUFFER=false \
        --env DISPATCHER_SERVER_IP_ADDRESS=${env.ADU_SERVER} \
        --env DISPATCHER_MAX_TRY_TIMES=36000 \
        --env USE_NEW_DISPATCHER=true \
        --shm-size=1gb \
        ${drive_img_name} /bin/bash
    """
    print("Running: " + docker_run_command)
    return sh(script: docker_run_command, returnStdout: true)
}

def record_bag_function(String recorder_container_name){
    def commandFile = "run-recorder.sh"
    def docker_exec_command = """
        echo "I'M comming! `date`" > LOGX
        source /opt/ros/melodic/setup.bash
        source /opt/plusai/setup.bash
        printf "\$BGreen%s\$Color_Off: %s" "USE_NEW_DISPATCHER" \${USE_NEW_DISPATCHER}
        export ROS_IP=localhost
        export ROS_MASTER_URI=http://localhost:11312
        export PLUSAI_DISPATCHER_SERVER_TCP_PORT=13010
        printf " \$BGreen%s\$Color_Off: %s" "ROS_IP" \${ROS_IP}
        printf " \$BGreen%s\$Color_Off: %s" "ROS_MASTER_URI" \${ROS_MASTER_URI}
        nohup rosmaster -p 11312 --core 2>&1 &

        grep -v "//" /opt/plusai/conf/recorder/recorder_qnx.json > /opt/plusai/conf/recorder/recorder_qnx_nocomments.json
        jq '.wanted_topics=["^((?!(plusai_record|plus.*view)).)*/latency_report\$","/system_metrics"]|.chunk_size="10K"' /opt/plusai/conf/recorder/recorder_qnx_nocomments.json > /opt/plusai/conf/recorder/recorder_large_scale.json
        sed -i 's/"count": 4/"count": 1/g' /opt/plusai/conf/recorder/recorder_large_scale.json

        printf "===== running dispatcher client"
        /opt/plusai/bin/dispatcher_client --dispatcher_server_tcp_port=13010 --dispatcher_topics_json_file=/opt/plusai/conf/recorder/recorder_large_scale.json > dispatcher_client.log 2>&1 &
        printf "===== running recorder"
        /opt/plusai/bin/recorder \
        --logtostderr \
        --minloglevel=0 \
        --logbuflevel=-1 \
        --stderrthreshold=0 \
        --config=/opt/plusai/conf/recorder/recorder_large_scale.json \
        --logbufsecs=0 \
        --v=0 \
        --shm_bus_dispatcher_forwarding_strategy=all \
        --shm_bus_dispatcher_enable=true \
        --ipc_pubsub_subscriber_modes=shm_bus > recorder.log 2>&1 &
    """
    writeFile(file: commandFile, text: docker_exec_command)
    def start_script = "sudo docker exec ${recorder_container_name} bash -xe ${commandFile}"
    sh(script: start_script, returnStdout: true)
    echo "start bag record success..."
}

def start_adu_dispatcher_server(String flag, Inet4Address v3na_linux_remote){
    def local_send_dir = ""
    def local_scratch_dir = "/tmp/plusai_linux_perf"
    def script_timeout = 60
    def adu_exec_command  = """
        vehicle=\$(cat < /data/VIN)
        source /opt/plusai/launch/pdb-l4e-\$vehicle/setup.sh
        cd /opt/plusai/bin
        export PLUSAI_DISPATCHER_SERVER_TCP_PORT=13010
        ./dispatcher_server ${flag} > dispatcher_server.log 2>&1 &
    """
    runOnV3NALinux(v3na_linux_remote, local_send_dir, local_scratch_dir, adu_exec_command, script_timeout)

    echo "Start Dispatcher Client Success!"
}

def get_record_bag_file(String record_bag_name, String recorder_container_name){
    def commandFile = "deal_bag.sh"
    def command = """
        kill -2 \$(pidof recorder) 2>/dev/null || true
        sleep 10s
        source /opt/ros/melodic/setup.bash
        cd /opt/plusai/run/recording
        rm -rf *.orig.bag
        rosbag reindex ${record_bag_name}_fallback_0.bag
        rm -rf ${env.WORKSPACE}/bag_results || true
        mkdir ${env.WORKSPACE}/bag_results
        cp ${record_bag_name}_fallback_0.bag ${env.WORKSPACE}/bag_results
        cd ${env.WORKSPACE}/bag_results
        mv ${record_bag_name}_fallback_0.bag large-scale-simulation-record-${env.BUILD_NUMBER}.bag
        # analysis bag
        bag_name="large-scale-simulation-record-${env.BUILD_NUMBER}.bag"
        python /opt/plusai/share/drive_common/aggregate_latency_reports_from_bags.py --topics /perception/latency_report --bags \$bag_name > bag-analysis-perception-${env.BUILD_NUMBER}.txt
        python /opt/plusai/share/drive_common/aggregate_latency_reports_from_bags.py --topics /localization/latency_report --bags \$bag_name > bag-analysis-localize-${env.BUILD_NUMBER}.txt
        python /opt/plusai/share/drive_common/aggregate_latency_reports_from_bags.py --topics /prediction/latency_report --bags \$bag_name > bag-analysis-prediction-${env.BUILD_NUMBER}.txt
        python /opt/plusai/share/drive_common/aggregate_latency_reports_from_bags.py --topics /scenario_planner/latency_report --bags \$bag_name > bag-analysis-planning-${env.BUILD_NUMBER}.txt
        python /opt/plusai/share/drive_common/aggregate_latency_reports_from_bags.py --topics /controller_ros_node/latency_report --bags \$bag_name > bag-analysis-control-${env.BUILD_NUMBER}.txt
        python /opt/plusai/share/drive_common/aggregate_latency_reports_from_bags.py --topics /system_metrics --bags \$bag_name > bag-analysis-system-metrics-${env.BUILD_NUMBER}.txt
        python /opt/plusai/share/drive_common/extract_system_metrics_from_bags.py --bags \$bag_name
    """
    writeFile(file: commandFile, text: command)
    def start_script = "sudo docker exec ${recorder_container_name} bash -xe ${commandFile}"
    sh(script: start_script, returnStdout: true)
    archiveArtifacts("bag_results/*")
    echo "Publish record result success..."
}

def start_x86_dispatcher_server(String flag, String dispatcher_server_container_name){
    def commandFile = "start_dispatcher_server.sh"
    def docker_exec_command = """
        kill -9 \$(pidof ./dispatcher_server) || true
        rm -rf /dev/shm/*
        source /opt/plusai/setup.bash
        source /opt/ros/melodic/setup.bash
        cd /opt/plusai/bin
        export PLUSAI_DISPATCHER_SERVER_TCP_PORT=13009
        ./dispatcher_server ${flag} > dispatcher_server.log 2>&1 &
    """
    writeFile(file: commandFile, text: docker_exec_command)
    def start_script = "docker exec ${dispatcher_server_container_name} bash -xe ${commandFile}"
    sh(script: start_script, returnStdout: true)
    echo "Start Dispatcher Server Success!"
}

def fastbag_play(String enable_topics, String dispatcher_server_container_name, String bag_name){
    def commandFile = "playbag.sh"
    def docker_exec_command = """
        source /opt/plusai/setup.bash
        source /opt/ros/melodic/setup.bash
        fastbag_play -envelope_timestamp -clock --paths ${bag_name} \
            --hz=600 \
            -enable_topics ${enable_topics}  \
            -rate=1 \
            -terminate_on_buffer_exhausted=false \
            --shm_bus_enable_simulated_clock_management=false > fastbag_play.log 2>&1
    """
    writeFile(file: commandFile, text: docker_exec_command)
    def start_script = "sudo docker exec ${dispatcher_server_container_name} bash -xe ${commandFile}"
    sh(script: start_script, returnStdout: true)
    echo "playbag Success!"
}

def start_adu_dispatcher_client(String flag, String dispatcher_server_ip, Inet4Address v3na_linux_remote){
    def local_send_dir = ""
    def local_scratch_dir = "/tmp/plusai_linux_perf"
    def script_timeout = 60
    def adu_exec_command  = """
        vehicle=\$(cat < /data/VIN)
        source /opt/plusai/launch/pdb-l4e-\$vehicle/setup.sh
        cd /opt/plusai/bin
        export PLUSAI_DISPATCHER_SERVER_IP_ADDRESS=${dispatcher_server_ip}
        export PLUSAI_DISPATCHER_SERVER_TCP_PORT=13009
        ./dispatcher_client ${flag} > dispatcher_client.log 2>&1 &
    """
    runOnV3NALinux(v3na_linux_remote, local_send_dir, local_scratch_dir, adu_exec_command, script_timeout)
    echo "Start Dispatcher Client Success!"
}

def start_driveos_node(String ros_ip, Inet4Address v3na_linux_remote, Boolean record_bag_bool){
    def local_send_dir = ""
    def local_scratch_dir = "/tmp/plusai_linux_perf"
    def script_timeout = 60
    def adu_exec_command  = """
        vehicle=\$(cat < /data/VIN)
        source /opt/plusai/launch/pdb-l4e-\$vehicle/setup.sh
        export PLUSAI_SHM_BUS_ENABLE_SIMULATED_CLOCK_MANAGEMENT=true
        export ROS_IP=${ros_ip}
        export ROS_MASTER_URI=http://localhost:11311
        export USE_NVSCI_FOR_LANE_RAW_IMAGE=true
        export USE_NVSCI_FOR_OBSTACLE_RAW_IMAGE=true
        export ENABLE_ROS_CLOCK=false
        export CLOCK_HZ=1000
        export PLUSAI_LOGGER_ENABLE_LOG_LEVELS=off,error,catastrophe,assert_failure
        export GLOG_minloglevel=2
        export ENABLE_PLUS_ODOM=true
        export GNSS_USE_ROS_TIME=true
        export USE_LOCALIZATION_TOPIC=true
        export USE_ODOMETRY_POSE=true
        export PLUSAI_LOGGER_DEFAULT_STRUCTURED_LOG_DESTINATIONS={}
        export PLUSAI_LOGGER_DEFAULT_LOG_DESTINATIONS='{"text": {"file": "[auto]"}}'
        export PLUSAI_LOGGER_LOG_VERBOSITY_LEVEL=0
        export PLUSAI_LOGGER_DEFAULT_LOG_VERBOSITY_LEVEL=0
        export PLUSAI_LOGGER_DEFAULT_LOG_LEVEL=error
        export PLUSAI_LOGGER_CREATE_TEMPORARY_BUFFER_IF_MISSING=false
        if [ "${record_bag_bool}" = "true" ]
        then
            export PLUSAI_LATENCY_AGGREGATOR_REPORT_PERIOD=5
            export PLUSAI_LATENCY_AGGREGATOR_OUTPUT_MODE=shm_bus,ipc
        else
            export PLUSAI_LATENCY_AGGREGATOR_REPORT_PERIOD=600000
            export PLUSAI_LATENCY_AGGREGATOR_OUTPUT_MODE=console
        fi
        env|grep AGGREGATOR_OUTPUT_MODE
        export PLUSAI_DISPATCHER_CLIENT_STABLE_FQ_PUBLISH=true
        export PLUSAI_IPC_PUBSUB_REWRITE_MESSAGE_TIMESTAMP=true
        export PLUSAI_IPC_PUBSUB_SUBSCRIBER_CONCURRENT_CALLBACK_ACTION=BLOCK
        export GNSS_FROM_SENSOR=true
        export IPC_PUBSUB_PUBLISHER_MODES=ros,shm_bus
        # add gmsl camear ENV for simulator
        export IPC_PUBSUB_SUBSCRIBER_MODES=shm_bus
        export CAMERAS_LINK_ENABLE_MASKS="0x0000 0x0111 0x1111 0x0000"
        export CAMERAS_HANDLER_PARALLEL=true
        export CAMERAS_ENABLE_SIMULATION=true
        export CAMERAS_RAW_IMAGE_USE_SCIBUF=true
        bash /opt/plusai/launch/l4e-common/start-component.sh gmsl_cam
        export ENABLE_LANE_DETECTOR=true
        export ENABLE_FUSION_OBJECT_TRACKER=true
        export ENABLE_AUTO_CALIBRATION=false
        export ENABLE_TRAFFIC_LIGHT_DETECTOR=false
        export ENABLE_PERCEPTION=true
        pkill mapsensor MapSensor
        sleep 2s
        export READY_TO_LOCALIZE=true
        bash /opt/plusai/launch/l4e-common/start-hdi-mapsensor-when-needed.sh
        sleep 1s
        bash /opt/plusai/launch/l4e-common/start-component.sh localization
        export READY_TO_LOCALIZE=false
        bash /opt/plusai/launch/l4e-common/start-component.sh perception
        bash /opt/plusai/launch/l4e-common/start-component.sh planning
        bash /opt/plusai/launch/l4e-common/start-component.sh control
        bash /opt/plusai/launch/l4e-common/start-component.sh prediction
        echo 4 | hamlaunch start --modules system_metrics
    """
    runOnV3NALinux(v3na_linux_remote, local_send_dir, local_scratch_dir, adu_exec_command, script_timeout)
    echo "Start driveos nodes Success!"
}

def get_node_log_file(String node, Inet4Address v3na_linux_remote, String result_dir, String package_version, String vehicle_name, Boolean record_bag_bool ){
    def local_send_dir = ""
    def local_scratch_dir = "/tmp/plusai_linux_perf"
    def script_timeout = 60
    def node_name = Globals.node_name_mapping[node]
    def module_name = node
    def adu_exec_command = """
        extract_log_and_latency_report() {
            pid=\$1
            log_file=`ls /tmp/*.log | grep \${pid}|xargs`
            if [ "${record_bag_bool}" != "true" ]; then
                latency_file="${result_dir}/latency_report_${node_name}.txt"
                grep "latency_aggregator_reporter.cpp" \${log_file} | awk -F"latency_aggregator_reporter.cpp:[0-9]+" '{print \$2}' > \${latency_file}
                # Give some marks for this latency report
                echo "=================== Running Marks ===================" >> \${latency_file}
                echo "${package_version} ${vehicle_name} ${node}" >> \${latency_file}
            fi
            mv \${log_file} ${result_dir}/

            launch_log_file=`ls /tmp/*.log | grep launch_${node}`
            echo "launch_log_file: \${launch_log_file}"
            mv \${launch_log_file} ${result_dir}/

            core_file=`ls /tmp/core* | grep "core.*${node_name}.*-\${pid}-"`
            if [ "\$core_file" != "" ]; then
                echo "core_file: \${core_file}"
                mv \${core_file} ${result_dir}/
            fi
        }
        vehicle=\$(cat < /data/VIN)
        source /opt/plusai/launch/pdb-l4e-\$vehicle/setup.sh
        source /opt/plusai/launch/l4e-common/functions.sh
        PID=`cat /tmp/${node_name}.pid|xargs`
        # bash /opt/plusai/launch/l4e-common/stop-component.sh ${module_name}
        kill -2 \$PID
        sleep 5s
        extract_log_and_latency_report \$PID
        pkill mapsensor MapSensor
    """
    runOnV3NALinux(v3na_linux_remote, local_send_dir, local_scratch_dir, adu_exec_command, script_timeout)
}

def get_enable_topics() {
    String ret = "/rear_left_camera/image_color/compressed,/rear_right_camera/image_color/compressed,/front_left_camera/image_color/compressed,/front_right_camera/image_color/compressed,/front_center_camera/image_color/compressed,/side_left_camera/image_color/compressed,/side_right_camera/image_color/compressed,/rear_left_radar/radar_tracks,/rslidar_points,/bumper_radar/radar_tracks,/rear_right_radar/radar_tracks,/side_left_radar/radar_tracks,/side_right_radar/radar_tracks,/plus/odom,/imu/data,/novatel_data/inspvax,/novatel_data/inspva,/vehicle/dbw_reports,/vehicle/truck_state,/vehicle/misc_1_report,/ublox/status_report,/watchdog/current_state"
    return ret;
}

def get_bag_names(String prefix, String bag_name, String start, String end) {
    def result = []
    def startInt = start.toInteger()
    def endInt = end.toInteger()

    if (startInt <= endInt) {
        for (int i = startInt; i <= endInt; i++) {
            result.add("${prefix}/${bag_name}_${i}.db")
        }
    }

    return result.join(',')
}

def download_bag_script = """
import requests
import sys
import os
bag_name = sys.argv[1]
start_index = (int)(sys.argv[2])
end_index = (int)(sys.argv[3])
for idx in range(start_index, end_index+1):
    db_file_name = bag_name + "_" + str(idx) + ".db"
    if os.path.exists(db_file_name):
        os.remove(db_file_name)
    url = "https://bagdb.pluscn.cn/api/v1/bags"
    bag_name_specific = bag_name + "_" + str(idx) + ".bag"
    query_params = {'bag_name': bag_name_specific}
    response = requests.get(url, params=query_params)
    if response.status_code == 200:
        data = response.json()
    link = data[0]["fastbag_path"].split("?")[0]
    print(link)
"""

pr_git_sha = ""
github_context = ""
repo = ""
runAndNotifyGH(pr_git_sha.trim(), repo, github_context.trim()) {
    timeout(time: 24, unit: 'HOURS') {
        timestamps {
            node("l4e-v3na-linux-perf-slaves") {
                def dregistry = getDockerRegistry()
                def repo_prefix = "${dregistry.short_name}:${dregistry.port}/"
                def drive_img_name = ensurePrefixed(repo_prefix, "plusai/drive:latest")
                def bag_name = "20231024T143225_pdb-l4e-b0005_8_100to300.db"
                def package_prefix = "http://192.168.10.18/gtx5/dists/nfs/v3na_linux/test_package"
                def package_version = "1.6.6116"
                def slave_node =  InetAddress.getByName(env.NODE_NAME)
                def host_ip = slave_node.getHostAddress()
                def dispatcher_server_container_name = "v3na-linux-large-scale-c1"
                def recorder_container_name = "v3na-linux-large-scale-c2"
                def record_bag_name = "large-scale-simulation"
                String v3na_linux_server = pick_v3na_linux_server()
                def v3na_linux_remote = InetAddress.getByName(v3na_linux_server)
                String v3na_linux_lock_name = "${v3na_linux_server}_v3na_linux_perf".replace(".", "_")
                String vehicle_name = "pdb-l4e-b0007"
                String brand = "pdb-l4e"
                String vin = "b0007"
                String result_dir = "/data/large_scale_${env.BUILD_NUMBER}"
                String ros_master_uri = "http://localhost:11311"
                String workspace = "/data"
                def job_failed = false
                def bag_download_dir = "/mnt/nfs2/offline_bags/"
                def record_bag_bool = false
                lock(v3na_linux_lock_name) {
                    try{
                        stage('Check and Init') {
                            def command = """
                                echo 'start...'
                                echo pwd = \$PWD
                                echo "dregistry.url = ${dregistry.url}"
                                echo "dregistry.credential = ${dregistry.credential}"
                                echo "WORKSPACE = ${env.WORKSPACE}"
                                echo repo_prefix = ${repo_prefix}
                                echo drive_img_name = ${drive_img_name}
                                echo package_version = ${package_version}
                                echo x86_host_ip = ${host_ip}
                                echo dispatcher_server_container_name = ${dispatcher_server_container_name}
                                echo v3na_linux_server = ${v3na_linux_server}
                                echo v3na_linux_remote = ${v3na_linux_remote}
                                echo v3na_linux_lock_name = ${v3na_linux_lock_name}
                                echo brand = ${brand}
                                echo vin = ${vin}
                                echo result_dir = ${result_dir}
                                echo ros_master_uri = ${ros_master_uri}
                                /usr/bin/docker stop ${dispatcher_server_container_name} 2>/dev/null || true
                                /usr/bin/docker rm ${dispatcher_server_container_name} 2>/dev/null || true
                                cd ${env.WORKSPACE}
                                sudo rm -rf *
                            """
                            sh(script: command, returnStdout: true)
                            echo 'Check and init OK...'
                        }
                        // stage("Pull Image"){
                        //     docker.withRegistry("${dregistry.url}", "${dregistry.credential}") {
                        //         echo "Start Pulling ${drive_img_name} from ${dregistry.short_name} registry..."
                        //         def drive_image = docker.image(drive_img_name)
                        //         drive_image.pull()
                        //         echo "Complete Pulling ${drive_img_name} from ${dregistry.short_name} registry"
                        //     }
                        // }
                        stage("Download bag"){
                            // writeFile(file: "download_bags.py", text: download_bag_script)
                            // def command = """
                            //     cd ${bag_download_dir}
                            //     python3 ${env.WORKSPACE}/download_bags.py ${bag_name} ${bag_start} ${bag_end} > downloadlink.txt
                            //     xargs -n 1 wget < downloadlink.txt
                            // """
                            // sh(script: command, returnStdout: true)
                            def command = """
                                cd ${bag_download_dir}
                                if [ ! -f "20231024T143225_pdb-l4e-b0005_8_100to300.db" ]; then
                                    wget http://192.168.10.18/gtx5/dists/single_node_perf_test/20231024T143225_pdb-l4e-b0005_8_100to300.db
                                fi
                                #if [ ! -f "20231031T085623_pdb-l4e-b0004_0.db" ]; then
                                #    wget https://bagdb.pluscn.cn:28443/raw/mnt/vault40/drives/2023-10-31/20231031T085623_pdb-l4e-b0004_0.db
                                #fi
                            """
                            sh(script: command, returnStdout: true)
                        }

                        stage("Deploy package"){
                                String local_send_dir = ""
                                String local_scratch_dir = "/tmp/plusai_linux_perf"
                                int script_timeout = 60
                                // def command = """
                                //     ps aux | grep "launch_" | awk '{print \$2}' | xargs kill -9 || true
                                //     ps aux | grep ".log" | awk '{print \$2}' | xargs kill -9 || true
                                //     ps aux | grep "dispatcher" | awk '{print \$2}' | xargs kill -9 || true
                                //     ps aux | grep "ros" | awk '{print \$2}' | xargs kill -9 || true
                                //     rm -rf /dev/shm/*
                                //     sleep 2s
                                //     cd ${workspace}
                                //     rm -f /tmp/launch_*.log
                                //     rm -f /tmp/fastbag_play*.log
                                //     echo "${brand}" > /data/RBAND
                                //     echo "${vin}" > /data/VIN
                                //     rm -rf opt.tar.gz || true
                                //     rm -rf opt || true
                                //     rm -rf plusai || true
                                //     rm -rf ros || true
                                //     curl ${package_prefix}/${package_version}.tar.gz -o opt.tar.gz
                                //     echo "download opt finished"
                                //     tar -xzvf opt.tar.gz >/dev/null
                                //     echo "uncompress opt finished"
                                //     sudo rm -rf /opt/plusai
                                //     sudo rm -rf /opt/ros
                                //     sudo ln -sf ${workspace}/plusai /opt/plusai
                                //     sudo ln -sf ${workspace}/ros /opt/ros
                                //     echo "link opt finished"
                                //     mkdir ${result_dir}
                                //     rm -rf /opt/plusai/launch/l4e-common/application_start*
                                //     sed -i 's/\\/navsat\\/odom/\\/navsat\\/odom_null/g' /opt/plusai/config/localization_param.prototxt.j7-l4e
                                //     sed -i "/wanted_topics/a\\\\ \\"^.*\\"," /opt/plusai/conf/recorder/recorder_qnx.json
                                //     vehicle=\$(cat < /data/VIN)
                                //     source /opt/plusai/launch/pdb-l4e-\$vehicle/setup.sh
                                //     hamlaunch stop
                                //     export ROS_MASTER_URI=${ros_master_uri}
                                //     bash /opt/plusai/tools/event_recorder/start_ros_master.sh > /dev/null 2>&1 &
                                //     sleep 2s
                                //     rosparam set /use_sim_time true
                                // """
                                def command = """
                                    ps aux | grep "launch_" | awk '{print \$2}' | xargs kill -9 || true
                                    ps aux | grep ".log" | awk '{print \$2}' | xargs kill -9 || true
                                    ps aux | grep "dispatcher" | awk '{print \$2}' | xargs kill -9 || true
                                    ps aux | grep "ros" | awk '{print \$2}' | xargs kill -9 || true
                                    rm -rf /dev/shm/*
                                    cd ${workspace}
                                    rm -f /tmp/launch_*.log
                                    rm -f /tmp/fastbag_play*.log
                                    echo "${brand}" > /data/RBAND
                                    echo "${vin}" > /data/VIN
                                    mkdir ${result_dir}
                                    vehicle=\$(cat < /data/VIN)
                                    source /opt/plusai/launch/pdb-l4e-\$vehicle/setup.sh
                                    hamlaunch stop
                                    export ROS_MASTER_URI=${ros_master_uri}
                                    bash /opt/plusai/tools/event_recorder/start_ros_master.sh > /dev/null 2>&1 &
                                    sleep 2s
                                    rosparam set /use_sim_time true
                                """
                                runOnV3NALinux(v3na_linux_remote, local_send_dir, local_scratch_dir, command, script_timeout)

                            
                        }
                        stage("Start x86 container"){
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
                                    rosparam set /use_sim_time true
                                    """
                                writeFile(file: commandFile, text: docker_exec_command)
                                def start_script = "sudo docker exec -itd ${dispatcher_server_container_name} bash -xe ${commandFile}"
                                echo "Start x86 container Success!"
                                sh(script: start_script, returnStdout: true)
                            }
                        }

                        stage("Run large scale simulation") {
                            String local_send_dir = ""
                            String local_scratch_dir = "/tmp/plusai_linux_perf"
                            int script_timeout = 60
                            def node_list = ["perception", "prediction", "planning", "control", "localization"]
                            String dispatcher_client_flags = "--dispatcher_decompress --dispatcher_rewrite_decompressed_topic --dispatcher_client_stable_fq_publish --dispatcher_server_tcp_port=13009"
                            String dispatcher_server_flags = "--dispatcher_decompress --dispatcher_rewrite_decompressed_topic --dispatcher_server_tcp_port=13009"
                            start_x86_dispatcher_server(dispatcher_server_flags, dispatcher_server_container_name)
                            start_adu_dispatcher_client(dispatcher_client_flags, host_ip, v3na_linux_remote, )
                            sh(script: "sleep 3s", returnStdout: true)

                            start_driveos_node(v3na_linux_server, v3na_linux_remote, record_bag_bool)
                            sh(script: "sleep 3s", returnStdout: true)

                            if (record_bag_bool) {
                                docker.withRegistry("${dregistry.url}", "${dregistry.credential}") {
                                    // start a record container in x86 host
                                    start_recorder_container(drive_img_name, recorder_container_name, record_bag_name, host_ip)
                                    record_bag_function(recorder_container_name)
                                }
                                String adu_dispatcher_server_flags = "--dispatcher_server_tcp_port=13010"
                                start_adu_dispatcher_server(adu_dispatcher_server_flags, v3na_linux_remote)
                                echo "==> start bag record successfully."
                                sh(script: "sleep 3s", returnStdout: true)
                            }


                            // fastbag_play(get_enable_topics(), dispatcher_server_container_name, get_bag_names(bag_download_dir, bag_name, bag_start, bag_end))
                            fastbag_play(get_enable_topics(), dispatcher_server_container_name, "${bag_download_dir}/${bag_name}")
                            for (node in node_list) {
                                echo "NODE ==> ${node}"
                                get_node_log_file(node, v3na_linux_remote, result_dir, package_version, vehicle_name, record_bag_bool)
                            }
                            sh(script: "sleep 5s", returnStdout: true)
                        }
                        if (record_bag_bool) {
                            docker.withRegistry("${dregistry.url}", "${dregistry.credential}") {
                                get_record_bag_file(record_bag_name, recorder_container_name)
                            }
                            sh(script: "sleep 3s", returnStdout: true)
                        }
                        stage("Archive Artifacts") {
                            def remote_addr = "plusai@${v3na_linux_server}:${result_dir}"
                            def local_addr = "./"
                            def command = """
                                sshpass -p "plusai" scp -o StrictHostKeyChecking=no -r ${remote_addr} ${local_addr} || echo "scp failed"
                                mkdir -p /mnt/nfs2/large_scale_simulation/${env.BUILD_NUMBER}
                                mv large_scale_${env.BUILD_NUMBER}/core-* /mnt/nfs2/large_scale_simulation/${env.BUILD_NUMBER} || true
                            """
                            sh(script: command, returnStdout: true)
                            archiveArtifacts("large_scale_${env.BUILD_NUMBER}/*")
                        }
                    }
                    catch (e) {
                        println("got an exception:${e}, task failed...")
                        job_failed = true
                    }
                    stage("Cleanup") {
                        String local_send_dir = ""
                        String local_scratch_dir = "/tmp/plusai_linux_perf"
                        def adu_clean_command = """
                            ps aux | grep "launch_" | awk '{print \$2}' | xargs kill -9 || true
                            ps aux | grep ".log" | awk '{print \$2}' | xargs kill -9 || true
                            ps aux | grep "ros" | awk '{print \$2}' | xargs kill -9 || true
                            ps aux | grep "disp" | awk '{print \$2}' | xargs kill -9 || true
                        """
                        runOnV3NALinux(v3na_linux_remote, local_send_dir, local_scratch_dir, adu_clean_command, 5)
                        def x86_clean_command = """
                            /usr/bin/docker stop ${dispatcher_server_container_name} 2>/dev/null || true
                            /usr/bin/docker rm ${dispatcher_server_container_name} 2>/dev/null || true
                            /usr/bin/docker stop ${recorder_container_name} 2>/dev/null || true
                            /usr/bin/docker rm ${recorder_container_name} 2>/dev/null || true
                        """
                        sh(script: x86_clean_command, returnStdout: true)
                        currentBuild.setDescription("large scale simulation running on v3na_linux has been finished on ${v3na_linux_server}")
                    }
                    if(job_failed) {
                        throw new Exception("large scale simulation job failed!")
                    }
                }// release lock_v3na_linux
            }
        }
    }
}
