#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
using namespace std;

struct token {
    string type;
    string lexeme;
};
extern vector<token> t;

void lexer();
#endif