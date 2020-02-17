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
    }

    //

    BoardBase::BoardBase()
        : m_pieces()
        , m_cells()
    {}

    void BoardBase::printStatus() const
    {
        std::cout << "\nBOARD STATUS:";
        std::cout << "\n\t cell_counts          = " << m_cells.counts;
        std::cout << "\n\t cells_total          = " << m_cells.count_total;
        std::cout << "\n\t cell_size            = " << m_cells.size;
        std::cout << "\n\t pieces_vec_size      = " << m_pieces.size();
        std::cout << "\n\t pieces_vec_capacity  = " << m_pieces.capacity();

        // std::cout << "\n\t pieces_not_in_play   = "
        //          << (m_pieces.size() -
        //              std::distance(std::cbegin(m_piecesInPlay), std::cend(m_piecesInPlay)));

        std::map<Piece::Enum, std::pair<std::size_t, std::size_t>> pieceCountMap;

        for (const IPieceUPtr_t & piece : m_pieces)
        {
            if (piece->isInPlay())
            {
                ++pieceCountMap[piece->which()].first;
            }
            else
            {
                ++pieceCountMap[piece->which()].second;
            }
        }

        for (const auto & [piece, pair] : pieceCountMap)
        {
            std::cout << "\n\t\t" << Piece::name(piece) << "\t " << pair.first << '/'
                      << pair.second;
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

    IPieceOpt_t BoardBase::pieceAt(const BoardPos_t & posToFind) const
    {
        const auto foundIter{ std::find_if(
            std::begin(m_pieces), std::end(m_pieces), [&](const IPieceUPtr_t & piece) {
                return (piece->isInPlay() && (piece->position() == posToFind));
            }) };

        if (foundIter == std::end(m_pieces))
        {
            return std::nullopt;
        }

        return { **foundIter };
    }

    Piece::Enum BoardBase::pieceEnumAt(const BoardPos_t & pos) const
    {
        const IPieceOpt_t pieceUPtrOpt{ pieceAt(pos) };

        if (pieceUPtrOpt)
        {
            return pieceUPtrOpt->get().which();
        }
        else
        {
            return Piece::NotInPlay;
        }
    }

    bool BoardBase::isPieceAt(const BoardPos_t & pos) const
    {
        return (pieceEnumAt(pos) != Piece::NotInPlay);
    }

    BoardPosOpt_t BoardBase::findRandomEmptyPos(const Context & context) const
    {
        // start with a copy of all valid/on-board positions
        std::set<BoardPos_t> positions;
        for (int vert(0); vert < m_cells.counts_int.y; ++vert)
        {
            for (int horiz(0); horiz < m_cells.counts_int.x; ++horiz)
            {
                positions.insert(BoardPos_t{ horiz, vert });
            }
        }

        M_ASSERT_OR_THROW(positions.size() == m_cells.count_total);

        // remove any that are alraedy occupied
        for (const IPieceUPtr_t & piece : m_pieces)
        {
            if (piece->isInPlay())
            {
                positions.erase(piece->position());
            }
        }

        if (positions.empty())
        {
            return std::nullopt;
        }

        return context.random.from(positions);
    }

    void BoardBase::addPiece(Context & context, const Piece::Enum piece, const BoardPos_t & pos)
    {
        addPiece(context, makePiece(context, piece, pos));
    }

    void BoardBase::addPiece(Context &, IPieceUPtr_t iPieceUPtr)
    {
        M_ASSERT_OR_THROW(iPieceUPtr);
        M_ASSERT_OR_THROW(iPieceUPtr->isInPlay());

        const BoardPos_t pos{ iPieceUPtr->position() };
        const Piece::Enum newEnum{ iPieceUPtr->which() };

        removeFromPlay(pos);

        M_ASSERT_OR_THROW(m_pieces.capacity() > m_pieces.size());
        m_pieces.push_back(std::move(iPieceUPtr));
        M_ASSERT_OR_THROW(pieceEnumAt(pos) == newEnum);
    }

    void BoardBase::removeFromPlay(const BoardPos_t & pos)
    {
        IPieceOpt_t pieceUPtrOpt{ pieceAt(pos) };
        if (pieceUPtrOpt)
        {
            pieceUPtrOpt->get().removeFromPlay();
        }

        M_ASSERT_OR_THROW(!isPieceAt(pos));
    }

    void BoardBase::eraseAllPiecesNotInPlay()
    {
        m_pieces.erase(
            std::remove_if(
                std::begin(m_pieces),
                std::end(m_pieces),
                [&](const IPieceUPtr_t & uptr) { return (!uptr || !uptr->isInPlay()); }),
            std::end(m_pieces));
    }

    //

    void SnakeBoard::reset(Context & context)
    {
        TailPiece::reset();
        m_pieces.clear();
        setupCellSizesAndCounts(context);
        m_pieces.reserve(m_cells.count_total * 2);
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

        addPiece(context, Piece::Head, centerPos);
        addPiece(context, Piece::Tail, (centerPos + sf::Vector2i(0, 1)));
        addPiece(context, Piece::Food, (centerPos + sf::Vector2i(0, -4)));

        for (int horiz(0); horiz < counts.x; ++horiz)
        {
            addPiece(context, Piece::Wall, BoardPos_t(horiz, 0));
            addPiece(context, Piece::Wall, BoardPos_t(horiz, (counts.y - 1)));
        }
        for (int vert(0); vert < counts.y; ++vert)
        {
            addPiece(context, Piece::Wall, BoardPos_t(0, vert));
            addPiece(context, Piece::Wall, BoardPos_t((counts.x - 1), vert));
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
            case Piece::NotInPlay:
            default: return std::make_unique<WallPiece>(context, pos);
        }
    }
} // namespace boardgame