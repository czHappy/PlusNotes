#include <iostream>
#include <fstream>
#include <deque>
#include <unordered_map>
#include <chrono>
#include <jsoncpp/json/json.h>
#include <sys/stat.h>

class FuelMonitor{
  public:
    FuelMonitor() {
        size_t last_slash = record_file_.find_last_of('/'); // /data/fuel_monitor
        if (last_slash != std::string::npos) {
            std::string record_file_dir = record_file_.substr(0, last_slash);
            if(!CreateDirectory(record_file_dir)){
                printf("Create directory %s for fuel monitor failed!", record_file_dir.c_str());
            }
        }
    }
    bool CheckFuelAbnormal(float fuel_level, double speed, std::string& annotation_label, double current_timestamp);
    bool CheckFuelAbnormal(float fuel_level, double speed, std::string& annotation_label);
  private:
    bool CreateDirectory(const std::string &path);
    bool WriteToJsonFile(float fuel_level, double timestamp);
    bool ReadFromJsonFile(float& fuel_level, double& timestamp);
    float CalculateAverageFuel();
    // for fuel monitor
    const unsigned int fuel_monitor_period_ = 5; //seconds, fuel_level update freq 1hz.
    const unsigned int fuel_alert_period_ = 45;
    const unsigned int fuel_first_alert_period_ = 10;
    bool fuel_is_monitored_ = false;  // if the vehicle is being monitored.
    std::deque<float> monitor_fuel_data_; // fuel monitor window, len = 5s
    const double monitor_speed_threshold_ = 2.0; // 2km/h
    std::vector<float> alert_period_fuel_data_; // fuel alert period window, len = 45s [0,10)U[10, 45)
    bool vehicle_running_ = false; // true if speed >= monitor_speed_threshold_
    double fuel_last_alert_timestamp_ = 0; // second
    const double fuel_alert_interval_ = 30; // second
    bool restart_ = true;  // if the vehicle restarts, set in outside init function
    const float restart_fuel_diff_threshold = 1.5;
    const double write_fuel_record_interval = 5.0; // second
    double fuel_last_write_fuel_timestamp_ = 0; // second
    std::string record_file_ = "/home/plusai/workspace/fuel_monitor/fuel_state.json";
    const unsigned int fuel_record_window_ = 5;
    std::deque<float> fuel_record_window_data_;
};
