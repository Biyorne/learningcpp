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
    SimpleGameCoordinator::SimpleGameCoordinator()
        : m_map()
        , m_config()
        , m_window()
        , m_random()
        , m_soundPlayer(m_random)
        , m_animationPlayer(m_random)
        , m_layout()
        , m_media()
        , m_board()
        , m_game()
        , m_context(
              m_game,
              m_map,
              m_config,
              m_layout,
              m_media,
              m_board,
              m_random,
              m_soundPlayer,
              m_animationPlayer)
    {
        runFullCheck();
    }

    void SimpleGameCoordinator::reset(const GameConfig & config, const Map_t & map)
    {
        runFullCheck();

        m_config = config;
        M_CHECK_SS(std::filesystem::exists(m_config.media_dir_path), m_config.media_dir_path);
        M_CHECK_SS(std::filesystem::is_directory(m_config.media_dir_path), m_config.media_dir_path);

        m_window.close();
        openWindow();
        m_window.setFramerateLimit(m_config.frame_rate_limit);

        m_soundPlayer.volume(0.0f);
        m_soundPlayer.stopAll();
        m_soundPlayer.setMediaPath((m_config.media_dir_path / "sfx").string());
        m_soundPlayer.volume(75.0f);

        m_animationPlayer.stopAll();
        m_animationPlayer.setMediaPath((m_config.media_dir_path / "animation").string());

        m_media.setup(m_config);

        switchToMap(map);

        runFullCheck();
    }

    void SimpleGameCoordinator::switchToMap(const Map_t & map)
    {
        runFullCheck();

        m_map = map;

        M_CHECK_SS(
            (!m_map.empty() && !m_map.front().empty()),
            "Map is invalid.  Map either had no row strings or the first row string was empty.");

        m_layout.setup(m_map, m_config);
        m_board.reset(m_context);
        m_game.reset();

        runFullCheck();
    }

    void SimpleGameCoordinator::openWindow()
    {
        const auto style{ (m_config.is_fullscreen) ? sf::Style::Fullscreen : sf::Style::Default };

        m_window.create(m_config.video_mode, m_config.game_name, style);

        M_CHECK_SS(
            m_window.isOpen(),
            "Failed to open the window specified: " << m_config.windowSize<int>() << ":"
                                                    << m_config.video_mode.bitsPerPixel);

        // verify the window size is what was specified/expected,
        // otherwise all the size/positions calculations will be wrong
        const sf::Vector2u windowExpectedSize{ m_config.windowSize<unsigned>() };

        const sf::Vector2u windowActualSize{ m_window.getSize() };

        M_CHECK_LOG_SS(
            (windowActualSize == windowExpectedSize),
            "Failed to create a window at the resolution specified: "
                << windowExpectedSize
                << ".  Strangely, a window did open, just at a different resolution: "
                << windowActualSize << ".  So...meh.  Let's just run with it.");

        m_config.video_mode.width = windowActualSize.x;
        m_config.video_mode.height = windowActualSize.y;
        m_config.video_mode.bitsPerPixel = m_window.getSettings().depthBits;
    }

    void SimpleGameCoordinator::run()
    {
        runFullCheck();

        sf::Clock frameClock;

        while (m_window.isOpen() && !m_game.isGameOver())
        {
            handleEvents();
            update(frameClock.restart().asSeconds());
            draw();
        }

        printFinalStatusToConsole();

        runFullCheck();
    }

    void SimpleGameCoordinator::printFinalStatusToConsole()
    {
        if (m_game.isGameOver())
        {
            std::cout << "Game Over.   You " << ((m_game.didPlayerWin()) ? "WON!" : "LOST  (loser)")
                      << std::endl;
        }
        else
        {
            std::cout << "Application shutdown before game was over." << std::endl;
        }

        std::cout << "Final score = " << m_game.score() << std::endl;
    }

    void SimpleGameCoordinator::handleEvents()
    {
        sf::Event event;
        while (m_window.isOpen() && !m_game.isGameOver() && m_window.pollEvent(event))
        {
            handleEvent(event);
        }
    }

    bool SimpleGameCoordinator::handleExitEvents(const sf::Event & event)
    {
        if (sf::Event::Closed == event.type)
        {
            m_window.close();
            return true;
        }

        if (sf::Event::KeyPressed == event.type)
        {
            if ((sf::Keyboard::Q == event.key.code) || (sf::Keyboard::Escape == event.key.code))
            {
                m_game.endGame(false);
                return true;
            }

            // if (sf::Keyboard::R == event.key.code)
            //{
            //    reset(m_config, m_map);
            //    return true;
            //}
        }

        return false;
    }

    void SimpleGameCoordinator::handleEvent(const sf::Event & event)
    {
        if (SimpleGameCoordinator::handleExitEvents(event))
        {
            return;
        }

        for (const IPieceUPtr_t & piece : m_board.pieces())
        {
            piece->handleEvent(m_context, event);
        }
    }

    void SimpleGameCoordinator::update(const float elapsedTimeSec)
    {
        for (const IPieceUPtr_t & piece : m_board.pieces())
        {
            piece->update(m_context, elapsedTimeSec);
        }
    }

    void SimpleGameCoordinator::draw()
    {
        m_window.clear(m_config.background_color);

        for (const IPieceUPtr_t & piece : m_board.pieces())
        {
            m_window.draw(*piece);
        }

        m_window.display();
    }

    //

    void TestingFrenzyGame::reset(const GameConfig & configOld, const Map_t & map)
    {
        GameConfig configNew{ configOld };
        configNew.game_name = "Testing Frenzy";
        configNew.background_color = sf::Color(32, 26, 26);

        SimpleGameCoordinator::reset(configNew, map);
        m_soundPlayer.volume(0.0f);
    }

    void TestingFrenzyGame::update(const float elapsedTimeSec)
    {
        runFullCheck();

        m_frameCount += 1.0f;

        if (m_oneSecondClock.getElapsedTime().asSeconds() > 1.0f)
        {
            std::cout << "fps=" << (m_frameCount / m_oneSecondClock.getElapsedTime().asSeconds())
                      << std::endl;

            m_frameCount = 0.0f;
            m_oneSecondClock.restart();
            SimpleGameCoordinator::draw();
        }

        m_timeUntilPopulationChecks -= elapsedTimeSec;
        if (m_timeUntilPopulationChecks < 0.0f)
        {
            if (m_random.boolean())
            {
                m_timeUntilPopulationChecks = m_random.zeroTo(0.25f);
            }
            else
            {
                m_timeUntilPopulationChecks = 0.0f;
            }

            const auto & pieces{ m_board.pieces() };

            const std::size_t eatersCurrentCount = std::count_if(
                std::begin(pieces), std::end(pieces), [&](const IPieceUPtr_t & piece) {
                    return (piece->piece() == Piece::Eater);
                });

            for (std::size_t i(eatersCurrentCount); i < m_eaterCount; ++i)
            {
                m_board.addPieceAtRandomFreePos(m_context, Piece::Eater);
            }

            const std::size_t foodCurrentCount = std::count_if(
                std::begin(pieces), std::end(pieces), [&](const IPieceUPtr_t & piece) {
                    return (piece->piece() == Piece::Food);
                });

            for (std::size_t i(foodCurrentCount); i < m_foodCount; ++i)
            {
                m_board.addPieceAtRandomFreePos(m_context, Piece::Food);
            }

            SimpleGameCoordinator::draw();
            return;
        }

        m_mapResizeTimeRemainingSec -= elapsedTimeSec;
        if (m_mapResizeTimeRemainingSec < 0.0f)
        {
            m_mapResizeTimeRemainingSec = m_random.fromTo(1.0f, 10.0f);
            restartWithNewMapSize();

            SimpleGameCoordinator::draw();
            return;
        }

        for (const IPieceUPtr_t & piece : m_board.pieces())
        {
            if (piece->piece() != Piece::Eater)
            {
                continue;
            }

            piece->takeTurn(m_context);
        }
    }

    void SimpleGameCoordinator::runFullCheck()
    {
        static std::map<BoardPos_t, std::pair<std::size_t, std::string>> allTakenPositions;
        allTakenPositions.clear();

        static std::map<IPiece *, std::pair<std::size_t, std::string>> allTakenPointers;
        allTakenPointers.clear();

        for (const IPieceUPtr_t & piece : m_board.pieces())
        {
            M_CHECK_SS((piece->piece() != Piece::Count), piece->piece());

            const auto pos{ piece->position() };
            const auto ptr{ piece.get() };

            M_CHECK_SS(m_layout.isPositionValid((pos)), pos);

            {
                auto & [count, str] = allTakenPositions[pos];

                std::ostringstream ss;

                ss << "\n pos=" << pos << ", type=" << piece->piece()
                   << ", bounds=" << piece->bounds() << ", ptr=" << ptr << ", count=" << count;

                str += ss.str();

                ++count;
            }

            {
                auto & [count, str] = allTakenPointers[ptr];

                std::ostringstream ss;

                ss << "\n pos=" << pos << ", type=" << piece->piece()
                   << ", bounds=" << piece->bounds() << ", ptr=" << ptr << ", count=" << count;

                str += ss.str();

                ++count;
            }
        }

        std::size_t duplicateCount{ 0 };
        {
            for (const auto & [pos, pair] : allTakenPositions)
            {
                const auto & [count, str] = pair;

                if (count > 1)
                {
                    std::cout << "MULTIPLE PIECES (z" << count << ") IN THE SAME BOARD POS (" << pos
                              << "):" << str << std::endl
                              << std::endl;

                    ++duplicateCount;
                }
            }
        }

        {
            for (const auto & [ptr, pair] : allTakenPointers)
            {
                const auto & [count, str] = pair;

                if (count > 1)
                {
                    std::cout << "DUPLICATE POINTERS (x" << count << ") ON THE BOARD (" << ptr
                              << "):" << str << std::endl
                              << std::endl;

                    ++duplicateCount;
                }
            }
        }

        M_CHECK_SS((0 == duplicateCount), duplicateCount);
    }

    void TestingFrenzyGame::restartWithNewMapSize()
    {
        m_timeUntilPopulationChecks = m_random.zeroTo(0.25f);

        const Map_t newMap{ makeMapOfSize(
            m_random.fromTo(2_st, 7_st), m_random.fromTo(2_st, 7_st)) };

        SimpleGameCoordinator::switchToMap(newMap);

        std::size_t availablePosCount{ 0 };
        for (const std::string & row : m_map)
        {
            availablePosCount += std::count(std::begin(row), std::end(row), ' ');
        }

        // determine how many things will be in the arena
        m_eaterCount = m_random.zeroTo(newMap.size() * m_random.fromTo(1_st, 8_st));
        if (m_random.ratio() < 0.025f)
        {
            std::cout << "Rare zero out of EATERS" << std::endl;
            m_eaterCount = 0;
        }

        m_foodCount = m_random.zeroTo(newMap.front().size() * m_random.fromTo(1_st, 8_st));
        if (m_random.ratio() < 0.025f)
        {
            std::cout << "Rare zero out of FOOD" << std::endl;
            m_foodCount = 0;
        }

        m_obstacleCount = m_random.zeroTo(
            (newMap.size() * newMap.front().size()) /
            m_random.fromTo((newMap.size() / 4), newMap.size()));

        if (m_random.ratio() < 0.025f)
        {
            std::cout << "Rare zero out of OBSTACLES" << std::endl;
            m_obstacleCount = 0;
        }

        auto calcPopulationCount = [&]() { return (m_eaterCount + m_foodCount + m_obstacleCount); };

        std::size_t populationCount{ calcPopulationCount() };

        if (populationCount > availablePosCount)
        {
            std::cout << "*** population count " << populationCount
                      << " too high for avaliable : " << availablePosCount;

            m_eaterCount /= 2;
            m_foodCount /= 2;
            m_obstacleCount /= 2;

            populationCount = calcPopulationCount();

            std::cout << ", so halfed all and now pop count is " << populationCount << std::endl;
        }

        if (m_random.ratio() < 0.01f)
        {
            std::cout << "Random full!";

            while (calcPopulationCount() < availablePosCount)
            {
                std::cout << '.';
                const int whichToSpawn{ m_random.zeroTo(2) };

                switch (whichToSpawn)
                {
                    case 0: {
                        ++m_eaterCount;
                        break;
                    }
                    case 1: {
                        ++m_foodCount;
                        break;
                    }
                    case 2:
                    default: {
                        ++m_obstacleCount;
                        break;
                    }
                }
            }

            std::cout << std::endl;
        }
        else if (m_random.ratio() < 0.01f)
        {
            std::cout << "Random Empty!" << std::endl;
            m_eaterCount = 0;
            m_foodCount = 0;
            m_obstacleCount = 0;
        }

        populationCount = calcPopulationCount();

        std::cout << "\nChanging Map Size: " << m_layout.cellCounts()
                  << " (total=" << m_layout.cellCountTotal() << ")";

        std::cout << "\n eater_count      = " << m_eaterCount;
        std::cout << "\n food_count       = " << m_foodCount;
        std::cout << "\n obstacle_count   = " << m_obstacleCount;
        std::cout << "\n population_count = " << populationCount;
        std::cout << "\n percent_full     = "
                  << 100.0f * (static_cast<float>(populationCount) /
                               static_cast<float>(m_layout.cellCountTotal()))
                  << "%";

        std::cout << std::endl << std::endl;

        auto freePositions{ m_layout.allValidPositions() };

        freePositions.erase(
            std::remove_if(
                std::begin(freePositions),
                std::end(freePositions),
                [&](const BoardPos_t & pos) {
                    const char mapChar{ m_map.at(pos.y).at(pos.x) };
                    return (pieceToMapChar(Piece::Wall) == mapChar);
                }),
            std::end(freePositions));

        std::cout << "Free positions started at " << m_layout.allValidPositions().size()
                  << ", but after removing walls it was " << freePositions.size() << std::endl
                  << std::endl;

        // M_CHECK_SS(
        //    (freePositions.size() == ((m_map.size() * 2) + (m_map.front().size() * 2) -
        //    2)), "freePositions.size()=" << freePositions.size() << ", vs="
        //                            << ((m_map.size() * 2) + (m_map.front().size() * 2) -
        //                            2));

        m_random.shuffle(freePositions);

        for (std::size_t i(0); ((i < m_obstacleCount) && !freePositions.empty()); ++i)
        {
            m_board.addPiece(m_context, Piece::Wall, freePositions.back());
            freePositions.pop_back();
        }

        for (std::size_t i(0); ((i < m_foodCount) && !freePositions.empty()); ++i)
        {
            m_board.addPiece(m_context, Piece::Food, freePositions.back());
            freePositions.pop_back();
        }

        for (std::size_t i(0); ((i < m_eaterCount) && !freePositions.empty()); ++i)
        {
            m_board.addPiece(m_context, Piece::Eater, freePositions.back());
            freePositions.pop_back();
        }

        std::cout << "final avail count=" << freePositions.size() << std::endl;
    }

    void TestingFrenzyGame::handleEvent(const sf::Event & event)
    {
        if (SimpleGameCoordinator::handleExitEvents(event))
        {
            runFullCheck();
            return;
        }
    }

    void TestingFrenzyGame::draw()
    {
        // m_window.clear(m_config.background_color);
        //
        // for (const IPieceUPtr_t& piece : m_board.pieces())
        //{
        //    m_window.draw(*piece);
        //}
        //
        // m_window.display();
    }

    Map_t TestingFrenzyGame::makeMapOfSize(std::size_t horiz, std::size_t vert)
    {
        horiz = std::clamp((horiz * 10), 10_st, 30_st);
        vert = std::clamp((vert * 10), 10_st, 30_st);

        // important that they all start on (or there, or whatever)
        Map_t map;

        for (std::size_t v(0); v < vert; ++v)
        {
            std::string row;
            if ((0 == v) || ((vert - 1) == v))
            {
                row = std::string(horiz, pieceToMapChar(Piece::Wall));
            }
            else
            {
                row = std::string(horiz, pieceToMapChar(Piece::Count));
                row.front() = pieceToMapChar(Piece::Wall);
                row.back() = pieceToMapChar(Piece::Wall);
            }

            map.push_back(row);
        }

        M_CHECK(map.size() == vert);
        M_CHECK(map.front().size() == horiz);
        M_CHECK(map.back().size() == horiz);

        return map;
    }

    void TestingFrenzyGame::switchToMap(const Map_t &) { restartWithNewMapSize(); }
} // namespace boardgame