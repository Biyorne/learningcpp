#ifndef BOARDGAME_BOARD_HPP_INCLUDED
#define BOARDGAME_BOARD_HPP_INCLUDED
//
// board.hpp
//
#include "context.hpp"

#include <list>
#include <memory>
#include <optional>

#include <SFML/Graphics.hpp>

//

namespace boardgame
{
    enum class Piece;
    struct IMedia;
    class BoardPosKeeper;

    using Map_t = std::vector<std::string>;

    using BoardPos_t = sf::Vector2i;
    using BoardPosOpt_t = std::optional<BoardPos_t>;

    struct IPiece;
    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;
    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUList_t = std::list<IPieceUPtr_t>;

    // Keeps all pieces, allows find/add/move/remove simply by their enum and BoardPos_t
    // Pieces should use these add/remove/move functions instead of trying to do it themselves.
    struct IBoard
    {
        virtual ~IBoard() = default;

        virtual void reset(Context &) = 0;

        virtual bool isPieceAt(const BoardPos_t & pos) const = 0;
        virtual Piece pieceEnumAt(const BoardPos_t & pos) = 0;

        virtual IPieceOpt_t pieceAtOpt(const BoardPos_t & pos) = 0;
        virtual const IPieceOpt_t pieceAtOpt(const BoardPos_t & pos) const = 0;

        virtual IPieceUList_t & pieces() = 0;
        virtual const IPieceUList_t & pieces() const = 0;

        virtual void removePiece(const BoardPos_t & pos) = 0;
        virtual void addPiece(Context &, IPieceUPtr_t iPieceUPtr) = 0;
        virtual void addPiece(Context &, const Piece, const BoardPos_t &) = 0;
        virtual void addMapPieces(Context & context) = 0;

        virtual BoardPosOpt_t findRandomFreePos(const Context & context) const = 0;
        virtual IPieceUPtr_t makePiece(Context &, const Piece, const BoardPos_t & pos) = 0;

        // TODO do I really need more than one?
        virtual void movePiece(const Context &, const BoardPos_t & from, const BoardPos_t & to) = 0;
        void movePiece(const Context &, BoardPosKeeper & posManager, const BoardPos_t & newPos);
    };

    // A simple reference implementation of an IBoard that should suffice for most games.
    class SimpleBoard : public IBoard
    {
      public:
        SimpleBoard() = default;
        virtual ~SimpleBoard() = default;

        void reset(Context &) override;

        bool isPieceAt(const BoardPos_t & pos) const override;

        IPieceOpt_t pieceAtOpt(const BoardPos_t & pos) override;
        const IPieceOpt_t pieceAtOpt(const BoardPos_t & pos) const override;

        Piece pieceEnumAt(const BoardPos_t & pos) override;

        IPieceUList_t & pieces() override { return m_pieces; }
        const IPieceUList_t & pieces() const override { return m_pieces; }

        void removePiece(const BoardPos_t & pos) override;
        void addPiece(Context &, IPieceUPtr_t iPieceUPtr) override;
        void addPiece(Context &, const Piece piece, const BoardPos_t & pos) override;
        void addMapPieces(Context & context) override;

        void movePiece(const Context &, const BoardPos_t & from, const BoardPos_t & to) override;

        BoardPosOpt_t findRandomFreePos(const Context & context) const override;
        IPieceUPtr_t makePiece(Context &, const Piece piece, const BoardPos_t & pos) override;

      protected:
        // std::list is used because:
        //   * size will almost never exceed 100
        //   * it never invalidates iterators (unless you try to erase yourself)
        //   * it never reorders or reallocates
        //
        //   All these properties above are essential because any piece, at any time, can
        //   change/move/erase/etc any other piece -in the middle of a loop through that same
        //   container of pieces.
        IPieceUList_t m_pieces;
    };
} // namespace boardgame

#endif // BOARDGAME_BOARD_HPP_INCLUDED