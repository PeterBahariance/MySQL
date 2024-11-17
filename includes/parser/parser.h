#ifndef PARSER_H
#define PARSER_H

#include "../../includes/table/typedefs.h"
#include "../../includes/queue/MyQueue.h"
#include "../../includes/stack/MyStack.h"


#include "../../includes/tokenizor/subToken.h"
#include "../../includes/bplustree/bplustree.h"
#include "../../includes/bplustree/map.h"
#include "../../includes/bplustree/multimap.h"
#include "../../includes/binary_files/file_record.h"

#include "../../includes/stokenizer/state_machine_functions.h"
#include "../../includes/stokenizer/constants3.h"
#include "../../includes/stokenizer/stokenize.h"



#include "../../includes/parser/constants2.h"



#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>

using namespace std;

//int Parser::_tableParser[100][100];


class Parser
{

public:
    Parser(char* s){

        //set_string(s);

        tokenBuffer = string(s);

        // cout << "This is the tokenBuffer:" << tokenBuffer << endl;
        STokenizer stk(s);
        Token_STK t;
        stk>>t;
        while(stk.more()){
            //process token here...
            //<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;

            if(t.type() != TOKEN_SPACE_PARSER)
                tokens.push_back(t.token_str());

            t = Token_STK();

            stk>>t;
        }

     
        make_table_parser(adjacencyTable);

        
        _keyWords = mapKeys();

       
       


    }

    map_sl mapKeys()
    {
       
        


        map_sl newKeys;
        for(int i = 0; i < tokens.size(); i++)
        {
            if(tokens[i] == "select")
                newKeys["select"] = SELECT;
            else if(tokens[i] == "from")
                newKeys["from"] = FROM;
            else if(tokens[i] == "*")
                newKeys["*"] = STAR;
            else if(tokens[i] == "where")
                newKeys["where"] = WHERE;
            else if(tokens[i] == "insert")
                newKeys["insert"] = INSERT;
            else if(tokens[i] == "into")
                newKeys["into"] = INTO;
            else if(tokens[i] == "values")
                newKeys["values"] = VALUES;
            else if(tokens[i] == "create")
                newKeys["create"] = CREATE;
            else if(tokens[i] == "make")
                newKeys["make"] = MAKE;
            else if(tokens[i] == "table")
                newKeys["table"] = TABLE;
            else if(tokens[i] == "fields")
                newKeys["fields"] = FIELDS;
            
            else if(tokens[i] == ",")
                newKeys[tokens[i]] = COMMA;

            else 
                newKeys[tokens[i]] = SYMBOLS;    

          

        }


        return newKeys;
    }

    mmap_ss parse_tree()
    {
        int commaCount = 0;
        int fieldCount = 0;
        mmap_ss ourPtree;
        int startState = 0;
        int priorStartState;
        int i = 0;

        while(i < tokens.size())
        {
            // cout << "Here is the corresponding start state: " << startState << endl;
            // cout << "Here is the corresponding token map: " << _keyWords[tokens[i]] << endl;
            // cout << "next state: "<< adjacencyTable[startState][_keyWords[tokens[i]]] << endl;

            // keeps track of the last start state 
             priorStartState = startState;
        
            

            if(adjacencyTable[startState][_keyWords[tokens[i]]] != -1)
            {
               // cout << "entering our if statement" << endl;
                startState = adjacencyTable[startState][_keyWords[tokens[i]]];




                //cout << tokens[i] << ": " << startState << endl;

                
                if(startState == 1 || startState == 8 || startState == 13)
                    ourPtree["command"].push_back(tokens[i]);

                else if(startState == 2)
                {
                    if(tokens[i] != ",")
                        ourPtree["fields"].push_back(tokens[i]);
                }

                else if(startState == 3)
                {
                    if(tokens[i] == ",")
                        commaCount++;
                    else    
                        fieldCount++;
                   
                    if(tokens[i] != ",")
                    ourPtree["fields"].push_back(tokens[i]);
                }

                else if(startState == 4)
                {
                    if(commaCount >= fieldCount && commaCount != 0 && fieldCount != 0)
                        throw "Missing a field name";
                    
                    else if(fieldCount > commaCount+1)
                        throw "Missing a comma";
                }

                else if(startState == 5 || startState == 10 || startState == 15)
                    ourPtree["table_name"].push_back(tokens[i]);    

                else if(startState == 6)
                    ourPtree["where"].push_back("yes");

                else if(startState == 7)
                    ourPtree["condition"].push_back(tokens[i]);

                else if(startState == 12)
                {
                    if(tokens[i] != ",")
                    ourPtree["values"].push_back(tokens[i]);
                }

                else if(startState == 17)
                {
                    if(tokens[i] != ",")
                    ourPtree["col"].push_back(tokens[i]);
                }

            }

            else
            {
                flagCondition = false;

                if(priorStartState == 0)
                    throw "INVALID COMMAND, you must start your command with either make, create, insert, or select!";
               
                else if(priorStartState == 1)
                    throw "INVALID COMMAND, the select command must be followed by either * for all fields, or the specified field names";

               // else if(priorStartState == 3 && startState == 3)
                    //throw "INVALID COMMAND, missing a comma for the fields";

                else if(priorStartState == 3)
                    throw "INVALID COMMAND, the field names must be followed by the keyword: from";
               
               else if(priorStartState == 4)
                    throw "INVALID COMMAND, missing the table name";

                else if(priorStartState == 5)
                    throw "INVALID COMMAND, commands followed after the table name for select must be where";

                else if(priorStartState == 6)
                    throw "INVALID COMMAND, the conditions are missing";
               
               else if(priorStartState == 8)
                    throw "INVALID COMMAND, commands followed after the insert MUST be into";

                else if(priorStartState == 9)
                    throw "INVALID COMMAND, missing the table name";

                else if(priorStartState == 10)
                    throw "INVALID COMMAND, table name must be followed by the keyword: values";

                else if(priorStartState == 13)
                    throw "INVALID COMMAND, key word create/make MUST be followed by keyword: table";

                else if(priorStartState == 15)
                    throw "INVALID COMMAND, table name MUST be followed up keyword: fields";
               else if(priorStartState == 18 && startState == 18)
                    throw "INVALID COMMAND, missing a field name";


                return ourPtree;

            }



            i++;
        }

        // these are all success states, if we did not end with one of these, we know that we have an issue
        if(startState != 5 && startState != 7 && startState != 12 && startState != 17)
        {
            flagCondition = false;
            // throw "INVALID COMMAND, command is incomplete";
            if(startState == 0)
                    throw "INVALID COMMAND, you must start your command with either make, create, insert, or select!";
               
                else if(startState == 1)
                    throw "INVALID COMMAND, the select command must be followed by either * for all fields, or the specified field names";
               
                else if(startState == 3)
                    throw "INVALID COMMAND, the field names must be followed by the keyword: from";
               
               else if(startState == 4)
                    throw "INVALID COMMAND, missing the table name";

                else if(startState == 5)
                    throw "INVALID COMMAND, commands followed after the table name for select must be where";
               
                else if(startState == 6)
                    throw "INVALID COMMAND, the conditions are missing";

               else if(startState == 8)
                    throw "INVALID COMMAND, commands followed after the insert MUST be into";

                else if(startState == 9)
                    throw "INVALID COMMAND, missing the table name";

                else if(startState == 10)
                    throw "INVALID COMMAND, table name must be followed by the keyword: values";

                else if(startState == 13)
                    throw "INVALID COMMAND, key word create/make MUST be followed by keyword: table";

                else if(startState == 15)
                    throw "INVALID COMMAND, table name MUST be followed up keyword: fields";
               

            
        }
        else
            flagCondition = true;
        
        ourPtree["where"].push_back("no");
        
        
        
        return ourPtree;
    }

    void set_string(char* s)
    {
        tokens.clear();
        tokenBuffer = string(s);
        STokenizer stk(s);
        Token_STK t;
        stk>>t;
        while(stk.more()){
            //process token here...

            if(t.type() != TOKEN_SPACE_PARSER && t.type() != TOKEN_PUNC_PARSER)
            tokens.push_back(t.token_str());

            t = Token_STK();

            stk>>t;
        }


       

        _keyWords = mapKeys();
        

    }

 
   

    void make_table_parser(int passedInTable[][MAX_COLUMNS])
    {
        init_table(passedInTable);

        mark_cell(0, passedInTable, SELECT, 1);
        mark_cell(0, passedInTable, INSERT, 8);
        mark_cell(0, passedInTable, CREATE, 13);
        mark_cell(0, passedInTable, MAKE, 13);


        mark_cell(1, passedInTable, STAR, 2);
        mark_cell(1, passedInTable, SYMBOLS, 3);

        mark_cell(2, passedInTable, FROM, 4);

        mark_cell(3, passedInTable, FROM, 4);
        mark_cell(3, passedInTable, SYMBOLS, 3);
        mark_cell(3, passedInTable, COMMA, 3);

        mark_cell(4, passedInTable, SYMBOLS, 5);

        mark_cell(5, passedInTable, WHERE, 6);

        mark_cell(6, passedInTable, SYMBOLS, 7);

        mark_cell(7, passedInTable, SYMBOLS, 7);

        mark_cell(8, passedInTable, INTO, 9);

        mark_cell(9, passedInTable, SYMBOLS, 10);

        mark_cell(10, passedInTable, VALUES, 11);

        mark_cell(11, passedInTable, SYMBOLS, 12);
       
        mark_cell(12, passedInTable, SYMBOLS, 12);
        mark_cell(12, passedInTable, COMMA, 12);

       
        mark_cell(13, passedInTable, TABLE, 14);

        mark_cell(14, passedInTable, SYMBOLS, 15);

        mark_cell(15, passedInTable, FIELDS, 16);

        mark_cell(16, passedInTable, SYMBOLS, 17);

        mark_cell(17, passedInTable, SYMBOLS, 17);
        mark_cell(17, passedInTable, COMMA, 17);



 



        // MAKE SURE TO MARK SUCCESS STATES!
        mark_cell(0, passedInTable, 0, 0);
        mark_cell(1, passedInTable, 0, 0);
        mark_cell(2, passedInTable, 0, 0);
        mark_cell(3, passedInTable, 0, 0);
        mark_cell(4, passedInTable, 0, 0);
        mark_cell(5, passedInTable, 0, 0);
        mark_cell(6, passedInTable, 0, 0);
        mark_cell(7, passedInTable, 0, 1);
        mark_cell(8, passedInTable, 0, 0);
        mark_cell(9, passedInTable, 0, 0);
        mark_cell(10, passedInTable, 0, 0);
        mark_cell(11, passedInTable, 0, 0);
        mark_cell(12, passedInTable, 0, 1);
        mark_cell(13, passedInTable, 0, 0);
        mark_cell(14, passedInTable, 0, 0);
        mark_cell(15, passedInTable, 0, 0);
        mark_cell(16, passedInTable, 0, 0);
        mark_cell(17, passedInTable, 0, 1);
        //mark_cell(18, passedInTable, 0, 0);






    }

    void printMMap()
    {
        cout << "Printing our Parse Tree:" << endl;
        ptree.print_lookup();

    }


   

    bool flagCondition;


private:
    
    mmap_ss ptree;
    map_sl _keyWords;
    int adjacencyTable[MAX_ROWS][MAX_COLUMNS];

    string tokenBuffer;
    vector<string> tokens;





};





#endif