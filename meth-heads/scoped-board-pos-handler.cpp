// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// scoped-board-pos-handler.cpp
//
#include "scoped-board-pos-handler.hpp"

#include "display-constants.hpp"
#include "random.hpp"
#include "sim-context.hpp"

#include <cassert>

namespace methhead
{
    ScopedBoardPosHandler::ScopedBoardPosHandler(const SimContext & context)
        : m_index(findRandomFreePosIndex(context))
        , m_boardPos(context.display.indexToBoardPos(m_index))
    {
        occupy();
    }

    void ScopedBoardPosHandler::set(const SimContext & context, const BoardPos_t & newPos) noexcept
    {
        free();
        m_boardPos = newPos;
        m_index = context.display.boardPosToIndex(newPos);
        occupy();
    }

    void ScopedBoardPosHandler::reset(const SimContext & context)
    {
        s_refCounts.clear();
        s_refCounts.resize(context.display.cell_count, 0);
    }

    std::size_t ScopedBoardPosHandler::findRandomFreePosIndex(const SimContext & context) noexcept
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
} // namespace methhead