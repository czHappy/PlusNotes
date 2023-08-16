
import time
import json
from datetime import datetime, timezone, timedelta
from collections import deque
import pdb

oil_monitor_period = 5  # second
oil_alert_period = 45
oil_first_alert_period = 10  # [0,10) [10, 45)
oil_monitor = {}  # oil_monitor[vehicle] = true/false
oil_monitor_fuel_data = {}  # oil_monitor_fuel_data[vehicle] = [0,1,2,3,4]
oil_alert_period_data = {}  # oil_alert_period_data[vehicle] = [0,1,2,3,...9, 10,11,...,43,44]
vehicle_last_oil_alert_timestamp = {}
oil_alert_min_intervel_secs = 30
oil_monitor_speed_threshold = 2 / 3.6
oil_alert_speed_data = {}  # latest 5s speed record.
oil_monitor_speed_data = {}
oil_latest_period = 10
oil_restart_fuel_threshold = 10
vehicle_last_seq_num = {}
vehicle_last_start_timestamp = {}
oil_latest_fuel_data = {}

data1 = []

# 生成50行数据
for i in range(1, 51):
    seq_num = i
    speed = 0
    # fuel_level = 60 - (i * (5/50)) + random.uniform(-0.5, 0.5)  
    fuel_level = 60.0 - (i * 0.1)  # 从60逐渐减少到55
    
    # 创建 JSON 对象并添加到数据数组中
    item = {"seq_num": seq_num, "speed": speed, "fuel_level": fuel_level}
    data1.append(item)

data2 = []
# 生成50行数据
for i in range(1, 51):
    seq_num = i
    speed = 0
    
    if 1 <= i <= 5:
        fuel_level = round(60 - ((i-1) * 0.05), 1)  # 区间[1,5]内，从60下降到59.7
    elif 6 <= i <= 15:
        fuel_level = round(59.6 - ((i-6) * 0.03), 1)  # 区间[6,15]内，从59.6下降到59.3
    elif 16 <= i <= 50:
        fuel_level = round(59.3 - ((i-16) * 0.02), 1)  # 区间[16,50]内，从59.3下降到58.2
    
    # 创建 JSON 对象并添加到数据数组中
    item = {"seq_num": seq_num, "speed": speed, "fuel_level": fuel_level}
    data2.append(item)
data3 = []

# 生成50行数据
for i in range(0, 50):
    seq_num = i
    speed = 0
    
    if 0 <= i < 5:
        fuel_level = round(60 - (0.35) / 4 * i, 1)  # 区间[1,5]内，从60下降到59.7
    elif 5 <= i < 15:
        fuel_level = round(59.6 - ((0.2) / 9)* (i - 5), 1)  # 区间[6,15]内，从59.6下降到59.4
    elif 15 <= i < 50:
        fuel_level = round(59.4 - (0.4) / 34 * (i - 15), 1)  # 区间[16,50]内，从59.4下降到59.0
    
    # 创建字典对象并添加到数据数组中
    item = {"seq_num": seq_num, "speed": speed, "fuel_level": fuel_level}
    data3.append(item)
data = data3
# 输出数据
for item in data:
    print(item)
def process_oil_alert(vehicle_name, m, timestamp):
    global oil_monitor_period  # length of monitor window, 5
    global oil_alert_period  # alert period, 45s, [0,10)U[10, 45)
    global oil_first_alert_period  # first alert period, [0,10)
    global oil_monitor  # whether the vehicle is being monitored
    global oil_monitor_fuel_data  # record vehicle fuel when the vehicle is not being monitored
    global oil_alert_period_data  # record vehicle fuel when the vehicle is being monitored
    global vehicle_last_oil_alert_timestamp  # last alert timestamp
    global oil_alert_min_intervel_secs  # min alert interval
    global oil_monitor_speed_threshold  # speed threshold, 2km/h
    global oil_monitor_speed_data  # record vehicle speed when the vehicle is not being monitored
    global vehicle_last_seq_num  # vehicle last seq id
    global vehicle_last_start_timestamp  # vehicle last start timestamp
    global oil_latest_fuel_data  # recoed latest fuel data whether being monitored or not
    global oil_latest_period  # recoed latest fuel data length, 10
    global oil_restart_fuel_threshold  # restart fuel decrease threshold, 10%
    oil_monitor_threshold = 0.3  # default monitor triger fuel threshold, speed < 2km/h
    oil_first_alert_period_threshold = 0.3  # default first alert period fuel threshold, speed < 2km/h
    oil_second_alert_period_threshold = 1.0  # default second alert period fuel threshold, speed < 2km/h
    if vehicle_name != "pdb-l4e-a0001":  # wait for test vehicle
        return
    # try:
    
    # pdb.set_trace()
    fuel_level = m["fuel_level"]
    speed = m["speed"]
    # restart check
    seq_num = m["seq_num"]
    if vehicle_name not in vehicle_last_seq_num.keys():
        vehicle_last_seq_num[vehicle_name] = seq_num
    if vehicle_name not in vehicle_last_start_timestamp.keys():
        vehicle_last_start_timestamp[vehicle_name] = timestamp
    time_diff = timestamp - vehicle_last_start_timestamp[vehicle_name]
    # meet restart conditions
    if vehicle_last_seq_num[vehicle_name] != 1 and seq_num == 1 and time_diff > 60:
        vehicle_last_start_timestamp[vehicle_name] = timestamp  # now restart
        print("vehicle restart, last seq num = {}, current num = {}, \
                    last timestamp = {}, current timestamp = {}".format(
                    vehicle_last_seq_num[vehicle_name],
                    seq_num,
                    vehicle_last_start_timestamp[vehicle_name],
                    timestamp))
        # no enough latest fuel data, maybe FP.
        if len(oil_latest_fuel_data[vehicle_name]) != oil_latest_period:
            return
        last_avg_fuel = sum(oil_latest_fuel_data[vehicle_name]) / oil_latest_period
        if last_avg_fuel - fuel_level >= oil_restart_fuel_threshold:
            alert_message = (
                vehicle_name
                + ": fuel abnormal during system off."
            )
            # sendSlackNotify(alert_message, "#fuel-monitor-track", slack_token, False)
            print("process_oil_alert send a oil alert to slack, alert_message = ", alert_message, " seq = ", seq_num)
            oil_monitor_speed_data[vehicle_name].clear()  # reset speed window
            oil_monitor_fuel_data[vehicle_name].clear()  # reset oil window
            oil_monitor[vehicle_name] = False  # reset monitor status
            oil_latest_fuel_data[vehicle_name].clear()  # reset oil latest record
            vehicle_last_seq_num[vehicle_name] = 1
            return
    vehicle_last_seq_num[vehicle_name] = seq_num
    if vehicle_name not in oil_monitor.keys():
        oil_monitor[vehicle_name] = False
    # latest 10s fuel_level record.
    if vehicle_name not in oil_latest_fuel_data.keys():
        oil_latest_fuel_data[vehicle_name] = deque()
    oil_latest_fuel_data[vehicle_name].append(fuel_level)
    if len(oil_latest_fuel_data[vehicle_name]) > oil_latest_period:
        oil_latest_fuel_data[vehicle_name].popleft()

    # fuel_level record for monitor.
    if vehicle_name not in oil_monitor_fuel_data.keys():
        oil_monitor_fuel_data[vehicle_name] = deque()
    if oil_monitor[vehicle_name] is False:
        oil_monitor_fuel_data[vehicle_name].append(fuel_level)
    if len(oil_monitor_fuel_data[vehicle_name]) > oil_monitor_period:  # latest 5s oil
        oil_monitor_fuel_data[vehicle_name].popleft()
    # speed record for monitor.
    if vehicle_name not in oil_monitor_speed_data.keys():
        oil_monitor_speed_data[vehicle_name] = deque()
    if oil_monitor[vehicle_name] is False:  # only record speed in no monitor period
        oil_monitor_speed_data[vehicle_name].append(speed)
    if len(oil_monitor_speed_data[vehicle_name]) > oil_monitor_period:  # latest 5s speed
        oil_monitor_speed_data[vehicle_name].popleft()

    # decide whether this vehicle need to be monitored
    # check speed, all data must less than oil_monitor_speed_threshold or greater equal to oil_monitor_speed_threshold
    if min(oil_monitor_speed_data[vehicle_name]) < oil_monitor_speed_threshold and (
            max(oil_monitor_speed_data[vehicle_name]) >= oil_monitor_speed_threshold):
        oil_monitor_speed_data[vehicle_name].clear()  # reset speed window
        oil_monitor_fuel_data[vehicle_name].clear()  # reset oil window
        oil_monitor[vehicle_name] = False  # reset monitor status
        return
    # check error or adding oil
    if len(oil_monitor_fuel_data[vehicle_name]) == oil_monitor_period:
        diff = oil_monitor_fuel_data[vehicle_name][0] - oil_monitor_fuel_data[vehicle_name][-1]
        if diff < 0:
            print("process_oil_alert diff negative: diff = ", str(diff), " seq = ", seq_num)
            return
    # no enough fuel_level data or speed data
    if len(oil_monitor_fuel_data[vehicle_name]) < oil_monitor_period or (
            len(oil_monitor_speed_data[vehicle_name]) < oil_monitor_period):
        return
    # thresholds for speed > 2km/h
    if oil_monitor_speed_data[vehicle_name][-1] > oil_monitor_speed_threshold:
        oil_monitor_threshold = 1.8
        oil_first_alert_period_threshold = 2
        oil_second_alert_period_threshold = 5
    # oil check
    if oil_monitor[vehicle_name] is False and diff >= oil_monitor_threshold:
        oil_monitor[vehicle_name] = True
        print("process_oil_alert vehicle_name = ",vehicle_name, "is being monitored", " seq = ", seq_num)
    # deal vehicle if it was monitored
    if oil_monitor[vehicle_name] is True:
        if vehicle_name not in oil_alert_period_data.keys():
            oil_alert_period_data[vehicle_name] = []
        oil_alert_period_data[vehicle_name].append(fuel_level)
        # check speed, speed must be consistent with the monitor period.
        if (max(oil_monitor_speed_data[vehicle_name]) < oil_monitor_speed_threshold and speed >= oil_monitor_speed_threshold) or (
                (min(oil_monitor_speed_data[vehicle_name]) >= oil_monitor_speed_threshold and speed < oil_monitor_speed_threshold)):
            oil_monitor[vehicle_name] = False
            oil_alert_period_data[vehicle_name].clear()
            oil_monitor_speed_data[vehicle_name].clear()
            oil_monitor_fuel_data[vehicle_name].clear()
            return

        need_alert = False
        # look [0,10)
        if len(oil_alert_period_data[vehicle_name]) == oil_first_alert_period:
            if (oil_alert_period_data[vehicle_name][0] - oil_alert_period_data[vehicle_name][oil_first_alert_period - 1]) >= oil_first_alert_period_threshold:
                print("process_oil_alert alert in first 5s, vehicle = ", vehicle_name, " seq = ", seq_num)
                need_alert = True
        # look [10,45)
        if len(oil_alert_period_data[vehicle_name]) == oil_alert_period:
            if (oil_alert_period_data[vehicle_name][oil_first_alert_period] - oil_alert_period_data[vehicle_name][oil_alert_period - 1]) >= oil_second_alert_period_threshold:
                print("process_oil_alert alert in second 25s, vehicle = ", vehicle_name, " seq = ", seq_num)
                need_alert = True
        # if no alert in 45s, reset
        if len(oil_alert_period_data[vehicle_name]) > oil_alert_period and need_alert is False:
            oil_monitor[vehicle_name] = False
            oil_alert_period_data[vehicle_name].clear()
            oil_monitor_speed_data[vehicle_name].clear()
            oil_monitor_fuel_data[vehicle_name].clear()
            print("process_oil_alert no alert in 30s, vehicle = ", vehicle_name, " seq = ", seq_num)
        if need_alert:
            if vehicle_name not in vehicle_last_oil_alert_timestamp.keys():
                vehicle_last_oil_alert_timestamp[vehicle_name] = timestamp
            else:
                prev_timestamp = vehicle_last_oil_alert_timestamp[vehicle_name]
                timestamp_diff = abs(timestamp - prev_timestamp)
                if timestamp_diff < oil_alert_min_intervel_secs:
                    return
            vehicle_last_oil_alert_timestamp[vehicle_name] = timestamp
            # send alert
            beijing_tz = timezone(timedelta(hours=8))
            vehicle_time = datetime.fromtimestamp(timestamp)
            vehicle_time_bj_tz = vehicle_time.astimezone(beijing_tz)
            vehicle_time_bj_tz_str = vehicle_time_bj_tz.strftime("%m-%d %H:%M:%S")
            alert_message = vehicle_name
            over_str = "over" if speed >= oil_monitor_speed_threshold else "under"
            alert_message = (
                alert_message
                + ": fuel abnormal "
                + over_str
                + " 2kmh, time:"
                + vehicle_time_bj_tz_str
            )
            # sendSlackNotify(alert_message, "#fuel-monitor-track", slack_token, False)
            print("process_oil_alert send a oil alert to slack, alert_message = ", alert_message,  " seq = ", seq_num)
            # clear status after alert, the next alarm is 30 seconds later at the earliest.
            oil_monitor[vehicle_name] = False
            oil_alert_period_data[vehicle_name].clear()
            oil_monitor_speed_data[vehicle_name].clear()
            oil_monitor_fuel_data[vehicle_name].clear()
    # except Exception as e:
    #     alert_message = "got exception when process process oil alert: {}".format(e)
    #     print(alert_message)



if __name__ == "__main__":
    for m in data:
        timestamp=time.time()
        process_oil_alert("pdb-l4e-a0001", m, timestamp)