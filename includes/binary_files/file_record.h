#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;

struct FileRecord{
    //when you construct a Record, it's either empty or it
    //  contains a word 
    FileRecord(){
        for (int i = 0; i < MAX+1; ++i) 
        {
            for (int j = 0; j < MAX+1; ++j) {
                _record[i][j] = '\0';
            }
        }
        _record[0][0] = '\0';
        recno = -1;
    }

    FileRecord(char str[]){
        //strncpy(_record, str, MAX);
    }

    // originally taking just string, but now takes in a vector of type strings
    FileRecord(vector<string> s){
        
        for (int i = 0; i < MAX+1; ++i) 
        {
            for (int j = 0; j < MAX+1; ++j) {
                _record[i][j] = '\0';
            }
        }

        for(int i = 0; i < s.size(); i++)
        {
            strncpy(_record[i], s[i].c_str(), MAX);

        }   
       
    }

    // the functions from utilities.h: (may just remove utilities.h)

    void open_fileRW(fstream& f, const char filename[]);
    void open_fileW(fstream& f, const char filename[]);
    bool file_exists(const char filename[]);

    long write(fstream& outs);              //returns the record number
    long read(fstream& ins, long recno);    //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file


    friend ostream& operator<<(ostream& outs,
                               const FileRecord& r);
    static const int MAX = 100;
    int recno;
    char _record[MAX+1][MAX+1];
};




#endif