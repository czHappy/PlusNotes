#include <iostream>
#include <sys/inotify.h>
#include <cstring>
#include <unistd.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (32 * (EVENT_SIZE + 16))

int main()
{
    int fd = inotify_init();
    if (fd == -1)
    {
        std::cerr << "Failed to initialize inotify." << std::endl;
        return 1;
    }

    const char *core_dump_dir = "/home/plusai/core_dump"; //
    int wd = inotify_add_watch(fd, core_dump_dir, IN_CREATE);
    if (wd == -1)
    {
        std::cerr << "Failed to add watch for " << core_dump_dir << std::endl;
        return 1;
    }

    char buffer[BUF_LEN];
    while (true)
    {
        int num_bytes = read(fd, buffer, BUF_LEN);
        if (num_bytes <= 0)
        {
            std::cerr << "Failed to read inotify events." << std::endl;
            return 1;
        }

        int i = 0;
        while (i < num_bytes)
        {
            struct inotify_event *event = reinterpret_cast<struct inotify_event *>(&buffer[i]);
            if (event->len)
            {
                if (event->mask & IN_CREATE)
                {
                    // cat /proc/sys/kernel/core_pattern /tmp/core-%e-%p-%t
                    if (std::strncmp(event->name, "core", 4) == 0)
                    { // 如果这个新增文件的文件名前4个字符是core那么这个新增的文件就是core dump文件
                        std::cout << "Core dump file detected: " << event->name << std::endl;
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }
    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}
