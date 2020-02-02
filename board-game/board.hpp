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
    using IndexVecs_t = std::vector<std::vector<std::size_t>>;

    //

    struct Board
    {
        Board(const ImageHandler & images, const sf::Vector2u & windowSize, const Map & level);

        void reset(const ImageHandler & images);

        bool isPosValid(const BoardPos_t & pos) const { return map.isPosValid(pos); }

        static int walkDistance(const BoardPos_t & from, const BoardPos_t & to)
        {
            return (std::abs(to.x - from.x) + std::abs(to.y - from.y));
        }

        static int walkDistance(const IPiece & from, const IPiece & to);

        std::optional<BoardPos_t> findRandomEmptyPos(const Context & context);

        BoardPos_t indexToPos(const std::size_t index) const
        {
            assert(index < map.cell_count_total);

            const sf::Vector2s posSt(
                (index % map.cell_count_horiz), (index / map.cell_count_horiz));

            const BoardPos_t pos(posSt);
            assert(isPosValid(pos));

            return pos;
        }

        std::size_t posToIndex(const BoardPos_t & pos) const
        {
            assert(isPosValid(pos));

            const std::size_t index{ static_cast<std::size_t>(
                (pos.y * map.cell_count_vert) + pos.x) };

            return index;
        }

        const IPiece & posToPiece(const BoardPos_t & pos) const
        {
            const IPieceUPtr_t & pieceUPtr{ pieces.at(posToIndex(pos)) };
            assert(pieceUPtr.get() != nullptr);
            return *pieceUPtr;
        }

        const Map map;
        const sf::Vector2f window_size;
        const sf::FloatRect window_rect;
        const float region_pad_ratio;
        const float board_region_vert_size_ratio;
        const float between_region_vert_scale;

        sf::FloatRect board_region;
        sf::FloatRect status_region;
        sf::Vector2f cell_size;
        sf::FloatRect board_rect;

        // the size of this must NEVER change, because:
        //  (1) BoardPos_t are converted to indexes into this vector
        //  (2) removed pieces are replace by EmptyPieces
        IPieceUVec_t pieces;

      private:
        IPieceUPtr_t makePiece(
            const ImageHandler & images,
            const Piece piece,
            const BoardPos_t & boardPos,
            const sf::FloatRect & bounds) const;
    };
} // namespace boardgame

#endif // BOARDGAME_BOARD_HPP_INCLUDED