#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>

using namespace std;

enum TokenType
{
    TOKEN,
    LPAREN,
    RPAREN,
    TOKEN_STR,
    OPERATOR,
    RELATIONAL,
    LOGICAL,
    RESULT_SET,
    TOKEN_NUMBER,
    TOKEN_ALPHA,
    TOKEN_SPACE,
    TOKEN_OPERATOR,
    TOKEN_PUNC,

    TOKEN_UNKNOWN,
    TOKEN_END
};



#endif