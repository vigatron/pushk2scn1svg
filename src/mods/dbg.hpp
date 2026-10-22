#pragma once

#include <stdint.h>
#include <stdio.h>
#include <cstdio>
#include <string>
#include <fstream>

class DBG {

    public:

        DBG();
        ~DBG();

        void setfname( std::string fname );

        void nl          ();
        void spc         ();

        void txt         (const char *str);
        void txtnl       (const char *str);

        void hx8         (uint8_t val);

        void itxt        (const char *txt, int v);
        void i2txt       (const char *txt, int v1, int v2);
        void i3txt       (const char *txt, int v1, int v2, int v3);

        void ftxt        (const char *txt, float f);

        void sep         ();

        void mem         (uint8_t  * ptr, int wdt, int align=-1);
        
        void memblk      (uint8_t  * ptr, int w, int icnt = 32 );
        void memblk16spc (int16_t  * ptr, int w, int icnt = 32 );

        void flush       ();
        void disiobuff   ();

    private:

        std::string _fname;
        std::ofstream file;

        // void lout(const char * txt);
        void prnbuf();
        void lout(const char * txt);
        void lout_h8(uint8_t val);
        void lout_itxt(const char *txt, int v);
        void lout_i2txt(const char *txt, int v1, int v2);
        void lout_i3txt(const char *txt, int v1, int v2, int v3);
        void lout_ftxt(const char *txt, float v);

};
