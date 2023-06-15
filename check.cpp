#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <unordered_set>



void check_coredump(std::string folderPath){
    std::unordered_set<std::string> dump_process;
    while(true){
        try {
            if (!std::filesystem::exists(folderPath)) {
                continue;;
            }
            for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
                if (entry.is_regular_file() && dump_process.count(entry.path().filename()) == 0) {
                    std::cout << entry.path().filename() << std::endl;
                    dump_process.insert(entry.path().filename());
                }
            }
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }
        sleep(3);
    }
}

int main() {
    std::string folderPath = "/tmp/coredump/";  // 替换为实际文件夹的路径
    check_coredump(folderPath);
    

    return 0;
}
