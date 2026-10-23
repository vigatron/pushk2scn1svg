#pragma once

#include <string>

class VHSVGParam
{
public:
    /** */
    VHSVGParam(std::string pn, std::string pv)
    {
        set(pn, pv);
    }

    /** */
    VHSVGParam(std::string pn, int pv)
    {
        set(pn, std::to_string(pv));
    }

    /** */
    void set(std::string pn, std::string pv)
    {
        key = pn;
        val = pv;
    }

    /** */
    operator std::string() const
    {
        return key + "=" + quotes(val) + " ";
    }

private:
    std::string key;
    std::string val;
    std::string result;

    /** */
    std::string quotes(const std::string &str) const
    {
        return "\"" + str + "\"";
    }
};
