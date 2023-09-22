#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;
#include "fuel_monitor.h"
#include <chrono>
#include <thread>

// For static fuel decrease | Real data | PASS TEST. OK
vector<float> fuel_level_vec1 = {
    74.4, 74.4, 74.4, 74.0, 74.0,  // Vehicle restarts check pass; monitored
    74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 73.6, // fisrt alert, send FirstPeriodAbnormalReduction
    73.6, 73.6, 73.6, 73.6, 73.6, 73.6, 73.6, 73.6, 
    73.6, 73.6, 73.6, 73.6, 73.2,  // monitored
    73.2, 73.2, 73.2, 73.2, 72.8, 72.8, 72.8, 72.8, 72.8, // alert in first period but too quickly
    72.8, 72.8, 72.8, 72.8, 72.8, 
    72.8, 72.8, 72.8, 72.8, 72.4,  // monitored
    72.4, 72.4, 72.4, 72.4, 72.4, 72.0 // not enough, see avg is 
};
// 52 data, 5s存一次均值，最后存的是第46～50的均值72.4, 72.4, 72.4, 72.4, 72.4, avg = 72.4

// For restart fuel decrease | Fake data | PASS TEST. OK
vector<float> fuel_level_vec2 = {
    74.4, 74.4, 74.4, 74.0, 74.0,  // Vehicle restarts; 第5s结束报警 RestartAbnormalReduction
    74.0, // here monitor continue
    74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 73.6, 73.6, //  alert in first period but too quickly
    73.6, 73.6, 73.6, 73.6, 73.6, 73.6, 73.6, 
    73.6, 73.6, 73.6, 73.6, 73.2, // monitored. 26
    73.2, 73.2, 73.2, 73.2, 72.8, 72.8, 72.8, 72.8, 72.8, // FirstPeriodAbnormalReduction, 35-5=30 interval ok
    72.8, 72.8, 72.8, 72.8, 72.8, 
    72.8, 72.8, 72.8, 72.8, 72.4, // monitored.
    72.4, 72.4, 72.4, 72.4, 72.4, 72.0
};

// For add oil | Fake data | PASS TEST. OK
vector<float> fuel_level_vec3 = {
    60, 61, 62, 63, 64, 65, 66, 67, 71, 72, 73, 74.0, // add oil, Vehicle restarts check pass;
    74.4, 74.4, 74.4, 74.0, 74.0,  //  monitored 16
    74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 73.6, // fisrt alert, send FirstPeriodAbnormalReduction, 25
    73.6, 73.6, 73.6, 73.6, 73.6, 73.6, 73.6, 73.6, 
    73.6, 73.6, 73.6, 73.6, 73.2,  // monitored
    73.2, 73.2, 73.2, 73.2, 72.8, 72.8, 72.8, 72.8, 72.8, // alert in first period but too quickly, 47
    72.8, 72.8, 72.8, 72.8, 72.8, 
    72.8, 72.8, 72.8, 72.8, 72.4,  // monitored 57
    72.4, 72.4, 72.4, 72.4, 72.4, 72.0 // not enough, see avg
};

/*
场景一案例
a抽油泵（1L/s）抽油20s（预期触发fuel abnormal under 2kmh (0-10s)）
b抽油泵抽油10s（预期触发 fuel abnormal under 2kmh (0-10s)）等到第15秒左右触发
c抽油泵抽油6s（预期不触发）
d抽油泵抽油7s（预期不触发）
e抽油泵抽油7s，停20s，再抽20s（预期触发fuel abnormal under 2kmh (10-45s)）
f抽油泵抽油7s，停20s，再抽10s（预期触发fuel abnormal under 2kmh (10-45s)）
场景二案例
a断电情况下抽油泵抽油20s，再上电（预期触发fuel abnormal during system off）
b断电情况下抽油泵抽油15s，再上电（预期触发fuel abnormal during system off）
c断电情况下抽油泵抽油8s，再上电（预期不触发）
*/
// 以下测试数据是根据真实数据fuel_level_vec1修改而来，即停止抽油的时候会padding一些相同值，可以认为是真实数据

// test ok 0-10s, FirstPeriodAbnormalReduction
vector<float> case1a = {
    74.4, 74.4, 74.4, 74.0, 74.0,  // Vehicle restarts check pass; monitored
    74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 73.6, // fisrt alert, send FirstPeriodAbnormalReduction
    73.6, 73.6, 73.6, 73.6, 73.6, 73.6
};

// test fail, update not in time. 0.4 is too large unit
vector<float> case1b = {
    74.4, 74.4, 74.4, 74.0, 74.0,  // Vehicle restarts check pass; monitored
    74.0, 74.0, 74.0, 74.0, 74.0, // 抽完10秒，之后停止抽油，油量不变
    74.0, 74.0, 74.0, 74.0, 74.0 // padding.....
};

// 根据b的测试, c/d test units 肯定不报警 符合预期
vector<float> case1c = {
    74.4, 74.4, 74.4, 74.0, 74.0, 74.0, // 抽完6s，之后停止抽油，油量不变
    74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0 // padding.....
};

vector<float> case1d = {
    74.4, 74.4, 74.4, 74.0, 74.0, 74.0, 74.0, // 抽完7s，之后停止抽油，油量不变
    74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0 // padding.....
};

// test fail, update not in time. 0.4 is too large unit
vector<float> case1e = {
    74.4, 74.4, 74.4, 74.0, 74.0, // monitored
    74.0, 74.0, //抽7s 
    74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0,
    74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, //停20s
    // 再抽20s
    74.0, 74.0, 74.0, 74.0, 74.0, 
    74.0, 73.6, 73.6, 73.6, 73.6, 
    73.6, 73.6, 73.6, 73.6, 73.6, 
    73.6, 73.6, 73.6, 73.6, 73.2,
    73.2, 73.2, // 这里差了74-73.2=0.8，无法触发， 把这里73.2改成73.0就能正常触发
    73.2, 73.2, 72.8
};

// test fail, update not in time. 0.4 is too large unit
vector<float> case1f = {
    74.4, 74.4, 74.4, 74.0, 74.0, // monitored
    74.0, 74.0, //抽7s 
    74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0,
    74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, 74.0, //停20s
    // 再抽10s
    74.0, 74.0, 74.0, 74.0, 74.0, 
    74.0, 73.6, 73.6, 73.6, 73.6, // 之后停止抽油，油量不变
    73.6, 73.6, 73.6, 73.6, 73.6, // padding
    73.6, 73.6, 73.6, 73.6, 73.6,  // padding
    73.6, 73.6 // padding, 这里差了74-73.6=0.4，无法触发
};

// 这里我们可以设置记录文件中的油量是74.4，熄火之后，抽油20s会抽到73.6，然后上电
vector<float> case2a = {
    // 以下是抽油20s的数据
    // 74.4, 74.4, 74.4, 74.0, 74.0, 
    // 74.0, 74.0, 74.0, 74.0, 74.0, 
    // 74.0, 74.0, 74.0, 73.6, 73.6,
    // 73.6, 73.6, 73.6, 73.6, 73.6
    73.6, 73.6, 73.6, 73.6, 73.6, // 上电后静止，不再抽油，油量维持在73.6,减少量0.8 < 1.5, 不报警，不符合预期
    73.6, 73.6, 73.6, 73.6, 73.6
};

vector<float> case2b = {
    // 以下是抽油10s的数据
    // 74.4, 74.4, 74.4, 74.0, 74.0, 
    // 74.0, 74.0, 74.0, 74.0, 74.0, 
    74.0, 74.0, 74.0, 74.0, 74.0, // 上电后静止，不再抽油，油量维持在74.0,减少量0.4 < 1.5, 不报警，不符合预期
    74.0, 74.0, 74.0, 74.0, 74.0,
};


vector<float> case2c = {
    // 以下是抽油8s的数据
    // 74.4, 74.4, 74.4, 74.0, 74.0, 
    // 74.0, 74.0, 74.0
    74.0, 74.0, 74.0, 74.0, 74.0, // 上电后静止，不再抽油，油量维持在74.0,减少量0.4 < 1.5, 不报警，符合预期
    74.0, 74.0, 74.0, 74.0, 74.0,
};

vector<float> fuel_level_vec = case1a;
int main(){
    FuelMonitor fuel_monitor;
    string label = "";
    // double timestamp = 1000.0;
    for(int i=0; i<fuel_level_vec.size(); i++){
        cout<<"idx = "<<i<<endl;
        bool ret = fuel_monitor.CheckFuelAbnormal(fuel_level_vec[i], 0, label);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // timestamp = timestamp + 1.0;
        if(ret){
            cout<<"alert label = "<<label<<endl;
        }
    }
}