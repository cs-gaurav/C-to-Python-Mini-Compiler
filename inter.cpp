#include <iostream>
#include <string>
#include <vector>
#include "lexer.h"
using namespace std;

string expression(int &i);
string term(int &i);
string factor(int &i);

static int tempCount = 1, labelCount = 1;

string newTemp() { return "t" + to_string(tempCount++); }
string newLabel() { return "L" + to_string(labelCount++); }

// ---------------- FACTOR ----------------
string factor(int &i)
{
    if (i >= t.size())
        exit(1);

    if (t[i].type == "IDENTIFIER" || t[i].type == "NUMBER")
        return t[i++].lexeme;

    else if (t[i].type == "LPAREN")
    {
        i++;
        string val = expression(i);

        if (t[i].type != "RPAREN")
        {
            cout << "Missing )\n";
            exit(1);
        }
        i++;
        return val;
    }

    else
    {
        cout << "Invalid factor\n";
        exit(1);
    }
}

// ---------------- TERM ----------------
string term(int &i)
{
    string left = factor(i);

    while (t[i].type == "STAR" || t[i].type == "SLASH" || t[i].type == "MOD")
    {
        string op = t[i].lexeme;
        i++;
        string right = factor(i);

        string temp = newTemp();
        cout << temp << " = " << left << " " << op << " " << right << "\n";

        left = temp;
    }
    return left;
}

// ---------------- EXPRESSION ----------------
string expression(int &i)
{
    string left = term(i);

    while (t[i].type == "PLUS" || t[i].type == "MINUS")
    {
        string op = t[i].lexeme;
        i++;
        string right = term(i);

        string temp = newTemp();
        cout << temp << " = " << left << " " << op << " " << right << "\n";

        left = temp;
    }
    return left;
}

// ---------------- CONDITION ----------------
string condition_expr(int &i)
{
    string left = expression(i);
    string op = t[i].lexeme;
    i++;
    string right = expression(i);

    return left + " " + op + " " + right;
}

// ---------------- DECLARATION ----------------
void declaration(int &i)
{
    i++;
    while (true)
    {
        string name = t[i].lexeme;
        i++;

        if (t[i].type == "ASSIGN")
        {
            i++;
            string res = expression(i);
            cout << name << " = " << res << "\n";
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
        cout << "Missing ;\n";
        exit(1);
    }
    i++;
}


void assignment(int &i)
{
    string name = t[i].lexeme;
    i++;

    if (t[i].type != "ASSIGN")
    {
        cout << "Expected =\n";
        exit(1);
    }

    i++;
    string res = expression(i);

    cout << name << " = " << res << "\n";

    if (t[i].type != "SEMI")
    {
        cout << "Missing ;\n";
        exit(1);
    }
    i++;
}


void inc_dec(int &i)
{
    string var = t[i].lexeme;
    i++;

    string temp = newTemp();

    if (t[i].type == "INCREMENT")
    {
        i++;
        cout << temp << " = " << var << " + 1\n";
        cout << var << " = " << temp << "\n";
    }
    else if (t[i].type == "DECREMENT")
    {
        i++;
        cout << temp << " = " << var << " - 1\n";
        cout << var << " = " << temp << "\n";
    }

    if (t[i].type == "SEMI")
        i++;
}

void statements(int &i);

// ---------------- IF ----------------
void if_statement(int &i)
{
    i += 2;

    string cond = condition_expr(i);
    i++;

    string L1 = newLabel(), L2 = newLabel(), L3 = newLabel();

    cout << "if " << cond << " goto " << L1 << "\n";
    cout << "goto " << L2 << "\n";

    cout << L1 << ":\n";

    i++;

    while (t[i].type != "RBRACE" && t[i].type != "EOF")
        statements(i);

    if (t[i].type == "RBRACE")
        i++;

    if (t[i].type == "ELSE")
    {
        cout << "goto " << L3 << "\n";
        cout << L2 << ":\n";

        i += 2;

        while (t[i].type != "RBRACE" && t[i].type != "EOF")
            statements(i);

        if (t[i].type == "RBRACE")
            i++;

        cout << L3 << ":\n";
    }
    else
    {
        cout << L2 << ":\n";
    }
}

// ---------------- FOR (FIXED) ----------------
void for_loop(int &i)
{
    i += 2;

    
    if (t[i].type == "IDENTIFIER" && t[i+1].type == "ASSIGN")
        assignment(i);
    else
        i++;

    string L1 = newLabel(), L2 = newLabel();

    cout << L1 << ":\n";

    string cond = condition_expr(i);
    cout << "ifFalse " << cond << " goto " << L2 << "\n";

    i++;

    int incIndex = i;

    while (t[i].type != "RPAREN")
        i++;

    i++; 
    i++; 

    while (t[i].type != "RBRACE" && t[i].type != "EOF")
        statements(i);

    // increment
    if (t[incIndex].type == "IDENTIFIER")
    {
        string var = t[incIndex].lexeme;
        string temp = newTemp();

        if (t[incIndex + 1].type == "INCREMENT")
        {
            cout << temp << " = " << var << " + 1\n";
            cout << var << " = " << temp << "\n";
        }
    }

    cout << "goto " << L1 << "\n";
    cout << L2 << ":\n";

    if (t[i].type == "RBRACE")
        i++;
}

// ---------------- IO (FIXED) ----------------
void io_statement(int &i)
{
    if (t[i].type == "PRINTF")
    {
        int j = i + 2;
        int paramCount = 0;

        while (t[j].type == "STRING" || t[j].type == "IDENTIFIER")
        {
            if (t[j].type == "STRING")
                cout << "param \"" << t[j].lexeme << "\"\n";
            else
                cout << "param " << t[j].lexeme << "\n";

            paramCount++;
            j++;

            if (t[j].type == "COMMA")
                j++;
            else
                break;
        }

        cout << "call printf, " << paramCount << "\n";

        while (t[i].type != "SEMI")
            i++;
        i++;
    }
}

// ---------------- STATEMENTS ----------------
void statements(int &i)
{
    if (t[i].type == "INT")
        declaration(i);

    else if (t[i].type == "IDENTIFIER" &&
             (t[i + 1].type == "INCREMENT" || t[i + 1].type == "DECREMENT"))
        inc_dec(i);

    else if (t[i].type == "IDENTIFIER")
        assignment(i);

    else if (t[i].type == "IF")
        if_statement(i);

    else if (t[i].type == "FOR")
        for_loop(i);

    else if (t[i].type == "PRINTF")
        io_statement(i);

    else if (t[i].type == "RETURN")
    {
        cout << "return\n";
        while (t[i].type != "SEMI")
            i++;
        i++;
    }

    else
        i++;
}

// ---------------- DRIVER ----------------
void intermediate_code()
{
    int i = 0;
    i += 5;

    while (t[i].type != "EOF")
    {
        if (t[i].type == "RBRACE")
        {
            i++;
            break;
        }
        statements(i);
    }

    cout << "Intermediate code generated\n";
}
