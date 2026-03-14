#include<iostream>
#include "lexer.h"
using namespace std;
int main() {
    cout<<"compiler has started!"<<endl;
    cout<<"give some expression!"<<endl;
    lexer();
    for (auto i:t) {
        cout<<i.type<<" -> "<<i.lexeme<< endl;
    }
    return 0;
}