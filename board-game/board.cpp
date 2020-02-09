// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "board.hpp"

#include "context.hpp"
#include "pieces.hpp"
#include "random.hpp"

#include <set>

namespace boardgame
{
    int walkDistance(const BoardPos_t & from, const BoardPos_t & to)
    {
        const float dist = util::distance(sf::Vector2f(from), sf::Vector2f(to));
        return static_cast<int>(dist * 10'000.0f);
    }

    int walkDistance(const IPiece & from, const IPiece & to)
    {
        return walkDistance(from.boardPos(), to.boardPos());
    }

    MapBase::MapBase(const MapLayout_t & rowStrings)
        : m_layout(rowStrings)
        , m_cellCounts(0, 0)
        , m_allValidPositions()
    {
        if (rowStrings.empty() || rowStrings.front().empty())
        {
            const std::string errorMessage("Error:  The std::vector<std::string> map is invalid.");

            std::cout << errorMessage << std::endl;

            for (const std::string & row : m_layout)
            {
                std::cout << "\"" << row << "\"" << std::endl;
            }

            throw std::runtime_error(errorMessage);
        }

        m_cellCounts.x = rowStrings.front().size();
        m_cellCounts.y = rowStrings.size();

        for (std::size_t vert(0); vert < rowStrings.size(); ++vert)
        {
            const std::string & row = rowStrings.at(vert);

            M_ASSERT_OR_THROW(row.length() == m_cellCounts.x);

            for (std::size_t horiz(0); horiz < row.length(); ++horiz)
            {
                const BoardPos_t boardPos{ sf::Vector2s(horiz, vert) };
                m_allValidPositions.push_back(boardPos);
            }
        }

        M_ASSERT_OR_THROW((m_cellCounts.x * m_cellCounts.y) == m_allValidPositions.size());
    }

    //

    bool MapBase::isPosValid(const BoardPos_t & boardPos) const
    {
        if ((boardPos.x < 0) || (boardPos.y < 0))
        {
            return false;
        }

        const sf::Vector2s indexes{ boardPos };

        return ((indexes.y < m_layout.size()) && (indexes.x < m_layout.at(indexes.y).length()));
    }

    char MapBase::charAt(const BoardPos_t & boardPos) const
    {
        if (!isPosValid(boardPos))
        {
            return '\0';
        }

        const sf::Vector2s indexes{ boardPos };
        return m_layout.at(indexes.y).at(indexes.x);
    }

    //

    BoardView::BoardView(const sf::Vector2u & windowSize, const sf::Vector2s & cellCounts)
        : window_size(windowSize)
        , window_rect({}, window_size)
    {
        board_region = window_rect;
        board_region.height *= board_region_vert_size_ratio;
        util::scaleRectInPlace(board_region, region_pad_ratio);

        status_region = window_rect;
        status_region.height *= (1.0f - board_region_vert_size_ratio);
        status_region.top = (window_size.y - status_region.height);
        util::scaleRectInPlace(status_region, region_pad_ratio);

        const sf::Vector2f cellRegionSize{ util::size(board_region) / sf::Vector2f(cellCounts) };

        const float cellSize{ std::min(cellRegionSize.x, cellRegionSize.y) };
        cell_size.x = cellSize;
        cell_size.y = cellSize;

        const sf::Vector2f boardRectSize{ sf::Vector2f(cellCounts) * cell_size };
        const sf::Vector2f boardRectPos{ util::center(board_region) - (boardRectSize * 0.5f) };
        board_rect = sf::FloatRect(boardRectPos, boardRectSize);
    }

    //

    BoardBase::BoardBase(const sf::Vector2u & windowSize, const IMap & map)
        : m_view(windowSize, map.cellCounts())
        , m_pieces()
    {
        reset();
    }

    sf::FloatRect BoardBase::calcCellBounds(const BoardPos_t & boardPos) const
    {
        const sf::Vector2f windowPos{ util::position(m_view.board_rect) +
                                      (sf::Vector2f(boardPos) * m_view.cell_size) };

        return { windowPos, m_view.cell_size };
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

    bool BoardBase::isAnyPieceAt(const BoardPos_t & pos) const
    {
        return (pieceEnumAt(pos) != Piece::Count);
    }

    BoardPosOpt_t BoardBase::findRandomEmptyPos(const Context & context) const
    {
        // start with all valid/on-board positions
        BoardPosVec_t positions = context.map.allValidPositions();

        // remove any that are alraedy occupied
        for (const IPieceUPtr_t & pieceUPtr : m_pieces)
        {
            if (!pieceUPtr->isAlive())
            {
                continue;
            }

            const BoardPos_t occupiedPos = pieceUPtr->boardPos();

            positions.erase(
                std::remove_if(
                    std::begin(positions),
                    std::end(positions),
                    [&](const BoardPos_t & pos) { return (pos == occupiedPos); }),
                std::end(positions));
        }

        if (positions.empty())
        {
            return std::nullopt;
        }

        return context.random.from(positions);
    }
} // namespace boardgame