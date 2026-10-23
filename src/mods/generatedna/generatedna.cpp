#include "generatedna.hpp"

#include "widerange/widerange.hpp"


verr VHDocDNA::GenerateDocument(
    std::vector<uint8_t> arrsrc,
    const std::string &fnamedna)
{

    // WideRange Scan
    VHWideRangeScan wscan;

    if (vok != wscan.scan(arrsrc))
        return verrmsg(1, "Wide Range analisys failed");


    return vok;
}
