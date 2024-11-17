#include "./state_machine_functions.h"


//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS])
{
    for(int i = 0; i < MAX_ROWS; i++)
        for(int j = 0; j < MAX_COLUMNS; j++)
            _table[i][j] = -1;
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 1;
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 0;

}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state)
{
    return (_table[state][0] == 1);
}

//Mark a range of cells in the array. 
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state)
{
    if(from < 0 || to > 256)
    {
        cout << "Either from is less than 0, or to is greater than 256" << endl;
        return;
    }

    else
    {
    for(int j = from; j < to; j++)
        _table[row][j] = state;
    }
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state)
{
    // strlen gives us the size of the CString "columns"
    for(int i = 0; i < strlen(columns); i++)
    {
        _table[row][columns[i]] = state;

    }
}

//Mark this row and column
void mark_cell(int row, int table[][MAX_COLUMNS], int column, int state)
{
    if(row < 0 || row > 99 || column < 0 || column > 255)
    {
        cout << "Either row is less than 0 or greater than 99. OR, column is greater than 255 or less than 0." << endl;
        return;
    }
    else
        table[row][column] = state;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS])
{
    for(int i = 0; i < MAX_ROWS; i++)
    {
        for(int j = 0; j < MAX_COLUMNS; j++)
        {
            cout << _table[i][j] << " ";
        }
        cout << endl;
        
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//       ^
void show_string(char s[], int _pos)
{
    if(_pos > strlen(s))
        cout << "The position you want to find is greater than the amount of characters!" << endl;

    else if(_pos < 1)
        cout << "The position passed in was less than 1!" << endl;

    else
    {
        cout << s << endl;
        for(int i = 1; i < _pos; i++)
            cout << " ";
        
        cout << "^" << endl;
    }
}