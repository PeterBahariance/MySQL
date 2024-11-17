#include "../../includes/table/table.h"
#include <sstream>
#include <algorithm>
int Table::serialNumber = 0;
Table::Table()
{
    // We cannot have a table that has NO parameters
    assert("Cannot be empty! Check Table default constructor!!");
}
Table::Table(string tableName)
{   

    nameOfDataFile = tableName;
    vector<long> createdTableRecords;

    // sets our dataRecordNumbers to -1, that is so that if a table has nothing in it,
    // we know hit a case in our insertion operator that does (return outs) when its first called
    dataRecordNumbers = -1;

    // We will be reading from an already existing bin and txt file
    string binFile = tableName + ".bin";
    string txtFile = tableName + ".txt";


    char charBinFile[binFile.length()+1];
    char charTxtFile[txtFile.length()+1];

    strcpy(charBinFile, binFile.c_str());
    strcpy(charTxtFile, txtFile.c_str());

    
    

    // opens the txt (fields file), and stores our fields into our fields vector member variable
    ifstream readFromTxt(charTxtFile);
    string grabFields;

    while(getline(readFromTxt, grabFields))
    {
        vectorFieldNames.push_back(grabFields);
        fieldRecordNumbers++; 
    }

    // we now have everything into our txt file
    readFromTxt.close();

    for(int i = 0; i < fieldRecordNumbers; i++)
    {
        // sizes our indicides (which is our member variable of vector of multimaps)
        MMap<string, long> newMMap;
        indicies.push_back(newMMap);

        // Map each field name to a numeric value
        bucketVariable[vectorFieldNames[i]] = i;
    }

    fstream recordsFile;

    // opens our binary file 
    dataBase.open_fileRW(recordsFile, charBinFile);

    
    long recordCount = 0;
    for(int i = 0; i < (sizeof(dataBase._record)/sizeof(dataBase._record[0])); i++)
    {   
         // read from the file, depending on the position from i
        dataBase.read(recordsFile, i);
        if(strlen(dataBase._record[0]) <= 0)
        {
            // its -1 since we dont want to include this iteration of the loop!
            dataRecordNumbers = (i-1); 
            
            break;
        }
        
        vector<string> newRowValues;
        for(int i = 0; i < fieldRecordNumbers; i++)
        {
            // push into our specific value into our vector
            newRowValues.push_back(string(dataBase._record[i]));
        }
        

        // takes our values of vectors and reindexes our multimap
        for(int j = 0; j < newRowValues.size(); j++)
        {
            indicies[j][newRowValues[j]].push_back(i);
        }

        // updates our record number vectors
        recordNumbersVector.push_back(i);

    }

    // closes our binary file
    recordsFile.close();

   
}
Table::Table(string tableName, vectorstr fieldTypes)
{

    fstream recordsFile;

    // sets our member variable of vector field names to the passed in parameter of vector field names 
    vectorFieldNames = fieldTypes;
    
    // maps our field names to numbered values 
    for(int i = 0; i < fieldTypes.size(); i++)
    {
        // properly sizes of vector of multimaps to the amount of fields that are passed (a specific multimap for each field name)
         MMap<string, long> newMMap;
        indicies.push_back(newMMap);

        // maps our field names to numbered values 
        bucketVariable[fieldTypes[i]] = i;
    }

    nameOfDataFile = tableName;

    tableName +=  ".txt";

    // clears the contents of the txt file, prior to opening
    ofstream outputFile(tableName, ios::trunc);
    if (!outputFile) {
        cout << "Error opening the file." << endl;
    }

    // reads into our txt file with our corresponding field names
    for (int i = 0; i < fieldTypes.size(); i++) {
        outputFile << fieldTypes[i] << endl;
    }
    
    tableName = nameOfDataFile;
    tableName += ".bin";
    
    char openDataChar[tableName.length()+1];
    strcpy(openDataChar, tableName.c_str());
    
    // resets our bin file incase a table with this same binary file name has been created before in a prior run
    dataBase.open_fileW(recordsFile, openDataChar);
    recordsFile.close();


    // increases our serial number so that the next table will be unique
    serialNumber++;

  
}


void Table::insert_into(vectorstr row)
{
    fstream recordsFile;

    // gets name of binary file so we can be prepared to open
    string binaryFileName = nameOfDataFile + ".bin";
    char dataFileName[binaryFileName.length()];
    strcpy(dataFileName, binaryFileName.c_str());
    
    // sets our 2d char array in our FileRecord dataBase with our corresponding vector of values
    dataBase = FileRecord(row);


    // if a table is newly created, we reset its contents incase its same file name was created in a prior run
    if(dataRecordNumbers == -1)
    {
        dataBase.open_fileW(recordsFile, dataFileName);
    }

    else
        dataBase.open_fileRW(recordsFile, dataFileName);
    
    
    // reads into our file, and holds the current record number where we just read into
    dataRecordNumbers = dataBase.write(recordsFile);


    // pushes that recordNumber into our member variable that keeps track of our recordNumbers
    recordNumbersVector.push_back(dataRecordNumbers);

    // reindexes our vector of mulimaps to now contain the updated recordNumber for each multimap
    for(int i = 0; i < row.size(); i++)
    {

        indicies[i][row[i]].push_back(dataRecordNumbers);
    }

    // closes our file
    recordsFile.close();
    
}
Table Table::select(vectorstr names, string temp1, string temp2, string temp3)
{
    fstream recordsFile;
   
    // takes in the case where we are selecting all the field names 
    if(names[0] == "*")
    {
        Table newTable("selected_table", vectorFieldNames);

        string openFileName = nameOfDataFile + ".bin";
        char dataFileName[openFileName.length() + 1];
        strcpy(dataFileName, openFileName.c_str());


        // goes through our new file and reads in everything from the old one 
        // this is only called in my SQL when we have *  for fields, and NO conditions
        for(int i = 0; i < recordNumbersVector.size(); i++)
        {
            newTable.dataBase.open_fileRW(recordsFile, dataFileName);
            vector<string> insertInto;
            newTable.dataBase.read(recordsFile, recordNumbersVector[i]);
            recordsFile.close();
            for(int i = 0; i < vectorFieldNames.size(); i++)
            {
                insertInto.push_back((newTable.dataBase._record[i]));
            }
            
        
            // modify our vector before we push
            vector<string> officalInsert;
            for(int i = 0; i < vectorFieldNames.size(); i++)
            {
                // allows us to our table to have all values that actually corresponded to their vector field names
                // howevers since this is *, it does not matter
                if(bucketVariable.contains(vectorFieldNames[i]))
                    officalInsert.push_back(insertInto[bucketVariable[vectorFieldNames[i]]]);
            }
            
            newTable.insert_into(officalInsert);
        }
       
        return newTable;
        
    }

    else
    {

    // creates a new table
    Table newTable("selected_table", names); 
    if(temp2 == "=")
    {
       
        int correspondingMMap = bucketVariable[temp1];
        // sets our recordNumber vector to the one found it its corresponding multimap 
        vector<long> correspondingRecNo = indicies[correspondingMMap][temp3];
        recordNumbersVector = correspondingRecNo;
        
        char* dataFileName = new char[nameOfDataFile.length()];
        strcpy(dataFileName, nameOfDataFile.c_str());

        // reads our new record numbers into its newly created binary file that correspodns to the selected table  
        for(int i = 0; i < correspondingRecNo.size(); i++)
        {
            newTable.dataBase.open_fileRW(recordsFile, dataFileName);
            vector<string> insertInto;
    
            newTable.dataBase.read(recordsFile, correspondingRecNo[i]);
            recordsFile.close();

            // obtains all the values obtained from the old table at their corresponding recnos, and stores them into vectors
            for(int i = 0; i < vectorFieldNames.size(); i++)
            {
                insertInto.push_back((newTable.dataBase._record[i]));
        
            }

            // takes those vectors of values and calls our insert_into function to enter them into our table
            newTable.insert_into(insertInto);
        }
        return newTable;
    }
    else if(temp2 == ">")
    {   
        
        // Initializes our upper bound and ending bound (the iterator prior to null)
        int correspondingMMap = bucketVariable[temp1];
        vector<long> correspondingRecNo;
        MMap<string, long>::Iterator upperBoundIterator;
        upperBoundIterator = indicies[correspondingMMap].upper_bound(temp3);
        MMap<string, long>::Iterator it;
        it = upperBoundIterator;
        MMap<string, long>::Iterator end = indicies[correspondingMMap].end();
        for(it = upperBoundIterator; it != end; it++)
        {
            // as it goes through all the values that pertain to the upperBound, it takes their record numbers and 
            // stores them into the record numbers vector for our newly created table
            correspondingRecNo = (*it).value_list;
            for(int i = 0; i < correspondingRecNo.size(); i++)
            {
                newTable.recordNumbersVector.push_back(correspondingRecNo[i]);
            }
            
        }
       
        recordNumbersVector = newTable.recordNumbersVector;
        correspondingRecNo = recordNumbersVector;
        char* dataFileName = new char[nameOfDataFile.length()];
        strcpy(dataFileName, nameOfDataFile.c_str());

        // reads our new record numbers into its newly created binary file that correspodns to the selected table  
        for(int i = 0; i < correspondingRecNo.size(); i++)
        {
            newTable.dataBase.open_fileRW(recordsFile, dataFileName);
            vector<string> insertInto;
            newTable.dataBase.read(recordsFile, correspondingRecNo[i]);
            recordsFile.close();

            // obtains all the values obtained from the old table at their corresponding recnos, and stores them into vectors
            for(int i = 0; i < vectorFieldNames.size(); i++)
            {
                insertInto.push_back((newTable.dataBase._record[i]));
            }
            // modify our vector before we push
           
            vector<string> officalInsert;
            for(int i = 0; i < vectorFieldNames.size(); i++)
            {
                officalInsert.push_back(insertInto[bucketVariable[newTable.vectorFieldNames[i]]]);
            }

            // takes those vectors of values and calls our insert_into function to enter them into our table

            newTable.insert_into(officalInsert);
        }
       
        return newTable;
    }
    else if(temp2 == "<")
    {   
        
        int correspondingMMap = bucketVariable[temp1];
        
        vector<long> correspondingRecNo;
        // we want to stop before we get to this iterator
        MMap<string, long>::Iterator lessThanIterator;

        // call lowerBound to just give us the iterator at that position
        lessThanIterator = indicies[correspondingMMap].lower_bound(temp3);
        MMap<string, long>::Iterator it;
       
        MMap<string, long>::Iterator end = lessThanIterator;
        for(it = indicies[correspondingMMap].begin(); it != lessThanIterator; it++)
        {
            // as it goes through all the values that pertain to the upperBound, it takes their record numbers and 
            // stores them into the record numbers vector for our newly created table
            correspondingRecNo = (*it).value_list;
            for(int i = 0; i < correspondingRecNo.size(); i++)
            {
                newTable.recordNumbersVector.push_back(correspondingRecNo[i]);
            }
            
        }
        
     
        recordNumbersVector = newTable.recordNumbersVector;
        correspondingRecNo = recordNumbersVector;
        char* dataFileName = new char[nameOfDataFile.length()];
        strcpy(dataFileName, nameOfDataFile.c_str());

        // reads our new record numbers into its newly created binary file that correspodns to the selected table  
        for(int i = 0; i < correspondingRecNo.size(); i++)
        {
            
            newTable.dataBase.open_fileRW(recordsFile, dataFileName);
            vector<string> insertInto;
            newTable.dataBase.read(recordsFile, correspondingRecNo[i]);
            recordsFile.close();

            // obtains all the values obtained from the old table at their corresponding recnos, and stores them into vectors
            for(int i = 0; i < vectorFieldNames.size(); i++)
            {
                insertInto.push_back((newTable.dataBase._record[i]));
        
            }
            // modify our vector before we push
            vector<string> officalInsert;
            for(int i = 0; i < vectorFieldNames.size(); i++)
            {
                officalInsert.push_back(insertInto[bucketVariable[newTable.vectorFieldNames[i]]]);
            }

            // takes those vectors of values and calls our insert_into function to enter them into our table

            newTable.insert_into(officalInsert);
        }
       
        return newTable;
    }
    else if(temp2 == "<=")
    {   
        // we must utilize upper_bound
        int correspondingMMap = bucketVariable[temp1];
        
        vector<long> correspondingRecNo;
        // we want to stop before we get to this iterator
        MMap<string, long>::Iterator lessThanIterator;
        // call lowerBound to just give us the iterator at that position
        lessThanIterator = indicies[correspondingMMap].lower_bound(temp3);
        MMap<string, long>::Iterator it;
        it = indicies[correspondingMMap].begin();
        MMap<string, long>::Iterator end = lessThanIterator;
        lessThanIterator++;
        for(it = indicies[correspondingMMap].begin(); it != lessThanIterator; it++)
        {
            // as it goes through all the values that pertain to the upperBound, it takes their record numbers and 
            // stores them into the record numbers vector for our newly created table
            correspondingRecNo = (*it).value_list;
            for(int i = 0; i < correspondingRecNo.size(); i++)
            {
                newTable.recordNumbersVector.push_back(correspondingRecNo[i]);
            }
            
        }
        recordNumbersVector = newTable.recordNumbersVector;
        correspondingRecNo = recordNumbersVector;
        char* dataFileName = new char[nameOfDataFile.length()];
        strcpy(dataFileName, nameOfDataFile.c_str());

        // reads our new record numbers into its newly created binary file that correspodns to the selected table  
        for(int i = 0; i < correspondingRecNo.size(); i++)
        {
           
            newTable.dataBase.open_fileRW(recordsFile, dataFileName);
            vector<string> insertInto;
            newTable.dataBase.read(recordsFile, correspondingRecNo[i]);
            recordsFile.close();

            // obtains all the values obtained from the old table at their corresponding recnos, and stores them into vectors
            for(int i = 0; i < vectorFieldNames.size(); i++)
            {
                insertInto.push_back((newTable.dataBase._record[i]));
        
            }
           // modify our vector before we push
            vector<string> officalInsert;
            for(int i = 0; i < vectorFieldNames.size(); i++)
            {
                officalInsert.push_back(insertInto[bucketVariable[newTable.vectorFieldNames[i]]]);
            }

            // takes those vectors of values and calls our insert_into function to enter them into our table
            newTable.insert_into(officalInsert);
        }
       
        return newTable;
    }
else if(temp2 == ">=")
    {   
        // we must utilize upper_bound
        int correspondingMMap = bucketVariable[temp1];
        
        vector<long> correspondingRecNo;
        MMap<string, long>::Iterator lowerBoundIterator;
        lowerBoundIterator = indicies[correspondingMMap].lower_bound(temp3);
        MMap<string, long>::Iterator it;
        it = lowerBoundIterator;
        MMap<string, long>::Iterator end = indicies[correspondingMMap].end();
        for(it = lowerBoundIterator; it != end; it++)
        {
            // as it goes through all the values that pertain to the upperBound, it takes their record numbers and 
            // stores them into the record numbers vector for our newly created table
            correspondingRecNo = (*it).value_list;
            for(int i = 0; i < correspondingRecNo.size(); i++)
            {
                newTable.recordNumbersVector.push_back(correspondingRecNo[i]);
            }
            
        }
        recordNumbersVector = newTable.recordNumbersVector;
        correspondingRecNo = recordNumbersVector;
        char* dataFileName = new char[nameOfDataFile.length()];
        strcpy(dataFileName, nameOfDataFile.c_str());

        // reads our new record numbers into its newly created binary file that correspodns to the selected table  
        for(int i = 0; i < correspondingRecNo.size(); i++)
        {
            
            newTable.dataBase.open_fileRW(recordsFile, dataFileName);
            vector<string> insertInto;
            newTable.dataBase.read(recordsFile, correspondingRecNo[i]);
            recordsFile.close();

            // obtains all the values obtained from the old table at their corresponding recnos, and stores them into vectors
            for(int i = 0; i < vectorFieldNames.size(); i++)
            {
                insertInto.push_back((newTable.dataBase._record[i]));
        
            }
            // modify our vector before we push
            vector<string> officalInsert;
            for(int i = 0; i < vectorFieldNames.size(); i++)
            {
                officalInsert.push_back(insertInto[bucketVariable[newTable.vectorFieldNames[i]]]);
            }

            // takes those vectors of values and calls our insert_into function to enter them into our table
            newTable.insert_into(officalInsert);
        }
        
        return newTable;
    }
    else 
    {   
        // Is called in our sql for a selected table with NO conditions, 
        // and will hit this case when they want to select specific field types
        
        // UTILIZE THE recordNumbersVector FROM THE ORIGINAL
        // TABLE, SINCE IT WILL BE THE SAME FOR THIS ONE WITH NO CONDITIONS
        string openFileName = nameOfDataFile + ".bin";
        
        char dataFileName[openFileName.length() + 1];
        strcpy(dataFileName, openFileName.c_str());

        // reads our new record numbers into its newly created binary file that correspodns to the selected table  
        for(int i = 0; i < recordNumbersVector.size(); i++)
        {
            
            newTable.dataBase.open_fileRW(recordsFile, dataFileName);
            vector<string> insertInto;
            newTable.dataBase.read(recordsFile, recordNumbersVector[i]);
            recordsFile.close();

            // obtains all the values obtained from the old table at their corresponding recnos, and stores them into vectors
            for(int i = 0; i < vectorFieldNames.size(); i++)
            {
                insertInto.push_back((newTable.dataBase._record[i]));
        
            }
            
        
            // modify our vector before we push
            vector<string> officalInsert;
            for(int i = 0; i < names.size(); i++)
            {
                // allows us to our table to have all values that actually corresponded to their vector field names
                if(bucketVariable.contains(names[i]))
                    officalInsert.push_back(insertInto[bucketVariable[names[i]]]);
            }
            
            // takes those vectors of values and calls our insert_into function to enter them into our table
            newTable.insert_into(officalInsert);
        }
        
        return newTable;
    }
    
    }
    
    Table temp;
    return temp;
}
ostream& operator<<(ostream& outs, const Table& r)
{   
    // if the recordNumbers is still -1, we know that the table has not been inserted
    // into since it was first initally created, making it empty. Therefore, we just return an empty outs
    if(r.dataRecordNumbers == -1) 
    {
        return outs;
    }

    // gets prepared to open the corresponding binary file
    string binaryFile = r.nameOfDataFile + ".bin";
    char dataFileName[binaryFile.length()+1];
    strcpy(dataFileName, binaryFile.c_str());

    
    outs << setw(25) << "record" << setw(25);
    
    // print all the field names
    for(int i = 0; i < r.vectorFieldNames.size(); i++)
    {
        outs << r.vectorFieldNames[i] << setw(25);
    }

    outs << endl << endl;
    FileRecord tempRecords;
    fstream tempFileOpener;
    tempRecords.open_fileRW(tempFileOpener, dataFileName);
    // Now, print the actual data
    for(int i = 0; i < (r.dataRecordNumbers)+1; i++)
    {
        // reads from the file into our variable FileRecord then prints it 
        tempRecords.read(tempFileOpener, i);
        outs << setw(25) << i << setw(25) << tempRecords << endl;
        tempRecords = FileRecord();
    }
    tempFileOpener.close();
    return outs;
}

// returns our the recordNumbers that corresponding to the table variable of stored records
vector<long> Table::select_recnos()
{
    return recordNumbersVector;
}

// utilizes the same logic from the select 
// EXCEPT, we return a token that contains record numbers to the matching condition, rather than a table
Token *Table::evaluate(Token *value, Token *fieldName, Token *relation)
{
        recordNumbersVector.clear();
        if(relation->token_str() == "=")
        {
               
                int correspondingMMap = bucketVariable[fieldName->token_str()];
                vector<long> correspondingRecNo = indicies[correspondingMMap][value->token_str()];
                recordNumbersVector = correspondingRecNo;
                string resultString = "";
                for(int i = 0; i < recordNumbersVector.size(); i++)
                {
                    resultString += to_string(recordNumbersVector[i]) + " ";
                }
                Token *temp = new Token(resultString, RESULT_SET);
                return temp;
        }
    else if(relation->token_str() == ">")
    {   
        
 
       
        // we must utilize upper_bound
        int correspondingMMap = bucketVariable[fieldName->token_str()];
        
      
        vector<long> correspondingRecNo;
        MMap<string, long>::Iterator upperBoundIterator;
        upperBoundIterator = indicies[correspondingMMap].upper_bound(value->token_str());
        
        MMap<string, long>::Iterator it;
        it = upperBoundIterator;
        
        if(*it == value->token_str() || *it < value->token_str())
        {
            upperBoundIterator++;
        }
        
        MMap<string, long>::Iterator end = indicies[correspondingMMap].end();
        for(it = upperBoundIterator; it != end; it++)
        {
            correspondingRecNo = (*it).value_list;
            if(correspondingRecNo.size() == 0)
            {
                break;
            }
            for(int i = 0; i < correspondingRecNo.size(); i++)
            {
                recordNumbersVector.push_back(correspondingRecNo[i]);
            
            }
            
        }
       
        correspondingRecNo = recordNumbersVector;
                string resultString = "";
                for(int i = 0; i < recordNumbersVector.size(); i++)
                {
                    resultString += to_string(recordNumbersVector[i]) + " ";
                }
                Token *temp = new Token(resultString, RESULT_SET);
                return temp;
    }
    else if(relation->token_str() == ">=")
    {   
       
        // we must utilize upper_bound
        int correspondingMMap = bucketVariable[fieldName->token_str()];
        
       
        vector<long> correspondingRecNo;
        MMap<string, long>::Iterator lowerBoundIterator;
        lowerBoundIterator = indicies[correspondingMMap].lower_bound(value->token_str());
        MMap<string, long>::Iterator it;
        it = lowerBoundIterator;
        MMap<string, long>::Iterator end = indicies[correspondingMMap].end();
        for(it = lowerBoundIterator; it != end; it++)
        {
            correspondingRecNo = (*it).value_list;
            if(correspondingRecNo.size() == 0)
            {
                break;
            }
            for(int i = 0; i < correspondingRecNo.size(); i++)
            {
                recordNumbersVector.push_back(correspondingRecNo[i]);
            
            }
            
        }
       
        correspondingRecNo = recordNumbersVector;
                string resultString = "";
                for(int i = 0; i < recordNumbersVector.size(); i++)
                {
                    resultString += to_string(recordNumbersVector[i]) + " ";
                }
                Token *temp = new Token(resultString, RESULT_SET);
                return temp;
    }
        
        else if(relation->token_str() == "<")
        {   
        
        
        // we must utilize upper_bound
        int correspondingMMap = bucketVariable[fieldName->token_str()];
        
        
        vector<long> correspondingRecNo;
        // we want to stop before we get to this iterator
        MMap<string, long>::Iterator lessThanIterator;
        // call lowerBound to just give us the iterator at that position
        lessThanIterator = indicies[correspondingMMap].lower_bound(value->token_str());
        MMap<string, long>::Iterator it;
      
        
        MMap<string, long>::Iterator end = lessThanIterator;
        for(it = indicies[correspondingMMap].begin(); it != lessThanIterator; it++)
        {
            correspondingRecNo = (*it).value_list;
            for(int i = 0; i < correspondingRecNo.size(); i++)
            {
                recordNumbersVector.push_back(correspondingRecNo[i]);
            }
            
        }
        
        correspondingRecNo = recordNumbersVector;
                string resultString = "";
                for(int i = 0; i < recordNumbersVector.size(); i++)
                {
                    resultString += to_string(recordNumbersVector[i]) + " ";
                }
                Token *temp = new Token(resultString, RESULT_SET);
                return temp;
    }
    else if(relation->token_str() == "<=")
    {   
        // we must utilize upper_bound
        int correspondingMMap = bucketVariable[fieldName->token_str()];
        
        vector<long> correspondingRecNo;
       
        // we want to stop before we get to this iterator
        MMap<string, long>::Iterator lessThanIterator;
        // call lowerBound to just give us the iterator at that position
        lessThanIterator = indicies[correspondingMMap].lower_bound(value->token_str());
        MMap<string, long>::Iterator it;
        it = indicies[correspondingMMap].begin();
        MMap<string, long>::Iterator end = lessThanIterator;
        MMap<string, long>::Iterator nullIterator(nullptr);
        if(lessThanIterator != nullIterator)
        {
            if(value->token_str() == *lessThanIterator)
            {
                lessThanIterator++;
            }
        }
        for(it = indicies[correspondingMMap].begin(); it != lessThanIterator; it++)
        {
            correspondingRecNo = (*it).value_list;
            for(int i = 0; i < correspondingRecNo.size(); i++)
            {
                recordNumbersVector.push_back(correspondingRecNo[i]);
            }
            
        }
        correspondingRecNo = recordNumbersVector;
                string resultString = "";
                for(int i = 0; i < recordNumbersVector.size(); i++)
                {
                    resultString += to_string(recordNumbersVector[i]) + ' ';
                }
                Token *temp = new Token(resultString, RESULT_SET);
                return temp;
    }
        Token *temp;
        return temp;
        
}  

// converts our string of record numbers into a vector of type integers 
vector<int> Table::convertVector(string stringVec) {
    vector<int> intVector;
    istringstream iss(stringVec);
    int num;
        while (iss >> num) {
        intVector.push_back(num);
    }
    return intVector;
}

Token *Table::evaluateUnion(Token *set1, Token *set2, Token *relation)
{
   
    string resultantSetString = "";
    string set1String = set1->token_str();
    string set2String = set2->token_str();
    
    // converts our string tokens to int vectors that correspond to record numbers 
    vector<int> set1Int = convertVector(set1String);
    vector<int> set2Int = convertVector(set2String);

    // sort the 2 vectors:
    // The built in set_intersection and set_union functions built
    // into the <algorithim> library require that the vectors be sorted before hand
    sort(set1Int.begin(), set1Int.end());
    sort(set2Int.begin(), set2Int.end());

    vector<int> finalIntVector;


    if(relation->token_str() == "and")
    {
        vector<int> differentiatedVectors;

        // takes the vectors and takes the record numbers only existing in BOTH vectors
        set_intersection(set1Int.begin(), set1Int.end(), set2Int.begin(), set2Int.end(), back_inserter(differentiatedVectors));


        finalIntVector = differentiatedVectors;
    
    }

    else if(relation->token_str() == "or")
    {
        vector<int> combinedVector(set1Int.size() + set2Int.size());
        sort(set1Int.begin(), set1Int.end());
        sort(set2Int.begin(), set2Int.end());

        // takes the 2 vectors and merges them, and does not include it duplicates
        // for ex, if 1 exits in both vectors, only one of the 1's will be put into the resultant vector
        vector<int>::iterator it = set_union(set1Int.begin(), set1Int.end(), set2Int.begin(), set2Int.end(), combinedVector.begin());       
        
        combinedVector.resize(it - combinedVector.begin());

        finalIntVector = combinedVector;

    }

    // converts our finalized vector back to string form
    for(int i = 0; i < finalIntVector.size(); i++)
    {
        resultantSetString += to_string(finalIntVector[i]) + " ";
    }
    
    // puts our resultant vector into a token of type RESULT SET, then returns it
    Token *resultantSet = new Token(resultantSetString, RESULT_SET);
    return resultantSet;
}

Table Table::select(vectorstr name, Queue<Token *> tempQueue)
{
    

    fstream recordsFile;
    string selectedTableName = "selected_table_" + to_string(serialNumber);
    Table newTableQueue(selectedTableName, name);
    newTableQueue.vectorFieldNames = name;
    Stack<Token *> tempStack;

    for(int i = tempQueue.size(); i != 0; i--)
    {
      
        
        Token *checkCondition = tempQueue.pop();
        //cout << "This is the current checkCondition string: " << checkCondition->token_str() << endl;

        // if its a symbol, for ex "John" we push to our stack
        if(checkCondition->type() == TOKEN_STR)
        {
            tempStack.push(checkCondition);
        }

        // if its an operator, we pop from our stacks, and set the tokens to the operation to be evaluated
        else if(checkCondition->type() == OPERATOR)
        {
            // POP TWICE!

          
            if(tempStack.empty())
            {
                    throw "Issue with conditional: You are either missing a field name or value when comparing with an operator";
            }

            Token *value = tempStack.pop();

            if(tempStack.empty())
            {
                    throw "Issue with conditional: You are either missing a field name or value when comparing with an operator";

            }
            Token *fieldName = tempStack.pop();

            Token *evaluated = evaluate(value, fieldName, checkCondition);
            tempStack.push(evaluated);
        }

        // if its a logical, it takes the 2 other tokens in the stack (both of type Result_Set),
        // then calls send both records to be evaluated 
        else if(checkCondition->type() == LOGICAL)
        {

         
            if(tempStack.empty())
            {
                    throw "Issue with logical: You are missing missing actual conditions to compare";

            }
            Token *set1 = tempStack.pop();


            if(tempStack.empty())
            {
                    throw "Issue with logical: You are missing one of the conditionals to compare";

            }
            Token *set2 = tempStack.pop();

            Token *evaluatedSet = evaluateUnion(set1, set2, checkCondition);
            tempStack.push(evaluatedSet);
        }
        else
        {
        }

    }
   

    if(tempStack.size() > 1)
        throw "Missing a logical operator";

    if(tempStack.empty())
    {
            throw "Issue with conditionals, nothing in resultant stack!";
    }    
    Token *result = tempStack.pop();


    vector<long> finalRecords;

    string convertStringVector = result->token_str();

    stringstream ss(convertStringVector);
    int number;
    
    // Converts our vector string in the result set to an int vector by
    // extracting the intergers from our stringstream
    while (ss >> number) {
        finalRecords.push_back(number);
    }

    // sorts our vector
    insert_sort_function(finalRecords, finalRecords.size());

    
    

    string binFileName = nameOfDataFile + ".bin";

    char dataFileName[binFileName.length()];
    strcpy(dataFileName, binFileName.c_str());

    // reads our new record numbers into its newly created binary file that correspodns to the selected table  
    for(int i = 0; i < finalRecords.size(); i++)
    {
        newTableQueue.dataBase.open_fileRW(recordsFile, dataFileName);
        vector<string> insertInto;

        newTableQueue.dataBase.read(recordsFile, finalRecords[i]);
        recordsFile.close();
        
        // obtains all the values obtained from the old table at their corresponding recnos, and stores them into vectors
        for(int i = 0; i < vectorFieldNames.size(); i++)
        {

            insertInto.push_back((newTableQueue.dataBase._record[i]));
        }
        
        vector<string> officalInsert;
        for(int i = 0; i < name.size(); i++)
        {
            officalInsert.push_back(insertInto[bucketVariable[newTableQueue.vectorFieldNames[i]]]);
        }
        
        // takes those vectors of values and calls our insert_into function to enter them into our table

        
        newTableQueue.insert_into(officalInsert);
    }


    // clears whatever prior records that existed our recordNumbers vector for the newly created vector
    newTableQueue.recordNumbersVector.clear();

    // then sets it to the vector of ints that correspond to the evaluated conditions in our RPN 
    newTableQueue.recordNumbersVector = finalRecords;

    return newTableQueue;
    
}
void Table::insert_sort_function(vector<long> &a, unsigned int size)
{
    int temp;
    for(int i = 1; i < size; i++)
    {
        temp = a[i];
        int j = i;
        for(int j = i; j >= 0; j--)
        {
            if(a[j-1] > temp && (j-1) >= 0) // check for out of bounds issue
                a[j] = a[j-1]; // if its greater than the temps value, we shift it over to the right
            else
            {
                a[j] = temp; // if its not greater, than the temp value just takes that spot
                break;
            }
        }
       
    }
}
Table Table::select(vectorstr name, vector<string> tempString)
{
    
    Queue<Token*> returnQueue;
    Stack<Token*> tempStack;


    for(int i = 0; i < tempString.size(); i++)
    {
        // If we have a SYMBOL, we push it to the return queue immediately 
        if(tempString[i] != "=" && tempString[i] != ">"  && tempString[i] != "<" 
                && tempString[i] != ">=" && tempString[i] != "<=" && tempString[i] != "and" && tempString[i] != "or"
                && tempString[i] != ")" && tempString[i] != "(")
        {

            Token *newToken = new Token(tempString[i],TOKEN_STR);
            returnQueue.push(newToken);
        }

        // If we have an OPERATOR, we push it to our temp stack
        else if(tempString[i] == "=" || tempString[i] == ">"  || tempString[i] == "<" 
                || tempString[i] == ">=" || tempString[i] == "<=" )
        {
            Token *newToken = new Token(tempString[i], OPERATOR);
            tempStack.push(newToken);
        }

        // if we have OR, we check for the following 
        else if(tempString[i] == "or")
        {
            bool pushedOr = false;
            Token *orLogical = new Token(tempString[i], LOGICAL);

            // if the stack is empty, we just push OR immediately
            if(tempStack.empty())
            {
                tempStack.push(orLogical);
            }
            else
            {
                // while the stack is NOT empty, we check to see if we find something that has a greater presendence,
                // such as an operator, or an AND logical
                while(!tempStack.empty())
                {
                    // if it is an operator, then we pop that operator from the stack, then push it into the return queue
                    // HOWEVER, we cannot immediately push the OR in since the next thing in the stack may be and AND logical,
                    // which serves higher presendence and would have to go into the return queue first
                    if((tempStack.top())->type() == OPERATOR)
                    {
                        // pop from stack
                        Token *newToken = tempStack.pop();
                        returnQueue.push(newToken);


                    }

                    // and has higher presendence than or, so it gets popped and put into the return queue 
                    else if((tempStack.top())->token_str() == "and")
                    {
                        // pop from stack
                        Token *newToken = tempStack.pop();
                        returnQueue.push(newToken);


                    }


                    // if its either an and/operator, we can finally push our OR to the stack
                    else
                    {
                        tempStack.push(orLogical);
                        pushedOr = true;
                        break;
                    }
                }
                // if the OR hasnt been pushed at all, and the stack is now empty, we can push into the stack
                if(!pushedOr)
                {
                    tempStack.push(orLogical);
                }
            }
            
            
        }
        // if its an AND, all we need to check for if is there is an operator already in the stack 
        else if(tempString[i] == "and")
        {
            Token *andLogical = new Token(tempString[i], LOGICAL);


            if(!tempStack.empty())
            {
                if((tempStack.top())->type() == OPERATOR)
                {
                    // if we find an operator, then we pop from the stack, and push that operator
                    // into the return queue
                    Token *newToken = tempStack.pop();
                    returnQueue.push(newToken);

                    // then we push the AND logical into the stack since nothing else in it would serve higher presendence
                    // that must be popped and put into the return queue
                    tempStack.push(andLogical);

                }  
                else
                {
                    // if its not an operator, then we can just push into the stack immediately 
                    tempStack.push(andLogical);
                }

            }
            // if the stack is empty from the start, then we can just push the logical AND into the stack
            else
            {
                tempStack.push(andLogical);
            }
        }

        // if we have an open paranthesis, we push it into the stack
        else if(tempString[i] == "(")
        {
            Token *openParen = new Token(tempString[i], LPAREN);
            tempStack.push(openParen);
   
        }

        // if we have a closed paranthesis, we pop everything from the stack, 
        // and put in to the return queue until we hit its corresponding open parenthesis. 
        // When we find the open parenthesis in the stack, we pop it and DONT put it into the return queue.
        else if(tempString[i] == ")")
        {
            bool foundOpenParen = false;
            Token *newToken;
            // this is our base case just in case we have invalid input, but we will
            // break the moment we find the open paren
            while(!tempStack.empty())
            {
                newToken = tempStack.pop();
                if(newToken->type() == LPAREN)
                {
                    // we found our open parenthesis so we are done with the parnethesis now
                   foundOpenParen = true;
                   
                    break;
                }
                else
                {
                    returnQueue.push(newToken);
                }

            }
            // if we never found a corresponding open parenthesis, we know there is an issue with the conditional
            if(!foundOpenParen)
                throw "INVALID, missing open parenthesis: ( "; 
        }

       


    }

    // when we're done going through our vector of strings, we are finally able to get whatever is left in the stack
    // by just popping in and pushing into our return queue
    while(!tempStack.empty())
    {
        Token *newToken = tempStack.pop();

        // if we find an open parenthesis in there, we know we have an issue, since it 
        // would have been popped by now if it had its corresponding closed paranthesis
        if(newToken->token_str() == "(")
        {
            throw "INVALID, missing close parenthesis: ) "; 
        }

        returnQueue.push(newToken);

    }

    // cout << "Here is the return queue: " << endl;
    // printQueue(returnQueue);

    if(returnQueue.empty())
    {
      

        throw "No conditional was entered";


    }
    return select(name, returnQueue);



    // Peter's Yard (Old Code)
    /*
    Queue<Token*> returnQueue;
    Stack<Token*> tempStack;
    int tokenStrCount = 0;
    int conditions = 0;
    int parenConditions = 0;
    Token *OR = new Token("or", LOGICAL);
    Token *AND = new Token("and", LOGICAL);
    int LParenCount = 0;
    int parenOr = 0;
    int parenAnd = 0;
    int orCount = 0;
    int andCount = 0;
    int currentSymbols = 0;
    int lParen = 0;
    int openParenCount = 0;
    int closeParenCount = 0;

    for(int i = 0; i < tempString.size(); i++)
    {
        cout << "This is the current string value: " << tempString[i] << endl;
        // If its a symbol
        if(tempString[i] != "=" && tempString[i] != ">"  && tempString[i] != "<" 
                && tempString[i] != ">=" && tempString[i] != "<=" && tempString[i] != "and" && tempString[i] != "or"
                && tempString[i] != ")" && tempString[i] != "(")
        {

            // age = 
            // cout << "PUSHING A SYMBOL" << endl;
            Token *newToken = new Token(tempString[i],TOKEN_STR);
            returnQueue.push(newToken);
            currentSymbols++;
            if(currentSymbols == 2)
            {
                Token *poppedStack = tempStack.pop();
                returnQueue.push(poppedStack);
                currentSymbols = 0;
                if(LParenCount > 0)
                {
                    parenConditions++;
                    cout << "This is the new parenConditions: " << parenConditions << endl;
                }
                else
                    conditions++;
                
                if(conditions == 2)
                {
                   
                    if(andCount > 0 && LParenCount == 0)
                    {
                        returnQueue.push(AND);
                        andCount--;
                        cout << "New andCount: " << andCount << endl;

                    }
                   
                    conditions--;
                    
                    
                }
                else if(parenConditions == 2)
                {
                    cout << "We entered our parenConditions!" << endl;
                    cout << "Here is the andParenCount: " << parenAnd << endl;
                    if(parenAnd > 0 && openParenCount < 2) 
                    {
                        cout << "We are pushing AND with correspind string " << tempString[i] << endl;
                        returnQueue.push(AND);
                        parenAnd--;
                    }
                    parenConditions--;
                }
            }
        
        }

        // Doesnt go straight into stack, 
        // age < 20 and lname = yao
        // age 20 < lname yao = and 


        // MY LOGIC:
        // queue: age 20 > lname yao = and 
        // stack: 
        // currentSymbols = 2
        // andCount = 1
        // conditions = 2;


        else if(tempString[i] == "=" || tempString[i] == ">"  || tempString[i] == "<" 
                || tempString[i] == ">=" || tempString[i] == "<=" )
        {
            Token *newToken = new Token(tempString[i], OPERATOR);
            tempStack.push(newToken);
        }
        else if(tempString[i] == "or")
        {
            if(LParenCount == 0)
                orCount++;
            else 
                parenOr++;
        }
        else if(tempString[i] == "and")
        {
           // cout << "our current token is and!" << endl;
            
            if(LParenCount > 0)
            {
                cout << "We incremented our parenAnd!" << endl;
                 parenAnd++;
            }
            else
            {
                andCount++;
                cout << "Current andCount: " << andCount << endl;
            }
        }
        else  if(tempString[i] == "(")
        {
            //cout << "Pushing ( into our stack" << endl;
            openParenCount++;
            Token *newToken = new Token(tempString[i], LPAREN);
            tempStack.push(newToken);
            LParenCount++;
        }
        else  if(tempString[i] == ")")
        {
            // get rid of the open paranthesis
            closeParenCount++;
            conditions = parenConditions;
            tempStack.pop();



            if(parenAnd > 0)
            {
                        returnQueue.push(AND);
                        parenAnd--;
            }
            
            else if(parenOr > 0)
            {
                returnQueue.push(OR);
                parenOr--;
            }
           

            else if(parenOr > 0)
             {
                        returnQueue.push(OR);
                        parenOr--;
            }

            if(andCount > 0)
            {
                returnQueue.push(AND);
                andCount--;
            }

            LParenCount--;
            parenConditions--;
        }
        
    }
    cout << "Here is the current OR count: " << orCount << endl;
    cout << "Here is the current AND count: " << andCount << endl;
    
    for(int i = 0; i < andCount; i++)
    {
        
        returnQueue.push(AND);
    }
    for(int i = 0; i < orCount; i++)
    {
        returnQueue.push(OR);
    }

    cout << "Here is the openParen count: " << openParenCount << endl;
    cout << "Here is the closeParen count: " << openParenCount << endl;

   
    cout << "Here is the return queue: " << endl;
    printQueue(returnQueue);
    
    return select(name, returnQueue);

    */
}
vector<string> Table::getFieldNames()
{
    return vectorFieldNames;
}
void Table::printQueue(Queue<Token *> tempQueue)
{
    Queue<Token*>::Iterator it;
    for(it = tempQueue.begin(); it != tempQueue.end(); it++)
    {
        cout << (*it)->token_str() << " " << endl;
    }
}