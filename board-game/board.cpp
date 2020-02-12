// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "board.hpp"

#include "context.hpp"
#include "pieces.hpp"
#include "random.hpp"

namespace boardgame
{
    // MapBase::MapBase(const PositionPieceMap_t & map)
    //    : m_cellSize()
    //    , m_cellCounts(0, 0)
    //    , m_mapPieces(map)
    //{
    //    M_ASSERT_OR_THROW(!map.empty());
    //
    //    for (const auto & [boardPos, mapChar] : map)
    //    {
    //        M_ASSERT_OR_THROW((boardPos.x >= 0) && (boardPos.y >= 0));
    //
    //        if (m_cellCounts.x < boardPos.x)
    //        {
    //            m_cellCounts.x = boardPos.x;
    //        }
    //
    //        if (m_cellCounts.y < boardPos.y)
    //        {
    //            m_cellCounts.y = boardPos.y;
    //        }
    //    }
    //
    //    M_ASSERT_OR_THROW((m_cellCounts.x > 0) && (m_cellCounts.y > 0));
    //}
    //
    // MapBase::MapBase(const MapRowStrings_t & map)
    //    : MapBase(rowStringsToPositionPieceMap(map))
    //{}

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

    void BoardBase::reset() { m_pieces.clear(); }

    sf::FloatRect BoardBase::cellBounds(const BoardPos_t & boardPos) const
    {
        const sf::Vector2f cellPos{ m_topLeftCellPos + (sf::Vector2f(boardPos) * m_cellSize) };
        return { cellPos, m_cellSize };
    }

    IPieceOpt_t BoardBase::pieceAt(const BoardPos_t & pos) const
    {
        const auto foundIter = std::find_if(
            std::begin(m_pieces), std::end(m_pieces), [pos](const IPieceUPtr_t & piece) {
                return (piece->isAlive() && (piece->boardPos() == pos));
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

    void BoardBase::setupCellSizeAndCounts(
        const sf::Vector2f & windowSize, const float cellSizeAswindowRatio)
    {
        const float windowSizeAvg{ (windowSize.x + windowSize.y) / 2.0f };

        const float cellSideLength{ std::floor(windowSizeAvg * cellSizeAswindowRatio) };

        m_cellSize.x = cellSideLength;
        m_cellSize.y = cellSideLength;

        m_cellCounts = sf::Vector2s{ windowSize / cellSideLength };

        const sf::Vector2f boardSize{ sf::Vector2f(m_cellCounts) * m_cellSize };

        m_topLeftCellPos = ((windowSize - boardSize) / 2.0f);
    }

    //

    void SnakeBoard::setupMap(const Context & context)
    {
        setupCellSizeAndCounts(context.window_size, 0.02f);

        for (std::size_t horiz(0); horiz < m_cellCounts.x; ++horiz)
        {
            const BoardPos_t top{ sf::Vector2s(horiz, 0_st) };
            m_pieces.push_back(makePiece(context, Piece::Wall, top));

            const BoardPos_t bottom{ sf::Vector2s(horiz, (m_cellCounts.y - 1_st)) };
            m_pieces.push_back(makePiece(context, Piece::Wall, bottom));
        }

        for (std::size_t vert(0); vert < m_cellCounts.y; ++vert)
        {
            const BoardPos_t left{ sf::Vector2s(0_st, vert) };
            m_pieces.push_back(makePiece(context, Piece::Wall, left));

            const BoardPos_t right{ sf::Vector2s((m_cellCounts.x - 1_st), vert) };
            m_pieces.push_back(makePiece(context, Piece::Wall, right));
        }

        const BoardPos_t centerPos{ m_cellCounts / 2_st };
        m_pieces.push_back(makePiece(context, Piece::Food, centerPos));

        const BoardPos_t lowerThanCenterPos{ centerPos.x, (centerPos.y + 4) };
        m_pieces.push_back(makePiece(context, Piece::SnakeHead, lowerThanCenterPos));
    }

    IPieceUPtr_t SnakeBoard::makePiece(
        const Context & context, const Piece::Enum piece, const BoardPos_t & boardPos) const
    {
        M_ASSERT_OR_THROW(Piece::Count != piece);

        switch (piece)
        {
            case Piece::Wall: return std::make_unique<WallPiece>(context, boardPos);
            case Piece::Food: return std::make_unique<FoodPiece>(context, boardPos);
            case Piece::SnakeHead: return std::make_unique<HeadPiece>(context, boardPos);
            case Piece::SnakeBody: return std::make_unique<BodyPiece>(context, boardPos);

            case Piece::Count:
            default:
                throw std::runtime_error(
                    "SnakeBoard::makePiece(piece_enum=" + std::to_string(piece) +
                    ") but that enum value is out of bounds.");
        }
    }

    // BoardPosOpt_t BoardBase::findRandomEmptyPos(const Context & context) const
    //{
    //    // start with all valid/on-board positions
    //    BoardPosVec_t positions = context.map.allValidPositions();
    //
    //    // remove any that are alraedy occupied
    //    for (const IPieceUPtr_t & pieceUPtr : m_pieces)
    //    {
    //        if (!pieceUPtr->isAlive())
    //        {
    //            continue;
    //        }
    //
    //        const BoardPos_t occupiedPos = pieceUPtr->boardPos();
    //
    //        positions.erase(
    //            std::remove_if(
    //                std::begin(positions),
    //                std::end(positions),
    //                [&](const BoardPos_t & pos) { return (pos == occupiedPos); }),
    //            std::end(positions));
    //    }
    //
    //    if (positions.empty())
    //    {
    //        return std::nullopt;
    //    }
    //
    //    return context.random.from(positions);
    //}
} // namespace boardgame