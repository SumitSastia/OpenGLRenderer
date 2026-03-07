#include <iostream>
#include <string>
#include <vector>

using namespace std;

class c1 {
    int a;
public:
    int b;
};

class c2 : public c1 {
    int c;
};

int main(){
    
    int && a = 24;
    cout << a << endl;

    return 0;
}
