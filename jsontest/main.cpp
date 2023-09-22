// #include <iostream>
// #include <fstream>
// #include <jsoncpp/json/json.h>


// #include <string>
// class JsonFileManager {
// public:
// // JsonFileManager(const std::string& filename):outputFile(filename),inputFile(filename) {}
//     // 写入 JSON 文件
//     void writeToJsonFile(const std::string& filename, float fuel, double timestamp, double speed) {
//         Json::Value jsonData;
//         jsonData["fuel"] = fuel;
//         jsonData["timestamp"] = timestamp;
//         jsonData["speed"] = speed;

//         Json::StreamWriterBuilder writer;
//         std::ofstream outputFile(filename);
//         if (outputFile.is_open()) {
//             outputFile << Json::writeString(writer, jsonData);
//             // outputFile.close();
//             std::cout << "JSON 数据已写入文件" << std::endl;
//         }
//     }

//     // 从 JSON 文件读取数据
//     void readFromJsonFile(const std::string& filename) {
//         Json::Value jsonData;

//         Json::CharReaderBuilder reader;
//         std::ifstream inputFile(filename);
//         if (inputFile.is_open()) {
//             Json::parseFromStream(reader, inputFile, &jsonData, nullptr);
//             inputFile.close();

//             float fuel = jsonData["fuel"].asFloat();
//             double timestamp = jsonData["timestamp"].asDouble();
//             double speed = jsonData["speed"].asDouble();

//             std::cout << "Fuel: " << fuel << std::endl;
//             std::cout << "Timestamp: " << timestamp << std::endl;
//             std::cout << "Speed: " << speed << std::endl;
//         }
//         else{
//             std::cout<<"file not exists"<<std::endl;
//         }
//     }
// private:
//     Json::StreamWriterBuilder writer;
//     std::ofstream outputFile;
//     Json::CharReaderBuilder reader;
//     std::ifstream inputFile;
// };

 
// int main()
// {

//     // JsonFileManager jsonManager("data.json");
//     JsonFileManager jsonManager;
//     try {
//         // 从 JSON 文件读取数据
//         jsonManager.readFromJsonFile("data.json");
//         // 写入 JSON 文件
//         jsonManager.writeToJsonFile("data.json", 60.5f, 1630172100.0, 100.0);

        

//         jsonManager.writeToJsonFile("data.json", 90.5f, 21630172100.0, 1900.0);
//         jsonManager.readFromJsonFile("data.json");
//     } catch (const std::exception& e) {
//         std::cerr << "发生异常: " << e.what() << std::endl;
//         return 1;
//     }

//     return 0;
// }

#include <iostream>
#include <sys/stat.h>
// #include <sys/types.h>
// #include <cerrno>

bool createDirectory(const std::string &path) {
    auto ret = mkdir(path.c_str(), 0755);
    std::cout<<"ret = "<<ret<<" EEXIST = "<<EEXIST<<" errno = "<<errno<<std::endl;
    if (ret == 0 || errno == EEXIST) {
        return true;
    } else {
        // std::cerr << "无法创建目录 " << path << ": " << strerror(errno) << std::endl;
        return false;
    }
}

int main() {
    std::string record_file_ = "/data/fuel_monitor/fuel_state.json";
    size_t last_slash = record_file_.find_last_of('/'); // /data/fuel_monitor
    if (last_slash != std::string::npos) {
        std::string record_file_dir = record_file_.substr(0, last_slash);
        std::cout<<record_file_dir<<std::endl;
        // if(!CreateDirectory(record_file_dir)){
        //     PLUS_LOG(error, "Create directory %% for fuel monitor failed!", record_file_dir);
        // }
    }
    std::string dirPath = "/home/plusai/workspace/fuel_monitor";

    if (createDirectory(dirPath)) {
        std::cout << "目录已创建或已存在：" << dirPath << std::endl;
    } else {
        // std::cerr << "无法创建目录：" << dirPath << std::endl;
    }

    return 0;
}
