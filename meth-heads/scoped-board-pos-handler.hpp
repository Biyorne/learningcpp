#ifndef METHHEADS_SCOPED_BOARD_POS_HANDLER_HPP_INCLUDED
#define METHHEADS_SCOPED_BOARD_POS_HANDLER_HPP_INCLUDED
//
// scoped-board-pos-handler.hpp
//
#include "utils.hpp"

#include <vector>

namespace methhead
{
    struct SimContext;

    //

    class ScopedBoardPosHandler
    {
      protected:
        ScopedBoardPosHandler(const SimContext & context);
        virtual ~ScopedBoardPosHandler() { free(); }

        inline BoardPos_t get() const noexcept { return m_boardPos; }

        void set(const SimContext & context, const BoardPos_t & newPos) noexcept;

      public:
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