// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "board.hpp"

#include "context.hpp"
#include "pieces.hpp"
#include "random.hpp"
#include "settings.hpp"

#include <map>
#include <set>

namespace boardgame
{
    void BoardBase::printStatus() const
    {
        // gather all the details
        std::size_t aliveCount{ 0 };
        std::size_t deadCount{ 0 };
        std::map<Piece::Enum, std::pair<std::size_t, std::size_t>> countMap;

        for (const IPieceUPtr_t & piece : m_pieces)
        {
            if (piece->isInPlay())
            {
                ++aliveCount;
                ++countMap[piece->piece()].first;
            }
            else
            {
                ++deadCount;
                ++countMap[piece->piece()].second;
            }
        }

        // print all the details
        std::cout << "\nBOARD STATUS:";
        std::cout << "\n\tmap_size/cell_counts=" << m_cellCounts;
        std::cout << "  (" << (m_cellCounts.x * m_cellCounts.y) << " total)";
        std::cout << "\n\tin_play    =" << aliveCount;
        std::cout << "\n\tout_of_play=" << deadCount;
        std::cout << "\n\tpieces_vec_size=" << m_pieces.size();
        std::cout << "\n\tpieces_vec_capacity=" << m_pieces.capacity();

        for (const auto & [which, pair] : countMap)
        {
            std::cout << "\n\t\t" << int(which) << "\talive=" << pair.first
                      << ", dead=" << pair.second;
        }

        std::cout << "\n\t-" << std::endl;

        // print the actual state of the map
        const sf::Vector2i cellCountsI{ cellCountsAs<int>() };
        for (int vert(0); vert < cellCountsI.y; ++vert)
        {
            for (int horiz(0); horiz < cellCountsI.x; ++horiz)
            {
                const auto which = pieceEnumAt(BoardPos_t(horiz, vert));

                // clang-format off
                switch (which)
                {
                    case Piece::Wall: { std::cout << 'W'; break; }
                    case Piece::Food: { std::cout << 'F'; break; }
                    case Piece::Head: { std::cout << 'H'; break; }
                    case Piece::Tail: { std::cout << 'T'; break; }
                    case Piece::Count: { std::cout << ' '; break; }
                    default: { std::cout << '?'; break; }
                }
                // clang-format on
            }

            std::cout << '\n';
        }

        std::cout << std::endl << std::endl;
    }

    sf::FloatRect BoardBase::cellBounds(const BoardPos_t & boardPos) const
    {
        const sf::Vector2f cellPos{ m_topLeftCellPos + (sf::Vector2f(boardPos) * m_cellSize) };
        return { cellPos, m_cellSize };
    }

    IPieceOpt_t BoardBase::pieceAt(const BoardPos_t & pos) const
    {
        const auto foundIter = std::find_if(
            std::begin(m_pieces), std::end(m_pieces), [pos](const IPieceUPtr_t & piece) {
                return (piece->isInPlay() && (piece->boardPos() == pos));
            });

        if (foundIter == std::end(m_pieces))
        {
            return std::nullopt;
        }

        return { *foundIter->get() };
    }

    Piece::Enum BoardBase::pieceEnumAt(const BoardPos_t & pos) const
    {
        const auto opt = pieceAt(pos);

        if (!opt)
        {
            return Piece::Count;
        }

        return opt.value().get().piece();
    }

    bool BoardBase::isAnyPieceAt(const BoardPos_t & pos) const { return pieceAt(pos).has_value(); }

    void BoardBase::setupCellSizeAndCounts(Context & context)
    {
        const sf::Vector2f windowSize{ context.settings.windowSize() };
        const float windowSizeAvg{ (windowSize.x + windowSize.y) / 2.0f };

        const float cellSideLength{ std::floor(
            windowSizeAvg * context.settings.cell_size_window_ratio) };

        m_cellSize.x = cellSideLength;
        m_cellSize.y = cellSideLength;

        m_cellCounts = sf::Vector2s{ windowSize / cellSideLength };

        const sf::Vector2f boardSize{ sf::Vector2f(m_cellCounts) * m_cellSize };

        m_topLeftCellPos = ((windowSize - boardSize) / 2.0f);
    }

    BoardPosOpt_t BoardBase::findRandomEmptyPos(const Context & context) const
    {
        const sf::Vector2i cellCountsI{ cellCountsAs<int>() };

        // start with all valid/on-board positions
        std::set<BoardPos_t> positions;
        for (int vert(0); vert < cellCountsI.y; ++vert)
        {
            for (int horiz(0); horiz < cellCountsI.x; ++horiz)
            {
                positions.insert(BoardPos_t{ horiz, vert });
            }
        }

        // remove any that are alraedy occupied
        for (const IPieceUPtr_t & pieceUPtr : m_pieces)
        {
            if (!pieceUPtr->isInPlay())
            {
                continue;
            }

            positions.erase(pieceUPtr->boardPos());
        }

        if (positions.empty())
        {
            return std::nullopt;
        }

        return context.random.from(positions);
    }

    void BoardBase::removePiecesThatAreNoLongerInPlay()
    {
        m_pieces.erase(
            std::remove_if(
                std::begin(m_pieces),
                std::end(m_pieces),
                [](const IPieceUPtr_t & piece) { return !piece->isInPlay(); }),
            std::end(m_pieces));
    }

    //

    void SnakeBoard::reset(Context & context)
    {
        m_pieces.clear();

        setupCellSizeAndCounts(context);

        m_pieces.reserve(m_cellCounts.x * m_cellCounts.y * 2);

        const sf::Vector2i cellCountsI{ cellCountsAs<int>() };

        const BoardPos_t centerPos{ cellCountsI / 2 };
        const BoardPos_t lowerThanCenterPos{ centerPos.x, (centerPos.y + 4) };

        placePiece(context, Piece::Head, lowerThanCenterPos);
        placePiece(context, Piece::Food, centerPos);

        for (int horiz(0); horiz < cellCountsI.x; ++horiz)
        {
            placePiece(context, Piece::Wall, BoardPos_t(horiz, 0));
            placePiece(context, Piece::Wall, BoardPos_t(horiz, (cellCountsI.y - 1)));
        }

        for (int vert(0); vert < cellCountsI.y; ++vert)
        {
            placePiece(context, Piece::Wall, BoardPos_t(0, vert));
            placePiece(context, Piece::Wall, BoardPos_t((cellCountsI.x - 1), vert));
        }
    }

    IPieceUPtr_t SnakeBoard::makePiece(
        const Context & context, const Piece::Enum piece, const BoardPos_t & boardPos)
    {
        M_ASSERT_OR_THROW((piece >= 0) && (piece < Piece::Count));

        switch (piece)
        {
            case Piece::Food: return std::make_unique<FoodPiece>(context, boardPos);
            case Piece::Head: return std::make_unique<HeadPiece>(context, boardPos);
            case Piece::Tail: return std::make_unique<TailPiece>(context, boardPos);

            case Piece::Wall:
            case Piece::Count:
            default: return std::make_unique<WallPiece>(context, boardPos);
        }
    }
} // namespace boardgame

//

// PositionPieceMap_t MapBase::rowStringsToPositionPieceMap(const MapRowStrings_t & rowStrings)
//{
//    M_ASSERT_OR_THROW(!rowStrings.empty());
//
//    PositionPieceMap_t posToPieceMap;
//
//    for (std::size_t vert(0); vert < rowStrings.size(); ++vert)
//    {
//        const std::string & row = rowStrings.at(vert);
//        M_ASSERT_OR_THROW(!row.empty());
//
//        for (std::size_t horiz(0); horiz < row.length(); ++horiz)
//        {
//            const char mapChar{ row.at(horiz) };
//            const Piece::Enum piece{ Piece::mapCharTo(mapChar) };
//
//            if (piece == Piece::Count)
//            {
//                continue;
//            }
//
//            const BoardPos_t boardPos{ sf::Vector2s(horiz, vert) };
//
//            posToPieceMap[boardPos] = piece;
//        }
//    }
//
//    return posToPieceMap;
//}

// char MapBase::charAt(const BoardPos_t & boardPos, const char returnOnError) const
//{
//    if ((boardPos.x < 0) || (boardPos.x >= m_cellCounts.x) || (boardPos.y < 0) ||
//        (boardPos.y >= m_cellCounts.y))
//    {
//        return returnOnError;
//    }
//
//    const auto foundIter{ m_mapPieces.find(boardPos) };
//    if (foundIter == std::end(m_mapPieces))
//    {
//        return returnOnError;
//    }
//    else
//    {
//        return foundIter->second;
//    }
//}