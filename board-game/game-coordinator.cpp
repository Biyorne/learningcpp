// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// game-coordinator.cpp
//
#include "game-coordinator.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>

namespace boardgame
{
    GameCoordinator::GameCoordinator(const std::filesystem::path & mediaDirPath)
        : m_simTimeMult(1.0f)
        , m_frameClock()
        , m_enableSpecialEffects(false)
        , m_videoMode(2880u, 1800u, sf::VideoMode::getDesktopMode().bitsPerPixel)
        , m_window(m_videoMode, "bogeyman", sf::Style::Default)
        // , m_bloomWindow(m_window)
        , m_map(m_mapStrings)
        , m_board(m_window.getSize(), m_map)
        , m_resources(mediaDirPath)
        , m_random()
        , m_soundPlayer(m_random, (mediaDirPath / "sfx").string())
        , m_animationPlayer(m_random, (mediaDirPath / "animation").string())
        , m_context(m_resources, m_map, m_board, m_random, m_soundPlayer, m_animationPlayer)
        , m_isPlayersTurn(true)
        , m_lastUnhandledKeyEvent(makeInvalidEvent())
    {
        // m_window.setFramerateLimit(60);

        if (m_enableSpecialEffects)
        {
            m_soundPlayer.loadAll();
            m_animationPlayer.loadAll();
        }

        reset();
    }

    void GameCoordinator::reset()
    {
        m_context.is_game_over = false;

        m_isPlayersTurn = true;
        m_lastUnhandledKeyEvent = makeInvalidEvent();

        m_enableSpecialEffects = false;
        // m_bloomWindow.isEnabled(m_enableSpecialEffects);
        // std::cout << "Special Effets " << ((m_enableSpecialEffects) ? "ON" : "OFF") << std::endl;

        m_simTimeMult = 1.0f;
        m_frameClock.restart();

        m_soundPlayer.stopAll();
        m_animationPlayer.stopAll();

        m_board.reset();
        resetToStartingMapPieces();
    }

    void GameCoordinator::resetToStartingMapPieces()
    {
        m_board.pieces().clear();

        const BoardPosVec_t positions = m_map.allValidPositions();
        M_ASSERT_OR_THROW(!positions.empty());

        for (const BoardPos_t & boardPos : positions)
        {
            M_ASSERT_OR_THROW(m_map.isPosValid(boardPos));
            M_ASSERT_OR_THROW(!m_board.isAnyPieceAt(boardPos));

            const char mapChar = m_map.charAt(boardPos);
            M_ASSERT_OR_THROW('\0' != mapChar);

            const Piece::Enum pieceEnum{ Piece::mapCharTo(mapChar) };
            if (pieceEnum == Piece::Count)
            {
                continue;
            }

            m_board.pieces().push_back(makePiece(pieceEnum, boardPos));
            M_ASSERT_OR_THROW(m_board.pieces().back()->isAlive());
        }

        // sort in display order so the player and villan are never covered up
        auto & pieces = m_board.pieces();

        std::sort(
            std::begin(pieces),
            std::end(pieces),
            [&](const IPieceUPtr_t & left, const IPieceUPtr_t & right) {
                return (left->piece() < right->piece());
            });
    }

    IPieceUPtr_t
        GameCoordinator::makePiece(const Piece::Enum piece, const BoardPos_t & boardPos) const
    {
        M_ASSERT_OR_THROW(Piece::Count != piece);
        M_ASSERT_OR_THROW(m_map.isPosValid(boardPos));
        M_ASSERT_OR_THROW(!m_board.isAnyPieceAt(boardPos));

        switch (piece)
        {
            case Piece::Player: return std::make_unique<PlayerPiece>(m_context, boardPos);
            case Piece::Villan: return std::make_unique<VillanPiece>(m_context, boardPos);
            case Piece::Wall: return std::make_unique<WallPiece>(m_context, boardPos);
            case Piece::Door: return std::make_unique<DoorPiece>(m_context, boardPos);

            case Piece::Count:
            default:
                throw std::runtime_error(
                    "GameCoordinator::makePiece(piece_enum=" + Piece::name(piece) +
                    ") invalid enum.");
        }
    }

    void GameCoordinator::run()
    {
        std::vector<BoardPos_t> occupiedPositions;
        occupiedPositions.reserve(1000);

        sf::Clock fpsClock;
        float frameCounter = 0.0f;

        handleEvents();

        while (m_window.isOpen())
        {
            if (m_context.is_testing_enabled)
            {
                if (m_context.is_game_over)
                {
                    // draw();
                    reset();
                }
            }

            handleMovingPieces();

            {
                bool areAnyDead = false;
                auto & pieces = m_board.pieces();
                for (const auto & piece : pieces)
                {
                    if (piece->isAlive())
                    {
                        occupiedPositions.push_back(piece->boardPos());
                    }
                    else
                    {
                        areAnyDead = true;
                    }
                }

                if (areAnyDead)
                {
                    const auto beginIter = std::begin(pieces);
                    const auto endIter = std::end(pieces);

                    pieces.erase(
                        std::remove_if(
                            beginIter,
                            endIter,
                            [](const auto & piece) { return !piece->isAlive(); }),
                        endIter);
                }

                std::sort(std::begin(occupiedPositions), std::end(occupiedPositions));

                M_ASSERT_OR_THROW(
                    std::adjacent_find(
                        std::begin(occupiedPositions), std::end(occupiedPositions)) ==
                    std::end(occupiedPositions));

                occupiedPositions.clear();
            }

            // update(m_frameClock.restart().asSeconds() * m_simTimeMult);
            // draw();

            frameCounter += 1.0f;
            const float fpsTimeElapsedSec = fpsClock.getElapsedTime().asSeconds();
            if (fpsTimeElapsedSec > 1.0f)
            {
                std::cout << "fps=" << int(frameCounter) << "  " << playerWinCount << '/'
                          << villanWinCount << std::endl;

                fpsClock.restart();
                frameCounter = 0.0f;
                handleEvents();
                draw();
            }
        }

        reset();
    }

    void GameCoordinator::handleMovingPieces()
    {
        if (m_isPlayersTurn)
        {
            if (m_context.is_testing_enabled)
            {
                m_lastUnhandledKeyEvent = sf::Event();
                m_lastUnhandledKeyEvent.type = sf::Event::EventType::KeyPressed;
                m_lastUnhandledKeyEvent.key = sf::Event::KeyEvent{};
                m_lastUnhandledKeyEvent.key.code = sf::Keyboard::Left; // avoid sf::Keyboard::Uknown
            }

            if (!isEventValidKeypress(m_lastUnhandledKeyEvent))
            {
                return;
            }

            static BoardPos_t doorPos = { -1, -1 };

            std::size_t deadPlayerCount = 0;
            std::size_t livingPlayerCount = 0;
            for (IPieceUPtr_t & piece : m_board.pieces())
            {
                if (piece->piece() == Piece::Door)
                {
                    doorPos = piece->boardPos();
                }

                if (!piece->isAlive() || (piece->piece() != Piece::Player))
                {
                    continue;
                }

                // if (piece->takeTurn(m_context, m_lastUnhandledKeyEvent))
                piece->takeTurn(m_context, m_lastUnhandledKeyEvent);
                {
                    m_isPlayersTurn = false;

                    if (piece->boardPos() == doorPos)
                    {
                        ++playerWinCount;
                        piece->kill();
                    }

                    if (piece->isAlive())
                    {
                        ++livingPlayerCount;
                    }
                    else
                    {
                        ++deadPlayerCount;
                    }
                }
            }

            if (0 == livingPlayerCount)
            {
                // std::cout << "All " << deadPlayerCount << "Players are dead:" << std::endl;
                for (IPieceUPtr_t & piece : m_board.pieces())
                {
                    // if (!piece->isAlive() || (piece->piece() == Piece::Player) ||
                    //    (piece->piece() == Piece::Villan))
                    //{
                    M_ASSERT_OR_THROW(!piece->isAlive() == (piece->piece() == Piece::Player));

                    // std::cout << "\n  " << Piece::name(piece->piece()) << ' '
                    //          << ((piece->isAlive()) ? "alive" : "DEAD");
                    //}
                }

                // std::cout << std::endl << std::endl;

                m_isPlayersTurn = false;
                m_lastUnhandledKeyEvent = makeInvalidEvent();
                ++villanWinCount;
                draw();
                reset();
                return;
            }
            else
            {
            }

            if (!m_isPlayersTurn)
            {
                m_lastUnhandledKeyEvent = makeInvalidEvent();

                if (m_context.is_game_over)
                {
                    ++playerWinCount;
                }
            }
        }
        else
        {
            for (IPieceUPtr_t & piece : m_board.pieces())
            {
                if (!piece->isAlive() || (piece->piece() != Piece::Villan))
                {
                    continue;
                }

                if (piece->takeTurn(m_context))
                {
                    // if (m_context.is_game_over)
                    //{
                    //    ++villanWinCount;
                    //}

                    m_isPlayersTurn = true;
                }
            }
        }
    }

    void GameCoordinator::handleEvents()
    {
        sf::Event event;
        while (m_window.isOpen() && m_window.pollEvent(event))
        {
            if (!handleEvent(event))
            {
                break;
            }
        }
    }

    bool GameCoordinator::handleEvent(const sf::Event & event)
    {
        if ((event.type == sf::Event::Closed) || (event.type == sf::Event::MouseButtonPressed) ||
            m_context.is_game_over)
        {
            m_window.close();
            return false;
        }

        // at this point only keypresses are valid/handled
        if (sf::Event::KeyPressed != event.type)
        {
            return true;
        }

        if (sf::Keyboard::Escape == event.key.code)
        {
            m_window.close();
            return false;
        }

        if (sf::Keyboard::R == event.key.code)
        {
            reset();
            return false;
        }

        if (sf::Keyboard::E == event.key.code)
        {
            m_enableSpecialEffects = !m_enableSpecialEffects;
            // m_bloomWindow.isEnabled(m_enableSpecialEffects);
            //
            // std::cout << "Special Effets turned " << ((m_enableSpecialEffects) ? "ON" : "OFF")
            //          << std::endl;

            return true;
        }

        if (sf::Keyboard::B == event.key.code)
        {
            // m_bloomWindow.isEnabled(!m_bloomWindow.isEnabled());
            return true;
        }

        m_lastUnhandledKeyEvent = event;
        return false;
    }

    void GameCoordinator::update(const float elapsedTimeSec)
    {
        for (IPieceUPtr_t & piece : m_context.board.pieces())
        {
            if (!piece->isAlive())
            {
                continue;
            }

            piece->update(m_context, elapsedTimeSec);

            if (m_context.is_game_over)
            {
                break;
            }
        }

        if (m_enableSpecialEffects)
        {
            m_animationPlayer.update(elapsedTimeSec);
        }
    }

    void GameCoordinator::draw()
    {
        m_window.clear();

        for (const IPieceUPtr_t & piece : m_context.board.pieces())
        {
            if (!piece->isAlive())
            {
                continue;
            }

            m_window.draw(*piece);
        }

        // if (m_enableSpecialEffects)
        //{
        //    m_animationPlayer.draw(m_bloomWindow);
        //}

        m_window.display();
    }
} // namespace boardgame