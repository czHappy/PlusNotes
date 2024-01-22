#include <iostream>
#include <ctime>
#include <sys/sysinfo.h>

int main() {
    struct sysinfo info;

    if (sysinfo(&info) == 0) {
        time_t bootTime = time(NULL) - info.uptime;
        std::cout<<"timestamp = "<<static_cast<double>(bootTime)<<std::endl;
        std::cout << "System boot time: " << std::ctime(&bootTime);
    } else {
        std::cerr << "Failed to retrieve system information." << std::endl;
    }
    uint64_t a = 10;
    uint64_t b = 99;
    // uint64_t c = abs(a-b);
    std::cout<< abs(a-b)<<std::endl;
    return 0;
}
