import csv

# 打开 CSV 文件
with open('/home/plusai/Documents/PlusNotes/PlusNotes/postgres_test/data-1.csv') as csvfile:
    # 创建带有字典读取器
    csv_reader = csv.DictReader(csvfile, delimiter='|')
    lines = 0
    # 读取每一行数据
    for row in csv_reader:
        print(row)
        lines = lines + 1
    print("lines - ",lines)
