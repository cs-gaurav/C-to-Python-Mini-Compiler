#include<iostream>
#include "lexer.h"
#include <cctype>
#include<unordered_set>
using namespace std;

vector<token>t;
void lexer() {
    char ch;
    while(cin.get(ch)) {
        if (isspace(ch)) continue;

        if (isalpha(ch) || ch=='_') {
            string str="";
            str+=ch;
            while(isalnum(cin.peek()) || cin.peek()=='_') {
                cin.get(ch);
                str+=ch;
            }
            if (str=="int")     t.push_back({"INT",str});
            else if (str=="break")   t.push_back({"BREAK",str});   
            else if (str=="return")  t.push_back({"RETURN",str});  
            else if (str=="if")      t.push_back({"IF",str});
            else if (str=="for")     t.push_back({"FOR",str});
            else if (str=="while")   t.push_back({"WHILE",str});
            else if (str=="else")   t.push_back({"ELSE",str});
            else    t.push_back({"IDENTIFIER",str});
        }
        
        else if (isdigit(ch)) {
            string num;
            num+=ch;
            while(isdigit(cin.peek())) {
                ch=cin.get();
                num+=ch;
            }
            t.push_back({"NUMBER", num});
        }

        else if (ch=='+')   t.push_back({"PLUS","+"});
        else if (ch=='-')   t.push_back({"MINUS","-"});
        else if (ch=='*')   t.push_back({"STAR","*"});
        else if (ch=='/')   t.push_back({"SLASH","/"});
        else if (ch=='%')   t.push_back({"MOD","%"});
        else if (ch=='=')   {
            if (cin.peek()=='=') {
                cin.get();
                t.push_back({"EQ","=="});
            }
            else t.push_back({"ASSIGN","="});
        }
        else if (ch=='&' && cin.peek()=='&') {
            cin.get();
            t.push_back({"AND","&&"});
        }

        else if (ch=='(')   t.push_back({"LPAREN","{"});
        else if (ch==')')   t.push_back({"RPAREN","}"});
        else if (ch=='[')   t.push_back({"LBRACKET","["});
        else if (ch==']')   t.push_back({"RBRACKET","]"});
        else if (ch=='{')   t.push_back({"LBRACE","("});
        else if (ch=='}')   t.push_back({"RBRACE",")"});
        else if (ch==';')   t.push_back({"SEMI",";"});
        else if (ch==',')   t.push_back({"COMMA",","});
    }
    t.push_back({"EOF","EOF"});
    return;
}