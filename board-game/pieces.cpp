// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// meth-head.cpp
//
#include "pieces.hpp"

#include "board.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "util.hpp"

#include <cassert>

namespace boardgame
{
    PieceBase::PieceBase(
        const Context & context,
        const Piece piece,
        const sf::Color & color,
        const sf::Texture & texture,
        const std::vector<Piece> & walkablePieces,
        const BoardPos_t & pos)
        : m_piece(piece)
        , m_boardPos((InvalidBoardPos == pos) ? context.board.randomAvailBoardPos(context) : pos)
        , m_text("", context.resources.font, 99)
        , m_sprite(texture)
        , m_color(color)
        , m_waitedSoFarSec(0.0f)
        , m_waitBetweenTurnsSec(0.5f)
        , m_walkAllowedPieces(walkablePieces)
    {
        m_text.setFillColor(m_color);
        context.board.setupSprite(m_sprite, boardPos(), color);
    }

    //

    // TODO this just picks the first, so eventually it will hae to do something smart
    BoardPos_t SeekerPieceBase::pickTarget(const Context & context)
    {
        if ((Piece::Hero == m_targetPiece) && !context.board.players.empty())
        {
            return context.board.players.front()->boardPos();
        }

        if ((Piece::Demon == m_targetPiece) && !context.board.demons.empty())
        {
            return context.board.demons.front()->boardPos();
        }

        if ((Piece::Child == m_targetPiece) && !context.board.kids.empty())
        {
            return context.board.kids.front()->boardPos();
        }

        // default to no target
        return boardPos();
    }

    BoardPos_t SeekerPieceBase::takeTurn(const Context & context)
    {
        // if has no target the try to find one
        if (boardPos() == m_targetBoardPos)
        {
            const BoardPos_t newPos{ pickTarget(context) };

            // bail if no new target was found
            if (boardPos() == newPos)
            {
                return boardPos();
            }
        }

        const auto movesEndIter{ findAllPossibleMoves(context) };

        // bail if no valid moves remain
        if (std::begin(m_moves) == movesEndIter)
        {
            return boardPos();
        }

        // if there is only one possible move, then move there and bail
        if ((std::begin(m_moves) + 1) == movesEndIter)
        {
            return m_moves[0].pos;
        }

        return pickMoveClosestToTarget(context, movesEndIter);
    }

    MovesArray_t::iterator SeekerPieceBase::findAllPossibleMoves(const Context & context)
    {
        const BoardPos_t currPos{ boardPos() };
        const sf::Vector2i cellCounts{ context.board.cellCounts<int>() };

        // start with all four possible direction moves
        // clang-format off
        m_moves = { WalkDistance{ 0, (currPos + sf::Vector2i(-1,  0)) },
                    WalkDistance{ 0, (currPos + sf::Vector2i( 1,  0)) },
                    WalkDistance{ 0, (currPos + sf::Vector2i( 0, -1)) },
                    WalkDistance{ 0, (currPos + sf::Vector2i( 0,  1)) } };
        // clang-format on

        // TODO remove this for looping doors etc
        // remove any off-board moves
        auto movesEndIter = std::remove_if(
            std::begin(m_moves), std::end(m_moves), [&](const WalkDistance & walkDist) {
                const BoardPos_t & pos{ walkDist.pos };
                return (
                    (pos.x < 0) || (pos.y < 0) || (pos.x >= cellCounts.x) ||
                    (pos.y >= cellCounts.y));
            });

        // remove any moves already taken by anything other than our target piece
        movesEndIter =
            std::remove_if(std::begin(m_moves), movesEndIter, [&](const WalkDistance & walkDist) {
                const Piece piece{ context.board.pieceAt(walkDist.pos) };
                return ((piece != Piece::Empty) && (piece != m_targetPiece));
            });

        return movesEndIter;
    }

    BoardPos_t SeekerPieceBase::pickMoveClosestToTarget(
        const Context & context, MovesArray_t::iterator movesEndIter)
    {
        assert(std::distance(std::begin(m_moves), movesEndIter) > 1);

        // calc how far each move is from the target
        for (auto iter(std::begin(m_moves)); iter != movesEndIter; ++iter)
        {
            iter->dist = Board::walkDistance(iter->pos, m_targetBoardPos);
        }

        // find the shortest distance among all remaining moves
        const int shortestWalkDist =
            std::min_element(
                std::begin(m_moves),
                movesEndIter,
                [&](const WalkDistance & left, const WalkDistance & right) {
                    return (left.dist < right.dist);
                })
                ->dist;

        // remove any moves that are farther than the shortest distance
        movesEndIter =
            std::remove_if(std::begin(m_moves), movesEndIter, [&](const WalkDistance & posDist) {
                return posDist.dist > shortestWalkDist;
            });

        // random select one
        const BoardPos_t newPos{ context.random.from(std::begin(m_moves), movesEndIter).pos };

        return newPos;
    }

    //

    WallPiece::WallPiece(const Context & context, const BoardPos_t & pos)
        : PieceBase(
              context, Piece::Wall, sf::Color(200, 105, 5), context.resources.wall_texture, {}, pos)
    {}

    ChildPiece::ChildPiece(const Context & context, const BoardPos_t & pos)
        : PieceBase(
              context,
              Piece::Child,
              sf::Color(255, 32, 255),
              context.resources.child_texture,
              { Piece::Empty, Piece::Hero },
              pos)
    {
        std::cout << pieceName(m_piece) << " at " << boardPos() << " or " << m_sprite.getPosition()
                  << std::endl;
    }

    PlayerPiece::PlayerPiece(const Context & context, const BoardPos_t & pos)
        : PieceBase(
              context,
              Piece::Hero,
              sf::Color(32, 255, 32),
              context.resources.hero_texture,
              { Piece::Empty, Piece::Child },
              pos)
        , m_nextMovePos(InvalidBoardPos)
    {
        std::cout << pieceName(m_piece) << " at " << boardPos() << " or " << m_sprite.getPosition()
                  << std::endl;
    }

    DemonPiece::DemonPiece(const Context & context, const BoardPos_t & pos)
        : SeekerPieceBase(
              context,
              Piece::Demon,
              sf::Color(255, 32, 32),
              context.resources.demon_texture,
              Piece::Hero,
              pos)
    {
        std::cout << pieceName(m_piece) << " at " << boardPos() << " or " << m_sprite.getPosition()
                  << std::endl;
    }
} // namespace boardgame