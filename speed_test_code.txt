
import time
import json
from datetime import datetime, timezone, timedelta
from collections import deque
import pdb
import random

oil_monitor_period = 5  # second
oil_alert_period = 45
oil_first_alert_period = 10  # [0,10) [10, 45)
oil_monitor = {}  # oil_monitor[vehicle] = true/false
oil_monitor_fuel_data = {}  # oil_monitor_fuel_data[vehicle] = [0,1,2,3,4]
oil_alert_period_data = {}  # oil_alert_period_data[vehicle] = [0,1,2,3,...9, 10,11,...,43,44]
vehicle_last_oil_alert_timestamp = {}
oil_alert_min_intervel_secs = 30
oil_monitor_speed_threshold = 2 # here we just use 2 to test
oil_alert_speed_data = {}  # latest 5s speed record.
oil_monitor_speed_data = {}
oil_latest_period = 10
oil_restart_fuel_threshold = 10
vehicle_last_seq_num = {}
vehicle_last_start_timestamp = {}
oil_latest_fuel_data = {}

"""
测试样例
1. 静止 在第一次触发 符合预期
2. 静止 在第二次触发 符合预期
3. 静止 监测滑动窗口触发监控 符合预期
4. 静止 断电重启测试 符合预期
5. 高速 不触发 符合预期
6. 高速 在第一次触发 符合预期
7. 不稳定速度测试 丢弃数据 符合预期
"""
# 静止 在第一次触发
data1 = []
for i in range(1, 51):
    seq_num = i
    speed = 0
    # fuel_level = 60 - (i * (5/50)) + random.uniform(-0.5, 0.5)  
    fuel_level = 60.0 - (i * 0.1)  # 从60逐渐减少到55
    
    # 创建 JSON 对象并添加到数据数组中
    item = {"seq_num": seq_num, "speed": speed, "fuel_level": fuel_level}
    data1.append(item)

# 静止 在第二次触发
data2 = []
for i in range(0, 50):
    seq_num = i+1
    speed = 0
    
    if 0 <= i < 5:
        fuel_level = round(60 - (0.35) / 4 * i, 3)  # 区间[1,5]内，从60下降到59.7
    elif 5 <= i < 15:
        fuel_level = round(59.6 - ((0.2) / 9)* (i - 5), 3)  # 区间[6,15]内，从59.6下降到59.4
    elif 15 <= i < 50:
        fuel_level = round(59.4 - (0.4) / 34 * (i - 15), 3)  # 区间[16,50]内，从59.4下降到59.0
    
    # 创建字典对象并添加到数据数组中
    item = {"seq_num": seq_num, "speed": speed, "fuel_level": fuel_level}
    data2.append(item)

# 静止 滑动窗口触发监控 注意当第5s触发监测点时，第5s的数据也会被记录到监控区间
data3 = []
for i in range(0, 55):
    seq_num = i
    speed = 0
    if 0 <= i < 5:
        fuel_level = round(60 - (0.25) / 4 * i, 3)  # 区间[0,5)内，从60下降到59.75
    elif 5 <= i < 15:
        fuel_level = round(59.5 - ((0.2) / 9) * (i - 5), 3)  # 区间[5,15)内，从59.5下降到59.3
    elif 15 <= i < 50:
        fuel_level = round(59.3 - (1.0) / 34 * (i - 15), 3)  # 区间[15,50)内，从59.3下降到58.3
    elif 50 <= i < 55:
        fuel_level = round(58.3 - (0.2) / 4 * (i - 50), 3)  # 区间[50,55)内，从58.3下降到58.1
    # 创建字典对象并添加到数据数组中
    item = {"seq_num": seq_num, "speed": speed, "fuel_level": fuel_level}
    data3.append(item)

# 静止 断电重启测试
data4 = [
    {'seq_num': 1, 'speed': 0, 'fuel_level': 59.938},
    {'seq_num': 2, 'speed': 0, 'fuel_level': 59.875},
    {'seq_num': 3, 'speed': 0, 'fuel_level': 59.812},
    {'seq_num': 4, 'speed': 0, 'fuel_level': 59.75},
    {'seq_num': 5, 'speed': 0, 'fuel_level': 59.5},
    {'seq_num': 6, 'speed': 0, 'fuel_level': 59.478},
    {'seq_num': 7, 'speed': 0, 'fuel_level': 59.456},
    {'seq_num': 8, 'speed': 0, 'fuel_level': 59.433},
    {'seq_num': 9, 'speed': 0, 'fuel_level': 59.411},
    {'seq_num': 10, 'speed': 0, 'fuel_level': 59.389},
    {'seq_num': 11, 'speed': 0, 'fuel_level': 59.367},
    {'seq_num': 12, 'speed': 0, 'fuel_level': 59.344},
    {'seq_num': 13, 'speed': 0, 'fuel_level': 59.322},
    {'seq_num': 14, 'speed': 0, 'fuel_level': 59.3},
    {'seq_num': 15, 'speed': 0, 'fuel_level': 59.3},
    {'seq_num': 16, 'speed': 0, 'fuel_level': 59.271},
    {'seq_num': 17, 'speed': 0, 'fuel_level': 59.241},
    {'seq_num': 18, 'speed': 0, 'fuel_level': 59.212},
    {'seq_num': 19, 'speed': 0, 'fuel_level': 59.182},
    {'seq_num': 20, 'speed': 0, 'fuel_level': 59.153},
    {'seq_num': 1, 'speed': 0, 'fuel_level': 29.124},
    {'seq_num': 2, 'speed': 0, 'fuel_level': 29.094},
    {'seq_num': 3, 'speed': 0, 'fuel_level': 29.065},
    {'seq_num': 4, 'speed': 0, 'fuel_level': 29.035},
    {'seq_num': 5, 'speed': 0, 'fuel_level': 29.006},
    {'seq_num': 6, 'speed': 0, 'fuel_level': 28.976},
    {'seq_num': 7, 'speed': 0, 'fuel_level': 28.947},
    {'seq_num': 8, 'speed': 0, 'fuel_level': 28.918},
    {'seq_num': 9, 'speed': 0, 'fuel_level': 28.888},
    {'seq_num': 10, 'speed': 0, 'fuel_level': 28.859},
    {'seq_num': 11, 'speed': 0, 'fuel_level': 28.829},
    {'seq_num': 12, 'speed': 0, 'fuel_level': 28.8},
    {'seq_num': 13, 'speed': 0, 'fuel_level': 28.771},
    {'seq_num': 14, 'speed': 0, 'fuel_level': 28.741},
    {'seq_num': 15, 'speed': 0, 'fuel_level': 28.712},
    {'seq_num': 16, 'speed': 0, 'fuel_level': 28.682},
    {'seq_num': 17, 'speed': 0, 'fuel_level': 28.653},
    {'seq_num': 18, 'speed': 0, 'fuel_level': 28.624},
    {'seq_num': 19, 'speed': 0, 'fuel_level': 28.594},
    {'seq_num': 20, 'speed': 0, 'fuel_level': 28.565},
    {'seq_num': 21, 'speed': 0, 'fuel_level': 28.535},
    {'seq_num': 22, 'speed': 0, 'fuel_level': 28.506},
    {'seq_num': 23, 'speed': 0, 'fuel_level': 28.476},
    {'seq_num': 24, 'speed': 0, 'fuel_level': 28.447},
    {'seq_num': 25, 'speed': 0, 'fuel_level': 28.418},
    {'seq_num': 26, 'speed': 0, 'fuel_level': 28.388},
    {'seq_num': 27, 'speed': 0, 'fuel_level': 28.359},
    {'seq_num': 28, 'speed': 0, 'fuel_level': 28.329},
    {'seq_num': 29, 'speed': 0, 'fuel_level': 28.3},
    {'seq_num': 30, 'speed': 0, 'fuel_level': 28.3},
    {'seq_num': 31, 'speed': 0, 'fuel_level': 28.25},
    {'seq_num': 32, 'speed': 0, 'fuel_level': 28.2},
    {'seq_num': 33, 'speed': 0, 'fuel_level': 28.15},
    {'seq_num': 34, 'speed': 0, 'fuel_level': 28.1}
]


# 高速 不触发
data5 = []
for i in range(1, 51):
    seq_num = i
    speed = 50
    fuel_level = 60.0 - (i * 0.1)  # 从60逐渐减少到55
    
    # 创建 JSON 对象并添加到数据数组中
    item = {"seq_num": seq_num, "speed": speed, "fuel_level": fuel_level}
    data5.append(item)

# 高速 触发第一次
data5 = []
for i in range(1, 51):
    seq_num = i
    speed = 50
    fuel_level = 60.0 - (i * 0.5)  # 从60逐渐减少到35
    
    # 创建 JSON 对象并添加到数据数组中
    item = {"seq_num": seq_num, "speed": speed, "fuel_level": fuel_level}
    data5.append(item)

# 速度不稳定测试 预期不处理
data6 = []
for i in range(1, 51):
    seq_num = i
    speed = random.randint(0, 4) * 1.00
    fuel_level = 60.0 - (i * 0.5)  # 从60逐渐减少到35
    
    # 创建 JSON 对象并添加到数据数组中
    item = {"seq_num": seq_num, "speed": speed, "fuel_level": fuel_level}
    data6.append(item)
data = data6



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
    # meet restart conditions, here we set time diff threshold 10 for test.
    if vehicle_last_seq_num[vehicle_name] != 1 and seq_num == 1 and time_diff > 10:
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
        print("last_avg_fuel = ",last_avg_fuel, "cur fuel_level = ", fuel_level)
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
    print("Speed window = ", oil_monitor_speed_data[vehicle_name], "oil_monitor_speed_threshold = ", oil_monitor_speed_threshold, "min = ",min(oil_monitor_speed_data[vehicle_name]), "max = ", max(oil_monitor_speed_data[vehicle_name]))
    if min(oil_monitor_speed_data[vehicle_name]) < oil_monitor_speed_threshold and max(oil_monitor_speed_data[vehicle_name]) >= oil_monitor_speed_threshold:
        print("check speed window, reset!, window = ",oil_monitor_speed_data[vehicle_name])
        oil_monitor_speed_data[vehicle_name].clear()  # reset speed window
        oil_monitor_fuel_data[vehicle_name].clear()  # reset oil window
        oil_monitor[vehicle_name] = False  # reset monitor status
        return
    # check error or adding oil
    if len(oil_monitor_fuel_data[vehicle_name]) == oil_monitor_period:
        diff = oil_monitor_fuel_data[vehicle_name][0] - oil_monitor_fuel_data[vehicle_name][-1]
        print("diff = ", diff, "seq_num = ", seq_num)
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
    print("oil_monitor_threshold = ", oil_monitor_threshold)
    # oil check
    if oil_monitor[vehicle_name] is False and diff >= oil_monitor_threshold:
        oil_monitor[vehicle_name] = True
        print("READY MONITORED! oil_monitor_speed_data[vehicle_name] = ", oil_monitor_speed_data[vehicle_name])
        print("process_oil_alert vehicle_name = ",vehicle_name, "is being monitored", " seq = ", seq_num)
    # deal vehicle if it was monitored
    if oil_monitor[vehicle_name] is True:
        if vehicle_name not in oil_alert_period_data.keys():
            oil_alert_period_data[vehicle_name] = []
        oil_alert_period_data[vehicle_name].append(fuel_level)
        # check speed, speed must be consistent with the monitor period.
        if (max(oil_monitor_speed_data[vehicle_name]) < oil_monitor_speed_threshold and speed >= oil_monitor_speed_threshold) or (
                (min(oil_monitor_speed_data[vehicle_name]) >= oil_monitor_speed_threshold and speed < oil_monitor_speed_threshold)):
            print("check speed failed, speed unstable, monitored cancel, speed = ", speed, " window = ",oil_monitor_speed_data[vehicle_name])
            oil_monitor[vehicle_name] = False
            oil_alert_period_data[vehicle_name].clear()
            oil_monitor_speed_data[vehicle_name].clear()
            oil_monitor_fuel_data[vehicle_name].clear()
            return

        need_alert = False
        # look [0,10)
        if len(oil_alert_period_data[vehicle_name]) == oil_first_alert_period:
            if (oil_alert_period_data[vehicle_name][0] - oil_alert_period_data[vehicle_name][oil_first_alert_period - 1]) >= oil_first_alert_period_threshold:
                print("process_oil_alert alert in first 10s, vehicle = ", vehicle_name, " seq = ", seq_num)
                need_alert = True
        # look [10,45)
        if len(oil_alert_period_data[vehicle_name]) == oil_alert_period:
            if (oil_alert_period_data[vehicle_name][oil_first_alert_period] - oil_alert_period_data[vehicle_name][oil_alert_period - 1]) >= oil_second_alert_period_threshold:
                print("process_oil_alert alert in second 35s, vehicle = ", vehicle_name, " seq = ", seq_num)
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
    idx = 1
    for m in data:
        timestamp = time.time() + idx
        idx += 1
        process_oil_alert("pdb-l4e-a0001", m, timestamp)