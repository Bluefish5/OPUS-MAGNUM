#include <iostream>

using namespace std;

struct A{
    char a;
    int b;
};

struct B{
    char a;
    long long int b;
};

struct C{
    char a;
    char b;
};

int main(){

    char a;
    int b;

    cout <<"Size of struc A: "<<sizeof(A)<<endl;

    cout <<"Size of struc B: "<<sizeof(B)<<endl;

    cout <<"Size of struc C: "<<sizeof(C)<<endl;



    return 0;

}