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
            std::cout << "Game Over:  You " << ((m_game.didPlayerWin()) ? "WON!" : "LOST  (loser)")
                      << std::endl;
        }
        else
        {
            std::cout << "Application shutdown before game was over." << std::endl;
        }
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

    int LightsOutGame::Score::finalScore() const
    {
        return (
            all_cells_off_perfect_starting_score + click_count_adj + win_bonus +
            time_spent_playing_adj - cells_left_on_penalty);
    }

    void LightsOutGame::Score::print() const
    {
        std::cout << "Final Score                   = " << finalScore();
        std::cout << "\n   winner_bonus               = " << win_bonus;
        std::cout << "\n   avg_high_score_possible    = " << all_cells_off_perfect_starting_score;
        std::cout << "\n   cells_left_on              = " << cells_left_on;
        std::cout << "\n   cells_left_on_penalty      = " << -cells_left_on_penalty;
        std::cout << "\n   click_count                = " << click_count;
        std::cout << "\n   click_count_adj            = " << click_count_adj;

        std::cout << "\n   time_spent_playing         = " << std::setw(2) << std::right
                  << std::setfill('0') << (time_spent_playing_sec / 60) << ":" << std::setw(2)
                  << std::right << std::setfill('0') << (time_spent_playing_sec % 60) << "s";

        std::cout << "\n   time_spent_playing_adj     = " << time_spent_playing_adj;
        std::cout << std::endl << std::endl;
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

    void LightsOutGame::switchToMap(const Map_t & map)
    {
        std::cout << "Switching to new map." << std::endl;

        if (m_score.click_count > 0)
        {
            m_score.print();
        }

        m_score = Score{};
        SimpleGameCoordinator::switchToMap(map);
        randomizeOffPieces();
        m_gameElapsedClock.restart();
    }

    void LightsOutGame::randomizeOffPieces()
    {
        std::vector<BoardPos_t> positionsToTurnOff{ m_layout.allValidPositions() };

        m_random.shuffle(positionsToTurnOff);

        const std::size_t turnOffCount{ m_random.zeroTo(positionsToTurnOff.size() - 1) };
        positionsToTurnOff.resize(turnOffCount);

        for (const BoardPos_t & pos : positionsToTurnOff)
        {
            togglePiece(pos);
        }
    }

    void LightsOutGame::togglePiece(const BoardPos_t & pos)
    {
        const auto pieceOpt{ m_board.pieceAtOpt(pos) };
        M_CHECK_SS((pieceOpt.has_value()), pos);
        pieceOpt->get().takeTurn(m_context);
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

        const sf::Vector2f clickWindowPos{ sf::Vector2i(event.mouseButton.x, event.mouseButton.y) };

        const IPieceUList_t & pieces{ m_board.pieces() };

        const auto foundIter = std::find_if(
            std::begin(pieces), std::end(pieces), [&](const IPieceUPtr_t & maybePiece) {
                return maybePiece->bounds().contains(clickWindowPos);
            });

        if (foundIter == std::end(pieces))
        {
            return;
        }

        handlePieceClickedOn(**foundIter);
    }

    void LightsOutGame::handlePieceClickedOn(const IPiece & piece)
    {
        ++m_score.click_count;

        if (piece.piece() == Piece::On)
        {
            m_context.audio.play("tap", 0.6f);
        }
        else
        {
            m_context.audio.play("tap");
        }

        toggleAdjacentPieces(piece);
        handleIfGameWon();
        updateScore();
    }

    void LightsOutGame::toggleAdjacentPieces(const IPiece & clickedOnPiece)
    {
        const BoardPos_t clickedOnPos{ clickedOnPiece.position() };

        for (IPieceUPtr_t & otherPiece : m_board.pieces())
        {
            const BoardPos_t otherPos{ otherPiece->position() };

            if ((std::abs(clickedOnPos.x - otherPos.x) <= 1) &&
                (std::abs(clickedOnPos.y - otherPos.y) <= 1))
            {
                togglePiece(otherPos);
            }
        }
    }

    void LightsOutGame::handleIfGameWon()
    {
        if (countOffPieces() == m_layout.cellCountTotal())
        {
            m_game.endGame(true);
        }
    }

    void LightsOutGame::updateScore()
    {
        m_score = calcScore();
        m_game.score(m_score.finalScore());
    }

    LightsOutGame::Score LightsOutGame::calcScore() const
    {
        Score score;

        score.click_count = m_score.click_count;
        score.click_count_adj = (static_cast<int>(m_layout.cellCountTotal()) - score.click_count);
        if (score.click_count_adj < 0)
        {
            score.click_count_adj =
                -(score.click_count / static_cast<int>(std::sqrt(m_layout.cellCountTotal())));
        }

        const sf::Vector2i cellCounts{ m_layout.cellCounts() };

        score.all_cells_off_perfect_starting_score =
            ((cellCounts.x + cellCounts.y) + (cellCounts.x * cellCounts.y));

        score.cells_left_on = (m_layout.cellCountTotal() - countOffPieces());

        score.cells_left_on_penalty = static_cast<int>(
            (score.cells_left_on + score.cells_left_on) +
            (score.cells_left_on * score.cells_left_on));

        score.time_spent_playing_sec =
            static_cast<int>(m_gameElapsedClock.getElapsedTime().asSeconds());

        if (score.time_spent_playing_sec < 10)
        {
            score.time_spent_playing_adj = (10 - score.time_spent_playing_sec);
        }
        else
        {
            score.time_spent_playing_adj = -(score.time_spent_playing_sec / 10);
        }

        score.win_bonus = 0;

        if (m_game.didPlayerWin())
        {
            score.win_bonus = (static_cast<int>(m_layout.cellCountTotal()) * 2);
        }
        else
        {
            if (score.click_count_adj > 0)
            {
                score.click_count_adj = 0;
            }

            if (score.time_spent_playing_adj > 0)
            {
                score.time_spent_playing_adj = 0;
            }
        }

        return score;
    }

    std::size_t LightsOutGame::countOffPieces() const
    {
        std::size_t counter{ 0 };
        for (const IPieceUPtr_t & piece : m_board.pieces())
        {
            if (piece->piece() == Piece::Off)
            {
                ++counter;
            }
        }

        return counter;
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

        Map_t map;
        const std::string row(size, 'X'); // anything other than an empty space works here
        for (std::size_t vert(0); vert < size; ++vert)
        {
            map.push_back(row);
        }

        return map;
    }
} // namespace boardgame