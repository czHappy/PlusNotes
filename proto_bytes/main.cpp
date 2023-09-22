#include <iostream>
#include "your_message.pb.h"

int main() {
    // 创建一个 SerializedMessage 消息对象，并填充 serialized_data 字段
    SerializedMessage message;
    message.mutable_serialized_data()->assign("hello, cz");

    // 将 SerializedMessage 对象序列化为字节序列
    std::string serializedData = message.SerializeAsString();

    // 反序列化字节序列为 SerializedMessage 对象
    SerializedMessage deserializedMessage;
    if (deserializedMessage.ParseFromString(serializedData)) {
        // 获取反序列化后的字符串
        const std::string& deserializedString = deserializedMessage.serialized_data();

        // 打印反序列化后的字符串
        std::cout << "Deserialized String: " << deserializedString << std::endl;
    } else {
        std::cerr << "Failed to deserialize message." << std::endl;
    }

    return 0;
}
