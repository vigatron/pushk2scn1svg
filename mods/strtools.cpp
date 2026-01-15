#include "strtools.hpp"

#include <iostream>
#include <sstream>

static const char * _szhex = "0123456789ABCDEF";

const char * szhex () { return _szhex; }

bool check_str_digit(std::string str) {
    if(!str.size()) return false;
    for(char s : str) { if(s<'0' || s>'9') return false; }
    return true; }


bool check_str_ishex(std::string str) {
    for( char s : str) {
        bool p1 = (s >='0' && s <='9');
        bool p2 = (s >='A' && s <='F');
        bool p3 = (s >='a' && s <='f');
        bool p  = p1 || p2 || p3;
        if(!p) return false; }
    return true; }


std::vector<std::string> split(std::string str, char sym) {

    std::vector<std::string>    r;
    std::vector<int>            arrsep;

    if(!str.size()) return r;

    // Find all separators
    for(int i=0;i<str.size();i++) {
        if(str[i]==sym) arrsep.push_back(i); }
    
    if(!arrsep.size()) {
        r.push_back(str);
    } else {

        std::string     s;

        for(int i=0;i<arrsep.size();i++) {
            bool            first = !i;

            if(first) { s = arrsep[i] ? str.substr(0, arrsep[i]) : ""; } 
            else {
                int posf = arrsep[i-1];
                int post = arrsep[i]-1;
                s = str.substr(posf+1, post - posf); }
            r.push_back(s);
        }

        int     posl = arrsep.back();
        bool    empt = posl == str.size()-1;
        s = empt ? "" : str.substr(posl+1, str.size() - posl);
        r.push_back(s); }

    return r; }

std::string join(const std::vector<std::string>& elements, const std::string& delimiter) {
    std::ostringstream os;
    for (size_t i = 0; i < elements.size(); ++i) {
        os << elements[i]; if (i < elements.size() - 1) { os << delimiter; } }
    return os.str(); } // Get the final string
