#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct base {

    int x;

    virtual void render() {}
};

struct derived : public base {

    int y;

    void render() override {
        cout << x  << y << endl;
    }

    void hehe() {
        cout << "hehe";
    }
};

int main(){
    
    derived* b = new derived();
    b->x = 24;
    b->y = 36;

    base* mainframe = b;
    
    mainframe->render();
    mainframe->hehe();

    return 0;
}
