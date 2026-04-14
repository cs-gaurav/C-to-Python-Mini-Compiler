#include<iostream>
#include"lexer.h"
#include"parser.h"
#include"semantic.h"
#include "inter.h"
#include "target.h"
using namespace std;
int main() {
    cout<<"compiler has started!"<<endl;
    cout<<"give some expression!"<<endl;
    lexer();
    for (auto i:t) {
        cout<<i.type<<" -> "<<i.lexeme<< endl;
    }
    cout<<endl;
    parser();
    semantic_analyzer();
    intermediate_code();
    target_code();
    return 0;
}