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

#include <iostream>

namespace boardgame
{
    void SimpleBoard::movePiece(
        Context & context, const BoardPos_t & posFrom, const BoardPos_t & posTo)
    {
        const auto pieceOpt{ pieceAtOpt(posFrom) };
        M_CHECK_SS(pieceOpt.has_value(), "posFrom=" << posFrom << ", posTo=" << posTo);

        removePiece(posTo);
        pieceOpt->get().updateAfterMove(context, posTo);

        M_CHECK_SS(
            ((&pieceOpt->get()) == (&pieceAtOpt(posTo)->get())),
            "posFrom=" << posFrom << ", enumFrom-" << pieceOpt->get().piece() << ", posTo=" << posTo
                       << ", enumTo=" << pieceEnumAt(posTo));

        M_CHECK_SS(
            (pieceEnumAt(posTo) == pieceOpt->get().piece()),
            "posFrom=" << posFrom << ", enumFrom-" << pieceOpt->get().piece() << ", posTo=" << posTo
                       << ", enumTo=" << pieceEnumAt(posTo));
    }

    void SimpleBoard::reset(Context & context)
    {
        m_pieces.clear();
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

        switch (piece)
        {
            case Piece::Eater: return std::make_unique<EaterPiece>(context, pos, piece);
            case Piece::Food: return std::make_unique<FoodPiece>(context, pos, piece);
            case Piece::Wall:
            case Piece::Count:
            default: return std::make_unique<WallPiece>(context, pos, piece);
        }
    }

    IPieceOpt_t SimpleBoard::pieceAtOpt(const BoardPos_t & posToFind)
    {
        for (const IPieceUPtr_t & piece : m_pieces)
        {
            if (posToFind == piece->position())
            {
                return *piece;
            }
        }

        return std::nullopt;
    }

    const IPieceOpt_t SimpleBoard::pieceAtOpt(const BoardPos_t & posToFind) const
    {
        for (const IPieceUPtr_t & piece : m_pieces)
        {
            if (posToFind == piece->position())
            {
                return *piece;
            }
        }

        return std::nullopt;
    }

    bool SimpleBoard::isPieceAt(const BoardPos_t & posToFind) const
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

    void SimpleBoard::removePiece(const BoardPos_t & posToFind)
    {
        // WARNING:
        //  I'm NOT using the idiomatic erase-remove intentionally, to avoid use-after-free CRASHES.
        //
        // The Problem:
        //   If you use the erase-remove here AND this code is called from inside a loop or
        //   ranged-for, then it can lead to the loop's underlying iterator to become std::end() at
        //   the bottom of that loop.  I thought this would be perfecrtly safe because the next loop
        //   iteration should check for that.  Somehow, I'm wrong.
        //
        //   You might think this is easy to check at the bottom of the loop, like so:
        //      {  if (iter==end} break;  }
        //   You might even decide that this is a best-practice or must-have for any loops that
        //   might erase/change their container -but you would be WRONG.  While that trick does seem
        //   to work on windows/MSVC builds, it still crashes on clang and gcc.
        //
        // The Solution:
        //    Do NOT use the erase-reomove idiom, or anything similar. (i.e. swap & pop)
        //    -
        //    If only erasing one item, use:           std::list::erase()
        //     " -but you have to find it first, use:  std::find() then std::list::erase()
        //    If erasing multiple items, use:          std::list::remove() or remove_if()
        //    (both of those functions actually erase, and both will erase ALL matching items)
        //

        m_pieces.remove_if(
            [&](const IPieceUPtr_t & piece) { return (posToFind == piece->position()); });

        M_CHECK(!isPieceAt(posToFind));
    }

    void SimpleBoard::addPiece(Context & context, IPieceUPtr_t iPieceUPtr)
    {
        M_CHECK(iPieceUPtr);

        const Piece newEnum{ iPieceUPtr->piece() };
        M_CHECK(newEnum != Piece::Count);

        const BoardPos_t newPos{ iPieceUPtr->position() };
        M_CHECK_SS(context.layout.isPositionValid(newPos), newPos);

        removePiece(newPos);

        m_pieces.push_back(std::move(iPieceUPtr));

        M_CHECK(pieceEnumAt(newPos) == newEnum);
    }

    void SimpleBoard::addPiece(Context & context, const Piece piece, const BoardPos_t & pos)
    {
        M_CHECK_SS((piece != Piece::Count), piece);
        M_CHECK_SS(context.layout.isPositionValid(pos), pos);
        addPiece(context, makePiece(context, piece, pos));
    }

    void SimpleBoard::addPieceAtRandomFreePos(Context & context, const Piece piece)
    {
        const auto posOpt{ findRandomFreePos(context) };
        if (!posOpt)
        {
            std::cout << "Failed to add a " << piece << " piece because no free pos found!"
                      << std::endl;

            return;
        }

        addPiece(context, piece, posOpt.value());
    }

    BoardPosOpt_t SimpleBoard::findRandomFreePos(const Context & context) const
    {
        // start with a copy of all valid/on-board positions
        static std::vector<BoardPos_t> positions;
        positions = context.layout.allValidPositions();

        // remove any that are alraedy occupied
        for (const IPieceUPtr_t & piece : m_pieces)
        {
            positions.erase(
                std::remove(std::begin(positions), std::end(positions), piece->position()),
                std::end(positions));
        }

        if (positions.empty())
        {
            return std::nullopt;
        }

        return context.random.from(positions);
    }
} // namespace boardgame