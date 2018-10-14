#ifndef LIGHTS_OUT_SCREEN_UTIL_HPP_INCLUDED
#define LIGHTS_OUT_SCREEN_UTIL_HPP_INCLUDED

#include <SFML/Graphics.hpp>

namespace lightsout
{
    using GridPos_t = sf::Vector2i;

    struct GridRegion
    {
        GridPos_t grid_pos;
        sf::FloatRect region;
    };

    inline std::vector<GridRegion> splitRegionIntoGrids(
        const sf::FloatRect & REGION, const std::size_t HORIZ, const std::size_t VERT)
    {
        std::vector<GridRegion> grids;

        const std::size_t CELL_COUNT(HORIZ * VERT);
        grids.reserve(CELL_COUNT);

        const float PAD_SCREEN_FRACTION(0.01f);
        const float SCREEN_DIMENSION_AVG((REGION.width + REGION.height) * 0.5f);
        const float CELL_PAD(SCREEN_DIMENSION_AVG * PAD_SCREEN_FRACTION);

        const float CELL_WIDTH((REGION.width - ((HORIZ + 1) * CELL_PAD)) / HORIZ);

        const float CELL_HEIGHT((REGION.height - ((VERT + 1) * CELL_PAD)) / VERT);

        const sf::Vector2f CELL_SIZE_V(CELL_WIDTH, CELL_HEIGHT);

        for (std::size_t row(0); row < VERT; ++row)
        {
            const float ROW_FLOAT(static_cast<float>(row));

            for (std::size_t column(0); column < HORIZ; ++column)
            {
                const float COLUMN_FLOAT(static_cast<float>(column));

                const float LEFT_OFFSET(
                    (CELL_PAD * (COLUMN_FLOAT + 1.0f)) + (CELL_WIDTH * COLUMN_FLOAT));

                const float TOP_OFFSET((CELL_PAD * (ROW_FLOAT + 1.0f)) + (CELL_HEIGHT * ROW_FLOAT));

                const sf::Vector2f CELL_POSITION_V(
                    (REGION.left + LEFT_OFFSET), (REGION.top + TOP_OFFSET));

                const GridPos_t CELL_GRID_POSITION_V(
                    static_cast<int>(column), static_cast<int>(row));

                const sf::FloatRect CELL_REGION(CELL_POSITION_V, CELL_SIZE_V);

                GridRegion gridRegion;
                gridRegion.region = CELL_REGION;
                gridRegion.grid_pos = CELL_GRID_POSITION_V;

                grids.push_back(gridRegion);
            }
        }

        return grids;
    }

} // namespace lightsout

#endif // LIGHTS_OUT_SCREEN_UTIL_HPP_INCLUDED
