#include<iostream>
#include"lexer.h"
#include"parser.h"
#include"codegen.h"
#include"semantic.h"
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
    semantic_analyzer();
    generate_code();
    return 0;
}
