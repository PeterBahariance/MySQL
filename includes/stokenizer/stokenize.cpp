#include "stokenize.h"

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer()
{
    _pos = 0;
    strcpy(_buffer, ""); // copies an empty string into buffer
}

STokenizer::STokenizer(char str[])
{
   _pos = 0;
    strcpy(_buffer, str); // copies the passed in cstring into the buffer

    //_buffer[strlen(_buffer)] = '\0';
    //_buffer[strlen(_buffer)] = '\0'; 



    //cout << (strlen(_buffer)) << endl;

}

bool STokenizer::done()
{
    if(!(_pos >(strlen(_buffer))))
        return true;
    else
        return false;
}   

bool STokenizer::more()          
{
    //cout << "position: " << _pos << " and buffer length: " <<  (strlen(_buffer)) << endl;
    if((_pos <= (strlen(_buffer))))
    {
        return true;
    }
    else
    {
        //cout << "WE ARE DONE" << endl;

        return false;
    }
}


// extraction opeator 
STokenizer& operator >> (STokenizer& s, Token_STK& t)
{
    if(s._pos < strlen(s._buffer) && s._buffer[s._pos] != '\0')
    {
        int stringType;
        string updatedToken;
        int startState = 0; // we start at row zero of our table


        s.get_token(startState, updatedToken);



        t = Token_STK(updatedToken, startState);
    }
    else
        s._pos++; // to move us past that last null character, we dont print use get_token on it, and just increment position

    return s;
}


// sets the current buffer to the passed in string 
void STokenizer::set_string(char str[])
{
    strcpy(_buffer, str);
    //cout << "THIS IS THE BUFFER of stk: " << _buffer << endl;
    // cout << "The length of stk's BUFFER: " << strlen(_buffer) << endl;
     _pos = 0;
    
}

string STokenizer::getString()
{
    return _buffer;
    
}


void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
    init_table(_table);
    // first we must initialize the first row, 
    mark_cells(0, _table, DIGITS, 1);
    mark_cells(0, _table, ALFA, 2);
    mark_cells(0, _table, SPACES, 3);
    mark_cells(0, _table, OPERATORS, 4);
    mark_cells(0, _table, PUNC, 5);
    mark_cells(0, _table, PARSE, 8);
    mark_cells(0, _table, QUOTES, 9);



    // initialize the rest of the rows 
    mark_cells(1, _table, DIGITS, 1);
    mark_cell(1, _table, '.', 6);
    mark_cells(6, _table, DIGITS, 7);
    mark_cell(6, _table, 0, 2);
    mark_cell(7, _table, 0, 1);
    mark_cells(7, _table, DIGITS, 7);



    mark_cells(2, _table, ALFA, 2);

    mark_cells(3, _table, SPACES, 3);

    mark_cells(4, _table, OPERATORS, 4);

    mark_cells(5, _table, PUNC, 5);

    mark_cells(9, _table, DIGITS, 9);
    mark_cells(9, _table, ALFA, 9);
    mark_cells(9, _table, SPACES, 9);
    mark_cells(9, _table, OPERATORS, 9);
    mark_cells(9, _table, PUNC, 9);
    mark_cells(9, _table, PARSE, 9);
    mark_cells(9, _table, QUOTES, 10);

   



    // inititalize the truth states 
    mark_cell(0, _table, 0, 0);
    mark_cell(1, _table, 0, 1);
    mark_cell(2, _table, 0, 1);
    mark_cell(3, _table, 0, 1);
    mark_cell(4, _table, 0, 1);
    mark_cell(5, _table, 0, 1);
    mark_cell(8, _table, 0, 1);
    mark_cell(9, _table, 0, 0);
    mark_cell(10, _table, 0, 1);
   
}

bool STokenizer::get_token(int& start_state, string& token)
{
    // start_start is the type 
    // input is the same as our character buffer (_buffer)
    // pos matches _pos (our current position in the string)
    // start_state is the where we are continuing in the string
    // table[maxRow][maxColumn] is the same as _table[MAX_ROWS][MAX_COLUMNS]
    // token matches the token in this function

    int displayFrom = _pos;
   
    make_table(_table);
    //cout << "THIS IS THE VALUE OF . for digits " << _table[1]['.'] << endl;
    //strcpy(_buffer, "hello-there");
    int valid_pos = 0;
    
    while(_pos < strlen(_buffer))
    {
        //cout << "current parse: " << _buffer[_pos] << endl;



        if(_table[start_state][_buffer[_pos]] != -1)
        {
            

            // moves us over to the row depending on what the first input was
            start_state = _table[start_state][_buffer[_pos]];

            if(_table[start_state][0] == 1)
            {
                valid_pos = _pos;
            }

   
        }

        else
        {
            if(_pos != 0)
            {
                break;
            }
            else
                return false;
        }
        _pos++;
    }

    string buffer(_buffer);

    //cout << "This is buffer:" << buffer[valid_pos] << endl;

    start_state = _table[0][_buffer[valid_pos]];
   
    _pos = (valid_pos + 1); 

   // cout << "This is the start state: " << start_state << endl;
     
    token = buffer.substr(displayFrom, ((valid_pos+1)-displayFrom));

    if(token[0] == '\"')
    {
        //cout << "WE MUST SHORTEN THE TOKEN FOR THE QUOTES" << endl;
        token = token.substr(1, (token.length())-2);
    }

    //cout << "Here is the corresponding token: " << endl;
    //cout << token << endl;

    return true;

}


