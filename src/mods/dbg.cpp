#include "dbg.hpp"


#define     OUT_BUF_MAX     512

char outBuf[OUT_BUF_MAX];


DBG::DBG()   { }
DBG::~DBG()  { if (file.is_open()) file.close(); }

void DBG::flush             ()                                          { fflush(stdout); }
void DBG::disiobuff         ()                                          { setbuf(stdout, NULL); }
void DBG::nl                ()                                          { lout("\n"); }
void DBG::spc               ()                                          { lout(" "); }
void DBG::txt               (const char *str)                           { lout(str); }
void DBG::txtnl             (const char *str)                           { txt(str); nl();   }
void DBG::hx8               (uint8_t val)                               { lout_h8(val); }
void DBG::itxt              (const char *txt, int v)                    { lout_itxt(txt, v); }
void DBG::i2txt             (const char *txt, int v1, int v2)           { lout_i2txt(txt, v1, v2); }
void DBG::i3txt             (const char *txt, int v1, int v2, int v3)   { lout_i3txt(txt, v1, v2, v3); }
void DBG::ftxt              (const char *txt, float f)                  { lout_ftxt(txt, f); }
void DBG::sep               ()                                          { for(int i=0; i < 16; i++) lout("--------"); nl(); }

void DBG::mem   (uint8_t *ptr, int wdt, int align) { 
    for (int i = 0; i < wdt; i++) {hx8( ptr[i] ); if(i!=(wdt-1)) spc(); } 
    if(align != -1) { int ins = align - wdt; for(int i=0;i<ins;i++) { txt("   "); } } }

void DBG::memblk   (uint8_t * ptr, int w, int icnt) {
    int rows = (w / icnt) + ((w % icnt) ? 1:0);
    for(int j=0;j<rows;j++) {
        int offs = j * icnt; int cnt = (j != (rows-1)) ? icnt : ( w - offs );
        lout_i2txt("%8d %8X : ", offs, offs);
        for(int i=0;i < cnt;i++) { uint8_t v = ptr[offs+i];
            if(! (i%8) ) { lout(" "); } lout_itxt("%.2x ", v); } nl(); } flush(); }

void DBG::memblk16spc (int16_t * ptr, int w, int icnt) {
    int rows = (w / icnt) + ((w % icnt) ? 1:0);
    for(int j=0;j<rows;j++) {
        int offs = j * icnt; int cnt = (j != (rows-1)) ? icnt : ( w - offs );
        lout_i2txt("%8d %8X : ", offs, offs);
        for(int i=0;i < cnt;i++) { int16_t v = ptr[offs+i];
            if(! (i%8) ) lout(" "); if(v >= 0 ) lout_itxt("%.2x ", v); else lout("-- "); } nl(); } flush(); }


void DBG::setfname( std::string fname ) {
    _fname = fname;
    file.open(_fname); }


void DBG::prnbuf() {
    if (!file.is_open())
        printf("%s", outBuf);
    else { file << outBuf; } }

void DBG::lout(const char * txt) {
    snprintf(outBuf, OUT_BUF_MAX, "%s", txt);
    prnbuf(); }

void DBG::lout_h8(uint8_t val) {
    snprintf(outBuf, OUT_BUF_MAX, "%.2X", val);
    prnbuf(); }

void DBG::lout_itxt(const char *txt, int v) {
    snprintf(outBuf, OUT_BUF_MAX, txt, v);
    prnbuf(); }

void DBG::lout_i2txt(const char *txt, int v1, int v2) {
    snprintf(outBuf, OUT_BUF_MAX, txt, v1, v2);
    prnbuf(); }

void DBG::lout_i3txt(const char *txt, int v1, int v2, int v3) {
    snprintf(outBuf, OUT_BUF_MAX, txt, v1, v2, v3);
    prnbuf(); }

void DBG::lout_ftxt(const char *txt, float v) {
    snprintf(outBuf, OUT_BUF_MAX, txt, v);
    prnbuf(); }
