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
        exit(1);
    }

    while (t[i].type == "PLUS" || t[i].type == "MINUS" ||
           t[i].type == "STAR" || t[i].type == "SLASH" ||
           t[i].type == "MOD")
    {
        string op = t[i].lexeme;
        i++;

        string right = t[i].lexeme;
        i++;

        left = left + " " + op + " " + right;
    }

    return left;
}

void handle_assignment_or_inc(vector<string> &code, int &i, string indent = "")
{
    string var = t[i].lexeme;

    if (t[i + 1].type == "INCREMENT")
    {
        code.push_back(indent + var + " += 1");
        i += 2;
        if (t[i].type == "SEMI") i++;
        return;
    }
    else if (t[i + 1].type == "DECREMENT")
    {
        code.push_back(indent + var + " -= 1");
        i += 2;
        if (t[i].type == "SEMI") i++;
        return;
    }

    i++;

    if (t[i].type == "ASSIGN")
    {
        i++;
        string res = py_expression(i);
        code.push_back(indent + var + " = " + res);
    }

    while (t[i].type != "SEMI") i++;
    i++;
}

void handle_scanf(vector<string> &code, int &i, string indent = "")
{
    i++; 
    i++; 

    if (t[i].type == "STRING") i++;
    if (t[i].type == "COMMA") i++;
    if (t[i].type == "AMP") i++;

    string var = t[i].lexeme;

    code.push_back(indent + var + " = int(input())");

    while (t[i].type != "SEMI") i++;
    i++;
}

void handle_printf(vector<string> &code, int &i, string indent = "")
{
    i++; // printf
    i++; // (

    string text = "";
    string var = "";

    // 🔥 FIX: loop till ')'
    while (t[i].type != "RPAREN")
    {
        if (t[i].type == "STRING")
        {
            text = t[i].lexeme;
        }
        else if (t[i].type == "IDENTIFIER")
        {
            var = t[i].lexeme;
        }
        i++;
    }

    i++; // )
    if (t[i].type == "SEMI") i++;

    if (var != "")
        code.push_back(indent + "print(" + var + ")");
    else
        code.push_back(indent + "print(\"" + text + "\")");
}

void handle_if(vector<string> &code, int &i, string indent)
{
    i++; i++;

    string left = t[i].lexeme; i++;
    string op = t[i].lexeme; i++;
    string right = t[i].lexeme; i++;

    if (t[i].type == "RPAREN") i++;
    if (t[i].type == "LBRACE") i++;

    code.push_back(indent + "if " + left + " " + op + " " + right + ":");

    int brace = 1;

    while (brace > 0)
    {
        if (t[i].type == "LBRACE") brace++;
        else if (t[i].type == "RBRACE") brace--;

        if (brace == 0) break;

        if (t[i].type == "IF")
        {
            handle_if(code, i, indent + "    ");
        }
        else if (t[i].type == "PRINTF")
        {
            handle_printf(code, i, indent + "    ");
        }
        else if (t[i].type == "IDENTIFIER")
        {
            handle_assignment_or_inc(code, i, indent + "    ");
        }
        else
        {
            i++;
        }
    }

    i++;

    if (t[i].type == "ELSE")
    {
        i++;
        if (t[i].type == "LBRACE") i++;

        code.push_back(indent + "else:");

        int brace2 = 1;

        while (brace2 > 0)
        {
            if (t[i].type == "LBRACE") brace2++;
            else if (t[i].type == "RBRACE") brace2--;

            if (brace2 == 0) break;

            if (t[i].type == "IF")
            {
                handle_if(code, i, indent + "    ");
            }
            else if (t[i].type == "PRINTF")
            {
                handle_printf(code, i, indent + "    ");
            }
            else if (t[i].type == "IDENTIFIER")
            {
                handle_assignment_or_inc(code, i, indent + "    ");
            }
            else
            {
                i++;
            }
        }

        i++;
    }
}

void handle_for(vector<string> &code, int &i)
{
    i++; i++;

    string var = t[i].lexeme; i++;
    i++;
    string start = t[i].lexeme; i++;

    while (t[i].type != "SEMI") i++;
    i++;

    i++; i++;
    string end = t[i].lexeme; i++;

    while (t[i].type != "RPAREN") i++;
    i++;

    if (t[i].type == "LBRACE") i++;

    code.push_back("for " + var + " in range(" + start + ", " + end + "):");

    int brace = 1;

    while (brace > 0)
    {
        if (t[i].type == "LBRACE") brace++;
        else if (t[i].type == "RBRACE") brace--;

        if (brace == 0) break;

        if (t[i].type == "IF")
        {
            handle_if(code, i, "    ");
        }
        else if (t[i].type == "PRINTF")
        {
            handle_printf(code, i, "    ");
        }
        else if (t[i].type == "IDENTIFIER")
        {
            handle_assignment_or_inc(code, i, "    ");
        }
        else
        {
            i++;
        }
    }

    i++;
}

void target_code()
{
    vector<string> code;
    int i = 0;

    i += 5;

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

                if (t[i].type == "COMMA") i++;
                else break;
            }
            i++;
        }

        else if (t[i].type == "FOR")
        {
            handle_for(code, i);
        }

        else if (t[i].type == "IF")
        {
            handle_if(code, i, "");
        }

        else if (t[i].type == "IDENTIFIER")
        {
            handle_assignment_or_inc(code, i);
        }

        else if (t[i].type == "PRINTF")
        {
            handle_printf(code, i);
        }

        else if (t[i].type == "SCANF")
        {
            handle_scanf(code, i);
        }

        else
        {
            i++;
        }
    }

    cout << "\nGenerated Python Code:\n\n";
    for (auto &line : code)
        cout << line << endl;
}
