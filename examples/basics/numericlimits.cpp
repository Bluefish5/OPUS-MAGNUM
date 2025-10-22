#include <iostream>
#include <limits>
#include <iomanip>
#include <string>
using namespace std;

template <typename T>
void info_of_type(string opis)
{
    cout << setw(18)
    << opis <<":"
    <<setw(2) << sizeof(T)
    <<"|"
    <<setw(2)
    <<numeric_limits<T>::digits
    <<(numeric_limits<T>::is_signed ? "|sign|":"|withoutsign|")
    <<"["<<numeric_limits<T>::min()
    <<","<<numeric_limits<T>::max()
    <<"]"<<endl;
}

int main()
{
    cout<<"Printing info about types:"<<endl<<endl;

    info_of_type<short int>("short int");
    info_of_type<signed short int>("signed short int");
    info_of_type<unsigned short int>("unsigned short int");

    cout<<endl;

    info_of_type<int>("int");
    info_of_type<signed int>("signed int");
    info_of_type<unsigned int>("unsigned int");

    cout<<endl;

    info_of_type<long int>("long int");
    info_of_type<signed long int>("signed long int");
    info_of_type<unsigned long int>("unsigned long int");

    cout<<endl;

    info_of_type<char>("char");
    info_of_type<float>("float");
    info_of_type<double>("double");

    cout<<endl;

}
