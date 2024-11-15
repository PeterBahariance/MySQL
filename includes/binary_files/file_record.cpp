#include "file_record.h"

void FileRecord::open_fileRW(fstream& f, const char filename[]){
    
    //cout << "This is the filename in open_fileRW: " << filename << endl;
    const bool debug = false;
    //opening a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        //create the file
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}

void FileRecord::open_fileW(fstream& f, const char filename[]){
        // test
    // cout << "Attempting to open: " << filename << endl;
    f.open (filename, std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }
    // cout << "THE FILE WAS OPENED" << endl;


}

bool FileRecord::file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}


long FileRecord::read(fstream &ins, long recno){
    long pos= recno * sizeof(_record);
    ins.seekg(pos, ios_base::beg);

    for(int i = 0; i < (sizeof(_record)/sizeof(_record[0])); i++)
    {
        ins.read(_record[i], sizeof(_record[i]));
        _record[i][ins.gcount()] = '\0';
    }
    return ins.gcount();


}

long FileRecord::write(fstream &outs){
    //r.write(f); //take the Record r and write it into file f
    //  and return the record number of this Record

    //write to the end of the file.
    outs.seekg(0, outs.end);

    long pos = outs.tellp();    //retrieves the current position of the
                                //      file pointer


    for(int i = 0; i < (sizeof(_record)/sizeof(_record[0])); i++)
    {
        outs.write(_record[i], sizeof(_record[i])); // the size of _record[i] will always be the same 
    }


    return pos/sizeof(_record);  //record number 
}

ostream& operator<<(ostream& outs,
                    const FileRecord& r){
    
    for(int i = 0; i < (sizeof(r._record)/sizeof(r._record[0])); i++)
    {
        if(strlen(r._record[i]) > 0)
        {
            outs << r._record[i] << setw(25);
        }
    }
    
    return outs;
}