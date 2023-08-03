from multiprocessing import Process, Queue

def receiver_process(queue):
    while True:
        item = queue.get()  # 从队列中获取数据，此处会阻塞等待
        if item is None:
            break  # 收到结束标志，退出循环
        print("Received:", item)

if __name__ == "__main__":
    # 创建一个消息队列
    queue = Queue()

    # 创建进程B，用于接收数据
    receiver = Process(target=receiver_process, args=(queue,))

    # 启动进程B
    receiver.start()

    # 等待进程B结束
    receiver.join()

    print("Process B has finished.")
