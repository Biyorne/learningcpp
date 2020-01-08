#ifndef BOARDGAME_BOARD_HPP_INCLUDED
#define BOARDGAME_BOARD_HPP_INCLUDED
//
// board.hpp
//
#include "types.hpp"
#include "util.hpp"

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

namespace boardgame
{
    struct IPiece;

    struct Context;

    struct DemonPiece;
    struct PlayerPiece;
    struct ChildPiece;
    struct WallPiece;

    using DemonUPtr_t = std::unique_ptr<DemonPiece>;
    using PlayerUPtr_t = std::unique_ptr<PlayerPiece>;
    using ChildUPtr_t = std::unique_ptr<ChildPiece>;
    using WallUPtr_t = std::unique_ptr<WallPiece>;

    //

    struct Cell
    {
        Cell(const std::size_t ind, const BoardPos_t & pos, const sf::FloatRect & rec)
            : index(ind)
            , board_pos(pos)
            , rect(rec)
            , center(util::center(rec))
        {}

        std::size_t index;
        BoardPos_t board_pos;
        sf::FloatRect rect;
        sf::Vector2f center;
    };
    //

    struct Board
    {
        explicit Board(const sf::Vector2u & windowSize);

        const Cell & cell(const BoardPos_t & pos) const
        {
            const sf::Vector2i cellCountsI{ cellCounts<int>() };

            if ((pos.x < 0) || (pos.x >= cellCountsI.x) || (pos.y < 0) || (pos.y >= cellCountsI.y))
            {
                std::ostringstream ss;

                ss << "Board::cell(pos=" << pos
                   << ") is out of range with cellCounts=" << cell_counts;

                throw std::runtime_error(ss.str());
            }

            return cells.at(
                (static_cast<std::size_t>(pos.y) * (cell_counts.x)) +
                static_cast<std::size_t>(pos.x));
        }

        void setupSprite(
            sf::Sprite & sprite,
            const BoardPos_t & boardPos,
            const sf::Color & color = sf::Color::White) const
        {
            sprite.setColor(color);
            util::scale(sprite, cell_size);
            util::setOriginToCenter(sprite);
            sprite.setPosition(cell(boardPos).center);
        }

        template <typename T = float>
        sf::Vector2<T> cellSize() const
        {
            return sf::Vector2<T>{ cell_size };
        }

        template <typename T = std::size_t>
        sf::Vector2<T> cellCounts() const
        {
            return sf::Vector2<T>{ cell_counts };
        }

        template <typename T = std::size_t>
        T cellCountTotal() const
        {
            return static_cast<T>(cell_counts.x + cell_counts.y);
        }

        std::size_t count(const std::initializer_list<Piece> & list) const
        {
            std::size_t count{ 0 };

            for (const Piece piece : list)
            {
                switch (piece)
                {
                    case Piece::Hero:
                    {
                        count += players.size();
                        break;
                    }

                    case Piece::Demon:
                    {
                        count += demons.size();
                        break;
                    }

                    case Piece::Child:
                    {
                        count += kids.size();
                        break;
                    }

                    case Piece::Wall:
                    {
                        count += walls.size();
                        break;
                    }

                    case Piece::Empty:
                    default: break;
                }
            }

            return count;
        }

        std::size_t countAllExceptEmpty() const
        {
            return count({ Piece::Child, Piece::Demon, Piece::Hero, Piece::Wall });
        }

        static int walkDistance(const BoardPos_t & from, const BoardPos_t & to)
        {
            return (std::abs(to.x - from.x) + std::abs(to.y - from.y));
        }

        static int walkDistance(const IPiece & from, const IPiece & to);

        void reset();

        BoardPos_t randomAvailBoardPos(const Context & context) const;

        Piece pieceAt(const BoardPos_t & pos) const;

        const sf::Vector2f window_size;
        const sf::FloatRect window_rect;
        const float inner_window_scale;
        const float board_region_vert_size_ratio;
        const float cell_size_ratio;
        const float between_region_vert_scale;

        sf::FloatRect inner_window_rect;
        sf::Vector2f cell_size;
        sf::Vector2s cell_counts;
        std::size_t cell_count_total;
        sf::FloatRect board_region;
        sf::FloatRect status_region;

        std::vector<Cell> cells;

        // std::vector<BoardPos_t> empty_board_pos;
        std::vector<DemonUPtr_t> demons;
        std::vector<PlayerUPtr_t> players;
        std::vector<ChildUPtr_t> kids;
        std::vector<WallUPtr_t> walls;
    }; // namespace boardgame
} // namespace boardgame

#endif // BOARDGAME_BOARD_HPP_INCLUDED