#ifndef TABLE_H
#define TABLE_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <fstream>
#include <string>

#include "../../includes/table/typedefs.h"
#include "../../includes/queue/MyQueue.h"
#include "../../includes/stack/MyStack.h"

#include "../../includes/tokenizor/subToken.h"
#include "../../includes/bplustree/bplustree.h"
#include "../../includes/bplustree/map.h"
#include "../../includes/bplustree/multimap.h"
#include "../../includes/binary_files/file_record.h"




class Table
{
    public:


    Table();
    Table(string tableName);


    Table(string tableName, vectorstr fieldTypes);

    void insert_into(vectorstr row);
    Table select(vectorstr names, string temp1, string temp2, string temp3);

    friend ostream& operator<<(ostream& outs,
                               const Table& r);


    vector<long> select_recnos();

    Table select(vectorstr name, Queue<Token *> tempQueue);

    Table select(vectorstr name, vector<string> tempString);
    
    Token *evaluate(Token *value, Token *fieldName, Token *relation);
    Token *evaluateUnion(Token *set1, Token *set2,  Token *relation);

    void insert_sort_function(vector<long> &a, unsigned int size);

    void printQueue(Queue<Token *> tempQueue);

    vector<int> convertVector(string stringVec);


    vector<string> getFieldNames();



    string nameOfDataFile; 
    bool existingFile;


    private:
        // Basic Table Info

        static int serialNumber;

        // string nameOfDataFile; 
        int dataRecordNumbers = -1;
        int fieldRecordNumbers = 0;

        // variable for file name
        // file for actual filename with .bin appended to it 


        vector<MMap<string, long>> indicies;
        vector<string> vectorFieldNames;
        vector<long> recordNumbersVector;

        Map<string, int> bucketVariable;
        
        FileRecord dataBase;
        FileRecord fieldBaseRecord;
        


}; 



#endif 