// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// board.cpp
//
#include "board.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "pieces.hpp"
#include "random.hpp"
#include "settings.hpp"
#include "types.hpp"
#include "util.hpp"

namespace boardgame
{
    void SimpleBoard::reset(Context & context)
    {
        removeAllPieces();
        addMapPieces(context);
    }

    void SimpleBoard::addMapPieces(Context & context)
    {
        std::size_t rowWidth{ context.map.at(0).size() };
        for (std::size_t vert(0); vert < context.map.size(); ++vert)
        {
            const std::string row{ context.map.at(vert) };

            M_CHECK_SS((row.size() == rowWidth), "Not all of the map rows were the same length");

            for (std::size_t horiz(0); horiz < row.size(); ++horiz)
            {
                const BoardPos_t boardPos{ sf::Vector2s(horiz, vert) };

                const char mapChar{ row.at(horiz) };
                const Piece piece{ mapCharToPiece(mapChar) };

                if (piece != Piece::Count)
                {
                    addPiece(context, makePiece(context, piece, boardPos));
                }
            }
        }
    }

    IPieceUPtr_t
        SimpleBoard::makePiece(Context & context, const Piece piece, const BoardPos_t & pos)
    {
        M_CHECK_SS((piece != Piece::Count), piece);
        return std::make_unique<CellPiece>(context, pos, piece);
    }

    void SimpleBoard::update(Context & context, const float elapsedTimeSec)
    {
        for (IPieceUPtr_t & piece : m_pieces)
        {
            piece->update(context, elapsedTimeSec);
        }
    }

    void SimpleBoard::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const IPieceUPtr_t & piece : m_pieces)
        {
            target.draw(*piece, states);
        }
    }

    bool SimpleBoard::handleEvent(Context & context, const sf::Event & event)
    {
        bool didAnyPiecesHandleThEvent{ false };
        for (const IPieceUPtr_t & piece : m_pieces)
        {
            if (piece->handleEvent(context, event))
            {
                didAnyPiecesHandleThEvent = true;
            }
        }

        return didAnyPiecesHandleThEvent;
    }

    IPieceOpt_t SimpleBoard::pieceAtOpt(const BoardPos_t & posToFind)
    {
        const auto foundIter{ pieceIterAt(posToFind) };
        if (foundIter == std::end(m_pieces))
        {
            return std::nullopt;
        }
        else
        {
            return { **foundIter };
        }
    }

    bool SimpleBoard::isPieceAt(const BoardPos_t & posToFind)
    {
        return pieceAtOpt(posToFind).has_value();
    }

    Piece SimpleBoard::pieceEnumAt(const BoardPos_t & posToFind)
    {
        const auto opt{ pieceAtOpt(posToFind) };
        if (opt)
        {
            return opt.value().get().piece();
        }
        else
        {
            return Piece::Count;
        }
    }

    IPieceUListIter_t SimpleBoard::pieceIterAt(const BoardPos_t & posToFind)
    {
        return std::find_if(
            std::begin(m_pieces), std::end(m_pieces), [&](const IPieceUPtr_t & piece) {
                return (piece->position() == posToFind);
            });
    }

    void SimpleBoard::removePiece(Context &, const BoardPos_t & posToFind)
    {
        // M_CHECK_SS(m_layout.isPositionValid(posToFind), posToFind);

        const auto iter{ pieceIterAt(posToFind) };
        if (iter == std::end(m_pieces))
        {
            // this is not an error case, because we often just remove without checking first
            return;
        }

        m_pieces.erase(iter);

        M_CHECK(!isPieceAt(posToFind));
    }

    void SimpleBoard::removeAllPieces() { m_pieces.clear(); }

    void SimpleBoard::addPiece(Context & context, IPieceUPtr_t iPieceUPtr)
    {
        M_CHECK(iPieceUPtr);

        const Piece newEnum{ iPieceUPtr->piece() };
        M_CHECK(newEnum != Piece::Count);

        const BoardPos_t newPos{ iPieceUPtr->position() };
        M_CHECK_SS(context.layout.isPositionValid(newPos), newPos);

        // no need to check if there is a piece to remove or not
        removePiece(context, newPos);

        m_pieces.push_back(std::move(iPieceUPtr));

        M_CHECK(pieceEnumAt(newPos) == newEnum);
    }

    void SimpleBoard::addPiece(Context & context, const Piece piece, const BoardPos_t & pos)
    {
        M_CHECK_SS((piece != Piece::Count), piece);
        M_CHECK_SS(context.layout.isPositionValid(pos), pos);
        addPiece(context, makePiece(context, piece, pos));
    }

    void SimpleBoard::addPieceRandomFreePos(Context & context, const Piece piece)
    {
        const auto posOpt{ findRandomFreePos(context) };
        if (!posOpt.has_value())
        {
            M_CHECK_LOG_SS(
                posOpt.has_value(),
                "Let the game keep running, and maybe a free spot will open up later.");

            return;
        }

        addPiece(context, piece, posOpt.value());
    }

    BoardPosOpt_t SimpleBoard::findRandomFreePos(const Context & context) const
    {
        // start with a copy of all valid/on-board positions
        std::set<BoardPos_t> positions{ context.layout.allValidPositions() };

        // remove any that are alraedy occupied
        for (const IPieceUPtr_t & piece : m_pieces)
        {
            positions.erase(piece->position());
        }

        if (positions.empty())
        {
            return std::nullopt;
        }

        return context.random.from(positions);
    }

    //
} // namespace boardgame