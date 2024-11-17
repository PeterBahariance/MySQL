#ifndef TOKEN_STK_H
#define TOKEN_STK_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

class Token_STK
{
public:
    Token_STK();
    Token_STK(string str, int type);
    friend ostream& operator <<(ostream& outs, const Token_STK& t);
    int type() const;
    string type_string() const;
    string token_str() const;
private:
    string _token;
    int _type;
};

#endif 
