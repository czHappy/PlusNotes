#include <iostream>
#include <fstream> // 包含文件流头文件

int main() {
    // 假设要读取的 YUV 文件名为 "input.yuv"
    const char* fileName = "input.yuv";

    // 打开文件流，以二进制读取（输入）模式打开文件
    std::ifstream inputFile(fileName, std::ios::binary);

    // 检查文件是否成功打开
    if (!inputFile) {
        std::cerr << "无法打开文件！" << std::endl;
        return 1;
    }

    // 获取文件大小
    inputFile.seekg(0, std::ios::end);
    int dataSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    // 为数据缓冲区分配内存
    char* yuvData = new char[dataSize];

    // 使用文件流从 YUV 文件中读取数据到缓冲区
    inputFile.read(yuvData, dataSize);

    // 关闭文件流
    inputFile.close();

    // 现在，yuvData 缓冲区中存储了从 YUV 文件中读取的数据，
    // 可以根据 YUV 数据的排列方式和大小，解析图像数据或进行其他处理。


    std::cout << "从 YUV 文件中读取数据成功。dataSize = " << dataSize << std::endl;

    const char* fileName1 = "test_ouput.yuv";
    std::ofstream outputFile(fileName1, std::ios::binary);
    if (!outputFile) {
        std::cout << "无法打开文件！" << std::endl;
        return false;
    }

    // 使用文件流将内存中的数据写入文件
    outputFile.write(reinterpret_cast<const char*>(yuvData), dataSize);
    outputFile.close();
    std::cout<<"写入成功，关闭文件"<<std::endl;

    return 0;
}
