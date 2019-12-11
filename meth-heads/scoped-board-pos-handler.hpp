#ifndef METHHEADS_SCOPED_BOARD_POS_HANDLER_HPP_INCLUDED
#define METHHEADS_SCOPED_BOARD_POS_HANDLER_HPP_INCLUDED
//
// scoped-board-pos-handler.hpp
//
#include "display-constants.hpp"
#include "sim-context.hpp"
#include "utils.hpp"

#include <vector>

namespace methhead
{
    class ScopedBoardPosHandler
    {
      protected:
        ScopedBoardPosHandler(const SimContext & context);
        virtual ~ScopedBoardPosHandler() { free(); }

        inline BoardPos_t getPos() const noexcept { return m_boardPos; }

        void setPos(const SimContext & context, const BoardPos_t & newPos) noexcept;

      public:
        int refCount() const noexcept { return s_refCounts[m_index]; }

        inline static int refCount(const SimContext & context, const BoardPos_t & pos)
        {
            return s_refCounts[context.display.boardPosToIndex(pos)];
        }

        inline static bool isPosFree(const SimContext & context, const BoardPos_t & pos)
        {
            return (refCount(context, pos) == 0);
        }

        inline static bool isAnyPosFree(const SimContext & context) noexcept
        {
            return ((context.actors.size() + context.pickups.size()) < context.display.cell_count);
        }

        static void reset(const SimContext & context);

      private:
        // assumes there is at least one free space
        static std::size_t findRandomFreePosIndex(const SimContext & context) noexcept;

        inline void free() const noexcept { --s_refCounts[m_index]; }
        inline void occupy() const noexcept { ++s_refCounts[m_index]; }

      private:
        std::size_t m_index;
        BoardPos_t m_boardPos;
        static inline std::vector<int> s_refCounts;
    };
} // namespace methhead

#endif // METHHEADS_SCOPED_BOARD_POS_HANDLER_HPP_INCLUDED