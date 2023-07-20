import cv2
import numpy as np

def YUV2JPG(inputFileName, savepath):
    iWidth, iHeight = 320, 160 # 540->544 16 multiple
    iImageSize = iWidth * iHeight * 3 // 2

    fpln = open(inputFileName, "rb+")
    if fpln is None:
        print("Read YUV error.")
        return

    yuvImg = np.zeros((iHeight * 3 // 2, iWidth), dtype=np.uint8)
    rgbImg = np.zeros((iHeight, iWidth, 3), dtype=np.uint8)
    # https://docs.opencv.org/3.4/d8/d01/group__imgproc__color__conversions.html#gga4e0972be5de079fed4e3a10e24ef5ef0ab09d8186a9e5aaac83acd157a1be43b0
    pYUVbuf = np.fromfile(fpln, dtype=np.uint8, count=iImageSize)
    yuvImg.flat = pYUVbuf[:iImageSize]

    yuvImg = cv2.cvtColor(yuvImg, cv2.COLOR_YUV2RGB_NV12)
    rgbImg[:, :, 0:3] = yuvImg[:, :, 0:3]

    cv2.imshow("test", rgbImg)
    cv2.waitKey(1000)

    cv2.imwrite(savepath, rgbImg)

# 示例用法
inputFileName = "outputfile"
savepath = "output.jpg"
YUV2JPG(inputFileName, savepath)
