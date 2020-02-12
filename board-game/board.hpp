#ifndef BOARDGAME_BOARD_HPP_INCLUDED
#define BOARDGAME_BOARD_HPP_INCLUDED
//
// board.hpp
//
#include "types.hpp"
#include "util.hpp"

#include <functional>
#include <map>
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

    using BoardPos_t = sf::Vector2i;
    using BoardPosVec_t = std::vector<BoardPos_t>;
    using BoardPosOpt_t = std::optional<BoardPos_t>;

    using PositionPieceMap_t = std::map<BoardPos_t, Piece::Enum>;

    //

    struct IBoard
    {
        virtual ~IBoard() = default;

        virtual void reset() = 0;
        virtual void setupMap(const Context & context) = 0;

        virtual sf::Vector2f cellSize() const = 0;
        virtual sf::FloatRect cellBounds(const BoardPos_t & pos) const = 0;

        virtual sf::Vector2s cellCounts() const = 0;
        virtual std::size_t cellCountTotal() const = 0;

        template <typename T = std::size_t>
        sf::Vector2<T> cellCountsAs() const
        {
            return sf::Vector2<T>(cellCounts());
        }

        virtual IPieceUVec_t & pieces() = 0;
        virtual const IPieceUVec_t & pieces() const = 0;

        virtual bool isAnyPieceAt(const BoardPos_t & pos) const = 0;
        virtual IPieceOpt_t pieceAt(const BoardPos_t & pos) const = 0;
        virtual Piece::Enum pieceEnumAt(const BoardPos_t & pos) const = 0;

        // virtual BoardPosOpt_t findRandomEmptyPos(const Context & context) const = 0;

      protected:
        virtual void setupCellSizeAndCounts(
            const sf::Vector2f & windowSize, const float cellSizeAswindowRatio) = 0;

        virtual void setupStartingMapPieces() = 0;
    };

    //

    class BoardBase : public IBoard
    {
      public:
        BoardBase() { reset(); }
        virtual ~BoardBase() = default;

        void reset() override;

        sf::Vector2f cellSize() const override { return m_cellSize; }

        sf::FloatRect cellBounds(const BoardPos_t & boardPos) const override;

        sf::Vector2s cellCounts() const override { return m_cellCounts; }
        std::size_t cellCountTotal() const override { return (m_cellCounts.x * m_cellCounts.y); }

        IPieceUVec_t & pieces() override { return m_pieces; }
        const IPieceUVec_t & pieces() const override { return m_pieces; }

        IPieceOpt_t pieceAt(const BoardPos_t & pos) const override;
        Piece::Enum pieceEnumAt(const BoardPos_t & pos) const override;
        bool isAnyPieceAt(const BoardPos_t & pos) const override;

        // BoardPosOpt_t findRandomEmptyPos(const Context & context) const override;

      protected:
        void setupCellSizeAndCounts(
            const sf::Vector2f & windowSize, const float cellSizeAswindowRatio) override;

        // default does nothing so more complex boards can do it another way
        void setupStartingMapPieces() override {}

      protected:
        sf::Vector2f m_cellSize;
        sf::Vector2s m_cellCounts;
        sf::Vector2f m_topLeftCellPos;
        IPieceUVec_t m_pieces;
    };

    //

    struct SnakeBoard : public BoardBase
    {
        SnakeBoard() = default;
        virtual ~SnakeBoard() = default;

        void setupMap(const Context & context) override;

        IPieceUPtr_t makePiece(
            const Context & context, const Piece::Enum piece, const BoardPos_t & boardPos) const;
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