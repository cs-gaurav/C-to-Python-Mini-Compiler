#include<iostream>
#include<unordered_map>
#include<string>
#include "lexer.h"
using namespace std;

struct Var{ bool initialized=false; };
unordered_map<string,Var> table;

void sem_error(string msg,string name=""){
    cout<<"Error: "<<msg;
    if(name!="") cout<<" -> "<<name;
    cout<<endl;
    exit(1);
}

void sem_checkVar(string name){
    if(table.find(name)==table.end())
        sem_error("Undeclared variable",name);
    if(!table[name].initialized)
        sem_error("Used before initialization",name);
}

void sem_expect(int &i,string type){
    if(t[i].type==type) i++;
    else sem_error("Unexpected token",t[i].type);
}

void sem_value(int &i){
    if(t[i].type=="IDENTIFIER"){
        sem_checkVar(t[i].lexeme);
        i++;
    }
    else if(t[i].type=="NUMBER") i++;
    else sem_error("Invalid value");
}

void sem_unary(int &i){
    if(t[i].type=="INCREMENT"||t[i].type=="DECREMENT"){
        i++;
        sem_checkVar(t[i].lexeme);
        sem_expect(i,"IDENTIFIER");
    }
    else if(t[i].type=="IDENTIFIER"){
        sem_checkVar(t[i].lexeme);
        i++;
        if(t[i].type=="INCREMENT"||t[i].type=="DECREMENT") i++;
    }
    else if(t[i].type=="NUMBER") i++;
    else sem_error("Invalid unary");
}

void sem_expr(int &i){
    sem_unary(i);
    while(t[i].type=="PLUS"||t[i].type=="MINUS"||
          t[i].type=="STAR"||t[i].type=="SLASH"||
          t[i].type=="MOD"){
        i++;
        sem_unary(i);
    }
}

void sem_comparison(int &i){
    if(t[i].type=="GT"||t[i].type=="LT"||
       t[i].type=="GTQ"||t[i].type=="LTQ"||
       t[i].type=="EQ"||t[i].type=="NTQ") i++;
    else sem_error("Invalid comparison");
}

void sem_bool_factor(int &i);

void sem_bool_expr(int &i){
    sem_bool_factor(i);
    while(t[i].type=="OR"){
        i++;
        sem_bool_factor(i);
    }
}

void sem_bool_term(int &i){
    sem_bool_factor(i);
    while(t[i].type=="AND"){
        i++;
        sem_bool_factor(i);
    }
}

void sem_bool_factor(int &i){
    if(t[i].type=="LPAREN"){
        i++;
        sem_bool_expr(i);
        sem_expect(i,"RPAREN");
    }
    else{
        sem_value(i);
        sem_comparison(i);
        sem_value(i);
    }
}

void sem_declaration(int &i){
    sem_expect(i,"INT");
    while(true){
        string name=t[i].lexeme;

        if(table.find(name)!=table.end())
            sem_error("Variable already declared",name);

        table[name];
        sem_expect(i,"IDENTIFIER");

        if(t[i].type=="ASSIGN"){
            i++;
            sem_expr(i);
            table[name].initialized=true;
        }

        if(t[i].type=="COMMA") i++;
        else break;
    }
    sem_expect(i,"SEMI");
}

void sem_assignment(int &i){
    string name=t[i].lexeme;

    if(table.find(name)==table.end())
        sem_error("Undeclared variable",name);

    sem_expect(i,"IDENTIFIER");
    sem_expect(i,"ASSIGN");

    sem_expr(i);
    table[name].initialized=true;

    sem_expect(i,"SEMI");
}

void sem_return(int &i){
    sem_expect(i,"RETURN");
    sem_expr(i);
    sem_expect(i,"SEMI");
}

void sem_io(int &i){
    if(t[i].type=="PRINTF"){
        i++; sem_expect(i,"LPAREN"); sem_expect(i,"STRING");
        while(t[i].type=="COMMA"){
            i++;
            sem_value(i);
        }
        sem_expect(i,"RPAREN");
        sem_expect(i,"SEMI");
    }
    else{
        i++; sem_expect(i,"LPAREN"); sem_expect(i,"STRING");
        while(t[i].type=="COMMA"){
            i++;
            if(t[i].type=="AMP") i++;

            string name=t[i].lexeme;
            if(table.find(name)==table.end())
                sem_error("Undeclared variable",name);

            table[name].initialized=true;
            sem_expect(i,"IDENTIFIER");
        }
        sem_expect(i,"RPAREN");
        sem_expect(i,"SEMI");
    }
}

void sem_statements(int &i);

void sem_block(int &i){
    sem_expect(i,"LBRACE");
    while(t[i].type!="RBRACE")
        sem_statements(i);
    sem_expect(i,"RBRACE");
}

void sem_condition(int &i){
    sem_expect(i,"IF");
    sem_expect(i,"LPAREN");
    sem_bool_expr(i);
    sem_expect(i,"RPAREN");

    sem_block(i);

    if(t[i].type=="ELSE"){
        i++;
        sem_block(i);
    }
}

void sem_for(int &i){
    sem_expect(i,"FOR");
    sem_expect(i,"LPAREN");

    if(t[i].type!="SEMI"){
        if(t[i+1].type=="ASSIGN") sem_assignment(i);
        else{ sem_expr(i); sem_expect(i,"SEMI"); }
    }
    else sem_expect(i,"SEMI");

    if(t[i].type!="SEMI") sem_bool_expr(i);
    sem_expect(i,"SEMI");

    if(t[i].type!="RPAREN") sem_expr(i);
    sem_expect(i,"RPAREN");

    sem_block(i);
}

void sem_statements(int &i){
    if(t[i].type=="INT") sem_declaration(i);

    else if(t[i].type=="IDENTIFIER"){
        if(t[i+1].type=="ASSIGN") sem_assignment(i);
        else{ sem_expr(i); sem_expect(i,"SEMI"); }
    }

    else if(t[i].type=="INCREMENT"||t[i].type=="DECREMENT"){
        sem_expr(i);
        sem_expect(i,"SEMI");
    }

    else if(t[i].type=="RETURN") sem_return(i);
    else if(t[i].type=="IF") sem_condition(i);
    else if(t[i].type=="FOR") sem_for(i);
    else if(t[i].type=="PRINTF"||t[i].type=="SCANF") sem_io(i);
    else sem_error("Invalid statement");
}

void semantic_analyzer(){
    int i=0;

    sem_expect(i,"INT");
    sem_expect(i,"MAIN");
    sem_expect(i,"LPAREN");
    sem_expect(i,"RPAREN");

    sem_block(i);

    if(t[i].type!="EOF")
        sem_error("Unexpected tokens");

    cout<<"Semantic analysis done\n";
}