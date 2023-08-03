import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime
import pytz
import time
import random

with open('data.txt', 'a') as f:
    for i in range(1, 20):
        data = random.randint(1, 100)
        row = str(data) + ","+str(time.time()+i)+"\n"
        f.write(row)
