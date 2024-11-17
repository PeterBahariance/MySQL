#include "../../includes/table/typedefs.h"
#include "../../includes/queue/MyQueue.h"
#include "../../includes/stack/MyStack.h"
#include "../../includes/parser/parser.h"
#include "../../includes/table/table.h"
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
#include <algorithm>
using namespace std;
class SQL
{
    public:

        SQL()
        {

            vector<string> existingTableNames;
            cout << "SQL: Currently tracking these tables:" << endl;
            cout << "-------------------------------------" << endl;
           
           
            ifstream inputFile("existingTables.txt");

            if (!inputFile.is_open()) {
                cout << "No existing tables" << endl;
                cout << endl << endl;
            } 
            
            else 
            {
                string tableName;
                while(getline(inputFile, tableName))
                {
                    existingTableNames.push_back(tableName);
                }                    // Read from the file
                                    
                inputFile.close();


                for(int i = 0; i < existingTableNames.size(); i++)
                {
                    cout << existingTableNames[i] << endl;

                    Table priorTable(existingTableNames[i]);
                    //cout << priorTable << endl;
                    
                    ExistingTables.insert(existingTableNames[i], priorTable);

                }
                
                cout << endl << endl;
            }


        }


        void batch()
        {
            const vector<string> command_list = {

                /*00*/     "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
                /*01*/     "select lname fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
                /*02*/     "select lname, , major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
                /*03*/     "select lname, fname, major  student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
                /*04*/     "select lname, fname, major  student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",
                /*05*/     "select lname, fname, major from  where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",

                /*06*/     "select lname, fname, major from student where",

                /*07*/     "select lname, fname, major from student where (lname=Yang or major=CS) and age<23 )or lname=Jackson",

                /*08*/     "select lname, fname, major from student where ((lname=Yang or major=CS and age<23 )or lname=Jackson",

                /*09*/     "select lname, fname, major from student where ((lname= or major=CS) and age<23 )or lname=Jackson",

                /*10*/     "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",


                /*11*/     "select lname, fname, major from student where ((lname=Yang  major=CS) and age<23 )or lname=Jackson",


                /*12*/     "select lname, fname, major from student where ((lname=Yang or ) and age<23 )or lname=Jackson",


                /*13*/     "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or ",


                /*14*/     "select lname, fname, major from student where ((lname=Yang or major=CS) and age<23 )or lname=Jackson",

            };

            for(int i = 0; i < command_list.size(); i++)
            {
                 cout << command(command_list[i]) << endl;

                if(!emptyRec())
                cout << "Here are the recnos: " << select_recnos() << endl;

                else  
                cout << "No corresponding records!" << endl;
                        }
        }

        Table command(string cmd)
        {
       
                      
            try
            {
               
        
            //cout << "Here is our command as a string: "<< cmd << endl;
            char cmdChar[cmd.length()];
            // char *cmdChar = new char[cmd.length()];
            strcpy(cmdChar, cmd.c_str());
            //cout << "Here is our command: "<< cmdChar << endl;
            Parser getString(cmdChar);
            //cout << "Have we crashed 3" << endl;

            mmap_ss parsedTable;

            parsedTable = getString.parse_tree();
            //cout << "Have we crashed 4" << endl;
            if(!getString.flagCondition)
            {
                Table temp;
                SQLrecordNumbers = temp.select_recnos();

                return temp;
            }
            
            //cout << "Here is the parse tree:" << endl;
            //parsedTable.print_lookup();
            if(parsedTable["command"][0] == "make" || parsedTable["command"][0] == "create")
            {
                //cout << "We are making a table!" << endl;


                Table temp(parsedTable["table_name"][0], parsedTable["col"]);
                ExistingTables.insert(parsedTable["table_name"][0], temp);

                
                ofstream existingTablesFile;
                existingTablesFile.open("existingTables.txt", ios::app);

                if (existingTablesFile.is_open()) 
                {
                    existingTablesFile << parsedTable["table_name"][0] << endl;
                    existingTablesFile.close();    

                }

                else
                {
                    cout << "Unable to open file" << endl;
                }
                // if we are making a table, then add that to the file


                SQLrecordNumbers = temp.select_recnos();


                //cout << "The table is finished being made!" << endl;
                return temp;
            }
            else if(parsedTable["command"][0] == "insert")
            {
                //cout << "We are inserting into the table!" << endl;

                if(!ExistingTables.contains(parsedTable["table_name"][0]))
                {
                    cout << "This table does not exist!" << endl;
                    Table tempTable;
                    SQLrecordNumbers = tempTable.select_recnos();

                    return tempTable;
                }
                    
                Table insertTable = ExistingTables[parsedTable["table_name"][0]];
                //Table insertTable(parsedTable["table_name"][0]);
                insertTable.insert_into(parsedTable["values"]);
                SQLrecordNumbers = insertTable.select_recnos();

                ExistingTables.insert(parsedTable["table_name"][0], insertTable);
                return insertTable;
            }
            else if(parsedTable["command"][0] == "select")
            {

                Table originalTable = ExistingTables[parsedTable["table_name"][0]];


                //Table selectedTable(parsedTable["table_name"][0]);
                
               // cout << "Here our are field names: " << selectedTable.getFieldNames() << endl;
                
                if(parsedTable["fields"][0] == "*" && parsedTable["where"][0] != "yes")
                {
                    // gets every field from the table
                    // Table temp;
                    // return temp;
                    // cout << "The version of select was called where we just have * and no condition" << endl;
                    //cout << "This is the table name: " << parsedTable["table_name"] << endl;
                   
                    // gets everything from the original table
                   

                    SQLrecordNumbers = originalTable.select_recnos();

                    //cout << "About to call in the 3 parametered select" << endl;
                    //cout << "This is the field " << parsedTable["fields"] << endl;
                    Table selectedTable = originalTable.select(parsedTable["fields"], "", "", "");        


                   // Table selectedTable;
                    //cout << selectedTable << endl;
                    return selectedTable;

                }


                else if(parsedTable["fields"][0] == "*" && parsedTable["where"][0] == "yes")
                {
                    //cout << "The version of select was called where we just have * and WE HAVE condition" << endl;
                    // gets every field from the table 

                    // is this where our map of tables could come into play??

                
                    //cout << "Here are all the prior recnos:" << SQLrecordNumbers << endl;
                    // its the original tables field names since its not specifying the field names
                    Table selectedTable = originalTable.select(originalTable.getFieldNames(), parsedTable["condition"]);
                    SQLrecordNumbers = selectedTable.select_recnos();

                    return selectedTable;
                }
                // NO CONDITIONS
                else if(parsedTable["fields"][0] != "*" && parsedTable["where"][0] != "yes")
                {
                    //cout << "The version of select was called where we just have FIELDS and no condition" << endl;
                    // gets specific records from the table
                    //cout << "Here are all our parsed fields: " << parsedTable["fields"] << endl;
                   

                    SQLrecordNumbers = originalTable.select_recnos();
                    Table selectedTable = originalTable.select(parsedTable["fields"], "", "", "");        
                    //selectedTable.select()
                    
                    //cout << selectedTable << endl;
                    return selectedTable;
                }
                
                else if(parsedTable["fields"][0] != "*" && parsedTable["where"][0] == "yes")
                {
                    // gets every field from the table 
                  
                    //cout << "Here are the fields: " << parsedTable["fields"] << endl;

                    Table selectedTable = originalTable.select(parsedTable["fields"], parsedTable["condition"]);
                    SQLrecordNumbers = selectedTable.select_recnos();


                    return selectedTable;
                }

            // }
            
            
        
            
            }

            }

            catch(const char * txtException)
            {
                cout << "Exception: " << txtException << endl;
                Table temp;
                SQLrecordNumbers = temp.select_recnos();
                return temp;
            }


            Table temp;
            return temp;
            
            
            
        }
        vector<long> select_recnos()
        {
            return SQLrecordNumbers;
        }
        bool emptyRec()
        {
            return SQLrecordNumbers.empty();

        }

    private:

    vector<long> SQLrecordNumbers;
    Map<string, Table> ExistingTables;


};
