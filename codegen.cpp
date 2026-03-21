#include<iostream>
#include<vector>
#include<string>
#include "lexer.h"

using namespace std;

vector<string> python_code;

void generate_code() {

    int i = 0;

    while(t[i].type != "EOF") {

        // 🔹 Skip ONLY function: int main()
if(t[i].type=="INT" && t[i+1].type=="IDENTIFIER" && t[i+2].type=="LPAREN") {
    i += 4; // skip int main ( )
}

        // 🔹 Declaration: int a = 7;
        else if(t[i].type=="INT") {
            string var = t[i+1].lexeme;

            if(t[i+2].type=="ASSIGN") {
                string val = t[i+3].lexeme;

                python_code.push_back(var + " = " + val);
                i += 5;
            }
            else {
                i += 3;
            }
        }

        // 🔹 Assignment: a = 7;
        else if(t[i].type=="IDENTIFIER" && t[i+1].type=="ASSIGN") {
            string var = t[i].lexeme;
            string val = t[i+2].lexeme;

            python_code.push_back(var + " = " + val);
            i += 4;
        }

        // 🔹 Return → print
        else if(t[i].type=="RETURN") {
            string val = t[i+1].lexeme;

            python_code.push_back("print(" + val + ")");
            i += 3;
        }

        // 🔹 Ignore braces
        else if(t[i].type=="LBRACE" || t[i].type=="RBRACE") {
            i++;
        }

        else {
            i++;
        }
    }

    // 🔥 CODE OPTIMIZATION (Dead Code Elimination)
    // Remove statements AFTER return

    vector<string> optimized;
    bool foundReturn = false;

    for(auto line : python_code) {
        if(foundReturn) break;

        optimized.push_back(line);

        if(line.find("print") != string::npos) {
            foundReturn = true;
        }
    }

    python_code = optimized;

    // 🔹 Print final Python code
    cout<<"\nGenerated Python Code:\n";
    for(auto line : python_code) {
        cout<<line<<endl;
    }
}