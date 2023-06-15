#include <unordered_set>
#include <iostream>
#include <mutex>
#include <memory>
#include <cmath>
#include "process.h"
using namespace std;
struct CrashRecordHash {
    std::size_t operator()(const std::shared_ptr<CrashRecord> crash_record) const {
        std::size_t name_hash = std::hash<std::string>{}(crash_record->get_name());
        std::size_t time_hash = std::hash<double>{}(crash_record->get_time());
        return name_hash ^ time_hash;
    }
};
std::unordered_set<std::shared_ptr<CrashRecord>, CrashRecordHash> crash_record_queue_;
std::mutex unordered_set_mutex_;
void PushCrashRecord(const std::string& process_name , double dump_timestamp){
    std::lock_guard<std::mutex> lock(unordered_set_mutex_);
    std::shared_ptr<CrashRecord> crash_record = std::make_shared<CrashRecord>();
    crash_record->set_name(process_name);
    crash_record->set_time(dump_timestamp);
    crash_record_queue_.insert(crash_record);
}
std::shared_ptr<CrashRecord> PopCrashRecord(){
    std::lock_guard<std::mutex> lock(unordered_set_mutex_);
    if(crash_record_queue_.size() == 0) return nullptr;
    std::shared_ptr<CrashRecord> crash_record = *crash_record_queue_.begin();
    crash_record_queue_.erase(crash_record_queue_.begin());
    return crash_record;
}

bool operator==(const std::shared_ptr<CrashRecord> A, std::shared_ptr<CrashRecord> B) {
    return (A->get_name() == B->get_name() && fabs(A->get_time() - B->get_time()) < 1e-3 );
}

// bool operator==(const CrashRecord& A, const CrashRecord& B) {
//     return (A.get_name() == B.get_name() && fabs(A.get_time() - B.get_time()) < 1e-3 );
// }


int main(){
    PushCrashRecord("p1", 1.0);
    PushCrashRecord("p2", 2.0);
    PushCrashRecord("p3", 3.0);
    PushCrashRecord("p4", 4.0);
    PushCrashRecord("p4", 4.0);
    PushCrashRecord("p4", 4.0);

    while (std::shared_ptr<CrashRecord> crash_record = PopCrashRecord()) {
        cout<<crash_record->get_name()<<" "<<crash_record->get_time()<<endl;
    }
    return 0;
}