#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

string py_expression(int &i)
{
    string left;

    if (t[i].type == "IDENTIFIER" || t[i].type == "NUMBER")
    {
        left = t[i].lexeme;
        i++;
    }
    else
    {
        cout << "Error: Invalid expression\n";
        exit(1);
    }

    while (t[i].type == "PLUS" || t[i].type == "MINUS" ||
           t[i].type == "STAR" || t[i].type == "SLASH" || t[i].type == "MOD")
    {

        string op = t[i].lexeme;
        i++;

        string right;
        if (t[i].type == "IDENTIFIER" || t[i].type == "NUMBER")
        {
            right = t[i].lexeme;
            i++;
        }
        else
        {
            cout << "Error: Invalid expression\n";
            exit(1);
        }

        left = left + " " + op + " " + right;
    }

    return left;
}

void target_code()
{
    vector<string> code;
    int i = 0;

    if (t[i].type == "INT")
        i++;
    if (t[i].type == "IDENTIFIER")
        i++;
    if (t[i].type == "LPAREN")
        i++;
    if (t[i].type == "RPAREN")
        i++;
    if (t[i].type == "LBRACE")
        i++;

    while (t[i].type != "EOF" && t[i].type != "RBRACE")
    {

        if (t[i].type == "INT")
        {
            i++;

            while (true)
            {
                string name = t[i].lexeme;
                i++;

                if (t[i].type == "ASSIGN")
                {
                    i++;
                    string res = py_expression(i);
                    code.push_back(name + " = " + res);
                }

                if (t[i].type == "COMMA")
                {
                    i++;
                    continue;
                }
                break;
            }

            if (t[i].type != "SEMI")
            {
                cout << "Error: Missing ;\n";
                exit(1);
            }
            i++;
        }

        else if (t[i].type == "IDENTIFIER")
        {
            string name = t[i].lexeme;
            i++;

            if (t[i].type != "ASSIGN")
            {
                cout << "Error: Expected =\n";
                exit(1);
            }

            i++;

            string res = py_expression(i);
            code.push_back(name + " = " + res);

            if (t[i].type != "SEMI")
            {
                cout << "Error: Missing ;\n";
                exit(1);
            }
            i++;
        }

        else if (t[i].type == "RETURN")
        {
            i++;

            if (t[i].type == "NUMBER")
            {
                code.push_back("return " + t[i].lexeme);
                i++;
            }
            else
            {
                cout << "Error: Invalid return\n";
                exit(1);
            }

            if (t[i].type != "SEMI")
            {
                cout << "Error: Missing ;\n";
                exit(1);
            }
            i++;
        }

        else
        {
            cout << "Error: Invalid statement\n";
            exit(1);
        }
    }

    cout << "\nGenerated Python Code:\n\n";
    for (auto &line : code)
    {
        cout << line << endl;
    }
}
