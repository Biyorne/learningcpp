// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// scoped-board-position.cpp
//
#include "scoped-board-position.hpp"

#include "random.hpp"

#include <cassert>
#include <sstream>

namespace boardgame
{
    ScopedBoardPosition::ScopedBoardPosition(const Context & context, const Piece piece)
        : m_index(findRandomFreePosIndex(context))
        , m_boardPos(context.display.indexToBoardPos(m_index))
    {
        occupy(piece);
    }

    ScopedBoardPosition::ScopedBoardPosition(
        const Context & context, const Piece piece, const BoardPos_t & pos)
        : m_index(context.display.boardPosToIndex(pos))
        , m_boardPos(pos)
    {
        if (m_pieceAtIndexes[m_index] != Piece::None)
        {
            std::ostringstream ss;

            ss << "ScopedBoardPosition::ScopedBoardPosition(context, BoardPos_t=" << pos
               << ", index=" << m_index
               << ") tried to put something at a specific boardPos but there was already somthing "
                  "there!  ref_count="
               << name(m_pieceAtIndexes[m_index]);

            throw std::runtime_error(ss.str());
        }

        occupy(piece);
    }

    void ScopedBoardPosition::set(
        const Context & context, const Piece piece, const BoardPos_t & newPos) noexcept
    {
        if (m_boardPos == newPos)
        {
            return;
        }

        free();
        m_boardPos = newPos;
        m_index = context.display.boardPosToIndex(newPos);
        occupy(piece);
    }

    void ScopedBoardPosition::reset(const Context & context)
    {
        m_pieceAtIndexes.clear();
        m_pieceAtIndexes.resize(context.display.cell_count, Piece::None);
    }

    // TODO optimize by keeping a vector of free positions
    std::size_t ScopedBoardPosition::findRandomFreePosIndex(const Context & context) noexcept
    {
        if (m_pieceAtIndexes.empty())
        {
            reset(context);
        }

        const std::size_t size{ m_pieceAtIndexes.size() };
        const std::size_t finalIndex{ context.random.zeroTo(size - 1) };

        for (std::size_t index(finalIndex + 1); index != finalIndex; ++index)
        {
            if (index >= size)
            {
                index = 0;
            }

            if (Piece::None == m_pieceAtIndexes[index])
            {
                return index;
            }
        }

        return 0;
    }
} // namespace boardgame