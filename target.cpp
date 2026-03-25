#include<iostream>
#include<vector>
#include<string>
#include "lexer.h"

using namespace std;

static int tempCount = 1;

static string newTemp() {
    return "t" + to_string(tempCount++);
}

string expression(int &i, vector<string> &code) {
    string left;

    if(t[i].type=="IDENTIFIER" || t[i].type=="NUMBER") {
        left = t[i].lexeme;
        i++;
    }
    else {
        cout<<"Invalid expression\n";
        exit(1);
    }

    while(t[i].type=="PLUS" || t[i].type=="MINUS" ||
          t[i].type=="STAR" || t[i].type=="SLASH" || t[i].type=="MOD") {

        string op = t[i].lexeme;
        i++;

        string right;
        if(t[i].type=="IDENTIFIER" || t[i].type=="NUMBER") {
            right = t[i].lexeme;
            i++;
        }
        else {
            cout<<"Invalid expression\n";
            exit(1);
        }

        string temp = newTemp();
        code.push_back(temp+" = "+left+" "+op+" "+right);

        left = temp;
    }

    return left;
}

void target_code() {
    vector<string> code;
    int i = 0;

    if (t[i].type=="INT") i++;
    if (t[i].type=="IDENTIFIER") i++;
    if (t[i].type=="LPAREN") i++;
    if (t[i].type=="RPAREN") i++;
    if (t[i].type=="LBRACE") i++;

    while(t[i].type!="EOF" && t[i].type!="RBRACE") {

        if(t[i].type=="INT") {
            i++;

            while(true) {
                string name = t[i].lexeme;
                i++;

                if(t[i].type=="ASSIGN") {
                    i++;
                    string res = expression(i,code);
                    code.push_back(name+" = "+res);
                }

                if(t[i].type=="COMMA") {
                    i++;
                    continue;
                }
                break;
            }

            if(t[i].type!="SEMI") {
                cout<<"Missing ;\n";
                exit(1);
            }
            i++;
        }

        else if(t[i].type=="IDENTIFIER") {
            string name = t[i].lexeme;
            i++;

            if(t[i].type!="ASSIGN") {
                cout<<"Expected =\n";
                exit(1);
            }

            i++;

            string res = expression(i,code);
            code.push_back(name+" = "+res);

            if(t[i].type!="SEMI") {
                cout<<"Missing ;\n";
                exit(1);
            }
            i++;
        }

        else if(t[i].type=="RETURN") {
            i++;

            if(t[i].type=="NUMBER") {
                code.push_back("return "+t[i].lexeme);
                i++;
            }
            else {
                cout<<"Invalid return\n";
                exit(1);
            }

            if(t[i].type!="SEMI") {
                cout<<"Missing ;\n";
                exit(1);
            }
            i++;
        }

        else {
            cout<<"Invalid statement\n";
            exit(1);
        }
    }
    cout<<"--------------------";
    cout<<"\nGenerated Python Code:\n\n";
    cout<<"def main():\n";
    for(auto &line:code) {
        cout<<"    "<<line<<endl;
    }
    cout<<"--------------------";
}