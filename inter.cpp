#include <iostream>
#include <string>
#include "lexer.h"
using namespace std;

static int tempCount = 1, labelCount = 1;

string newTemp() { return "t" + to_string(tempCount++); }
string newLabel() { return "L" + to_string(labelCount++); }

string factor(int &i)
{
    if (t[i].type == "IDENTIFIER" || t[i].type == "NUMBER")
        return t[i++].lexeme;
    else
    {
        cout << "Invalid factor\n";
        exit(1);
    }
}

string term(int &i)
{
    string left = factor(i);
    while (t[i].type == "STAR" || t[i].type == "SLASH" || t[i].type == "MOD")
    {
        string op = t[i].lexeme;
        i++;
        string right = factor(i);
        string temp = newTemp();
        cout << temp << " = " << left << " " << op << " " << right << endl;
        left = temp;
    }
    return left;
}

string expression(int &i)
{
    string left = term(i);
    while (t[i].type == "PLUS" || t[i].type == "MINUS")
    {
        string op = t[i].lexeme;
        i++;
        string right = term(i);
        string temp = newTemp();
        cout << temp << " = " << left << " " << op << " " << right << endl;
        left = temp;
    }
    return left;
}

string condition_expr(int &i)
{
    string left = expression(i);
    string op = t[i].lexeme;
    i++;
    string right = expression(i);
    string temp = newTemp();
    cout << temp << " = " << left << " " << op << " " << right << endl;
    return temp;
}

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
            cout << name << " = " << res << endl;
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
    cout << name << " = " << res << endl;
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

    if (t[i].type == "INCREMENT")
    {
        i++;
        string temp = newTemp();
        cout << temp << " = " << var << " + 1" << endl;
        cout << var << " = " << temp << endl;
    }
    else if (t[i].type == "DECREMENT")
    {
        i++;
        string temp = newTemp();
        cout << temp << " = " << var << " - 1" << endl;
        cout << var << " = " << temp << endl;
    }

    if (t[i].type == "SEMI")
        i++;
}

void statements(int &i);

void if_statement(int &i)
{
    i += 2;

    string cond = condition_expr(i);
    i++;

    string L1 = newLabel(), L2 = newLabel(), L3 = newLabel();

    cout << "if " << cond << " goto " << L1 << endl;
    cout << "goto " << L2 << endl;

    cout << L1 << ":" << endl;
    i++;

    while (t[i].type != "RBRACE" && t[i].type != "EOF")
    {
        statements(i);
    }

    if (t[i].type == "RBRACE")
        i++;

    if (t[i].type == "ELSE")
    {
        cout << "goto " << L3 << endl;
        cout << L2 << ":" << endl;

        i += 2;

        while (t[i].type != "RBRACE" && t[i].type != "EOF")
        {
            statements(i);
        }

        if (t[i].type == "RBRACE")
            i++;

        cout << L3 << ":" << endl;
    }
    else
    {
        cout << L2 << ":" << endl;
    }
}

void for_loop(int &i)
{
    i += 2;

    while (t[i].type != "SEMI")
        i++;
    i++;

    string L1 = newLabel(), L2 = newLabel();

    cout << L1 << ":" << endl;

    string cond = condition_expr(i);
    cout << "ifFalse " << cond << " goto " << L2 << endl;

    i++;

    int incIndex = i;

    while (t[i].type != "RPAREN" && t[i].type != "EOF")
        i++;

    i++;
    i++;

    while (t[i].type != "RBRACE" && t[i].type != "EOF")
    {
        statements(i);
    }

    if (t[incIndex].type == "IDENTIFIER")
    {
        string var = t[incIndex].lexeme;

        if (t[incIndex + 1].type == "INCREMENT")
        {
            string temp = newTemp();
            cout << temp << " = " << var << " + 1" << endl;
            cout << var << " = " << temp << endl;
        }
        else if (t[incIndex + 1].type == "DECREMENT")
        {
            string temp = newTemp();
            cout << temp << " = " << var << " - 1" << endl;
            cout << var << " = " << temp << endl;
        }
    }

    cout << "goto " << L1 << endl;
    cout << L2 << ":" << endl;

    if (t[i].type == "RBRACE")
        i++;
}

void while_loop(int &i)
{
    i += 2;

    string L1 = newLabel();
    string L2 = newLabel();

    cout << L1 << ":" << endl;

    string cond = condition_expr(i);

    cout << "ifFalse " << cond << " goto " << L2 << endl;

    if (t[i].type == "RPAREN")
        i++;
    if (t[i].type == "LBRACE")
        i++;

    while (t[i].type != "RBRACE" && t[i].type != "EOF")
    {
        statements(i);
    }

    cout << "goto " << L1 << endl;
    cout << L2 << ":" << endl;

    if (t[i].type == "RBRACE")
        i++;
}

void io_statement(int &i)
{
    if (t[i].type == "PRINTF")
    {
        cout << "print " << t[i + 2].lexeme << endl;

        while (t[i].type != "SEMI" && t[i].type != "EOF")
            i++;
        if (t[i].type == "SEMI")
            i++;
    }
    else if (t[i].type == "SCANF")
    {
        i++;
        i++;

        if (t[i].type == "STRING")
            i++;

        if (t[i].type == "COMMA")
            i++;

        if (t[i].type == "AMP")
            i++;

        if (t[i].type == "IDENTIFIER")
        {
            cout << "input " << t[i].lexeme << endl;
            i++;
        }

        while (t[i].type != "SEMI" && t[i].type != "EOF")
            i++;
        if (t[i].type == "SEMI")
            i++;
    }
}

void statements(int &i)
{
    if (t[i].type == "INT")
        declaration(i);

    else if (t[i].type == "IDENTIFIER" &&
             (t[i + 1].type == "INCREMENT" || t[i + 1].type == "DECREMENT"))
    {
        inc_dec(i);
    }

    else if (t[i].type == "IDENTIFIER")
        assignment(i);

    else if (t[i].type == "IF")
        if_statement(i);

    else if (t[i].type == "FOR")
        for_loop(i);

    else if (t[i].type == "WHILE")
        while_loop(i);

    else if (t[i].type == "PRINTF" || t[i].type == "SCANF")
        io_statement(i);

    else if (t[i].type == "RETURN")
    {
        i++;
        if (t[i].type == "IDENTIFIER" || t[i].type == "NUMBER")
            i++;
        if (t[i].type == "SEMI")
            i++;
    }

    else if (t[i].type == "RBRACE")
    {
        return;
    }

    else
    {
        i++;
    }
}

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
