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
    {}

    void SimpleGameCoordinator::reset(const GameConfig & config, const Map_t & map)
    {
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
    }

    void SimpleGameCoordinator::switchToMap(const Map_t & map)
    {
        m_map = map;

        M_CHECK_SS(
            (!m_map.empty() && !m_map.front().empty()),
            "Map is invalid.  Map either had no row strings or the first row string was empty.");

        m_layout.setup(m_map, m_config);
        m_board.reset(m_context);
        m_game.reset();
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
        sf::Clock frameClock;

        while (m_window.isOpen() && !m_game.isGameOver())
        {
            handleEvents();
            update(frameClock.restart().asSeconds());
            draw();
        }

        printFinalStatusToConsole();
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

    void LightsOutGame::reset(const GameConfig & configOld, const Map_t & map)
    {
        GameConfig configNew{ configOld };
        configNew.game_name = "Lights Out";
        configNew.background_color = sf::Color(26, 26, 32);

        SimpleGameCoordinator::reset(configNew, map);

        m_soundPlayer.load({ "tap-1-a.ogg", "spawn-huge.ogg" });
    }

    void LightsOutGame::printFinalStatusToConsole()
    {
        const float inverseScoreRatio{ static_cast<float>(m_board.pieces().size()) /
                                       static_cast<float>(m_layout.cellCountTotal()) };

        m_game.score(static_cast<int>(100.0f * std::clamp((1.0f - inverseScoreRatio), 0.0f, 1.0f)));

        SimpleGameCoordinator::printFinalStatusToConsole();
    }

    void LightsOutGame::draw()
    {
        m_window.clear(m_config.background_color);

        for (const BoardPos_t & pos : m_layout.allValidPositions())
        {
            const auto pieceOpt{ m_board.pieceAtOpt(pos) };
            if (pieceOpt)
            {
                m_window.draw(pieceOpt.value());
            }
            else
            {
                m_window.draw(CellPiece::makeDefaultSprite(m_context, pos, Piece::Count));
            }
        }

        m_window.display();
    }

    void LightsOutGame::switchToMap(const Map_t & map)
    {
        printFinalStatusToConsole();
        SimpleGameCoordinator::switchToMap(map);
        randomizeCells();
    }

    void LightsOutGame::randomizeCells()
    {
        std::vector<BoardPos_t> positionsToTurnOff{ m_layout.allValidPositions() };

        m_random.shuffle(positionsToTurnOff);

        const std::size_t turnOffCount{ m_random.zeroTo(positionsToTurnOff.size() - 1) };
        positionsToTurnOff.resize(turnOffCount);

        for (const BoardPos_t & pos : positionsToTurnOff)
        {
            toggleCell(pos);
        }
    }

    void LightsOutGame::toggleCell(const BoardPos_t & pos)
    {
        auto pieceOpt{ m_board.pieceAtOpt(pos) };
        if (pieceOpt)
        {
            m_board.removePiece(pos);
        }
        else
        {
            m_board.addPiece(m_context, Piece::Cell, pos);
        }
    }

    bool LightsOutGame::handleBoardResizeMapEvent(const sf::Event & event)
    {
        if (sf::Event::KeyPressed != event.type)
        {
            return false;
        }

        const auto numberOpt{ keys::toNumberOpt<std::size_t>(event.key.code) };
        if (!numberOpt)
        {
            return false;
        }

        const Map_t newMap{ makeMapOfSize(numberOpt.value()) };
        if (newMap.empty())
        {
            return false;
        }

        switchToMap(newMap);
        return true;
    }

    void LightsOutGame::handleEvent(const sf::Event & event)
    {
        if (SimpleGameCoordinator::handleExitEvents(event))
        {
            return;
        }

        if (handleBoardResizeMapEvent(event))
        {
            return;
        }

        if (event.type != sf::Event::MouseButtonPressed)
        {
            return;
        }

        // check if the click was inside any cell's bounds
        const sf::Vector2f clickWindowPos{ sf::Vector2i(event.mouseButton.x, event.mouseButton.y) };
        auto boardPosOpt{ m_layout.windowPosToBoardPos(clickWindowPos) };
        if (!boardPosOpt)
        {
            return;
        }

        const BoardPos_t clickBoardPos{ boardPosOpt.value() };

        // play the sfx different if turning on/off
        if (m_board.isPieceAt(clickBoardPos))
        {
            m_context.audio.play("tap", 0.6f);
        }
        else
        {
            m_context.audio.play("tap");
        }

        // toggle the clicked cell and all around it
        for (const BoardPos_t & pos : findAllBoardPosToToggle(clickBoardPos))
        {
            toggleCell(pos);
        }

        // check if the player won
        if (m_board.pieces().empty())
        {
            m_game.endGame(true);
        }
    }

    std::vector<BoardPos_t>
        LightsOutGame::findAllBoardPosToToggle(const BoardPos_t & clickedBoardPos) const
    {
        std::vector<BoardPos_t> positionsToToggle;
        positionsToToggle.reserve(9);

        for (const BoardPos_t & otherBoardPos : m_layout.allValidPositions())
        {
            if ((std::abs(clickedBoardPos.x - otherBoardPos.x) <= 1) &&
                (std::abs(clickedBoardPos.y - otherBoardPos.y) <= 1))
            {
                positionsToToggle.push_back(otherBoardPos);
            }
        }

        return positionsToToggle;
    }

    Map_t LightsOutGame::makeMapOfSize(std::size_t size)
    {
        if (size < 1)
        {
            size = 1;
        }
        else if (size > 9)
        {
            size = 9;
        }

        // important that they all start on (or there, or whatever)
        return Map_t(size, std::string(size, pieceToMapChar(Piece::Cell)));
    }
} // namespace boardgame