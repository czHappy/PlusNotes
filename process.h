#include <string>
using namespace std;

class CrashRecord{
public:
    CrashRecord(string n, double t):name(n), time(t){}
    CrashRecord(){}
    void set_name(string n){
        name = n;
    }
    void set_time(double n){
        time = n;
    }
    string get_name() const {
        return name;
    }
    double get_time() const{
        return time;
    }
private:
    string name;
    double time;
};

class objx{
public:
    objx(){}
    objx(int x, int y):a(x),b(y){}
    int a;
    int b;
};