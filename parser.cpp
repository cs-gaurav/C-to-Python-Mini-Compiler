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

void parse_declaration() {
    match("INT");
    match("IDENTIFIER");
    while(t[pos].type=="COMMA") {
        match("COMMA");
        match("IDENTIFIER");
    }
    match("SEMI");
}

void parse_assignment() {
    match("IDENTIFIER");
    match("ASSIGN");
    match("NUMBER");
    match("SEMI");
}

void parse_return() {
    match("RETURN");
    match("NUMBER");
    match("SEMI");
}

void parse_statement() {
    if(t[pos].type=="INT")    parse_declaration();
    else if(t[pos].type=="IDENTIFIER")    parse_assignment();
    else if(t[pos].type=="RETURN")    parse_return();
    else {
        cout<<"Invalid Statement\n";
        exit(1);
    }
}

void parse_statement_list() {
    while(t[pos].type!="RBRACE" && t[pos].type!="EOF") {
        parse_statement();
    }
}

void parse_function() {
    match("INT");
    match("IDENTIFIER");
    match("LPAREN");
    match("RPAREN");
    match("LBRACE");
    parse_statement_list();
    match("RBRACE");
}

void parser() {
    parse_function();
    if(t[pos].type!="EOF")    cout<<"Unexpected Tokens\n";
}