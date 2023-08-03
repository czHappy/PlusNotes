from collections import deque

# 创建一个空的双端队列
my_deque = deque()

# 在队列的右侧添加元素
my_deque.append(1)
my_deque.append(2)
my_deque.append(3)

# 在队列的左侧添加元素
my_deque.appendleft(0)

print("双端队列:", my_deque)  # 输出: 双端队列: deque([0, 1, 2, 3])

# 从队列右侧移除一个元素
my_deque.popleft()
print("移除右侧元素后:", my_deque)  # 输出: 移除右侧元素后: deque([0, 1, 2])

# 从队列左侧移除一个元素
my_deque.append(99)
print("移除左侧元素后:", my_deque)  # 输出: 移除左侧元素后: deque([1, 2])


print(my_deque[0])
print(my_deque[1])
if (len(my_deque) > 9 and len(my_deque) < 5) and my_deque[0] > (
    4 * my_deque[1]):
    print("OK")
else:
    print("HH")