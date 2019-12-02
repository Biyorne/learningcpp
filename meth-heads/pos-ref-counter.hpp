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

    class ScopedBoardPositionRefCount
    {
      protected:
        ScopedBoardPositionRefCount(const SimContext & context);
        virtual ~ScopedBoardPositionRefCount() { free(); }

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

    //

    class ScopedBoardPositionBruteForce
    {
      protected:
        ScopedBoardPositionBruteForce(const SimContext & context)
            : m_boardPos(findRandomFreePos(context))
        {}

        virtual ~ScopedBoardPositionBruteForce() = default;

        inline BoardPos_t get() const noexcept { return m_boardPos; }

        void set(const SimContext &, const BoardPos_t & newPos) noexcept { m_boardPos = newPos; }

      public:
        static void reset(const SimContext &) {}

      private:
        // assumes there is at least one free space
        static BoardPos_t findRandomFreePos(const SimContext & context) noexcept;

      private:
        BoardPos_t m_boardPos;
        static inline std::vector<BoardPos_t> s_allPositions;
        static inline std::vector<BoardPos_t> s_freePositions;
    };

    using BoardPositionHandler_t = ScopedBoardPositionRefCount;

    //
} // namespace methhead

#endif // METHHEADS_POS_REF_COUNTER_HPP_INCLUDED