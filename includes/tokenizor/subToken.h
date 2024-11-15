#ifndef SUBTOKEN_H
#define SUBTOKEN_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>

#include "token.h"
#include "constants.h"

using namespace std;




class TokenStr:public Token{

public:

    TokenStr() : Token()
    {

    }

    TokenStr(string str) : Token(str, TOKEN_STR) // TOKEN_STR is 3
    {

    }

    virtual TokenType returnType()
    {
        return TOKEN_STR;
    }

    

private:
    //string Value

};

class LParen:public Token{

public:

    LParen()
    {

    }

    LParen(string str)
    {

    }

private:
    Token temp;


};

class RParen:public Token{

public:

    RParen()
    {

    }

    RParen(string str)
    {

    }

private:
    Token temp;



};

class ResultSet:public Token{

public:

    ResultSet()
    {

    }

    ResultSet(string str)
    {

    }

private:
    Token temp;



};

class Operator:public Token{

public:

    Operator() : Token()
    {

    }


    Operator(string str) : Token(str, OPERATOR) // TOKEN_STR is 3
    {

    }

    virtual TokenType returnType()
    {
        return OPERATOR;
    }
    
   



private:



};

class Relational:public Operator{


public: 
   Relational() : Operator()
    {

    }

    Relational(string str) : Operator(str) 
    {

    }

    virtual TokenType returnType()
    {
        return RELATIONAL;
    }

   


private:


};

class Logical:public Token{

public:

    Logical() : Token()
    {

    }

    Logical(string str) : Token(str, LOGICAL) // TOKEN_STR is 3
    {

    }

    virtual TokenType returnType()
    {
        return LOGICAL;
    }

private:
    //Operator temp;


};

#endif
