#include <iostream>
#include <cctype>
using namespace std;

int main() {
    char ch ;
    cin>>ch;

    if (isdigit(ch) || ch == '_')
        cout << "It is a number";
    else
        cout << "Not a number";
}