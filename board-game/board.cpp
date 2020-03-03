// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "board.hpp"

#include "check-macros.hpp"
#include "context.hpp"
#include "pieces.hpp"
#include "random.hpp"
#include "settings.hpp"
#include "util.hpp"

namespace boardgame
{
    void Layout::setup(const IGameSettings & settings)
    {
        board_bounds = settings.windowBounds();

        M_CHECK_SS(
            (!(board_bounds.left < 0.0f) && !(board_bounds.top < 0.0f) &&
             (board_bounds.width > 1.0f) && (board_bounds.height > 1.0f)),
            board_bounds);

        cell_counts = settings.cellCounts();
        M_CHECK_SS(((cell_counts.x > 0) && (cell_counts.y > 0)), cell_counts);

        cell_size = (util::size(board_bounds) / sf::Vector2f(cell_counts));
        M_CHECK_SS((!(cell_size.x < 1.0f) && !(cell_size.y < 1.0f)), cell_size);

        cell_count_total = static_cast<std::size_t>(cell_counts.x * cell_counts.y);
        M_CHECK_SS((cell_count_total > 0), cell_count_total);

        // create a set of all valid positions, this is needed all over
        all_valid_positions.clear();
        for (int vert(0); vert < cell_counts.y; ++vert)
        {
            for (int horiz(0); horiz < cell_counts.x; ++horiz)
            {
                all_valid_positions.insert(BoardPos_t{ horiz, vert });
            }
        }

        M_CHECK(all_valid_positions.size() == cell_count_total);
    }

    //

    // void BoardBase::printStatus() const
    //{
    //    std::cout << "\nBOARD STATUS:";
    //    std::cout << "\n\t board_bounds         = " << board_bounds;
    //    std::cout << "\n\t cell_counts          = " << m_cells.counts;
    //    std::cout << "\n\t cells_total          = " << m_cells.count_total;
    //    std::cout << "\n\t cell_size            = " << m_cells.size;
    //    std::cout << "\n\t pieces_vec_size      = " << m_pieces.size();
    //
    //    std::map<Piece, std::size_t> pieceCountMap;
    //    for (const IPieceUPtr_t & piece : m_pieces)
    //    {
    //        ++pieceCountMap[piece->piece()];
    //    }
    //
    //    for (const auto & [piece, count] : pieceCountMap)
    //    {
    //        std::cout << "\n\t\t" << Piece::name(piece) << "\t x" << count;
    //    }
    //
    //    // print the picture of the map on the console
    //    // std::cout << "\n\t-" << std::endl;
    //    // for (int vert(0); vert < m_cells.counts_int.y; ++vert)
    //    //{
    //    //    for (int horiz(0); horiz < m_cells.counts_int.x; ++horiz)
    //    //    {
    //    //        const BoardPos_t pos(horiz, vert);
    //    //        const std::string name{ Piece::name(pieceEnumAt(pos)) };
    //    //        std::cout << ((name.empty()) ? '?' : name.front());
    //    //    }
    //    //
    //    //    std::cout << '\n';
    //    //}
    //
    //    std::cout << std::endl;
    //}

    void BoardBase::setup(const IGameSettings & settings)
    {
        // reset(context);
        m_layout.setup(settings);
    }

    // void BoardBase::reset(Context &) { m_pieces.clear(); }

    void BoardBase::update(Context & context, const float elapsedTimeSec)
    {
        for (IPieceUPtr_t & piece : m_pieces)
        {
            piece->update(context, elapsedTimeSec);
        }
    }

    void BoardBase::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const IPieceUPtr_t & piece : m_pieces)
        {
            target.draw(*piece, states);
        }
    }

    void BoardBase::handleEvent(Context & context, const sf::Event & event)
    {
        for (const IPieceUPtr_t & piece : m_pieces)
        {
            piece->handleEvent(context, event);
        }
    }

    IPieceOpt_t BoardBase::pieceAtOpt(const BoardPos_t & posToFind)
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

    bool BoardBase::isPieceAt(const BoardPos_t & posToFind)
    {
        return pieceAtOpt(posToFind).has_value();
    }

    Piece BoardBase::pieceEnumAt(const BoardPos_t & posToFind)
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

    IPieceUListIter_t BoardBase::pieceIterAt(const BoardPos_t & posToFind)
    {
        return std::find_if(
            std::begin(m_pieces), std::end(m_pieces), [&](const IPieceUPtr_t & piece) {
                return (piece->position() == posToFind);
            });
    }

    void BoardBase::removePiece(Context &, const BoardPos_t & posToFind)
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

    // void BoardBase::move(Context & context, const BoardPos_t & posFrom, const BoardPos_t & posTo)
    //{
    //    const auto fromIter{ pieceIterAt(posFrom) };
    //    M_CHECK_SS((fromIter != std::end(m_pieces)), posFrom);
    //    M_CHECK_SS((fromIter->get()->piece() != Piece::Count), posFrom);
    //
    //    remove(context, posTo);
    //
    //    fromIter->get()->
    //
    //    placePiece(context, makePiece(context, piece, pos));
    //}

    void BoardBase::addPiece(Context & context, IPieceUPtr_t iPieceUPtr)
    {
        M_CHECK(iPieceUPtr);

        const Piece newEnum{ iPieceUPtr->piece() };
        M_CHECK(newEnum != Piece::Count);

        const BoardPos_t newPos{ iPieceUPtr->position() };
        M_CHECK_SS(m_layout.isPositionValid(newPos), newPos);

        // no need to check if there is a piece to remove or not
        removePiece(context, newPos);

        m_pieces.push_back(std::move(iPieceUPtr));

        M_CHECK(pieceEnumAt(newPos) == newEnum);
    }

    void BoardBase::addPiece(Context & context, const Piece piece, const BoardPos_t & pos)
    {
        M_CHECK_SS((piece != Piece::Count), piece);
        M_CHECK_SS(m_layout.isPositionValid(pos), pos);
        addPiece(context, makePiece(context, piece, pos));
    }

    void BoardBase::addPieceRandomFreePos(Context & context, const Piece piece)
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

    BoardPosOpt_t BoardBase::findRandomFreePos(const Context & context) const
    {
        // start with a copy of all valid/on-board positions
        std::set<BoardPos_t> positions{ m_layout.all_valid_positions };

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

    // void SnakeBoard::reset(Context & context)
    //{
    //    BoardBase::reset(context);
    //    populateBoardWithMapPieces(context);
    //}

    void SnakeBoard::populateBoardWithMapPieces(Context & context)
    {
        const sf::Vector2i counts{ m_layout.cell_counts };

        const BoardPos_t centerPos{ counts / 2 };

        addPiece(context, Piece::Head, centerPos);

        for (int horiz(0); horiz < counts.x; ++horiz)
        {
            addPiece(context, Piece::Wall, BoardPos_t(horiz, 0));
            addPiece(context, Piece::Wall, BoardPos_t(horiz, (counts.y - 1)));
        }
        for (int vert(0); vert < counts.y; ++vert)
        {
            addPiece(context, Piece::Wall, BoardPos_t(0, vert));
            addPiece(context, Piece::Wall, BoardPos_t((counts.x - 1), vert));
        }

        for (int i(0); i < 5; ++i)
        {
            addPieceRandomFreePos(context, Piece::Food);
        }
    }

    IPieceUPtr_t SnakeBoard::makePiece(Context & context, const Piece piece, const BoardPos_t & pos)
    {
        switch (piece)
        {
            case Piece::Food: return std::make_unique<FoodPiece>(context, pos);
            case Piece::Head: return std::make_unique<HeadPiece>(context, pos);
            case Piece::Tail: return std::make_unique<TailPiece>(context, pos);
            case Piece::Wall:
            case Piece::Count:
            default: return std::make_unique<WallPiece>(context, pos);
        }
    }
} // namespace boardgame