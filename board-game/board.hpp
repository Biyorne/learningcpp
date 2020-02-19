#ifndef BOARDGAME_BOARD_HPP_INCLUDED
#define BOARDGAME_BOARD_HPP_INCLUDED
//
// board.hpp
//
#include "types.hpp"
#include "util.hpp"

#include <functional>
#include <list>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

//

namespace boardgame
{
    struct Context;
    struct IResources;

    using BoardPos_t = sf::Vector2i;
    using BoardPosOpt_t = std::optional<BoardPos_t>;

    struct IPiece;
    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUList_t = std::list<IPieceUPtr_t>;
    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;

    //

    struct CellLayout
    {
        sf::Vector2f size;
        sf::Vector2s counts;
        sf::Vector2i counts_int;
        std::size_t count_total{ 0 };
        sf::Vector2f top_left_pos;

        void reset(const sf::Vector2f & windowSize, const float cellSideLengthOrig);

        sf::FloatRect bounds(const BoardPos_t & pos) const
        {
            return { (top_left_pos + (sf::Vector2f(pos) * size)), size };
        }

        std::set<BoardPos_t> allCellPositions() const { return all_cell_positions; }

      private:
        std::set<BoardPos_t> all_cell_positions;
    };

    //

    struct IBoard
    {
        virtual ~IBoard() = default;

        virtual void reset(Context &) = 0;
        virtual void printStatus() const = 0;

        virtual const CellLayout & cells() const = 0;

        virtual IPieceUList_t & pieces() = 0;
        virtual const IPieceUList_t & pieces() const = 0;

        virtual bool isPieceAt(const BoardPos_t & pos) = 0;
        virtual IPieceOpt_t pieceAt(const BoardPos_t & pos) = 0;
        virtual Piece::Enum pieceEnumAt(const BoardPos_t & pos) = 0;

        virtual BoardPosOpt_t findRandomEmptyPos(const Context & context) const = 0;

        virtual IPieceUPtr_t makePiece(Context &, const Piece::Enum, const BoardPos_t & pos) = 0;
        virtual void placePiece(Context &, IPieceUPtr_t) = 0;
        virtual void placePieceAtRandomPos(Context &, const Piece::Enum) = 0;
        virtual void placePiece(Context &, const Piece::Enum, const BoardPos_t & pos) = 0;
        virtual void removePiece(Context &, const BoardPos_t & pos) = 0;
    };

    //

    class BoardBase : public IBoard
    {
      public:
        BoardBase();
        virtual ~BoardBase() = default;

        void printStatus() const override;

        const CellLayout & cells() const override { return m_cells; }

        IPieceUList_t & pieces() override { return m_pieces; }
        const IPieceUList_t & pieces() const override { return m_pieces; }

        BoardPosOpt_t findRandomEmptyPos(const Context & context) const override;

        bool isPieceAt(const BoardPos_t & pos) override;
        IPieceOpt_t pieceAt(const BoardPos_t & pos) override;
        Piece::Enum pieceEnumAt(const BoardPos_t & pos) override;

        IPieceUPtr_t makePiece(Context &, const Piece::Enum, const BoardPos_t & pos) override = 0;
        void placePiece(Context &, IPieceUPtr_t) override;
        void placePieceAtRandomPos(Context &, const Piece::Enum) override;
        void placePiece(Context &, const Piece::Enum, const BoardPos_t & pos) override;
        void removePiece(Context &, const BoardPos_t & pos) override;

        IPieceUList_t::iterator findIterToPiece(const BoardPos_t & posToFind);

      protected:
        CellLayout m_cells;
        IPieceUList_t m_pieces;
    };

    //

    struct SnakeBoard : public BoardBase
    {
        virtual ~SnakeBoard() = default;

        void reset(Context &) override;
        void setupCellSizesAndCounts(Context &);
        void populateBoardWithMapPieces(Context & context);

        IPieceUPtr_t
            makePiece(Context & context, const Piece::Enum piece, const BoardPos_t & pos) override;
    };

    /*
     // use when cell size/counts are known but map layout is not, must override populateCustom()
        BoardBase();

        // use when map layout is known but cell size/counts are not
        BoardBase(const MapOfRowStrings_t & mapStrings);

         void BoardBase::reset(Context & context)
    {
        const sf::Vector2f windowSize{ context.settings.windowSize() };

        if (m_cellSizeRatio > 0.0f)
        {
            //  cell size/counts are known but map layout is not
            const float windowSizeAvg{ (windowSize.x + windowSize.y) / 2.0f };
            const float cellSideLength{ windowSizeAvg * m_cellSizeRatio };
            setupCells(windowSize, cellSideLength);
            populateCustom(context);
        }
        else
        {
            // map layout is known but cell size/counts are not
            const sf::Vector2s countsOrig(m_mapStrings.front().size(), m_mapStrings.size());
            const sf::Vector2f cellSizeOrig{ windowSize / sf::Vector2f(countsOrig) };
            const float cellSideLength{ std::min(cellSizeOrig.x, cellSizeOrig.y) };
            setupCells(windowSize, cellSideLength);
            populateFromMapStrings(context);
        }
    }

    struct MapFactory
    {
        static IPieceUList_t makeK
    };

    void BoardBase::populateFromMapStrings(Context & context)
    {
        m_pieces.clear();

        M_ASSERT_OR_THROW(!m_mapStrings.empty());
        M_ASSERT_OR_THROW(!m_mapStrings.front().empty());
        M_ASSERT_OR_THROW((m_mapStrings.size() * m_mapStrings.front().size()) == cellCountTotal());

        const MapCharToPieceMap_t charToPieceMap{ makeMapCharToPieceEnumMap() };

        for (std::size_t vert(0); vert < m_mapStrings.size(); ++vert)
        {
            const std::string & row{ m_mapStrings.at(vert) };
            M_ASSERT_OR_THROW(!row.empty());

            for (std::size_t horiz(0); horiz < row.length(); ++horiz)
            {
                const char mapChar{ row.at(horiz) };
                if (mapChar == ' ')
                {
                    continue;
                }

                const auto foundIter{ charToPieceMap.find(mapChar) };
                if (foundIter == std::end(charToPieceMap))
                {
                    return;
                }

                const Piece::Enum piece{ foundIter->second };
                if (Piece::NotInPlay == piece)
                {
                    continue;
                }

                const BoardPos_t pos{ sf::Vector2s(horiz, vert) };
                placePiece(context, piece, pos);
            }
        }

        M_ASSERT_OR_THROW((m_mapStrings.size() * m_mapStrings.front().size()) == cellCountTotal());
    }

    MapCharToPieceMap_t BoardBase::makeMapCharToPieceEnumMap() const
    {
        MapCharToPieceMap_t charToPieceMap;

        for (int i(0); i < Piece::Count; ++i)
        {
            const Piece::Enum piece{ static_cast<Piece::Enum>(i) };
            const std::string name{ Piece::name(piece) };
            if (!name.empty())
            {
                charToPieceMap[name.front()] = piece;
            }
        }

        M_ASSERT_OR_THROW(!charToPieceMap.empty());

        return charToPieceMap;
    }

    */
} // namespace boardgame

#endif // BOARDGAME_BOARD_HPP_INCLUDED