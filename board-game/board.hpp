#ifndef BOARDGAME_BOARD_HPP_INCLUDED
#define BOARDGAME_BOARD_HPP_INCLUDED
//
// board.hpp
//
#include <list>
#include <memory>
#include <optional>

#include <SFML/Graphics.hpp>

//

namespace boardgame
{
    enum class Piece;
    struct Context;
    struct IMedia;

    using Map_t = std::vector<std::string>;

    using BoardPos_t = sf::Vector2i;
    using BoardPosOpt_t = std::optional<BoardPos_t>;

    struct IPiece;
    using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;
    using IPieceUPtr_t = std::unique_ptr<IPiece>;
    using IPieceUList_t = std::list<IPieceUPtr_t>;
    using IPieceUListIter_t = IPieceUList_t::iterator;

    // Keeps all pieces, allows find/add/move/remove simply by their enum and BoardPos_t
    // Pieces should use these add/remove/move functions instead of trying to do it themselves.
    struct IBoard : public sf::Drawable
    {
        virtual ~IBoard() = default;

        virtual void reset(Context &) = 0;
        virtual void update(Context &, const float elapsedTimeSec) = 0;
        void draw(sf::RenderTarget &, sf::RenderStates) const override = 0;
        virtual void handleEvent(Context &, const sf::Event & event) = 0;

        virtual bool isPieceAt(const BoardPos_t & pos) = 0;
        virtual IPieceOpt_t pieceAtOpt(const BoardPos_t & pos) = 0;
        virtual Piece pieceEnumAt(const BoardPos_t & pos) = 0;
        virtual IPieceUListIter_t pieceIterAt(const BoardPos_t & pos) = 0;
        virtual const IPieceUList_t & pieces() const = 0;

        virtual void removeAllPieces() = 0;
        virtual void removePiece(Context &, const BoardPos_t & pos) = 0;
        virtual void addPiece(Context &, IPieceUPtr_t iPieceUPtr) = 0;
        virtual void addPiece(Context &, const Piece, const BoardPos_t &) = 0;
        virtual void addPieceRandomFreePos(Context &, const Piece) = 0;
        virtual void addMapPieces(Context & context) = 0;

        virtual BoardPosOpt_t findRandomFreePos(const Context & context) const = 0;
        virtual IPieceUPtr_t makePiece(Context &, const Piece, const BoardPos_t & pos) = 0;
    };

    // A simple reference implementation of an IBoard that should suffice for most games.
    class SimpleBoard : public IBoard
    {
      public:
        SimpleBoard() = default;
        virtual ~SimpleBoard() = default;

        void reset(Context &) override;
        void update(Context &, const float elapsedTimeSec) override;
        void draw(sf::RenderTarget &, sf::RenderStates) const override;
        void handleEvent(Context &, const sf::Event & event) override;

        bool isPieceAt(const BoardPos_t & pos) override;
        IPieceOpt_t pieceAtOpt(const BoardPos_t & pos) override;
        Piece pieceEnumAt(const BoardPos_t & pos) override;
        IPieceUListIter_t pieceIterAt(const BoardPos_t & pos) override;
        const IPieceUList_t & pieces() const override { return m_pieces; }

        void removeAllPieces() override;
        void removePiece(Context &, const BoardPos_t & pos) override;
        void addPiece(Context &, IPieceUPtr_t iPieceUPtr) override;
        void addPiece(Context &, const Piece piece, const BoardPos_t & pos) override;
        void addPieceRandomFreePos(Context &, const Piece piece) override;
        void addMapPieces(Context & context) override;

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