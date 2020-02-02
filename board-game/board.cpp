// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "board.hpp"

#include "context.hpp"
#include "pieces.hpp"
#include "random.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <set>

namespace boardgame
{
    Board::Board(const ImageHandler & images, const sf::Vector2u & windowSize, const Map & level)
        : map(level)
        , window_size(windowSize)
        , window_rect({}, window_size)
        , region_pad_ratio(0.95f)
        , board_region_vert_size_ratio(0.875f)
        , between_region_vert_scale(region_pad_ratio / 2.0f)
        , board_region()
        , status_region()
        , cell_size()
        , board_rect()
        , pieces()
    {
        board_region = window_rect;
        board_region.height *= board_region_vert_size_ratio;
        util::scaleRectInPlace(board_region, region_pad_ratio);

        status_region = window_rect;
        status_region.height *= (1.0f - board_region_vert_size_ratio);
        status_region.top = (window_size.y - status_region.height);
        util::scaleRectInPlace(status_region, region_pad_ratio);

        const sf::Vector2s cellCounts(map.cell_count_horiz, map.cell_count_vert);
        const sf::Vector2f cellRegionSize{ util::size(board_region) / sf::Vector2f(cellCounts) };

        const float cellSize{ std::min(cellRegionSize.x, cellRegionSize.y) };
        cell_size.x = cellSize;
        cell_size.y = cellSize;

        const sf::Vector2f boardRectSize{ sf::Vector2f(cellCounts) * cell_size };
        const sf::Vector2f boardRectPos{ util::center(board_region) - (boardRectSize * 0.5f) };
        board_rect = sf::FloatRect(boardRectPos, boardRectSize);

        reset(images, random);
    }

    int Board::walkDistance(const IPiece & from, const IPiece & to)
    {
        return walkDistance(from.boardPos(), to.boardPos());
    }

    void Board::reset(const ImageHandler & images)
    {
        pieces.clear();

        for (std::size_t i(0); i < map.cell_count_total; ++i)
        {
            const BoardPos_t boardPos{ indexToPos(i) };

            const sf::Vector2f windowPos{ util::position(board_rect) +
                                          (sf::Vector2f(boardPos) * cell_size) };

            const sf::FloatRect bounds{ windowPos, cell_size };

            const Piece piece{ map.posToPiece(boardPos) };

            pieces.push_back(makePiece(piece, boardPos, bounds));
        }
    }

    IPieceUPtr_t Board::makePiece(
        const ImageHandler & images,
        const Piece piece,
        const BoardPos_t & boardPos,
        const sf::FloatRect & bounds) const
    {
        switch (piece)
        {
            case Piece::Player: {
                const sf::Sprite sprite{ images.makeSprite(
                    Image::Player, bounds, sf::Color::Green) };

                return std::make_unique<PlayerPiece>(boardPos, sprite);
            }

            case Piece::Villan: {
                const sf::Sprite sprite{ images.makeSprite(Image::Demon, bounds) };
                return std::make_unique<VillanPiece>(boardPos, sprite);
            }

            case Piece::Obstacle: {
                return std::make_unique<WallPiece>(boardPos, bounds);
            }

            case Piece::Pickup:
            case Piece::Empty:
            default: {
                return std::make_unique<EmptyPiece>(boardPos, bounds);
            }
        }
    }

    // TODO optimize this crap
    std::optional<BoardPos_t> Board::findRandomEmptyPos(const Context & context)
    {
        std::vector<BoardPos_t> emptyPositions;
        emptyPositions.reserve(map.cell_count_total);

        for (const IPieceUPtr_t & uptr : pieces)
        {
            if (uptr->piece() == Piece::Empty)
            {
                emptyPositions.push_back(uptr->boardPos());
            }
        }

        if (emptyPositions.empty())
        {
            return std::nullopt;
        }

        return context.random.from(emptyPositions);
    }
} // namespace boardgame