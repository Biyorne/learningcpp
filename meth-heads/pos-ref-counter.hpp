#ifndef METHHEADS_POS_REF_COUNTER_HPP_INCLUDED
#define METHHEADS_POS_REF_COUNTER_HPP_INCLUDED
//
// pos-ref-counter.hpp
//
#include "utils.hpp"

#include <vector>

namespace methhead
{
    struct SimContext;

    //

    class ScopedBoardPosition
    {
      protected:
        ScopedBoardPosition(const SimContext & context);
        virtual ~ScopedBoardPosition() { free(); }

        inline BoardPos_t get() const noexcept { return m_boardPos; }

        void set(const SimContext & context, const BoardPos_t & newPos) noexcept;

      public:
        static void reset(const SimContext & context);

      private:
        // assumes there is at least one free space
        static std::size_t findRandomFreePosIndex(const SimContext & context) noexcept;

        inline void free() const noexcept { --m_refCounts[m_index]; }
        inline void occupy() const noexcept { ++m_refCounts[m_index]; }

      private:
        std::size_t m_index;
        BoardPos_t m_boardPos;
        static inline std::vector<int> m_refCounts;
    };
} // namespace methhead

#endif // METHHEADS_POS_REF_COUNTER_HPP_INCLUDED