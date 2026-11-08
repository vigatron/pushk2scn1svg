#pragma once

#include "vhplatform.hpp"
#include "structs/blockprops.hpp"
#include "draw/draw.hpp"
#include "vhsvg.hpp"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Generates an SVG "window" document (2x4 grid) from a source byte buffer.
 *
 * For each row, data is sampled with a different chunk size and rendered as
 * a Section (left) and a Density view (right), plus a types legend.
 */
class VHDocWnd
{
public:
    /// SVG paper width in pixels.
    static constexpr uint16_t kPaperWidth = 3200;
    /// SVG paper height in pixels.
    static constexpr uint16_t kPaperHeight = 2400;
    /// Horizontal paper border in pixels.
    static constexpr uint16_t kPaperBorderX = 100;
    /// Vertical paper border in pixels.
    static constexpr uint16_t kPaperBorderY = 100;

    /// Header area height in pixels.
    static constexpr int kHeaderHeight = 200;
    /// Footer area height in pixels.
    static constexpr int kFooterHeight = 200;

    /// Number of grid columns.
    static constexpr int kGridCols = 2;
    /// Number of grid rows.
    static constexpr int kGridRows = 4;

    /// Base chunk size in bytes (row 0); each next row uses kBaseChunkSize << row.
    static constexpr size_t kBaseChunkSize = 64;

    /// Inner padding for each grid cell in pixels.
    static constexpr int kCellPadding = 64;
    /// Horizontal padding for the types legend area in pixels.
    static constexpr int kTypesPaddingX = 64;
    /// Vertical padding for the types legend area in pixels.
    static constexpr int kTypesPaddingY = 16;

    /**
     * @brief Builds the document layout, renders content, and saves the SVG file.
     *
     * @param arrsrc Source data buffer to analyze and visualize.
     * @param fnamewnd Output SVG file path.
     * @return vok on success, or an error status on failure.
     */
    verr GenerateDocument(const std::vector<uint8_t> &arrsrc, const std::string &fnamewnd) const;

private:
    /**
     * @brief Draws the 2x4 content grid (Section/Density per row).
     *
     * @param svg Target SVG document.
     * @param arrsrc Source data buffer.
     * @param rect Bounding rectangle for the grid area.
     */
    void Draw2X4Content(VHSVG &svg, const std::vector<uint8_t> &arrsrc, const VHSVGRect &rect) const;

    /**
     * @brief Splits source data into fixed-size blocks and scans properties for each.
     *
     * @param arrsrc Source data buffer.
     * @param subsize Block size in bytes; must be greater than 0.
     * @return Vector of scanned block properties (may be empty if subsize is 0 or data is smaller than one block).
     */
    static std::vector<BlockProps> GenSamplesArray(const std::vector<uint8_t> &arrsrc, size_t subsize);
};
