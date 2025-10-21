#include <iostream>

using namespace std;


void foo(const char*){
    cout<<"Const char here"<<endl;
}

void foo(short){
    cout<<"Short here"<<endl;
}

int main(){

    foo(nullptr);

    return 0;

}