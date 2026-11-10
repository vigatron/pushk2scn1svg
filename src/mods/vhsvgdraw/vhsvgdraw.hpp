#pragma once

#include "vhblockstat/vhblockstat.hpp"
#include "vhsvg.hpp"

#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Stateless drawing helpers for WND SVG document elements.
 */
class VHSVGDraw
{
public:
    /** @brief Draws center and quarter guide lines. */
    static void DrawCenterLine(VHSVG &svg, const VHSVGRect &rect);

    /** @brief Draws a shadowed box. */
    static void DrawShadowBox(VHSVG &svg, const VHSVGRect &rect);

    /** @brief Draws a horizontal scale strip. */
    static void DrawScale(VHSVG &svg, const VHSVGRect &rect, int size);

    /** @brief Draws a Section view for block statistics. */
    static void DrawSection(
        VHSVG &svg,
        const VHSVGRect &rect,
        const std::vector<VHBlockStat> &arr,
        int blksize,
        int ws);

    /** @brief Draws a Density view for block statistics. */
    static void DrawDensity(
        VHSVG &svg,
        const VHSVGRect &rect,
        const std::vector<VHBlockStat> &arr,
        int blksize,
        int ws);

    /** @brief Draws the data types legend. */
    static void DrawTypes(VHSVG &svg, const VHSVGRect &rect);

private:
    // Section helpers
    static void draw_column_spread(
        VHSVG &svg,
        int idx,
        const VHSVGRect &rect,
        const std::vector<VHBlockStat> &arr);
    static void draw_col_bck(VHSVG &svg, int idx, const VHSVGRect &rect);

    // Density helpers
    static void rgb_to_compo(uint32_t v, uint8_t *r, uint8_t *g, uint8_t *b);
    static uint8_t scaleval(uint8_t v1, uint8_t v2, uint8_t pos, uint8_t range);
    static std::string hexval(int v);
    static std::string strrgb(uint8_t r, uint8_t g, uint8_t b);
    static std::string strrgb(uint32_t v);
    static std::string lincolor(uint8_t vv);
    static void draw_column_mask(
        VHSVG &svg,
        int idx,
        const VHSVGRect &rect,
        const std::vector<VHBlockStat> &arr);

    // Types helpers
    static std::string gettype(int i, int j);
    static const std::vector<std::string> &types();
};
