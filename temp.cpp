#include <iostream>
using namespace std;

int main(){
    
    int arr1[2] = {1,2};
    int arr2[2] = {3,4};

    int** arrList;
    arrList[0] = arr1;
    arrList[1] = arr2;

    cout << arrList[0][0] << endl;

    return 0;
}