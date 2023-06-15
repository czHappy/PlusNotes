#include <iostream>
#include <unistd.h>
using namespace std;

int main(){
    int a = 1;
    while(1){
        a++;
        cout<<" a = "<<a<<endl;
        sleep(3);
    }
    return 0;
}