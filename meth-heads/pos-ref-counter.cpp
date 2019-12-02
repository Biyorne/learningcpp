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
    ScopedBoardPosition::ScopedBoardPosition(const SimContext & context)
        : m_index(findRandomFreePosIndex(context))
        , m_boardPos(context.display.indexToBoardPos(m_index))
    {
        occupy();
    }

    void ScopedBoardPosition::set(const SimContext & context, const BoardPos_t & newPos) noexcept
    {
        free();
        m_boardPos = newPos;
        m_index = context.display.boardPosToIndex(newPos);
        occupy();
    }

    void ScopedBoardPosition::reset(const SimContext & context)
    {
        m_refCounts.clear();
        m_refCounts.resize(context.display.cell_count, 0);
    }

    std::size_t ScopedBoardPosition::findRandomFreePosIndex(const SimContext & context) noexcept
    {
        if (m_refCounts.empty())
        {
            reset(context);
        }

        const std::size_t count{ m_refCounts.size() };
        const std::size_t startIndex{ context.random.zeroTo(count - 1) };
        std::size_t currentIndex{ startIndex };

        do
        {
            if (currentIndex >= count)
            {
                currentIndex = 0;
            }

            if (0 == m_refCounts[currentIndex])
            {
                break;
            }
        } while (++currentIndex != startIndex);

        assert(m_refCounts[currentIndex] == 0);

        return currentIndex;
    }
} // namespace methhead