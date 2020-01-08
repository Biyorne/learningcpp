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
        , inner_window_scale(1.0f - (1.0f / 100.0f))
        , board_region_vert_size_ratio(1.0f - (1.0f / 8.0f))
        , cell_size_ratio(1.0f / 16.0f)
        , between_region_vert_scale((1.0f - inner_window_scale) * 2.0f)
        , inner_window_rect()
        , cell_size()
        , cell_counts()
        , cell_count_total(0)
        , board_region()
        , status_region()
    {
        // inner window that puts a border around everything drawn
        inner_window_rect =
            sf::FloatRect(sf::IntRect(util::scaleRectInPlaceCopy(window_rect, inner_window_scale)));

        // cell size
        const float cellSizeLength{ std::floor(inner_window_rect.height * cell_size_ratio) };
        cell_size = { cellSizeLength, cellSizeLength };

        // cell counts
        const sf::FloatRect maxBoardRect{
            inner_window_rect.left,
            inner_window_rect.top,
            inner_window_rect.width,
            std::floor(inner_window_rect.height * board_region_vert_size_ratio)
        };

        cell_counts.x = static_cast<std::size_t>(maxBoardRect.width / cell_size.x);
        cell_counts.y = static_cast<std::size_t>(maxBoardRect.height / cell_size.y);
        cell_count_total = (cell_counts.x * cell_counts.y);

        // board region
        const sf::Vector2f boardRegionSize{ sf::Vector2i(cellCounts<float>() * cell_size) };

        board_region = { (util::center(inner_window_rect) - (boardRegionSize * 0.5f)),
                         boardRegionSize };

        board_region.top = board_region.left;

        // status region
        status_region = board_region;

        status_region.top =
            (util::bottom(board_region) + (inner_window_rect.height * between_region_vert_scale));

        status_region.height =
            ((util::bottom(window_rect) - board_region.left) - status_region.top);

        // cells
        for (std::size_t i(0); i < cell_count_total; ++i)
        {
            auto calcBoardPos = [&](const std::size_t index) {
                return BoardPos_t(
                    static_cast<int>(index % cell_counts.x),
                    static_cast<int>(index / cell_counts.x));
            };

            auto calcWindowRect = [&](const std::size_t index) {
                const BoardPos_t boardPos{ calcBoardPos(index) };

                return sf::FloatRect(
                    util::position(board_region) + (sf::Vector2f(boardPos) * cell_size), cell_size);
            };

            cells.push_back(Cell(i, calcBoardPos(i), calcWindowRect(i)));
        }

        reset();
    }

    int Board::walkDistance(const IPiece & from, const IPiece & to)
    {
        return walkDistance(from.boardPos(), to.boardPos());
    }

    void Board::reset()
    {
        demons.clear();
        players.clear();
        kids.clear();
        walls.clear();

        // empty_board_pos.clear();
        // for (const Cell & cell : cells)
        //{
        //    empty_board_pos.push_back(cell.board_pos);
        //}
    }

    BoardPos_t Board::randomAvailBoardPos(const Context & context) const
    {
        std::set<BoardPos_t> emptyPositions;
        for (const Cell & cell : cells)
        {
            emptyPositions.insert(cell.board_pos);
        }

        // clang-format off
        for (const auto & uptr : walls)   emptyPositions.erase(uptr->boardPos());
        for (const auto & uptr : players) emptyPositions.erase(uptr->boardPos());
        for (const auto & uptr : kids)    emptyPositions.erase(uptr->boardPos());
        for (const auto & uptr : demons)  emptyPositions.erase(uptr->boardPos());
        // clang-format on

        if (emptyPositions.empty())
        {
            throw std::runtime_error(
                "Board::randomAvailBoardPos() called when there are no avail spaces");
        }

        return context.random.from(emptyPositions);
    }

    Piece Board::pieceAt(const BoardPos_t & pos) const
    {
        Piece piece{ Piece::Empty };

        auto findAndSet = [&](const auto & container) {
            if (piece != Piece::Empty)
            {
                return;
            }

            for (const auto & uptr : container)
            {
                if (uptr->boardPos() == pos)
                {
                    piece = uptr->piece();
                    break;
                }
            }
        };

        findAndSet(demons);
        findAndSet(players);
        findAndSet(kids);
        findAndSet(walls);

        return piece;
    }
} // namespace boardgame