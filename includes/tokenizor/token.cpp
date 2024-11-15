#include "./token.h"

Token::Token()
{
    _token = "";
    _type = -1; // unknown
}

Token::Token(string str, TokenType type)
{
    _token = str;
    _type = type;
}

ostream& operator <<(ostream& outs, const Token& t)
{
    outs <<  "|" << t.token_str() << "|";
    return outs;


}

int Token::type() const
{
    return _type;
}

string Token::type_string() const
{
    if(_type == 1)
        return "NUMBER";
    else if(_type == 2)
        return "ALPHA";
    else if(_type == 3)
        return "SPACE";
    else if(_type == 4)
        return "OPERATOR";
    else if(_type == 5)
        return "PUNC";

    

    else if(_type == -1)
        return "UNKNOWN";
    else if(_type == -2)
        return "END";

    else
        return "DOESN'T MATCH ANY TYPES, NOT EVEN TOKEN UNKNOWN (NOT -1)";

}

string Token::token_str() const
{
    return _token;
}

