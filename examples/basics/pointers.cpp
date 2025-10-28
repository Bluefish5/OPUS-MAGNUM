#include <iostream>
using namespace std;

int main() {
    int var = 10;

    // declare pointer and store address of x
    int* ptr = &var;

    // print value and address
    cout << "Value of x: " << var << endl;
    cout << "Address of x: " << &var << endl;
    cout << "Value stored in pointer ptr: " << ptr << endl;
    cout << "Value pointed to by ptr: " << *ptr << endl;

    return 0;
}