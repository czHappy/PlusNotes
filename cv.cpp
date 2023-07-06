#include <mutex>
#include <deque>
#include <iostream>
#include <thread>
#include <condition_variable>

class PCModle
{
public:
    PCModle() : work_(true), max_num(30), next_index(0)
    {
    }

    void producer_thread()
    {
        while (work_)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            // 加锁
            std::unique_lock<std::mutex> lk(cvMutex);
            // 当队列未满时，继续添加数据
            cv.wait(lk, [this]()
                    { return this->data_deque.size() <= this->max_num; }); // 避免虚假唤醒

            next_index++;
            data_deque.push_back(next_index);
            std::cout << "producer " << next_index << ", queue size: " << data_deque.size() << std::endl;
            // 唤醒其他线程
            cv.notify_all();
            // 自动释放锁
        }
    }

    void consumer_thread()
    {
        while (work_)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            // 加锁
            std::unique_lock<std::mutex> lk(cvMutex);
            // 检测条件是否达成
            cv.wait(lk, [this]
                    { return !this->data_deque.empty(); });

            // 互斥操作，消息数据
            int data = data_deque.front();
            data_deque.pop_front();
            std::cout << "consumer " << data << ", deque size: " << data_deque.size() << std::endl;
            // 唤醒其他线程
            cv.notify_all();
            // 自动释放锁
        }
    }

private:
    bool work_;

    std::mutex cvMutex;
    std::condition_variable cv;

    // 缓存区
    std::deque<int> data_deque;
    // 缓存区最大数目
    size_t max_num;
    // 数据
    int next_index;
};

int main()
{
    PCModle obj;

    std::thread ProducerThread = std::thread(&PCModle::producer_thread, &obj);
    std::thread ConsumerThread = std::thread(&PCModle::consumer_thread, &obj);

    ProducerThread.join();
    ConsumerThread.join();

    return 0;
}