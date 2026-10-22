#pragma once

#include <stdint.h>
#include <string>

class VHFile {

    public:

        VHFile() : file(nullptr)            { }
        VHFile(std::string filename)        { assign(filename); }
        ~VHFile()                           { closefile(); }

        void    assign(std::string filename) { fname = filename; }
        
        int     flen() {
            int r = -1;
            if(!openfile()) { dmperr(); return r; }
            if(!fseek(file, 0, SEEK_END)) { r = ftell(file); }
            closefile();
            return r; }

        bool    read( void * ptr, int sz, int seekoffs = 0) {
            if (!openfile()) {  dmperr(); return false; }
            if (seekoffs) { if(fseek(file, seekoffs, SEEK_SET) != 0) { closefile(); return false; } }
            int result = fread( ptr, sz, 1, file);
            closefile();
            return (result<1) ? false : true; }

        bool    write(void *ptr, int sz) {
            if(!createfile()) { dmpwerr(); return false; }
            int result = fwrite( ptr, sz, 1, file);
            closefile();
            return result == 1;
        }

    private:

        bool openfile()     { file = fopen(fname.c_str(), "r"); return file != NULL; }
        bool createfile()   { file = fopen(fname.c_str(), "w"); return file != NULL; }
        bool closefile()    { bool v = (file != NULL); if(v) { fclose(file); file = NULL; } return v; }
        void dmperr()       { printf("ERROR: File (%s) cannot be opened.\n", fname.c_str()); }
        void dmpwerr()      { printf("ERROR: File (%s) cannot be created.\n", fname.c_str()); }

        std::string fname;
        FILE* file;

};
