// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "board.hpp"

#include "context.hpp"
#include "pieces.hpp"
#include "random.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <set>

namespace boardgame
{
    Board::Board(const sf::Vector2u & windowSize)
        : window_size(windowSize)
        , window_rect({}, window_size)
        , board_region_vert_size_ratio(1.0f - (1.0f / 8.0f))
        , between_region_vert_scale(1.0f / 100.0f)
        , board_region()
        , status_region()
        , cell_counts()
        , cell_count_total(0)
        , cell_countsI()
        , cell_size()
        , board_rect()
        , cells()
        , pieces()
    {
        board_region = window_rect;
        board_region.height *= board_region_vert_size_ratio;

        status_region = window_rect;
        status_region.height = (window_rect.height - board_region.height);
        status_region.top = util::bottom(board_region);

        cell_counts = sf::Vector2s(map_strings.front().size(), map_strings.size());
        cell_count_total = (cell_counts.x * cell_counts.y);
        cell_countsI = sf::Vector2i(cell_counts);

        const sf::Vector2f cellSizeOrig{ util::size(board_region) /
                                         sf::Vector2f(cell_counts) };

        // cell_size = cellSizeOrig;
        const float cellSideLength{ std::min(cellSizeOrig.x, cellSizeOrig.y) };
        cell_size.x = cellSideLength;
        cell_size.y = cellSideLength;

        const sf::Vector2f boardRectSize{ sf::Vector2f(cell_counts) * cell_size };

        const sf::Vector2f boardRectPosToCenter{ util::center(board_region) -
                                                 (boardRectSize * 0.5f) };

        board_rect = sf::FloatRect(boardRectPosToCenter, boardRectSize);

        reset();
    }

    int Board::walkDistance(const IPiece & from, const IPiece & to)
    {
        return walkDistance(from.boardPos(), to.boardPos());
    }

    void Board::reset()
    {
        pieces.clear();

        // cells
        for (std::size_t vert(0); vert < cell_counts.y; ++vert)
        {
            for (std::size_t horiz(0); horiz < cell_counts.x; ++horiz)
            {
                const BoardPos_t boardPos{ sf::Vector2s(horiz, vert) };

                const sf::Vector2f windowPos{ util::position(board_rect) +
                                              (sf::Vector2f(boardPos) * cell_size) };

                Cell cell(boardPos, windowPos, cell_size);

                const bool isWall{ Image::isWall(map_strings.at(vert).at(horiz)) };
                if (isWall)
                {
                    cell.piece_enum = Piece::Wall;
                }

                cells.insert({ boardPos, cell });
            }
        }
    }

    // TODO optimize this crap
    std::optional<BoardPos_t> Board::findRandomEmptyPos(const Context & context)
    {
        std::set<BoardPos_t> emptyPositions;
        for (auto & [boardPos, cell] : cells)
        {
            emptyPositions.insert(boardPos);
        }

        for (const auto & piece : pieces)
        {
            emptyPositions.erase(piece->boardPos());
        }

        if (emptyPositions.empty())
        {
            return std::nullopt;
        }

        return context.random.from(emptyPositions);
    }
} // namespace boardgame