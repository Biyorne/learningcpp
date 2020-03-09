#ifndef BOARDGAME_BOARD_HPP_INCLUDED
#define BOARDGAME_BOARD_HPP_INCLUDED
//
// board.hpp
//
#include "context.hpp"

#include <list>
#include <memory>
#include <optional>
#include <tuple>

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
    using PieceEnumOpt_t = std::optional<Piece>;

    //

    // const BoardPos_t not_a_pos{ std::numeric_limits<int>::lowest(),
    //                            std::numeric_limits<int>::lowest() };
    //
    // struct AdjacentInfo
    //{
    //    Piece piece{ Piece::Wall };
    //    BoardPos_t pos{ not_a_pos };
    //    sf::Keyboard::Key dir{ keys::not_a_key };
    //};
    //
    // using AdjacentInfoOpt_t = std::optional<AdjacentInfo>;
    //
    //[[nodiscard]] inline bool
    //    operator==(const AdjacentInfo & left, const AdjacentInfo & right) noexcept
    //{
    //    // clang-format off
    //    return (std::tie(
    //        left.piece,
    //        left.pos,
    //        left.dir)
    //        == std::tie(
    //            right.piece,
    //            right.pos,
    //            right.dir));
    //    // clang-format on
    //}
    //
    //[[nodiscard]] inline bool
    //    operator!=(const AdjacentInfo & left, const AdjacentInfo & right) noexcept
    //{
    //    return !(left == right);
    //}
    //
    //[[nodiscard]] inline bool
    //    operator<(const AdjacentInfo & left, const AdjacentInfo & right) noexcept
    //{
    //    // clang-format off
    //    return (std::tie(
    //        left.piece,
    //        left.pos,
    //        left.dir)
    //        < std::tie(
    //            right.piece,
    //            right.pos,
    //            right.dir));
    //    // clang-format on
    //}
    //
    ////
    //
    // struct Surroundings
    //{
    //    explicit Surroundings(const BoardPos_t & centerPos)
    //        : center_pos(centerPos)
    //        , adjacents()
    //    {
    //        adjacents.reserve(4);
    //    }
    //
    //    //
    //
    //    PieceEnumOpt_t pieceInDirOpt(const sf::Keyboard::Key dirToFind) const
    //    {
    //        for (const AdjacentInfo & ap : adjacents)
    //        {
    //            if (dirToFind == ap.dir)
    //            {
    //                return ap.piece;
    //            }
    //        }
    //
    //        return std::nullopt;
    //    }
    //
    //    BoardPosOpt_t posInDirOpt(const sf::Keyboard::Key dirToFind) const
    //    {
    //        for (const AdjacentInfo & ap : adjacents)
    //        {
    //            if (dirToFind == ap.dir)
    //            {
    //                return ap.pos;
    //            }
    //        }
    //
    //        return std::nullopt;
    //    }
    //
    //    BoardPos_t posInDir(const sf::Keyboard::Key dirToFind) const
    //    {
    //        for (const AdjacentInfo & ap : adjacents)
    //        {
    //            if (dirToFind == ap.dir)
    //            {
    //                return ap.pos;
    //            }
    //        }
    //
    //        return not_a_pos;
    //    }
    //
    //    //
    //
    //    DirKeyOpt_t dirOfPieceOpt(const Piece pieceToFind) const
    //    {
    //        for (const AdjacentInfo & ap : adjacents)
    //        {
    //            if (pieceToFind == ap.piece)
    //            {
    //                return ap.dir;
    //            }
    //        }
    //
    //        return std::nullopt;
    //    }
    //
    //    sf::Keyboard::Key dirOfPiece(const Piece pieceToFind) const
    //    {
    //        for (const AdjacentInfo & ap : adjacents)
    //        {
    //            if (pieceToFind == ap.piece)
    //            {
    //                return ap.dir;
    //            }
    //        }
    //
    //        return keys::not_a_key;
    //    }
    //
    //    BoardPosOpt_t posOfPieceOpt(const Piece pieceToFind) const
    //    {
    //        for (const AdjacentInfo & ap : adjacents)
    //        {
    //            if (pieceToFind == ap.piece)
    //            {
    //                return ap.pos;
    //            }
    //        }
    //
    //        return std::nullopt;
    //    }
    //
    //    BoardPos_t posOfPiece(const Piece pieceToFind) const
    //    {
    //        for (const AdjacentInfo & ap : adjacents)
    //        {
    //            if (pieceToFind == ap.piece)
    //            {
    //                return ap.pos;
    //            }
    //        }
    //
    //        return not_a_pos;
    //    }
    //
    //    //
    //
    //    PieceEnumOpt_t pieceAtPosOpt(const BoardPos_t & posToFind) const
    //    {
    //        for (const AdjacentInfo & ap : adjacents)
    //        {
    //            if (posToFind == ap.pos)
    //            {
    //                return ap.piece;
    //            }
    //        }
    //
    //        return std::nullopt;
    //    }
    //
    //    DirKeyOpt_t dirOfPosOpt(const BoardPos_t & posToFind) const
    //    {
    //        for (const AdjacentInfo & ap : adjacents)
    //        {
    //            if (posToFind == ap.pos)
    //            {
    //                return ap.dir;
    //            }
    //        }
    //
    //        return std::nullopt;
    //    }
    //
    //    sf::Keyboard::Key dirOfPos(const BoardPos_t & posToFind) const
    //    {
    //        for (const AdjacentInfo & ap : adjacents)
    //        {
    //            if (posToFind == ap.pos)
    //            {
    //                return ap.dir;
    //            }
    //        }
    //
    //        return keys::not_a_key;
    //    }
    //
    //    //
    //
    //    std::size_t pieceCount(const Piece pieceToFind) const
    //    {
    //        std::size_t count{ 0 };
    //        for (const AdjacentInfo & ap : adjacents)
    //        {
    //            if (pieceToFind == ap.piece)
    //            {
    //                ++count;
    //            }
    //        }
    //
    //        return count;
    //    }
    //
    //    std::size_t piecesCount(const std::initializer_list<Piece> & list) const
    //    {
    //        std::size_t count{ 0 };
    //        for (const Piece piece : list)
    //        {
    //            count += pieceCount(piece);
    //        }
    //
    //        return count;
    //    }
    //
    //    //
    //
    //    BoardPos_t center_pos{ not_a_pos };
    //    std::vector<AdjacentInfo> adjacents;
    //};

    //

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
        virtual void addPieceAtRandomFreePos(Context &, const Piece piece) = 0;
        virtual void addMapPieces(Context & context) = 0;

        virtual BoardPosOpt_t findRandomFreePos(const Context & context) const = 0;
        virtual IPieceUPtr_t makePiece(Context &, const Piece, const BoardPos_t & pos) = 0;

        virtual void movePiece(Context &, const BoardPos_t & from, const BoardPos_t & to) = 0;

        // virtual const AdjacentInfoOpt_t
        //    adjacentInfoOpt(const BoardPos_t & centerPos, const sf::Keyboard::Key dir) const = 0;
        //
        // virtual const Surroundings surroundings(const BoardPos_t & centerPos) const = 0;
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
        void addPieceAtRandomFreePos(Context &, const Piece piece) override;
        void addMapPieces(Context & context) override;

        void movePiece(Context &, const BoardPos_t & from, const BoardPos_t & to) override;

        BoardPosOpt_t findRandomFreePos(const Context & context) const override;
        IPieceUPtr_t makePiece(Context &, const Piece piece, const BoardPos_t & pos) override;

        // const AdjacentInfoOpt_t adjacentInfoOpt(
        //    const BoardPos_t & centerPos, const sf::Keyboard::Key dir) const override
        //{
        //    // const BoardPos_t adjPos{ keys::move(centerPos, dir) };
        //    // const auto enumOpt{ pieceEnumOptAt(adjPos) };
        //    // if (enumOpt)
        //    // {
        //    //     return AdjacentInfo{ enumOpt.value(), adjPos, dir };
        //    // }
        //    // else
        //    {
        //        return std::nullopt;
        //    }
        //}
        //
        // const Surroundings surroundings(const BoardPos_t & centerPos) const override
        //{
        //    Surroundings surr(centerPos);
        //
        //    // clang-format off
        //    if (const AdjacentInfoOpt_t opt{ adjacentInfoOpt(centerPos, sf::Keyboard::Up) }; opt)
        //    surr.adjacents.push_back(opt.value()); if (const AdjacentInfoOpt_t opt{
        //    adjacentInfoOpt(centerPos, sf::Keyboard::Down) };  opt)
        //    surr.adjacents.push_back(opt.value()); if (const AdjacentInfoOpt_t opt{
        //    adjacentInfoOpt(centerPos, sf::Keyboard::Left) };  opt)
        //    surr.adjacents.push_back(opt.value()); if (const AdjacentInfoOpt_t opt{
        //    adjacentInfoOpt(centerPos, sf::Keyboard::Right) }; opt)
        //    surr.adjacents.push_back(opt.value());
        //    // clang-format on
        //
        //    return surr;
        //}

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