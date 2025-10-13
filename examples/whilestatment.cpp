#include <iostream>

using namespace std;

int main(){

    int i = 0, starsCount;

    cout<<"How much start to print?"<<endl;

    cin>>starsCount;

    while(i<starsCount){
        cout<<"*";
        i++;
    }

    cout<<"Starts to printed:"<<starsCount <<endl;
    return 0;

}