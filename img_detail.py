import cv2

img = cv2.imread("/home/plusai/Downloads/9.jpeg", cv2.IMREAD_UNCHANGED)
# 返回行数，列数，通道数
print(img.shape)   # (515, 425, 3)