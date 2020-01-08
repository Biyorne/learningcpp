// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// game-coordinator.cpp
//
#include "game-coordinator.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace boardgame
{
    GameCoordinator::GameCoordinator(const std::string & mediaDirPath)
        : m_enableSpecialEffects(false)
        , m_resources(mediaDirPath)
        , m_simTimeMult(1.0f)
        , m_frameClock()
        , m_videoMode(2880u, 1800u, sf::VideoMode::getDesktopMode().bitsPerPixel)
        , m_window(m_videoMode, "bogeyman", sf::Style::Default)
        , m_bloomWindow(m_window)
        , m_random()
        , m_soundPlayer(m_random, (mediaDirPath + "/sfx"))
        , m_animationPlayer(m_random, (mediaDirPath + "/animation"))
        , m_board(m_window.getSize())
        , m_collider()
        , m_context(m_resources, m_board, m_random, m_soundPlayer, m_animationPlayer)
    {
        m_window.setFramerateLimit(60);

        m_bloomWindow.isEnabled(m_enableSpecialEffects);

        if (m_enableSpecialEffects)
        {
            m_soundPlayer.loadAll();
            m_animationPlayer.loadAll();
        }

        reset();
    }

    void GameCoordinator::reset()
    {
        m_enableSpecialEffects = false;
        m_bloomWindow.isEnabled(m_enableSpecialEffects);

        m_simTimeMult = 1.0f;
        m_frameClock.restart();

        m_soundPlayer.stopAll();
        m_animationPlayer.stopAll();

        // piece vector must be clear()  before ScopedBoardPosition::reset()
        m_board.reset();

        loadMap();
    }

    void GameCoordinator::loadMap()
    {
        const Board & display{ m_board };

        for (int horiz(0); horiz < display.cellCounts<int>().x; ++horiz)
        {
            m_board.walls.push_back(std::make_unique<WallPiece>(m_context, BoardPos_t(horiz, 0)));

            m_board.walls.push_back(std::make_unique<WallPiece>(
                m_context, BoardPos_t(horiz, (display.cellCounts<int>().y - 1))));
        }

        for (int vert(1); vert < (display.cellCounts<int>().y - 1); ++vert)
        {
            m_board.walls.push_back(std::make_unique<WallPiece>(m_context, BoardPos_t(0, vert)));

            m_board.walls.push_back(std::make_unique<WallPiece>(
                m_context, BoardPos_t((display.cellCounts<int>().x - 1), vert)));
        }

        for (std::size_t i(0); i < 3; ++i)
        {
            m_board.kids.push_back(std::make_unique<ChildPiece>(m_context));
        }

        m_board.players.push_back(std::make_unique<PlayerPiece>(m_context));
        m_board.demons.push_back(std::make_unique<DemonPiece>(m_context));
    }

    void GameCoordinator::run()
    {
        while (m_window.isOpen())
        {
            handleEvents();
            update(m_frameClock.restart().asSeconds() * m_simTimeMult);
            draw();
        }

        reset();
    }

    void GameCoordinator::handleEvents()
    {
        sf::Event event;
        while (m_window.isOpen() && m_window.pollEvent(event))
        {
            handleEvent(event);
        }
    }

    void GameCoordinator::handleEvent(const sf::Event & event)
    {
        if ((event.type == sf::Event::Closed) || (event.type == sf::Event::MouseButtonPressed))
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
            m_bloomWindow.isEnabled(m_enableSpecialEffects);
        }
        else if (sf::Keyboard::B == event.key.code)
        {
            m_bloomWindow.isEnabled(!m_bloomWindow.isEnabled());
        }
        else if (sf::Keyboard::Up == event.key.code)
        {
            for (PlayerUPtr_t & player : m_context.board.players)
            {
                player->nextMovePos({ 0, -1 });
            }
        }
        else if (sf::Keyboard::Down == event.key.code)
        {
            for (PlayerUPtr_t & player : m_context.board.players)
            {
                player->nextMovePos({ 0, 1 });
            }
        }
        else if (sf::Keyboard::Right == event.key.code)
        {
            for (PlayerUPtr_t & player : m_context.board.players)
            {
                player->nextMovePos({ 1, 0 });
            }
        }
        else if (sf::Keyboard::Left == event.key.code)
        {
            for (PlayerUPtr_t & player : m_context.board.players)
            {
                player->nextMovePos({ -1, 0 });
            }
        }
        else if ((sf::Keyboard::Equal == event.key.code) || (sf::Keyboard::Add == event.key.code))
        {
            m_simTimeMult *= 1.1f;

            if (m_simTimeMult > 100.0f)
            {
                m_simTimeMult = 100.0f;
            }
        }
        else if (sf::Keyboard::Dash == event.key.code)
        {
            m_simTimeMult *= 0.9f;

            if (m_simTimeMult < 0.1f)
            {
                m_simTimeMult = 0.1f;
            }
        }
    }

    void GameCoordinator::update(const float elapsedTimeSec)
    {
        // clang-format off
        for (auto & uptr : m_board.walls)   update(elapsedTimeSec, *uptr);
        for (auto & uptr : m_board.players) update(elapsedTimeSec, *uptr);
        for (auto & uptr : m_board.kids)    update(elapsedTimeSec, *uptr);
        for (auto & uptr : m_board.demons)  update(elapsedTimeSec, *uptr);
        // clang-format on

        if (m_enableSpecialEffects)
        {
            m_animationPlayer.update(elapsedTimeSec);
        }
    }

    void GameCoordinator::draw()
    {
        m_bloomWindow.clear(sf::Color(64, 64, 64));

        util::drawRectangleVerts(
            m_bloomWindow.renderTarget(), m_context.board.board_region, sf::Color(0, 0, 255, 64));

        util::drawRectangleVerts(
            m_bloomWindow.renderTarget(), m_context.board.status_region, sf::Color(0, 255, 0, 64));

        // for (const Cell & cell : m_board.cells)
        //{
        //    sf::Color color = m_random.from({ sf::Color::Blue,
        //                                      sf::Color::Cyan,
        //                                      sf::Color::Green,
        //                                      sf::Color::Magenta,
        //                                      sf::Color::Red,
        //                                      sf::Color::White,
        //                                      sf::Color::Yellow });
        //
        //    color.a = 127;
        //
        //    util::drawRectangleShape(m_bloomWindow.renderTarget(), cell.rect, color);
        //}

        // clang-format off
        for (const auto & uptr : m_board.walls)   m_bloomWindow.draw(*uptr);
        for (const auto & uptr : m_board.players) m_bloomWindow.draw(*uptr);
        for (const auto & uptr : m_board.kids)    m_bloomWindow.draw(*uptr);
        for (const auto & uptr : m_board.demons)  m_bloomWindow.draw(*uptr);
        // clang-format on

        // if (m_enableSpecialEffects)
        //{
        //    m_animationPlayer.draw(m_window, {});
        //}

        m_window.display();
    }
} // namespace boardgame