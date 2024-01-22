// my_library.h
#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H
#include <iostream>
void hello();
class my_library
{
private:
    /* data */
    int a = 0;
public:
    my_library(/* args */);
    void just(){
        std::cout << "just for func!" << std::endl;
    }
    ~my_library();
};



#endif
