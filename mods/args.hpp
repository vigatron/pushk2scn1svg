#pragma once

#include "vhplatform.hpp"


class VHArgsParser {

    public:

    void setappname(std::string str) {
        appname = str; }

    bool addopt(std::string opt, std::string strdesc="") {
        if( !opt.size() ) { return false; }
        if( -1 != FindValidOpt(opt)) return false;
        arropts.push_back(opt);
        arrdesc.push_back(strdesc);
        return true; }
    
    bool checkopt(std::string opt) {
        return -1 != FindOption(opt); }
    
    int getopt(std::string opt, int defval) {
        std::string param = getopt(opt);
        return param.size() ? std::stoi(param) : defval; }

    std::string getopt(std::string opt) {
        int idx = FindOption(opt);
        return (-1 == idx) ? "" : arrval[idx]; }

    verr ParseArgs( int argc, char * argv[] ) {

        bool            optdefined = false;
        std::string     curopt = "";

        for(int i=1; i < argc; i++) {
            char * arg = argv[i];
            if( !isopt(std::string(arg))) {
                if(optdefined) {
                    addpair( curopt, arg );
                    optdefined = false;
                } else {
                    arrsingle.push_back(arg);
                }
            }
            else {
                arg++;
                int oidx = FindValidOpt(arg);
                if( -1 == oidx ) return verrmsg(1, "Invalid key : " + std::string(argv[i]));
                optdefined = true; curopt = arg; }
        }

        return vok; }

    void Usage() {
        std::cout << "USAGE : " << getappname() + " [... params ...]" << std::endl;
        for( int i=0; i < arropts.size(); i++) {
            std::cout << "\t-" + arropts[i] << "\t" << arrdesc[i] << std::endl; } }

    std::string listparams() {

        std::string r;

        // opts first
        r += " ";
        for(int i=0; i < arrpfx.size(); i++) {
            bool last = i == arrpfx.size() - 1;
            r += "-" + arrpfx[i] + " " + arrval[i];
            if(!last) r += " "; }
        
        // args next
        r += " ";
        for(int i=0; i < arrsingle.size(); i++ ) {
            bool last = i == arrsingle.size() - 1;
            r += arrsingle[i];
            if(!last) r += " "; }

        return r; }

    const std::vector<std::string> & single() const { return arrsingle; };

    private:
    
        std::string              appname;

        std::vector<std::string> arropts;
        std::vector<std::string> arrdesc;

        // Singls
        std::vector<std::string> arrsingle;

        // Pairs
        std::vector<std::string> arrpfx;
        std::vector<std::string> arrval;

        int FindValidOpt(std::string opt) {
            std::vector<std::string>::iterator it = std::find(arropts.begin(), arropts.end(), opt);
            bool found = it != arropts.end();
            return found ? std::distance(arropts.begin(), it) : -1; }
        
        int FindOption(std::string opt) {
            std::vector<std::string>::iterator it = std::find(arrpfx.begin(), arrpfx.end(), opt);
            bool found = it != arrpfx.end();
            return found ? std::distance(arrpfx.begin(), it) : -1; }
        
        bool isopt(std::string str) {
            if(str.size() < 2) return false;
            return str[0] == '-'; }
        
        void addpair( std::string opt, std::string val ) {
            arrpfx.push_back(opt); arrval.push_back(val); }

        std::string getappname() {
            return appname.size() ? appname : "application";
        }
};
