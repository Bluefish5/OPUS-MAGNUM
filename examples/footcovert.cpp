#include <iostream>

using namespace std;

int main(){

    int meters;
    double foots;
    double factor = 0.3;

    cout << "Enter meters to convert to foots: " << endl;

    cin >> meters;

    foots = meters * factor;

    cout << "Meters convert to foots: " << foots << endl;


    return 0;
}