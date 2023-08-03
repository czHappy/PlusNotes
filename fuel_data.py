import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime
import pytz
import time
import random

# 从txt文件读取数据
data = []
timestamps = []
with open('data.txt', 'r') as file:
    for line in file:
        line = line.strip().split(',')
        data.append(int(line[0]))
        timestamps.append(float(line[1]))

# 将时间戳转换为北京时间（时分秒部分）
beijing_tz = pytz.timezone('Asia/Shanghai')
beijing_times = [datetime.fromtimestamp(ts).astimezone(beijing_tz).strftime('%H:%M:%S') for ts in timestamps]

# 绘制表格
plt.figure(figsize=(10, 6))
plt.plot(beijing_times, data, marker='o', linestyle='-')
plt.xlabel('time')
plt.ylabel('data')
plt.title('relation')
plt.xticks(rotation=45)
plt.grid(True)
plt.tight_layout()
plt.show()
