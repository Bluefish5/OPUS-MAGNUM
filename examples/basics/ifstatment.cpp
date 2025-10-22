#include <iostream>

using namespace std;

int main(){

    int height,points;
    cout<< " Enter height: [in centimeters] "<<endl;

    cin >>height;

    if(height < 180)
    {
        cout<<"Your height is to small!"<<endl;
    }
    else
    {
        cout<<"Your height is to okey!"<<endl;
    }

    return 0;

}