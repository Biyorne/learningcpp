#ifndef BOARDGAME_BOARD_HPP_INCLUDED
#define BOARDGAME_BOARD_HPP_INCLUDED
//
// board.hpp
//
#include "settings.hpp"
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
    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;
    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUList_t = std::list<IPieceUPtr_t>;
    using IPieceUListIter_t = IPieceUList_t::iterator;

    // describes the board as a rect on the screen divided into identicaly sized cells
    struct Layout
    {
        // void reset(Context&);
        void setup(const IGameSettings & settings);

        bool isPositionValid(const BoardPos_t & pos) const
        {
            return (
                (pos.x >= 0) && (pos.y >= 0) && (pos.x < cell_counts.x) && (pos.y < cell_counts.y));
        }

        sf::FloatRect bounds(const BoardPos_t & boardPos) const
        {
            return { (util::position(board_bounds) + (sf::Vector2f(boardPos) * cell_size)),
                     cell_size };
        }

        sf::FloatRect board_bounds{ 0.0f, 0.0f, 0.0f, 0.0f };
        sf::Vector2i cell_counts{ 0, 0 };
        sf::Vector2f cell_size{ 0.0f, 0.0f };
        std::size_t cell_count_total{ 0 };
        std::set<BoardPos_t> all_valid_positions;
    };

    // Keeps all pieces, allows find/add/move/remove simply by their enum and BoardPos_t
    // Pieces should use these add/remove/move functions instead of trying to do it themselves.
    struct IBoard : public IEntity
    {
        virtual ~IBoard() = default;

        virtual const Layout & layout() const = 0;

        sf::FloatRect bounds() const override = 0;
        // void reset(Context &) override = 0;
        void update(Context &, const float elapsedTimeSec) override = 0;
        void draw(sf::RenderTarget &, sf::RenderStates) const override = 0;
        void handleEvent(Context &, const sf::Event & event) override = 0;

        virtual bool isPieceAt(const BoardPos_t & pos) = 0;
        virtual IPieceOpt_t pieceAtOpt(const BoardPos_t & pos) = 0;
        virtual Piece pieceEnumAt(const BoardPos_t & pos) = 0;
        virtual IPieceUListIter_t pieceIterAt(const BoardPos_t & pos) = 0;
        virtual const IPieceUList_t & pieces() const = 0;

        virtual void removePiece(Context &, const BoardPos_t & pos) = 0;
        virtual void addPiece(Context &, IPieceUPtr_t iPieceUPtr) = 0;
        virtual void addPiece(Context &, const Piece, const BoardPos_t &) = 0;
        virtual void addPieceRandomFreePos(Context &, const Piece) = 0;

        virtual BoardPosOpt_t findRandomFreePos(const Context & context) const = 0;
        virtual IPieceUPtr_t makePiece(Context &, const Piece, const BoardPos_t & pos) = 0;
    };

    // A simple reference implementation of an IBoard that should suffice for most games.
    class BoardBase : public IBoard
    {
      public:
        BoardBase() = default;
        virtual ~BoardBase() = default;

        void setup(const IGameSettings & settings);

        const Layout & layout() const override { return m_layout; }

        sf::FloatRect bounds() const override { return m_layout.board_bounds; }

        // void reset(Context &) override;
        void update(Context &, const float elapsedTimeSec) override;
        void draw(sf::RenderTarget &, sf::RenderStates) const override;
        void handleEvent(Context &, const sf::Event & event) override;

        bool isPieceAt(const BoardPos_t & pos) override;
        IPieceOpt_t pieceAtOpt(const BoardPos_t & pos) override;
        Piece pieceEnumAt(const BoardPos_t & pos) override;
        IPieceUListIter_t pieceIterAt(const BoardPos_t & pos) override;
        const IPieceUList_t & pieces() const override { return m_pieces; }

        void removePiece(Context &, const BoardPos_t & pos) override;
        void addPiece(Context &, IPieceUPtr_t iPieceUPtr) override;
        void addPiece(Context &, const Piece piece, const BoardPos_t & pos) override;
        void addPieceRandomFreePos(Context &, const Piece piece) override;

        BoardPosOpt_t findRandomFreePos(const Context & context) const override;
        IPieceUPtr_t makePiece(Context &, const Piece, const BoardPos_t & pos) override = 0;

      protected:
        Layout m_layout;

        // std::list is used because:
        //   * size will almost never exceed 100
        //   * it never invalidates iterators (unless you try to erase yourself)
        //   * it never reorders or reallocates
        //
        //   All these properties above are essential because any piece, at any time, can
        //   change/move/erase/etc any other piece -in the middle of a loop through that same
        //   container of pieces.
        IPieceUList_t m_pieces;
    };

    //

    struct SnakeBoard : public BoardBase
    {
        virtual ~SnakeBoard() = default;

        void populateBoardWithMapPieces(Context & context);

        IPieceUPtr_t
            makePiece(Context & context, const Piece piece, const BoardPos_t & pos) override;
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

        M_CHECK(!m_mapStrings.empty());
        M_CHECK(!m_mapStrings.front().empty());
        M_CHECK((m_mapStrings.size() * m_mapStrings.front().size()) == cellCountTotal());

        const MapCharToPieceMap_t charToPieceMap{ makeMapCharToPieceEnumMap() };

        for (std::size_t vert(0); vert < m_mapStrings.size(); ++vert)
        {
            const std::string & row{ m_mapStrings.at(vert) };
            M_CHECK(!row.empty());

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

                const Piece piece{ foundIter->second };
                if (Piece::NotInPlay == piece)
                {
                    continue;
                }

                const BoardPos_t pos{ sf::Vector2s(horiz, vert) };
                placePiece(context, piece, pos);
            }
        }

        M_CHECK((m_mapStrings.size() * m_mapStrings.front().size()) == cellCountTotal());
    }

    MapCharToPieceMap_t BoardBase::makeMapCharToPieceEnumMap() const
    {
        MapCharToPieceMap_t charToPieceMap;

        for (int i(0); i < Piece::Count; ++i)
        {
            const Piece piece{ static_cast<Piece>(i) };
            const std::string name{ Piece::name(piece) };
            if (!name.empty())
            {
                charToPieceMap[name.front()] = piece;
            }
        }

        M_CHECK(!charToPieceMap.empty());

        return charToPieceMap;
    }

    */
} // namespace boardgame

#endif // BOARDGAME_BOARD_HPP_INCLUDED