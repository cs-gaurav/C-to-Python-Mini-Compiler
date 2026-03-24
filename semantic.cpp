#include <iostream>
#include <unordered_map>
#include <string>
#include "lexer.h"

using namespace std;

struct Var {
    bool declared = false;
    bool initialized = false;
};

unordered_map<string, Var> table;

void error(string msg, string name = "") {
    cout << "Error: " << msg;
    if (name != "") cout << " -> " << name;
    cout << endl;
    exit(1);
}
void checkVar(string name) {
    if (table.find(name) == table.end())
        error("Not declared", name);

    if (!table[name].initialized)
        error("Not initialized", name);
}

int expression(int i) {
    if (t[i].type == "IDENTIFIER") {
        checkVar(t[i].lexeme);
    } else if (t[i].type != "NUMBER") {
        error("Invalid expression");
    }
    i++;
    while (t[i].type == "PLUS" || t[i].type == "MINUS") {
        i++;
        if (t[i].type == "IDENTIFIER") {
            checkVar(t[i].lexeme);
        } else if (t[i].type != "NUMBER") {
            error("Invalid expression");
        }
        i++;
    }
    return i;
}
void semantic_analyzer() {
    int i = 0;
    i += 5;
    while (t[i].type != "EOF" && t[i].type != "RBRACE") {
        if (t[i].type == "INT") {
            i++;
            string name = t[i].lexeme;
            table[name].declared = true;
            i++;
            if (t[i].type == "ASSIGN") {
                i++;
                i = expression(i);
                table[name].initialized = true;
            }

            if (t[i].type != "SEMI")
                error("Missing ;");
            i++;
        }
        else if (t[i].type == "IDENTIFIER") {
            string name = t[i].lexeme;
            if (table.find(name) == table.end())
                error("Undeclared variable", name);
            i += 2;
            i = expression(i);
            table[name].initialized = true;
            if (t[i].type != "SEMI")
                error("Missing ;");
            i++;
        }
        else if (t[i].type == "RETURN") {
            i++;
            if (t[i].type == "IDENTIFIER")
                checkVar(t[i].lexeme);
            else if (t[i].type != "NUMBER")
                error("Invalid return");
            i++;
            if (t[i].type != "SEMI")
                error("Missing ;");
            i++;
        }
        else {
            error("Invalid statement");
        }
    }
    cout << "Semantic analysis done\n";
}