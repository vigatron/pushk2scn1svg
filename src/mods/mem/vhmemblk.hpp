#pragma once

#include <stdint.h>
#include <malloc.h>
#include "vhfile.hpp"

class VHMemBlock {

    public:

        VHMemBlock                  () : _ptr(nullptr), _sz(0)   { }
        VHMemBlock                  (int sz)    { alloc(sz); }
        ~VHMemBlock                 ()          { dealloc(); }

        bool        alloc           (int siz)   { _ptr = (uint8_t *)malloc(siz); _sz = valid() ? siz : 0; return valid(); }
        void        dealloc         ()          { if(valid()) { free(_ptr); _ptr = nullptr; _sz = 0; } }
        uint8_t *   ptr             () const    { return _ptr; }
        int         sz              () const    { return _sz; }
        uint8_t *   operator()      () const    { return (uint8_t *) ptr(); }
        int16_t *   ptrsi16         ()          { return (int16_t *) ptr(); }
        bool        empty()                     { return _ptr == nullptr; }
        bool        valid()                     { return _ptr != nullptr; }

        void clone(const VHMemBlock & src) {
            alloc(src._sz); if(valid()) { for(int i=0;i < src._sz;i++) {
                *(_ptr + i) = *(src._ptr + i); } } }
        
        bool LoadFromFile(const char * fname, int offset = -1, int sz = -1) {

            VHFile f(fname);
            int flen = f.flen();
            if(flen < 1) return false;

            int offs = (offset == -1) ? 0 : offset;
            if(offs >= flen) return false;

            int rlen = (sz == -1) ? (flen - offs ) : sz;
            if((rlen + offs) > flen) return false;

            if(!alloc(rlen)) return false;
            return f.read(_ptr, rlen, offs);
        }

        bool SaveToFile(const char * fname, int offset = -1, int sz = -1) {

            VHFile f(fname);

            uint8_t offs = (offset == -1) ? 0 : offset;
            int     wlen = (sz == -1) ? _sz : sz;
            
            if((offs + wlen) > _sz) return false;
            return f.write(_ptr + offs, wlen);
        }

    protected:

        uint8_t * _ptr; int _sz;

        uint8_t hh(uint8_t v) { return (v>>4) & 0xF; }
        uint8_t hl(uint8_t v) { return (v>>0) & 0xF; }

    private:

};
