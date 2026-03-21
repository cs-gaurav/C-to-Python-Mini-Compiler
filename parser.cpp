#include<iostream>
#include "lexer.h"
using namespace std;

int pos=0;
void match(string next) {
    if (t[pos].type==next) pos++;
    else {
        cout<<"Syntax Error!\n";
        exit(1);
    }
}

void expression() {
    if (t[pos].type=="NUMBER" || t[pos].type=="IDENTIFIER") pos++;
    else {
        cout<<"invalid expression\n";
        exit(1);
    }
    while (t[pos].type=="PLUS" || t[pos].type=="MINUS" || t[pos].type=="STAR" || t[pos].type=="SLASH" || t[pos].type=="MOD" )   {
        pos++;
        if (t[pos].type=="IDENTIFIER" || t[pos].type=="NUMBER") {
            pos++;
        }
        else {
            cout<<"invalid identifier\n";
            exit(1);
        }
    }
}

void declaration() {
    match("INT");
    match("IDENTIFIER");
    if (t[pos].type=="ASSIGN") {
        match("ASSIGN");
        expression();
    }
    else {
        while(t[pos].type=="COMMA") {
            match("COMMA");
            match("IDENTIFIER");
        }
    }
    match("SEMI");
}

void assignment() {
    match("IDENTIFIER");
    match("ASSIGN");
    expression();
    match("SEMI");
}

void parse_return() {
    match("RETURN");
    match("NUMBER");
    match("SEMI");
}

void statements() {
    if(t[pos].type=="INT")    declaration();
    else if(t[pos].type=="IDENTIFIER")    assignment();
    else if(t[pos].type=="RETURN")    parse_return();
    else {
        cout<<"Invalid Statement\n";
        exit(1);
    }
}

void inner_code() {
    while(t[pos].type!="RBRACE" && t[pos].type!="EOF") {
        statements();
    }
}

void begin() {
    match("INT");
    match("IDENTIFIER");
    match("LPAREN");
    match("RPAREN");
    match("LBRACE");
    inner_code();
    match("RBRACE");
}

void parser() {
    begin();
    if(t[pos].type!="EOF")    cout<<"Unexpected Tokens\n";
}