// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "board.hpp"

#include "context.hpp"
#include "pieces.hpp"
#include "random.hpp"
#include "settings.hpp"

#include <set>

namespace boardgame
{
    void CellLayout::reset(const sf::Vector2f & windowSize, const float cellSideLengthOrig)
    {
        const float cellSideLength{ /*std::floor ? TODO*/ (cellSideLengthOrig) };
        M_ASSERT_OR_THROW(!(cellSideLength < 1.0f));

        size.x = cellSideLength;
        size.y = cellSideLength;

        counts = sf::Vector2s{ windowSize / cellSideLength };
        counts_int = sf::Vector2i(counts);
        M_ASSERT_OR_THROW((counts.x > 0) && (counts.y > 0));

        count_total = (counts.x * counts.y);
        M_ASSERT_OR_THROW(count_total > 0);

        const sf::Vector2f boardSize{ sf::Vector2f(counts) * size };

        top_left_pos = ((windowSize - boardSize) / 2.0f);
        M_ASSERT_OR_THROW(!(top_left_pos.x < 0.0f));
        M_ASSERT_OR_THROW(!(top_left_pos.y < 0.0f));

        // create a set of all valid positions, this is needed all over
        all_cell_positions.clear();
        for (int vert(0); vert < counts_int.y; ++vert)
        {
            for (int horiz(0); horiz < counts_int.x; ++horiz)
            {
                all_cell_positions.insert(BoardPos_t{ horiz, vert });
            }
        }

        M_ASSERT_OR_THROW(all_cell_positions.size() == count_total);
    }

    //

    BoardBase::BoardBase()
        : m_cells()
        , m_pieces()
    {}

    void BoardBase::printStatus() const
    {
        std::cout << "\nBOARD STATUS:";
        std::cout << "\n\t cell_counts          = " << m_cells.counts;
        std::cout << "\n\t cells_total          = " << m_cells.count_total;
        std::cout << "\n\t cell_size            = " << m_cells.size;
        std::cout << "\n\t pieces_vec_size      = " << m_pieces.size();

        std::map<Piece::Enum, std::size_t> pieceCountMap;
        for (const IPieceUPtr_t & piece : m_pieces)
        {
            ++pieceCountMap[piece->which()];
        }

        for (const auto & [piece, count] : pieceCountMap)
        {
            std::cout << "\n\t\t" << Piece::name(piece) << "\t x" << count;
        }

        // print the picture of the map on the console
        // std::cout << "\n\t-" << std::endl;
        // for (int vert(0); vert < m_cells.counts_int.y; ++vert)
        //{
        //    for (int horiz(0); horiz < m_cells.counts_int.x; ++horiz)
        //    {
        //        const BoardPos_t pos(horiz, vert);
        //        const std::string name{ Piece::name(pieceEnumAt(pos)) };
        //        std::cout << ((name.empty()) ? '?' : name.front());
        //    }
        //
        //    std::cout << '\n';
        //}

        std::cout << std::endl;
    }

    IPieceOpt_t BoardBase::pieceAt(const BoardPos_t & posToFind)
    {
        const auto foundIter{ findIterToPiece(posToFind) };
        if (foundIter == std::end(m_pieces))
        {
            return std::nullopt;
        }
        else
        {
            return { **foundIter };
        }
    }

    Piece::Enum BoardBase::pieceEnumAt(const BoardPos_t & posToFind)
    {
        const auto foundIter{ findIterToPiece(posToFind) };
        if (foundIter == std::end(m_pieces))
        {
            return Piece::Count;
        }
        else
        {
            return foundIter->get()->which();
        }
    }

    bool BoardBase::isPieceAt(const BoardPos_t & posToFind)
    {
        return (pieceEnumAt(posToFind) != Piece::Count);
    }

    BoardPosOpt_t BoardBase::findRandomEmptyPos(const Context & context) const
    {
        // start with a copy of all valid/on-board positions
        std::set<BoardPos_t> positions{ m_cells.allCellPositions() };

        // remove any that are alraedy occupied
        for (const IPieceUPtr_t & piece : m_pieces)
        {
            positions.erase(piece->position());
        }

        if (positions.empty())
        {
            return std::nullopt;
        }

        return context.random.from(positions);
    }

    void BoardBase::placePieceAtRandomPos(Context & context, const Piece::Enum which)
    {
        const auto posOpt{ findRandomEmptyPos(context) };
        if (posOpt)
        {
            placePiece(context, which, posOpt.value());
        }
    }

    void BoardBase::placePiece(Context & context, const Piece::Enum piece, const BoardPos_t & pos)
    {
        placePiece(context, makePiece(context, piece, pos));
    }

    void BoardBase::placePiece(Context & context, IPieceUPtr_t iPieceUPtr)
    {
        M_ASSERT_OR_THROW(iPieceUPtr);

        const BoardPos_t pos{ iPieceUPtr->position() };
        const Piece::Enum newEnum{ iPieceUPtr->which() };

        removePiece(context, pos);

        m_pieces.emplace_back(std::move(iPieceUPtr))->setup(context);

        M_ASSERT_OR_THROW(pieceEnumAt(pos) == newEnum);
    }

    void BoardBase::removePiece(Context & context, const BoardPos_t & posToFind)
    {
        const auto foundIter{ findIterToPiece(posToFind) };
        if (foundIter == std::end(m_pieces))
        {
            return;
        }

        foundIter->get()->teardown(context);
        m_pieces.erase(foundIter);

        M_ASSERT_OR_THROW(!isPieceAt(posToFind));
    }

    IPieceUList_t::iterator BoardBase::findIterToPiece(const BoardPos_t & posToFind)
    {
        return std::find_if(
            std::begin(m_pieces), std::end(m_pieces), [&](const IPieceUPtr_t & piece) {
                return (piece->position() == posToFind);
            });
    }

    //

    void SnakeBoard::reset(Context & context)
    {
        m_pieces.clear();
        TailPiece::reset();
        setupCellSizesAndCounts(context);
        populateBoardWithMapPieces(context);
    }

    void SnakeBoard::setupCellSizesAndCounts(Context & context)
    {
        const sf::Vector2f windowSize{ context.settings.windowSize() };
        const float windowSizeAvg{ (windowSize.x + windowSize.y) / 2.0f };
        const float cellSideLength{ windowSizeAvg * context.settings.cell_size_window_ratio };
        m_cells.reset(windowSize, cellSideLength);
    }

    void SnakeBoard::populateBoardWithMapPieces(Context & context)
    {
        const sf::Vector2i counts{ m_cells.counts_int };

        const BoardPos_t centerPos{ counts / 2 };

        placePiece(context, Piece::Head, centerPos);
        // placePiece(context, Piece::Tail, (centerPos + sf::Vector2i(0, 1)));
        placePiece(context, Piece::Food, (centerPos + sf::Vector2i(0, -4)));

        for (int horiz(0); horiz < counts.x; ++horiz)
        {
            placePiece(context, Piece::Wall, BoardPos_t(horiz, 0));
            placePiece(context, Piece::Wall, BoardPos_t(horiz, (counts.y - 1)));
        }
        for (int vert(0); vert < counts.y; ++vert)
        {
            placePiece(context, Piece::Wall, BoardPos_t(0, vert));
            placePiece(context, Piece::Wall, BoardPos_t((counts.x - 1), vert));
        }

        for (int i(0); i < 20; ++i)
        {
            placePieceAtRandomPos(context, Piece::Food);
        }

        for (int i(0); i < 20; ++i)
        {
            placePieceAtRandomPos(context, Piece::Wall);
        }
    }

    IPieceUPtr_t
        SnakeBoard::makePiece(Context & context, const Piece::Enum piece, const BoardPos_t & pos)
    {
        M_ASSERT_OR_THROW((piece >= 0) && (piece < Piece::Count));

        switch (piece)
        {
            case Piece::Food: return std::make_unique<FoodPiece>(context, pos);
            case Piece::Head: return std::make_unique<HeadPiece>(context, pos);
            case Piece::Tail: return std::make_unique<TailPiece>(context, pos);

            case Piece::Wall:
            case Piece::Count:
            default: return std::make_unique<WallPiece>(context, pos);
        }
    }
} // namespace boardgame