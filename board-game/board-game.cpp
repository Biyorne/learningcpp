// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "board-game.hpp"

#include "scoped-board-position.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace boardgame
{
    BoardGame::BoardGame(const std::string & mediaDirPath)
        : m_mediaDirPath(mediaDirPath)
        , m_enableSpecialEffects(true)
        , m_simTimeMult(1.0f)
        , m_frameClock()
        , m_videoMode(1600u, 1200u, sf::VideoMode::getDesktopMode().bitsPerPixel)
        , m_window(m_videoMode, "bogeyman", sf::Style::Default)
        , m_bloomWindow(m_window)
        , m_resources(mediaDirPath)
        , m_random()
        , m_soundPlayer(m_random, (mediaDirPath + "/sfx"))
        , m_animationPlayer(m_random, (mediaDirPath + "/animation"))
        , m_display(m_window.getSize())
        , m_collider()
        , m_pieces()
        , m_context(
              m_mediaDirPath,
              m_display,
              m_resources,
              m_random,
              m_soundPlayer,
              m_animationPlayer,
              m_pieces)
    {
        // this grows a static vector that needs to NOT be empty
        ScopedBoardPosition::reset(m_context);

        m_window.setFramerateLimit(60);

        m_pieces.reserve(m_display.cell_count);

        if (m_enableSpecialEffects)
        {
            m_soundPlayer.loadAll();
            m_animationPlayer.loadAll();
        }

        reset();
    }

    void BoardGame::reset()
    {
        m_enableSpecialEffects = false;
        m_bloomWindow.isEnabled(false);

        m_simTimeMult = 1.0f;
        m_frameClock.restart();

        m_soundPlayer.stopAll();
        m_animationPlayer.stopAll();

        // piece vector must be clear()  before ScopedBoardPosition::reset()
        m_pieces.clear();
        ScopedBoardPosition::reset(m_context);

        loadMap();
    }

    void BoardGame::loadMap()
    {
        const DisplayConstants & display{ m_display };

        for (int horiz(0); horiz < display.cell_countsI.x; ++horiz)
        {
            m_pieces.push_back(
                std::move(std::make_unique<WallPiece>(m_context, BoardPos_t(horiz, 0))));

            m_pieces.push_back(std::move(std::make_unique<WallPiece>(
                m_context, BoardPos_t(horiz, (display.cell_countsI.y - 1)))));
        }

        for (int vert(1); vert < (display.cell_countsI.y - 1); ++vert)
        {
            m_pieces.push_back(
                std::move(std::make_unique<WallPiece>(m_context, BoardPos_t(0, vert))));

            m_pieces.push_back(std::move(std::make_unique<WallPiece>(
                m_context, BoardPos_t((display.cell_countsI.x - 2), vert))));
        }

        for (std::size_t i(0); i < 6; ++i)
        {
            m_pieces.push_back(std::move(std::make_unique<ChildPiece>(m_context)));
        }

        m_pieces.push_back(std::move(std::make_unique<PlayerPiece>(m_context)));
        m_pieces.push_back(std::move(std::make_unique<DemonPiece>(m_context)));
    }

    void BoardGame::run()
    {
        while (m_window.isOpen())
        {
            handleEvents();

            const float elapsedSimTimeSec{ m_frameClock.restart().asSeconds() * m_simTimeMult };

            update(elapsedSimTimeSec);
            draw();
        }

        reset();
    }

    void BoardGame::handleEvents()
    {
        sf::Event event;
        while (m_window.isOpen() && m_window.pollEvent(event))
        {
            handleEvent(event);
        }
    }

    void BoardGame::handleEvent(const sf::Event & event)
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
            return;
        }

        // at this point only keypresses are valid/handled
        if (sf::Event::KeyPressed != event.type)
        {
            return;
        }

        if (sf::Keyboard::Escape == event.key.code)
        {
            m_window.close();
        }
        else if (sf::Keyboard::R == event.key.code)
        {
            reset();
        }
        else if (sf::Keyboard::E == event.key.code)
        {
            m_enableSpecialEffects = !m_enableSpecialEffects;

            if (!m_enableSpecialEffects)
            {
                m_bloomWindow.isEnabled(false);
            }
        }
        else if (sf::Keyboard::B == event.key.code)
        {
            m_bloomWindow.isEnabled(!m_bloomWindow.isEnabled());
        }
        else if (sf::Keyboard::Up == event.key.code)
        {
            m_context.player_pos_change = { 0, -1 };
        }
        else if (sf::Keyboard::Down == event.key.code)
        {
            m_context.player_pos_change = { 0, 1 };
        }
        else if (sf::Keyboard::Right == event.key.code)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                m_simTimeMult *= 1.1f;

                if (m_simTimeMult > 100.0f)
                {
                    m_simTimeMult = 100.0f;
                }
            }
            else
            {
                m_context.player_pos_change = { 1, 0 };
            }
        }
        else if (sf::Keyboard::Left == event.key.code)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            {
                m_simTimeMult *= 0.9f;

                if (m_simTimeMult < 0.1f)
                {
                    m_simTimeMult = 0.1f;
                }
            }
            else
            {
                m_context.player_pos_change = { -1, 0 };
            }
        }
    }

    void BoardGame::update(const float elapsedSec)
    {
        for (PiceUPtr_t & piece : m_pieces)
        {
            const BoardPos_t newPos{ piece->update(m_context, elapsedSec) };

            // bail if the piece does not want to move
            if (piece->boardPos() == newPos)
            {
                continue;
            }
            else
            {
                // if the player moved, reset the move vector
                if (piece->piece() == Piece::Hero)
                {
                    m_context.player_pos_change = { 0, 0 };
                }

                m_collider.collide(m_context, (*piece), newPos);
            }
        }

        if (m_enableSpecialEffects)
        {
            m_animationPlayer.update(elapsedSec);
        }
    }

    void BoardGame::draw()
    {
        m_bloomWindow.clear(sf::Color(64, 64, 64));

        for (const PiceUPtr_t & piece : m_pieces)
        {
            m_bloomWindow.draw(*piece);
        }

        if (m_enableSpecialEffects)
        {
            m_animationPlayer.draw(m_window, {});
        }

        m_window.display();
    }
} // namespace boardgame