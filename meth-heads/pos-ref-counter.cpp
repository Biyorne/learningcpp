// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// pos-ref-counter.cpp
//
#include "pos-ref-counter.hpp"

#include "display-constants.hpp"
#include "random.hpp"
#include "sim-context.hpp"

#include <cassert>

namespace methhead
{
    ScopedBoardPositionRefCount::ScopedBoardPositionRefCount(const SimContext & context)
        : m_index(findRandomFreePosIndex(context))
        , m_boardPos(context.display.indexToBoardPos(m_index))
    {
        occupy();
    }

    void ScopedBoardPositionRefCount::set(
        const SimContext & context, const BoardPos_t & newPos) noexcept
    {
        free();
        m_boardPos = newPos;
        m_index = context.display.boardPosToIndex(newPos);
        occupy();
    }

    void ScopedBoardPositionRefCount::reset(const SimContext & context)
    {
        s_refCounts.clear();
        s_refCounts.resize(context.display.cell_count, 0);
    }

    std::size_t
        ScopedBoardPositionRefCount::findRandomFreePosIndex(const SimContext & context) noexcept
    {
        if (s_refCounts.empty())
        {
            reset(context);
        }

        const std::size_t count{ s_refCounts.size() };
        const std::size_t startIndex{ context.random.zeroTo(count - 1) };
        std::size_t currentIndex{ startIndex };

        do
        {
            if (currentIndex >= count)
            {
                currentIndex = 0;
            }

            if (0 == s_refCounts[currentIndex])
            {
                break;
            }
        } while (++currentIndex != startIndex);

        assert(s_refCounts[currentIndex] == 0);

        return currentIndex;
    }

    //

    BoardPos_t ScopedBoardPositionBruteForce::findRandomFreePos(const SimContext & context) noexcept
    {
        if (s_allPositions.empty())
        {
            s_allPositions.reserve(context.display.cell_count);

            for (int vert(0); vert < context.display.cell_countsI.y; ++vert)
            {
                for (int horiz(0); horiz < context.display.cell_countsI.x; ++horiz)
                {
                    s_allPositions.push_back(BoardPos_t(horiz, vert));
                }
            }
        }

        s_freePositions = s_allPositions;

        s_freePositions.erase(
            std::remove_if(
                std::begin(s_freePositions),
                std::end(s_freePositions),
                [&](const BoardPos_t & pos) { return context.isAnythingAt(pos); }),
            std::end(s_freePositions));

        return context.random.from(s_freePositions);
    }
} // namespace methhead