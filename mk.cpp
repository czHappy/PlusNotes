#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <iostream>
using namespace std;
struct CrashInfo{
  CrashInfo(){}
  CrashInfo(const std::string& name , double timestamp):
    process_name(name), dump_timestamp(timestamp){}
  std::string process_name;
  double dump_timestamp;
};

class DumpNotifyHandler {
  public:
    void PushCrashInfo(const std::string& process_name , double dump_timestamp){
      std::lock_guard<std::mutex> lock(queue_mutex_);
      shared_ptr<CrashInfo> crash_info = std::make_shared<CrashInfo>(process_name, dump_timestamp);
      crash_info_queue_.push(crash_info);
    }
    shared_ptr<CrashInfo> PopCrashInfo(){
      std::lock_guard<std::mutex> lock(queue_mutex_);
      if(crash_info_queue_.size() == 0) return nullptr;
      shared_ptr<CrashInfo> crash_info = crash_info_queue_.front();
      crash_info_queue_.pop();
      return crash_info;
    }
  private:
    std::queue<shared_ptr<CrashInfo> > crash_info_queue_;
    std::mutex queue_mutex_;
};

int main(){
  DumpNotifyHandler handler_;
  std::string process_name = "hello";
  double dump_timestamp = 123.567;
  handler_.PushCrashInfo(process_name, dump_timestamp);
  handler_.PushCrashInfo(process_name, dump_timestamp);
  while(shared_ptr<CrashInfo> x = handler_.PopCrashInfo()){
    cout<<x->process_name<<" : "<<x->dump_timestamp<<endl;
  }
  return 0;
}