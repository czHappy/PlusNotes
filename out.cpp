#include <iostream>
 
using namespace std;
 
int main( )
{
   char str[] = "Hello cz";
 
   cerr << "stderr message : " << str << endl;
   cout << "stdout message: " << str << endl;
   return 0;
}

// ./out 2 > error.log