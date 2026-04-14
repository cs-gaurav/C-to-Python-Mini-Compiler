#include<iostream>
#include "lexer.h"
using namespace std;

int pos=0;

void match(string next) {
    if (t[pos].type==next) pos++;
    else {
        cout<<"Syntax Error at token: "<<t[pos].type<<"\n";
        exit(1);
    }
}

void value() {
    if (t[pos].type=="IDENTIFIER" || t[pos].type=="NUMBER") pos++;
    else {
        cout<<"Invalid value at token: "<<t[pos].type<<"\n";
        exit(1);
    }
}

void unary_expr() {
    if (t[pos].type=="INCREMENT" || t[pos].type=="DECREMENT") {
        pos++;
        match("IDENTIFIER");
    }
    else if (t[pos].type=="IDENTIFIER") {
        match("IDENTIFIER");
        if (t[pos].type=="INCREMENT" || t[pos].type=="DECREMENT") pos++;
    }
    else if (t[pos].type=="NUMBER") {
        match("NUMBER");
    }
    else {
        cout<<"Invalid unary expression at token: "<<t[pos].type<<"\n";
        exit(1);
    }
}

void expression() {
    unary_expr();
    while (t[pos].type=="PLUS" || t[pos].type=="MINUS" ||
           t[pos].type=="STAR" || t[pos].type=="SLASH" ||
           t[pos].type=="MOD") {
        pos++;
        unary_expr();
    }
}

void comparison() {
    if (t[pos].type=="GT" || t[pos].type=="LT" ||
        t[pos].type=="GTQ" || t[pos].type=="LTQ" ||
        t[pos].type=="EQ" || t[pos].type=="NTQ") {
        pos++;
    } else {
        cout<<"Invalid comparison operator at token: "<<t[pos].type<<"\n";
        exit(1);
    }
}

void comparison_expr() {
    value();
    comparison();
    value();
}

void bool_factor();

void bool_expr() {
    bool_factor();
    while (t[pos].type=="OR") {
        match("OR");
        bool_factor();
    }
}

void bool_term() {
    bool_factor();
    while (t[pos].type=="AND") {
        match("AND");
        bool_factor();
    }
}

void bool_factor() {
    if (t[pos].type=="LPAREN") {
        match("LPAREN");
        bool_expr();
        match("RPAREN");
    } else {
        comparison_expr();
    }
}

void declaration() {
    match("INT");
    while (true) {
        match("IDENTIFIER");
        if (t[pos].type=="ASSIGN") {
            match("ASSIGN");
            expression();
        }
        if (t[pos].type=="COMMA") {
            match("COMMA");
        }
        else break;
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
    expression();
    match("SEMI");
}

void io_statement() {
    if (t[pos].type=="PRINTF") {
        match("PRINTF");
        match("LPAREN");
        match("STRING");

        while (t[pos].type=="COMMA") {
            match("COMMA");
            value();
        }

        match("RPAREN");
        match("SEMI");
    }
    else {
        match("SCANF");
        match("LPAREN");
        match("STRING");

        while (t[pos].type=="COMMA") {
            match("COMMA");
            if (t[pos].type=="AMP") match("AMP");
            match("IDENTIFIER");
        }

        match("RPAREN");
        match("SEMI");
    }
}

void inner_condition();

void condition() {
    match("IF");
    match("LPAREN");
    bool_expr();
    match("RPAREN");

    match("LBRACE");
    inner_condition();
    match("RBRACE");

    if (t[pos].type=="ELSE") {
        match("ELSE");
        match("LBRACE");
        inner_condition();
        match("RBRACE");
    }
}

void for_loop() {
    match("FOR");
    match("LPAREN");

    if (t[pos].type!="SEMI") {
        if (t[pos+1].type=="ASSIGN") assignment();
        else {
            expression();
            match("SEMI");
        }
    }
    else match("SEMI");

    if (t[pos].type!="SEMI") bool_expr();
    match("SEMI");

    if (t[pos].type!="RPAREN") expression();
    match("RPAREN");

    match("LBRACE");
    inner_condition();
    match("RBRACE");
}

void statements() {
    if (t[pos].type=="INT") declaration();

    else if (t[pos].type=="IDENTIFIER") {
        if (t[pos+1].type=="ASSIGN") assignment();
        else {
            expression();
            match("SEMI");
        }
    }

    else if (t[pos].type=="INCREMENT" || t[pos].type=="DECREMENT") {
        expression();
        match("SEMI");
    }

    else if (t[pos].type=="RETURN") parse_return();
    else if (t[pos].type=="IF") condition();
    else if (t[pos].type=="FOR") for_loop();
    else if (t[pos].type=="PRINTF" || t[pos].type=="SCANF") io_statement();

    else {
        cout<<"Invalid Statement at token: "<<t[pos].type<<"\n";
        exit(1);
    }
}

void inner_condition() {
    while (t[pos].type!="RBRACE" && t[pos].type!="EOF") {
        statements();
    }
}

void inner_code() {
    while (t[pos].type!="RBRACE" && t[pos].type!="EOF") {
        statements();
    }
}

void begin() {
    match("INT");
    match("MAIN");
    match("LPAREN");
    match("RPAREN");
    match("LBRACE");
    inner_code();
    match("RBRACE");
}

void parser() {
    begin();
    if (t[pos].type!="EOF") cout<<"Unexpected Tokens\n";
    else cout<<"Parsing Successful\n";
}