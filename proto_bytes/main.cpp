#include <iostream>
#include <cstdint>
#include "your_message.pb.h"

int main() {
    // 假设你有一个 YourMessage 消息对象
    YourMessage message;

    // 获取 byte_field 字段
    const std::string& byteField = message.byte_field();

    // 获取 byte_field 字段的首地址
    const uint8_t* firstAddress = reinterpret_cast<const uint8_t*>(byteField.data());

    // 获取 byte_field 字段的大小（字节数）
    size_t size = byteField.size();

    std::cout << "First Address: " << static_cast<const void*>(firstAddress) << std::endl;
    std::cout << "Size: " << size << " bytes" << std::endl;

    return 0;
}
