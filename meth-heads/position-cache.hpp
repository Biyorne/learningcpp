#ifndef METHHEADS_POSITION_CACHE_HPP_INCLUDED
#define METHHEADS_POSITION_CACHE_HPP_INCLUDED
//
// position-cache.hpp
//
#include "display-constants.hpp"

#include <optional>
#include <vector>

namespace methhead
{
    class PositionCache
    {
        struct Cell
        {
            explicit Cell(const BoardPos_t & pos)
                : is_free(true)
                , position(pos)
            {}

            bool is_free;
            BoardPos_t position;
        };

      public:
        explicit PositionCache(const DisplayConstants & displayConstants, const Random & random)
            : m_display(displayConstants)
            , m_random(random)
            , m_positions()
        {
            m_positions.reserve(m_display.cell_count);
            reset();
        }

        void reset()
        {
            m_positions.clear();

            for (std::size_t i(0); i < m_display.cell_count; ++i)
            {
                const Cell cell(m_display.indexToBoardPos(i));

                assert(i == m_display.boardPosToIndex(cell.position));
                assert(cell.position == m_display.indexToBoardPos(i));

                m_positions.push_back(cell);
            }

            assert(m_positions.size() == m_display.cell_count);
        }

        std::optional<BoardPos_t> findRandomFreeAndOccupy()
        {
            const std::size_t startingIndex{ m_random.index(m_positions) };

            std::size_t currentIndex{ startingIndex };
            while (++currentIndex != startingIndex)
            {
                if (currentIndex >= m_positions.size())
                {
                    currentIndex = 0;
                }

                Cell & currentCell{ m_positions[currentIndex] };
                if (currentCell.is_free)
                {
                    currentCell.is_free = false;
                    return currentCell.position;
                }
            };

            return std::nullopt;
        }

        inline void free(const BoardPos_t & pos) { findCell(pos).is_free = true; }

        inline void occupy(const BoardPos_t & pos) { findCell(pos).is_free = false; }

        inline void move(const BoardPos_t & from, const BoardPos_t & to)
        {
            free(from);
            occupy(to);
        }

        inline bool isFree(const BoardPos_t & pos) { return findCell(pos).is_free; }

      private:
        Cell & findCell(const BoardPos_t & pos)
        {
            return m_positions[m_display.boardPosToIndex(pos)];
        }

      private:
        const DisplayConstants & m_display;
        const Random & m_random;
        std::vector<Cell> m_positions;
    };
} // namespace methhead

#endif // METHHEADS_POSITION_CACHE_HPP_INCLUDED