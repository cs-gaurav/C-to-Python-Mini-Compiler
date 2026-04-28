#include<iostream>
#include "lexer.h"
#include <cctype>
using namespace std;

vector<token> t;

void lexer() {
    char ch;
    int line = 1;   // ✅ track line number

    while(cin.get(ch)) {

        if (ch == '\n') {
            line++;
            continue;
        }

        if (isspace(ch)) continue;

        if (isalpha(ch) || ch=='_') {
            string str="";
            str+=ch;

            while (isalnum(cin.peek()) || cin.peek()=='_') {
                cin.get(ch);
                str+=ch;
            }

            if (str=="int") t.push_back({"INT",str,line});
            else if (str=="printf") t.push_back({"PRINTF",str,line});
            else if (str=="scanf") t.push_back({"SCANF",str,line});
            else if (str=="break") t.push_back({"BREAK",str,line});
            else if (str=="return") t.push_back({"RETURN",str,line});
            else if (str=="if") t.push_back({"IF",str,line});
            else if (str=="else") t.push_back({"ELSE",str,line});
            else if (str=="for") t.push_back({"FOR",str,line});
            else if (str=="while") t.push_back({"WHILE",str,line});
            else if (str=="main") t.push_back({"MAIN",str,line});
            else t.push_back({"IDENTIFIER",str,line});
        }

        else if (isdigit(ch)) {
            string num="";
            num+=ch;

            while (isdigit(cin.peek())) {
                cin.get(ch);
                num+=ch;
            }

            t.push_back({"NUMBER", num, line});
        }

        else if (ch=='"') {
            string str="";
            while (cin.get(ch) && ch!='"') str+=ch;
            t.push_back({"STRING", str, line});
        }

        else if (ch=='+' && cin.peek()=='+') {
            cin.get();
            t.push_back({"INCREMENT","++",line});
        }
        else if (ch=='-' && cin.peek()=='-') {
            cin.get();
            t.push_back({"DECREMENT","--",line});
        }
        else if (ch=='+') t.push_back({"PLUS","+",line});
        else if (ch=='-') t.push_back({"MINUS","-",line});
        else if (ch=='*') t.push_back({"STAR","*",line});
        else if (ch=='/') t.push_back({"SLASH","/",line});
        else if (ch=='%') t.push_back({"MOD","%",line});

        else if (ch=='&' && cin.peek()=='&') {
            cin.get();
            t.push_back({"AND","&&",line});
        }
        else if (ch=='&') t.push_back({"AMP","&",line});

        else if (ch=='|' && cin.peek()=='|') {
            cin.get();
            t.push_back({"OR","||",line});
        }

        else if (ch=='<') {
            if (cin.peek()=='=') {
                cin.get();
                t.push_back({"LTQ","<=",line});
            } else t.push_back({"LT","<",line});
        }

        else if (ch=='>') {
            if (cin.peek()=='=') {
                cin.get();
                t.push_back({"GTQ",">=",line});
            } else t.push_back({"GT",">",line});
        }

        else if (ch=='!') {
            if (cin.peek()=='=') {
                cin.get();
                t.push_back({"NTQ","!=",line});
            } else t.push_back({"NT","!",line});
        }

        else if (ch=='=') {
            if (cin.peek()=='=') {
                cin.get();
                t.push_back({"EQ","==",line});
            } else t.push_back({"ASSIGN","=",line});
        }

        else if (ch=='{') t.push_back({"LBRACE","{",line});
        else if (ch=='}') t.push_back({"RBRACE","}",line});
        else if (ch=='(') t.push_back({"LPAREN","(",line});
        else if (ch==')') t.push_back({"RPAREN",")",line});
        else if (ch==';') t.push_back({"SEMI",";",line});
        else if (ch==',') t.push_back({"COMMA",",",line});
    }

    t.push_back({"EOF","EOF",line});
}