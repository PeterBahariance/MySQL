#include "./token_stk.h"

Token_STK::Token_STK()
{
    _token = "";
    _type = -1; // unknown
}

Token_STK::Token_STK(string str, int type)
{
    _token = str;
    _type = type;
}

ostream& operator <<(ostream& outs, const Token_STK& t)
{
    outs <<  "|" << t.token_str() << "|";
    return outs;


}

int Token_STK::type() const
{
    return _type;
}

string Token_STK::type_string() const
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

    else if(_type == 6)
        return "STAR";

    else if(_type == 8)
        return "PARSE";

    else if(_type == 9)
        return "QUOTE";
    
    else if(_type == -1)
        return "UNKNOWN";
    else if(_type == -2)
        return "END";

    else
        return "DOESN'T MATCH ANY TYPES, NOT EVEN TOKEN UNKNOWN (NOT -1)";

}

string Token_STK::token_str() const
{
    return _token;
}

