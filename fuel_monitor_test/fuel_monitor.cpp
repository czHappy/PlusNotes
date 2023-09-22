#include "fuel_monitor.h"

bool FuelMonitor::CheckFuelAbnormal(float fuel_level, double speed, std::string& annotation_label, double current_timestamp) {
        // record latest fuel data.
        fuel_record_window_data_.push_back(fuel_level);
        if(fuel_record_window_data_.size() > fuel_record_window_){
            fuel_record_window_data_.pop_front();
        }
        // check fuel when restart
        if(restart_ && fuel_record_window_data_.size() == fuel_record_window_) {
            float last_start_fuel_level = 0;
            double last_fuel_level_timestamp = 0;
            restart_ = false;
            if(!ReadFromJsonFile(last_start_fuel_level, last_fuel_level_timestamp)){
                printf("can't open fuel record file or fuel record file not exists.\n");
            }
            float current_avg_fuel = CalculateAverageFuel();
            if(last_start_fuel_level - current_avg_fuel > restart_fuel_diff_threshold && 
                last_fuel_level_timestamp < current_timestamp){
                annotation_label = "RestartAbnormalReduction";
                fuel_last_alert_timestamp_ = current_timestamp;
                printf("Vehicle restarts alert, the fuel is abnormal. Current fuel is %f, last fuel is %f\n", fuel_level, last_start_fuel_level);
                return true;
            }
            printf("Vehicle restarts, the fuel is normal. Current avg fuel is %f, last avg fuel is %f\n", current_avg_fuel, last_start_fuel_level);
        }
        // write fuel data to json, freq 1/write_fuel_record_interval hz
        if(current_timestamp - fuel_last_write_fuel_timestamp_ >= write_fuel_record_interval
            && fuel_record_window_data_.size() == fuel_record_window_){
            float avg_fuel_level = CalculateAverageFuel();
            printf("write fuel data to json, avg = %f\n", avg_fuel_level);
            WriteToJsonFile(avg_fuel_level, current_timestamp);
            fuel_last_write_fuel_timestamp_ = current_timestamp;
        }
        float fuel_monitor_threshold = 0.35;  // default monitor triger fuel threshold, speed < 2km/h
        float fuel_first_alert_period_threshold = 0.3;  // default first alert period fuel threshold, speed < 2km/h
        float fuel_second_alert_period_threshold = 1.0;  // default second alert period fuel threshold, speed < 2km/h
        float fuel_diff = 0;
        if(!fuel_is_monitored_) {  // if not being monitored
            monitor_fuel_data_.push_back(fuel_level);
            if(monitor_fuel_data_.size() > fuel_monitor_period_) {  // fuel_level record for monitor period.
                monitor_fuel_data_.pop_front();
            }
            // check speed in monitor window.
            //all data must less than monitor_speed_threshold_ or greater equal to monitor_speed_threshold_
            bool speed_invalid = (vehicle_running_ && speed < monitor_speed_threshold_) || 
                                 (!vehicle_running_ && speed >= monitor_speed_threshold_);
            if(speed_invalid) {
                monitor_fuel_data_.clear();  // reset fuel window
                fuel_is_monitored_ = false;  // reset monitor status
                vehicle_running_ = speed > monitor_speed_threshold_; // update vehicle_running_ when reset status
                printf("speed is not consistent in the monitor period. speed = %f.\n", speed);
                return false;
            }
            // check fuel, we won't reset status if fuel is invalid because fuel may be a little unstable
            fuel_diff = monitor_fuel_data_.front() - monitor_fuel_data_.back();
            if(fuel_diff < 0) {
                printf("fuel diff < 0, %f.\n", fuel_level);
                return false;  //just return false, wait next window.
            }
            // check enough fuel_level data 
            if(monitor_fuel_data_.size() < fuel_monitor_period_) {
                return false;
            }
            // decide whether the vehicle should be monitored.
            // thresholds for speed > 2km/h
            if(vehicle_running_) {
                fuel_monitor_threshold = 1.8;
                fuel_first_alert_period_threshold = 2;
                fuel_second_alert_period_threshold = 5;
            }
            if(fuel_diff >= fuel_monitor_threshold) {
                fuel_is_monitored_ = true;
                printf("fuel is being monitored. speed = %f\n", speed);
            }
        }
        
        // deal vehicle if it was monitored
        if(fuel_is_monitored_){
            alert_period_fuel_data_.push_back(fuel_level);
            bool consistent = true;
            // check speed in alert period, speed must be consistent with the monitor period.
            // fuel_monitor_threshold > 0.5 means monitor window speed >= 2
            if(fuel_monitor_threshold > 0.5 && speed < monitor_speed_threshold_) { 
                consistent = false;
            }
            if(fuel_monitor_threshold < 0.5 && speed >= monitor_speed_threshold_) {
                consistent = false;
            }
            if(!consistent){
                fuel_is_monitored_ = false;
                monitor_fuel_data_.clear();
                alert_period_fuel_data_.clear();
                vehicle_running_ = speed > monitor_speed_threshold_;
                printf("speed is not consistent in the alert period. speed = %f\n", speed);
            }
            bool need_alert = false;
            // look [0,10)
            if(alert_period_fuel_data_.size() == fuel_first_alert_period_) {
                if(alert_period_fuel_data_[0] - alert_period_fuel_data_.back() >= fuel_first_alert_period_threshold) {
                    need_alert = true;
                    annotation_label = "FirstPeriodAbnormalReduction";
                    printf("fuel is going to alert in first period. speed = %f\n", speed);
                }
            }
            // look [10,45)
            if(alert_period_fuel_data_.size() == fuel_alert_period_) {
                if(alert_period_fuel_data_[fuel_first_alert_period_] - alert_period_fuel_data_.back() >= fuel_second_alert_period_threshold) {
                    need_alert = true;
                    annotation_label = "SecondPeriodAbnormalReduction";
                    printf("fuel is going to alert in second period. speed = %f\n", speed);
                }
            }
            // if no alert in 45s, reset status
            if(alert_period_fuel_data_.size() == fuel_alert_period_ && !need_alert) {
                fuel_is_monitored_ = false;
                monitor_fuel_data_.clear();
                alert_period_fuel_data_.clear();
                vehicle_running_ = speed > monitor_speed_threshold_;
                printf("no alert in alert period. speed = %f\n", speed);
            }
            if(need_alert){
                if(current_timestamp - fuel_last_alert_timestamp_ < fuel_alert_interval_) {
                    need_alert = false;
                    printf("fuel is going to alert, but alert too quickly. speed = %f, current_timestamp = %lf, fuel_last_alert_timestamp_ = %lf\n", speed, current_timestamp, fuel_last_alert_timestamp_);
                }
                else {
                    fuel_last_alert_timestamp_ = current_timestamp;
                    printf("fuel abnormal!, speed = %f, label = %s\n", speed, annotation_label.c_str());
                }
                fuel_is_monitored_ = false;
                monitor_fuel_data_.clear();
                alert_period_fuel_data_.clear(); 
                vehicle_running_ = speed > monitor_speed_threshold_;
            }
            return need_alert;
        }
        return false;
    }

bool FuelMonitor::CheckFuelAbnormal(float fuel_level, double speed, std::string& annotation_label) {
    // get current timestamp
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
        double current_timestamp = duration.count();
        // record latest fuel data.
        fuel_record_window_data_.push_back(fuel_level);
        if(fuel_record_window_data_.size() > fuel_record_window_){
            fuel_record_window_data_.pop_front();
        }
        // check fuel when restart
        if(restart_ && fuel_record_window_data_.size() == fuel_record_window_) {
            float last_start_fuel_level = 0;
            double last_fuel_level_timestamp = 0;
            restart_ = false;
            if(!ReadFromJsonFile(last_start_fuel_level, last_fuel_level_timestamp)){
                printf("can't open fuel record file or fuel record file not exists.\n");
            }
            float current_avg_fuel = CalculateAverageFuel();
            if(last_start_fuel_level - current_avg_fuel > restart_fuel_diff_threshold && 
                last_fuel_level_timestamp < current_timestamp){
                annotation_label = "RestartAbnormalReduction";
                fuel_last_alert_timestamp_ = current_timestamp;
                printf("Vehicle restarts alert, the fuel is abnormal. Current fuel is %f, last fuel is %f\n", fuel_level, last_start_fuel_level);
                return true;
            }
            printf("Vehicle restarts, the fuel is normal. Current avg fuel is %f, last avg fuel is %f\n", current_avg_fuel, last_start_fuel_level);
        }
        // write fuel data to json, freq 1/write_fuel_record_interval hz
        if(current_timestamp - fuel_last_write_fuel_timestamp_ >= write_fuel_record_interval
            && fuel_record_window_data_.size() == fuel_record_window_){
            float avg_fuel_level = CalculateAverageFuel();
            printf("write fuel data to json, avg = %f\n", avg_fuel_level);
            WriteToJsonFile(avg_fuel_level, current_timestamp);
            fuel_last_write_fuel_timestamp_ = current_timestamp;
        }
        float fuel_monitor_threshold = 0.35;  // default monitor triger fuel threshold, speed < 2km/h
        float fuel_first_alert_period_threshold = 0.3;  // default first alert period fuel threshold, speed < 2km/h
        float fuel_second_alert_period_threshold = 1.0;  // default second alert period fuel threshold, speed < 2km/h
        float fuel_diff = 0;
        if(!fuel_is_monitored_) {  // if not being monitored
            monitor_fuel_data_.push_back(fuel_level);
            if(monitor_fuel_data_.size() > fuel_monitor_period_) {  // fuel_level record for monitor period.
                monitor_fuel_data_.pop_front();
            }
            // check speed in monitor window.
            //all data must less than monitor_speed_threshold_ or greater equal to monitor_speed_threshold_
            bool speed_invalid = (vehicle_running_ && speed < monitor_speed_threshold_) || 
                                 (!vehicle_running_ && speed >= monitor_speed_threshold_);
            if(speed_invalid) {
                monitor_fuel_data_.clear();  // reset fuel window
                fuel_is_monitored_ = false;  // reset monitor status
                vehicle_running_ = speed > monitor_speed_threshold_; // update vehicle_running_ when reset status
                printf("speed is not consistent in the monitor period. speed = %f.\n", speed);
                return false;
            }
            // check fuel, we won't reset status if fuel is invalid because fuel may be a little unstable
            fuel_diff = monitor_fuel_data_.front() - monitor_fuel_data_.back();
            if(fuel_diff < 0) {
                printf("fuel diff < 0, %f.\n", fuel_level);
                return false;  //just return false, wait next window.
            }
            // check enough fuel_level data 
            if(monitor_fuel_data_.size() < fuel_monitor_period_) {
                return false;
            }
            // decide whether the vehicle should be monitored.
            // thresholds for speed > 2km/h
            if(vehicle_running_) {
                fuel_monitor_threshold = 1.8;
                fuel_first_alert_period_threshold = 2;
                fuel_second_alert_period_threshold = 5;
            }
            if(fuel_diff >= fuel_monitor_threshold) {
                fuel_is_monitored_ = true;
                printf("fuel is being monitored. speed = %f\n", speed);
            }
        }
        
        // deal vehicle if it was monitored
        if(fuel_is_monitored_){
            alert_period_fuel_data_.push_back(fuel_level);
            bool consistent = true;
            // check speed in alert period, speed must be consistent with the monitor period.
            // fuel_monitor_threshold > 0.5 means monitor window speed >= 2
            if(fuel_monitor_threshold > 0.5 && speed < monitor_speed_threshold_) { 
                consistent = false;
            }
            if(fuel_monitor_threshold < 0.5 && speed >= monitor_speed_threshold_) {
                consistent = false;
            }
            if(!consistent){
                fuel_is_monitored_ = false;
                monitor_fuel_data_.clear();
                alert_period_fuel_data_.clear();
                vehicle_running_ = speed > monitor_speed_threshold_;
                printf("speed is not consistent in the alert period. speed = %f\n", speed);
            }
            bool need_alert = false;
            // look [0,10)
            if(alert_period_fuel_data_.size() == fuel_first_alert_period_) {
                if(alert_period_fuel_data_[0] - alert_period_fuel_data_.back() >= fuel_first_alert_period_threshold) {
                    need_alert = true;
                    annotation_label = "FirstPeriodAbnormalReduction";
                    printf("fuel is going to alert in first period. speed = %f\n", speed);
                }
            }
            // look [10,45)
            if(alert_period_fuel_data_.size() == fuel_alert_period_) {
                if(alert_period_fuel_data_[fuel_first_alert_period_] - alert_period_fuel_data_.back() >= fuel_second_alert_period_threshold) {
                    need_alert = true;
                    annotation_label = "SecondPeriodAbnormalReduction";
                    printf("fuel is going to alert in second period. speed = %f\n", speed);
                }
            }
            // if no alert in 45s, reset status
            if(alert_period_fuel_data_.size() == fuel_alert_period_ && !need_alert) {
                fuel_is_monitored_ = false;
                monitor_fuel_data_.clear();
                alert_period_fuel_data_.clear();
                vehicle_running_ = speed > monitor_speed_threshold_;
                printf("no alert in alert period. speed = %f\n", speed);
            }
            if(need_alert){
                if(current_timestamp - fuel_last_alert_timestamp_ < fuel_alert_interval_) {
                    need_alert = false;
                    printf("fuel is going to alert, but alert too quickly. speed = %f, current_timestamp = %lf, fuel_last_alert_timestamp_ = %lf\n", speed, current_timestamp, fuel_last_alert_timestamp_);
                }
                else {
                    fuel_last_alert_timestamp_ = current_timestamp;
                    printf("fuel abnormal!, speed = %f, label = %s\n", speed, annotation_label.c_str());
                }
                fuel_is_monitored_ = false;
                monitor_fuel_data_.clear();
                alert_period_fuel_data_.clear(); 
                vehicle_running_ = speed > monitor_speed_threshold_;
            }
            return need_alert;
        }
        return false;
    }

bool FuelMonitor::CreateDirectory(const std::string &path) {
    auto ret = mkdir(path.c_str(), 0755);
    if (ret == 0 || errno == EEXIST) {
        return true;
    }
    return false;
}

bool FuelMonitor::WriteToJsonFile(float fuel_level, double saved_timestamp) {
    Json::Value json_data;
    json_data["fuel_level"] = fuel_level;
    json_data["saved_timestamp"] = saved_timestamp;
    Json::StreamWriterBuilder writer;
    std::ofstream output_file(record_file_);
    if (output_file.is_open()) {
        output_file << Json::writeString(writer, json_data);
        return true;
    }
    return false;
}

bool FuelMonitor::ReadFromJsonFile(float& fuel_level, double& saved_timestamp) {
    Json::Value json_data;
    Json::CharReaderBuilder reader;
    std::ifstream input_file(record_file_);
    if (input_file.is_open()) {
        Json::parseFromStream(reader, input_file, &json_data, nullptr);
        input_file.close();
        fuel_level = json_data["fuel_level"].asFloat();
        saved_timestamp = json_data["saved_timestamp"].asDouble();
        return true;
    }
    return false;
}

float FuelMonitor::CalculateAverageFuel() {
    if (fuel_record_window_data_.empty()) {
        return 0.0;
    }
    float sum = 0.0;
    for (const float& value : fuel_record_window_data_) {
        sum += value;
    }
    return sum / fuel_record_window_data_.size();
}

