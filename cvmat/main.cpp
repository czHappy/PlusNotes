#include <opencv2/opencv.hpp>

int main() {
    // 读取 JPEG 图片
    cv::Mat image = cv::imread("myself.jpeg");

    if (image.empty()) {
        std::cerr << "Could not read the image." << std::endl;
        return 1;
    }
    // 获取图像数据的内存指针
    uchar* data_ptr = image.data;
    printf("data_ptr = %p\n", data_ptr);
    // 获取图像的行数（高度）和列数（宽度）
    int height = image.rows;
    int width = image.cols;

    // 获取图像的总像素数
    int total_pixels = height * width;

    // 获取图像的实际占用字节数
    size_t image_size = image.total() * image.elemSize();

    // 打印图像信息
    std::cout << "Image Height: " << height << " pixels" << std::endl;
    std::cout << "Image Width: " << width << " pixels" << std::endl;
    std::cout << "Total Pixels: " << total_pixels << " pixels" << std::endl;
    std::cout << "Image image.total(): " << image.total() << " elemSize=" << image.elemSize()<<std::endl;
    std::cout << "Image Size: " << image_size << " bytes" << std::endl;

    return 0;
}
