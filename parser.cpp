#include<iostream>
#include<vector>
#include<string>
#include "lexer.h"
using namespace std;

int pos = 0;

int node_id = 0;
vector<string> dot_nodes;
vector<string> dot_edges;
vector<int> parent_stack;

int addNode(string label) {
    int id = node_id++;
    dot_nodes.push_back("node" + to_string(id) + " [label=\"" + label + "\"];");

    if (!parent_stack.empty()) {
        dot_edges.push_back(
            "node" + to_string(parent_stack.back()) +
            " -> node" + to_string(id) + ";"
        );
    }
    return id;
}

void pushNode(string label) {
    int id = addNode(label);
    parent_stack.push_back(id);
}

void popNode() {
    if (!parent_stack.empty()) parent_stack.pop_back();
}


void match(string next) {
    if (t[pos].type == next) {
        addNode(t[pos].lexeme);
        pos++;
    }
    else {
        cout<<"Syntax Error at line "<<t[pos].line<<"\n";
        cout<<"Unexpected token: "<<t[pos].lexeme<<"\n";
        exit(1);
    }
}

void value() {
    if (t[pos].type=="IDENTIFIER" || t[pos].type=="NUMBER") {
        addNode(t[pos].lexeme);
        pos++;
    }
    else {
        cout<<"Syntax Error at line "<<t[pos].line<<"\n";
        cout<<"Unexpected token: "<<t[pos].lexeme<<"\n";
        exit(1);
    }
}

void unary_expr() {
    pushNode("unary");

    if (t[pos].type=="INCREMENT" || t[pos].type=="DECREMENT") {
        match(t[pos].type);
        match("IDENTIFIER");
    }
    else if (t[pos].type=="IDENTIFIER") {
        match("IDENTIFIER");
        if (t[pos].type=="INCREMENT" || t[pos].type=="DECREMENT")
            match(t[pos].type);
    }
    else if (t[pos].type=="NUMBER") {
        match("NUMBER");
    }
    else {
        cout<<"Syntax Error at line "<<t[pos].line<<"\n";
        cout<<"Unexpected token: "<<t[pos].lexeme<<"\n";
        exit(1);
    }

    popNode();
}

void expression() {
    pushNode("expr");

    unary_expr();

    if (t[pos].type=="PLUS" || t[pos].type=="MINUS" ||
        t[pos].type=="STAR" || t[pos].type=="SLASH" ||
        t[pos].type=="MOD") {

        pushNode("op");
        match(t[pos].type);
        popNode();

        expression();   // recursion = tree shape
    }

    popNode();
}

void comparison() {
    pushNode("comp");

    if (t[pos].type=="GT" || t[pos].type=="LT" ||
        t[pos].type=="GTQ" || t[pos].type=="LTQ" ||
        t[pos].type=="EQ" || t[pos].type=="NTQ") {
        match(t[pos].type);
    } 
    else {
        cout<<"Syntax Error at line "<<t[pos].line<<"\n";
        cout<<"Unexpected token: "<<t[pos].lexeme<<"\n";
        exit(1);
    }

    popNode();
}

void comparison_expr() {
    pushNode("cmp_expr");

    value();
    comparison();
    value();

    popNode();
}

void bool_factor();

void bool_expr() {
    pushNode("bool_expr");

    bool_factor();
    while (t[pos].type=="OR") {
        match("OR");
        bool_factor();
    }

    popNode();
}

void bool_term() {
    pushNode("bool_term");

    bool_factor();
    while (t[pos].type=="AND") {
        match("AND");
        bool_factor();
    }

    popNode();
}

void bool_factor() {
    pushNode("bool_factor");

    if (t[pos].type=="LPAREN") {
        match("LPAREN");
        bool_expr();
        match("RPAREN");
    } 
    else {
        comparison_expr();
    }

    popNode();
}

void declaration() {
    pushNode("DECL");

    match("INT");

    while (true) {
        match("IDENTIFIER");

        if (t[pos].type=="ASSIGN") {
            match("ASSIGN");
            expression();
        }

        if (t[pos].type=="COMMA") match("COMMA");
        else break;
    }

    match("SEMI");

    popNode();
}

void assignment() {
    pushNode("ASSIGN");

    match("IDENTIFIER");
    match("ASSIGN");
    expression();
    match("SEMI");

    popNode();
}

void parse_return() {
    pushNode("RETURN");

    match("RETURN");
    expression();
    match("SEMI");

    popNode();
}

void io_statement() {
    pushNode("IO");

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

    popNode();
}

void inner_condition();

void condition() {
    pushNode("IF");

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

    popNode();
}

void for_loop() {
    pushNode("FOR");

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

    popNode();
}

void statements() {
    pushNode("STMT");

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
        cout<<"Syntax Error at line "<<t[pos].line<<"\n";
        cout<<"Unexpected token: "<<t[pos].lexeme<<"\n";
        exit(1);
    }

    popNode();
}

void inner_condition() {
    while (t[pos].type!="RBRACE" && t[pos].type!="EOF") {
        statements();
    }
}

void inner_code() {
    while (t[pos].type!="RETURN" && t[pos].type!="RBRACE" && t[pos].type!="EOF") {
        statements();
    }
}

void begin() {
    pushNode("PROGRAM");

    match("INT");
    match("MAIN");
    match("LPAREN");
    match("RPAREN");

    pushNode("BLOCK");

    match("LBRACE");
    inner_code();
    if (t[pos].type != "RETURN") {
        cout<<"Syntax Error: main must end with return 0;\n";
        exit(1);
    }

    match("RETURN");

    if (t[pos].type != "NUMBER" || t[pos].lexeme != "0") {
        cout<<"Syntax Error: main must return 0\n";
        exit(1);
    }

    match("NUMBER");
    match("SEMI");

    match("RBRACE");

    popNode();
    popNode();
}

void parser() {
    begin();

    if (t[pos].type!="EOF") {
        cout<<"Unexpected Tokens\n";
    }
    else {
        cout<<"Parsing Successful\n";

        cout << "PARSE_TREE_START\n";
        cout << "digraph G {\n";

        for (auto &n : dot_nodes) cout <<n<<"\n";
        for (auto &e : dot_edges) cout <<e<<"\n";

        cout << "}\n";
        cout << "PARSE_TREE_END\n";
    }
}