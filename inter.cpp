#include<iostream>
#include<string>
#include "lexer.h"
using namespace std;

static int tempCount=1,labelCount=1;

string newTemp(){return "t"+to_string(tempCount++);}
string newLabel(){return "L"+to_string(labelCount++);}

string expression(int &i){
    string left;
    if(t[i].type=="IDENTIFIER"||t[i].type=="NUMBER"){left=t[i].lexeme;i++;}
    else{cout<<"Invalid expression\n";exit(1);}
    while(t[i].type=="PLUS"||t[i].type=="MINUS"||t[i].type=="STAR"||t[i].type=="SLASH"||t[i].type=="MOD"){
        string op=t[i].lexeme;i++;
        string right;
        if(t[i].type=="IDENTIFIER"||t[i].type=="NUMBER"){right=t[i].lexeme;i++;}
        else{cout<<"Invalid expression\n";exit(1);}
        string temp=newTemp();
        cout<<temp<<" = "<<left<<" "<<op<<" "<<right<<endl;
        left=temp;
    }
    return left;
}

string condition_expr(int &i){
    string left=t[i].lexeme;i++;
    string op=t[i].lexeme;i++;
    string right=t[i].lexeme;i++;
    string temp=newTemp();
    cout<<temp<<" = "<<left<<" "<<op<<" "<<right<<endl;
    return temp;
}

void declaration(int &i){
    i++;
    while(true){
        string name=t[i].lexeme;i++;
        if(t[i].type=="ASSIGN"){i++;string res=expression(i);cout<<name<<" = "<<res<<endl;}
        if(t[i].type=="COMMA"){i++;continue;}
        break;
    }
    if(t[i].type!="SEMI"){cout<<"Missing ;\n";exit(1);}
    i++;
}

void assignment(int &i){
    string name=t[i].lexeme;i++;
    if(t[i].type!="ASSIGN"){cout<<"Expected =\n";exit(1);}
    i++;
    string res=expression(i);
    cout<<name<<" = "<<res<<endl;
    if(t[i].type!="SEMI"){cout<<"Missing ;\n";exit(1);}
    i++;
}

void if_statement(int &i){
    i++;i++;
    string cond=condition_expr(i);
    i++;
    string L1=newLabel(),L2=newLabel();
    cout<<"if "<<cond<<" goto "<<L1<<endl;
    cout<<"goto "<<L2<<endl;
    cout<<L1<<":"<<endl;
    i++;
    while(t[i].type!="RBRACE"){
        if(t[i].type=="IDENTIFIER")assignment(i);
        else break;
    }
    i++;
    if(t[i].type=="ELSE"){
        string L3=newLabel();
        cout<<"goto "<<L3<<endl;
        cout<<L2<<":"<<endl;
        i++;i++;
        while(t[i].type!="RBRACE"){
            if(t[i].type=="IDENTIFIER")assignment(i);
            else break;
        }
        i++;
        cout<<L3<<":"<<endl;
    }else cout<<L2<<":"<<endl;
}

void for_loop(int &i){
    i++;i++;
    if(t[i].type!="SEMI")assignment(i);
    else i++;
    string L1=newLabel(),L2=newLabel();
    cout<<L1<<":"<<endl;
    string cond=condition_expr(i);
    cout<<"ifFalse "<<cond<<" goto "<<L2<<endl;
    i++;
    int inc=i;
    while(t[i].type!="RPAREN")i++;
    i++;i++;
    while(t[i].type!="RBRACE"){
        if(t[i].type=="IDENTIFIER")assignment(i);
        else break;
    }
    expression(inc);
    cout<<"goto "<<L1<<endl;
    cout<<L2<<":"<<endl;
    i++;
}

void io_statement(int &i){
    if(t[i].type=="PRINTF"){
        cout<<"print "<<t[i+2].lexeme<<endl;
        while(t[i].type!="SEMI")i++;
        i++;
    }else{
        cout<<"input"<<endl;
        while(t[i].type!="SEMI")i++;
        i++;
    }
}

void intermediate_code(){
    int i=0;
    i+=5;
    while(t[i].type!="EOF"&&t[i].type!="RBRACE"){
        if(t[i].type=="INT")declaration(i);
        else if(t[i].type=="IDENTIFIER")assignment(i);
        else if(t[i].type=="IF")if_statement(i);
        else if(t[i].type=="FOR")for_loop(i);
        else if(t[i].type=="PRINTF"||t[i].type=="SCANF")io_statement(i);
        else if(t[i].type=="RETURN"){
            i++;i++;
            if(t[i].type=="SEMI")i++;
        }
        else{cout<<"Invalid statement\n";exit(1);}
    }
    cout<<"Intermediate code generated\n";
}