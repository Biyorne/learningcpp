#ifndef BOARDGAME_BOARD_HPP_INCLUDED
#define BOARDGAME_BOARD_HPP_INCLUDED
//
// board.hpp
//
#include "resources.hpp"
#include "types.hpp"
#include "util.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <vector>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    struct Context;

    struct IPiece;
    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUVec_t = std::vector<IPieceUPtr_t>;
    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;
    using IPieceCOpt_t = std::optional<std::reference_wrapper<const IPiece>>;

    //

    struct Cell
    {
        Cell(
            const BoardPos_t & boardPos,
            const sf::Vector2f & pixelPos,
            const sf::Vector2f & pixelSize)
            : piece_enum(Piece::Count)
            , piece_index(0) // this value is only correct if piece_enum != Count
            , board_pos(boardPos)
            , window_pos(pixelPos)
            , window_size(pixelSize)
            , window_rect(pixelPos, pixelSize)
            , window_rect_center(util::center(window_rect))
        {}

        Piece::Enum piece_enum;
        std::size_t piece_index;
        BoardPos_t board_pos;
        sf::Vector2f window_pos;
        sf::Vector2f window_size;
        sf::FloatRect window_rect;
        sf::Vector2f window_rect_center;
    };

    //

    struct Board
    {
        explicit Board(const sf::Vector2u & windowSize);

        // std::size_t cellIndex(const BoardPos_t & pos) const
        //{
        //    assert(isPosValid(pos) == true);
        //
        //    return (
        //        (static_cast<std::size_t>(pos.y) * (cell_counts.x)) +
        //        static_cast<std::size_t>(pos.x));
        //}

        bool isPosValid(const BoardPos_t & pos) const
        {
            return (
                (pos.x >= 0) && (pos.x < cell_countsI.x) && (pos.y >= 0) &&
                (pos.y < cell_countsI.y));
        }

        const Cell & cellAt(const BoardPos_t & pos) const { return cells.find(pos)->second; }
        Cell & cellAt(const BoardPos_t & pos) { return cells.find(pos)->second; }

        const IPieceCOpt_t pieceAt(const BoardPos_t & pos) const
        {
            const Cell & cell{ cellAt(pos) };
            if (cell.piece_enum == Piece::Count)
            {
                return std::nullopt;
            }

            const std::size_t pieceIndex{ cell.piece_index };
            if (pieceIndex >= pieces.size())
            {
                return std::nullopt;
            }

            return { *pieces.at(pieceIndex) };
        }

        IPieceOpt_t pieceAt(const BoardPos_t & pos)
        {
            const Cell & cell{ cellAt(pos) };
            if (cell.piece_enum == Piece::Count)
            {
                return std::nullopt;
            }

            const std::size_t pieceIndex{ cell.piece_index };
            if (pieceIndex >= pieces.size())
            {
                return std::nullopt;
            }

            return { *pieces.at(pieceIndex) };
        }

        static int walkDistance(const BoardPos_t & from, const BoardPos_t & to)
        {
            return (std::abs(to.x - from.x) + std::abs(to.y - from.y));
        }

        static int walkDistance(const IPiece & from, const IPiece & to);

        void reset();

        std::optional<BoardPos_t> findRandomEmptyPos(const Context & context);

        // clang-format off
        const std::vector<std::string> map_strings = {
            "CHHHHHHHTHHHHHHHHHHHHHCHCHHHHT",
            "VFFFFFFFVFFFFFFFFFFFFFVFVFFFFV",
            "VFFFTFBHBFFBHTHBFTFFFFBFBFFFFV",
            "VFFFVFFFFFFFFVFFFVFFFFFFFFFFFV",
            "VFCHBFFBHTFFFBFFFBHTHBFFCHHBFV",
            "VFVFFFFFFVFFFFFFFFFVFFFFVFFFFV",
            "VFBFFFFFFVFTFFFFTFFBFFFFBFFBHB",
            "VFFFFFFFFBFVFFFFVFFFFFCFFFFFFV",
            "VFBHTHHBFFFVFCHHBFFFFFVFFFFBHT",
            "VFFFVFFFFBHBFVFFFFFFFFBHHTFFFV",
            "VFFFVFTFFFFFFBFFFTFFFFFFFVFFFV",
            "VFFFBFVFFFFFFFFFFVFFFFFFFVFFFV",
            "BHBFFFBHBFTHBFBHHBHBFTFTFBFFFV",
            "VFFFFFFFFFVFFFFFFFFFFVFVFFFFFV",
            "BHHHHHHHHHBHHHHHHHHHHBHBHHHHHB" };
        // clang-format on

        const sf::Vector2f window_size;
        const sf::FloatRect window_rect;
        const float board_region_vert_size_ratio;
        const float between_region_vert_scale;

        sf::FloatRect board_region;
        sf::FloatRect status_region;
        sf::Vector2s cell_counts;
        std::size_t cell_count_total;
        sf::Vector2i cell_countsI;
        sf::Vector2f cell_size;
        sf::FloatRect board_rect;

        std::map<BoardPos_t, Cell> cells;
        std::vector<IPieceUPtr_t> pieces;
    };
} // namespace boardgame

#endif // BOARDGAME_BOARD_HPP_INCLUDED