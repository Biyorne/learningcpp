#ifndef BOARDGAME_BOARD_HPP_INCLUDED
#define BOARDGAME_BOARD_HPP_INCLUDED
//
// board.hpp
//
#include "types.hpp"
#include "util.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

//

namespace sf
{
    using Vector2s = sf::Vector2<std::size_t>;
}

//

namespace boardgame
{
    struct Context;
    struct IResources;

    struct IPiece;
    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUVec_t = std::vector<IPieceUPtr_t>;
    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;

    //

    using BoardPos_t = sf::Vector2i;
    using BoardPosVec_t = std::vector<BoardPos_t>;
    using BoardPosOpt_t = std::optional<BoardPos_t>;

    // not an accurate count of steps
    int walkDistance(const BoardPos_t & from, const BoardPos_t & to);
    int walkDistance(const IPiece & from, const IPiece & to);

    //

    using MapLayout_t = std::vector<std::string>;

    //

    struct IMap
    {
        virtual ~IMap() = default;

        virtual sf::Vector2s cellCounts() const = 0;
        virtual char charAt(const BoardPos_t & boardPos) const = 0;
        virtual bool isPosValid(const BoardPos_t & boardPos) const = 0;
        virtual const MapLayout_t & layout() const = 0;
        virtual std::size_t cellCountTotal() const = 0;
        virtual const BoardPosVec_t allValidPositions() const = 0;

        template <typename T = std::size_t>
        sf::Vector2<T> cellCountsAs() const
        {
            return sf::Vector2<T>(cellCounts());
        }
    };

    //

    class MapBase : public IMap
    {
      public:
        explicit MapBase(const MapLayout_t & rowStrings);
        virtual ~MapBase() = default;

        sf::Vector2s cellCounts() const override { return m_cellCounts; }
        char charAt(const BoardPos_t & boardPos) const override;
        bool isPosValid(const BoardPos_t & boardPos) const override;
        const MapLayout_t & layout() const override { return m_layout; }
        std::size_t cellCountTotal() const override { return (m_cellCounts.x * m_cellCounts.y); }
        const BoardPosVec_t allValidPositions() const override { return m_allValidPositions; }

      private:
        MapLayout_t m_layout;
        sf::Vector2s m_cellCounts;
        BoardPosVec_t m_allValidPositions;
    };

    //

    struct BoardView
    {
        BoardView(const sf::Vector2u & windowSize, const sf::Vector2s & cellCounts);

        sf::Vector2f window_size;
        sf::FloatRect window_rect;
        sf::FloatRect board_region;
        sf::FloatRect status_region;
        sf::Vector2f cell_size;
        sf::FloatRect board_rect;

      private:
        static inline const float region_pad_ratio{ 0.95f };
        static inline const float board_region_vert_size_ratio{ 0.875f };
        static inline const float between_region_vert_scale{ region_pad_ratio / 2.0f };
    };

    //

    struct IBoard
    {
        virtual ~IBoard() = default;

        virtual void reset() = 0;

        virtual const BoardView & view() const = 0;

        virtual sf::FloatRect calcCellBounds(const BoardPos_t & pos) const = 0;

        virtual IPieceUVec_t & pieces() = 0;
        virtual const IPieceUVec_t & pieces() const = 0;

        virtual bool isAnyPieceAt(const BoardPos_t & pos) const = 0;
        virtual IPieceOpt_t pieceAt(const BoardPos_t & pos) const = 0;
        virtual Piece::Enum pieceEnumAt(const BoardPos_t & pos) const = 0;

        virtual BoardPosOpt_t findRandomEmptyPos(const Context & context) const = 0;
    };

    //

    class BoardBase : public IBoard
    {
      public:
        BoardBase(const sf::Vector2u & windowSize, const IMap & map);
        virtual ~BoardBase() = default;

        void reset() override { m_pieces.clear(); }

        const BoardView & view() const override { return m_view; }

        sf::FloatRect calcCellBounds(const BoardPos_t & pos) const override;

        IPieceUVec_t & pieces() override { return m_pieces; }
        const IPieceUVec_t & pieces() const override { return m_pieces; }

        IPieceOpt_t pieceAt(const BoardPos_t & pos) const override;
        Piece::Enum pieceEnumAt(const BoardPos_t & pos) const override;
        bool isAnyPieceAt(const BoardPos_t & pos) const override;

        BoardPosOpt_t findRandomEmptyPos(const Context & context) const override;

      private:
        BoardView m_view;
        IPieceUVec_t m_pieces;
    };
} // namespace boardgame

#endif // BOARDGAME_BOARD_HPP_INCLUDED

//

// BoardPos_t indexToPos(const std::size_t index) const
//{
//    assert(index < map.cell_count_total);
//
//    const sf::Vector2s posSt(
//        (index % map.cell_count_horiz), (index / map.cell_count_horiz));
//
//    const BoardPos_t pos(posSt);
//    assert(isPosValid(pos));
//
//    return pos;
//}
//
// std::size_t posToIndex(const BoardPos_t & pos) const
//{
//    assert(isPosValid(pos));
//
//    const std::size_t index{ static_cast<std::size_t>(
//        (pos.y * map.cell_count_vert) + pos.x) };
//
//    return index;
//}