// #include <iostream>
// #include <dirent.h>
// #include <sys/stat.h>
// #include <string>

// void TraverseDirectory(const std::string& directoryPath) {
//     DIR* dir = opendir(directoryPath.c_str());
//     if (dir == nullptr) {
//         std::cout << "Failed to open directory: " << directoryPath << std::endl;
//         return;
//     }

//     dirent* entry;
//     while ((entry = readdir(dir)) != nullptr) {
//         std::string fileName = entry->d_name;
//         std::string filePath = directoryPath + "/" + fileName;
//         struct stat fileInfo;
//         if (stat(filePath.c_str(), &fileInfo) == -1) {
//             std::cout << "Failed to get file info for: " << filePath << std::endl;
//             continue;
//         }

//         if (S_ISREG(fileInfo.st_mode)) {
//             std::cout << "File: " << filePath <<" File Name: "<<fileName<< std::endl;
//             fileName = "core-trt_tool-8043-1686105745";
//             size_t pos1 = fileName.rfind('-');
//             if (pos1 != std::string::npos) {
//             size_t pos2 = fileName.rfind('-', pos1 - 1);
//             if (pos2 != std::string::npos) {
//                 std::string field3 = fileName.substr(pos2 + 1, pos1 - pos2 - 1);
//                 std::string field4 = fileName.substr(pos1 + 1);
//                 std::cout<<field3<<" "<<field4<<std::endl;
//             }
//     }
//         }
//     }

//     closedir(dir);
// }

// int main() {
//     std::string directoryPath = "/home/plusai/Documents/PlusNotes/PlusNotes/imgs";
//     TraverseDirectory(directoryPath);
//     return 0;
// }


#include <iostream>


void extractFields(const std::string& str, char delimiter, std::string& field1, std::string& field2, std::string& field3, std::string& field4) {
    size_t pos1 = str.find(delimiter);
    size_t pos2 = str.find(delimiter, pos1 + 1);
    size_t pos3 = str.find(delimiter, pos2 + 1);
    std::cout<<pos1<<" "<<pos2<<" "<<pos3<<std::endl;
    field1 = str.substr(0, pos1);
    field2 = str.substr(pos1+1, pos2 - pos1 - 1);
    field3 = str.substr(pos2+1, pos3 - pos2 - 1);
    field4 = str.substr(pos3+1);
            
        

}

int main() {
    std::string str = "111-2222-33333-444444";
    char delimiter = '-';
    std::string field1, field2, field3, field4;

    extractFields(str, delimiter, field1, field2, field3, field4);

    // Print the extracted fields
    std::cout << "Field 1: " << field1 << std::endl;
    std::cout << "Field 2: " << field2 << std::endl;
    std::cout << "Field 3: " << field3 << std::endl;
    std::cout << "Field 4: " << field4 << std::endl;

    return 0;
}

