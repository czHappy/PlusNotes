from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import time
class FileCreationHandler(FileSystemEventHandler):
    def on_created(self, event):
        if event.is_directory:
            return
        print(f"Created file: {event.src_path}")

if __name__ == "__main__":
    folder_to_monitor = "/home/plusai"  # Replace this with the folder path you want to monitor
    event_handler = FileCreationHandler()
    observer = Observer()
    observer.schedule(event_handler, path=folder_to_monitor, recursive=False)
    observer.start()

    try:
        while True:
            time.sleep(100)
            pass
    except KeyboardInterrupt:
        observer.stop()

    observer.join()
                  


import json

# 示例字典变量
dict1 = {"name": "John", "age": 30, "city": "New York"}
dict2 = {"name": "Alice", "age": 25, "city": "San Francisco"}
dict3 = {"name": "Bob", "age": 35, "city": "Chicago"}

# 存入JSON文件
data = {
    "dict1": dict1,
    "dict2": dict2,
    "dict3": dict3
}

with open("data.json", "w") as json_file:
    json.dump(data, json_file)

print("字典变量已存入JSON文件。")

# 从JSON文件中读取数据
read_data = {}
with open("data.json", "r") as json_file:
    read_data = json.load(json_file)

# 输出读取后的字典变量
print("读取后的字典变量:")
dict1_read = read_data["dict1"]
dict2_read = read_data["dict2"]
dict3_read = read_data["dict3"]

print("dict1:", dict1_read)
print("dict2:", dict2_read)
print("dict3:", dict3_read)
