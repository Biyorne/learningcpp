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

    void ScopedBoardPosHandler::setPos(
        const SimContext & context, const BoardPos_t & newPos) noexcept
    {
        if (m_boardPos == newPos)
        {
            return;
        }

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

        const std::size_t size{ s_refCounts.size() };
        const std::size_t finalIndex{ context.random.zeroTo(size - 1) };

        for (std::size_t index(finalIndex + 1); index != finalIndex; ++index)
        {
            if (index >= size)
            {
                index = 0;
            }

            if (0 == s_refCounts[index])
            {
                return index;
            }
        }

        return 0;
    }
} // namespace methhead