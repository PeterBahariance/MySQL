#ifndef CONSTANTS3_H
#define CONSTANTS3_H

#include <iostream>
using namespace std;

const int MAX_COLUMNS = 256;
const int MAX_ROWS = 100;
const int MAX_BUFFER = 1000;

const char ALFA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char DIGITS[] = "0123456789";
const char OPERATORS[] = "><=!+-%&|";
const char SPACES[] = {' ', '\t', '\n', '\0'};
const char PUNC[] = "?.,:;'`~!";
const char PARSE[] = "*()";
const char QUOTES[] = "\"";




const int START_DOUBLE = 0;
const int START_SPACES = 4;
const int START_ALPHA = 6;
const int START_OPERATOR = 20;
const int START_PUNC = 10;

//token types:
const int TOKEN_NUMBER_PARSER = 1;
const int TOKEN_ALPHA_PARSER = 2;
const int TOKEN_SPACE_PARSER = 3;
const int TOKEN_OPERATOR_PARSER = 4;
const int TOKEN_PUNC_PARSER = 5;
const int TOKEN_STAR_PARSER = 6;

const int TOKEN_PARSE_PARSER = 8;
const int TOKEN_QUOTE_PARSER = 9;


const int TOKEN_UNKNOWN_PARSER = -1;
const int TOKEN_END_PARSER = -2;

#endif 