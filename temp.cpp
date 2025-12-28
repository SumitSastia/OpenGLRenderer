#include <iostream>
#include <string>

using namespace std;

int main(){
    
    const char* str1 = "hello";
    const char* str2 = "world";

    string s = "hello";

    cout << (s+"asdas").c_str() << endl;

    return 0;
}