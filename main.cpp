#include<iostream>
#include"lexer.h"
#include"parser.h"
#include<codegen.h>
using namespace std;
int main() {
    cout<<"compiler has started!"<<endl;
    cout<<"give some expression!"<<endl;
    lexer();
    for (auto i:t) {
        cout<<i.type<<" -> "<<i.lexeme<< endl;
    }
    parser();
    cout<<"parsing complete\n";
    generate_code();
    return 0;
}
