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
            std::cout << "Game Over:  You " << ((m_game.didPlayerWin()) ? "WON!" : "LOST  (loser)");
        }
        else
        {
            std::cout << "Application shutdown before game was over.";
        }

        std::cout << "  (final score=" << calcFinalScore() << ")" << std::endl;
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

    bool SimpleGameCoordinator::handleEvent(const sf::Event & event)
    {
        if (SimpleGameCoordinator::handleExitEvents(event))
        {
            return true;
        }

        return m_board.handleEvent(m_context, event);
    }

    void SimpleGameCoordinator::update(const float elapsedTimeSec)
    {
        if (m_game.isGameOver())
        {
            return;
        }

        m_board.update(m_context, elapsedTimeSec);
    }

    void SimpleGameCoordinator::draw()
    {
        m_window.clear(m_config.background_color);
        m_window.draw(m_board);
        m_window.display();
    }

    //

    void LightsOutGame::reset(const GameConfig & configOld, const Map_t & mapOrig)
    {
        GameConfig configNew{ configOld };
        configNew.game_name = "Lights Out";
        configNew.background_color = sf::Color(26, 26, 32);
        configNew.is_fullscreen = false;
        configNew.video_mode.width = 1600;
        configNew.video_mode.height = 1200;

        Map_t mapToUse{ mapOrig };
        if (mapToUse.empty())
        {
            mapToUse = makeMapOfSize(5);
        }

        SimpleGameCoordinator::reset(configNew, mapToUse);

        m_soundPlayer.load({ "tap-1-a.ogg", "spawn-huge.ogg" });
    }

    void LightsOutGame::switchToMap(const Map_t & map)
    {
        m_moveCount = 0;
        SimpleGameCoordinator::switchToMap(map);

        // randomize starting board
        const std::set<BoardPos_t> temp{ m_layout.allValidPositions() };
        std::vector<BoardPos_t> allPositions(std::begin(temp), std::end(temp));

        m_random.shuffle(allPositions);

        const std::size_t toggleCount{ m_random.zeroTo(allPositions.size() - 1) };
        allPositions.resize(toggleCount);

        for (const BoardPos_t & pos : allPositions)
        {
            const auto pieceOpt{ m_board.pieceAtOpt(pos) };
            M_CHECK_SS((pieceOpt.has_value()), pos);
            const sf::Vector2f cellCenterWinPos{ util::center(pieceOpt->get().bounds()) };
            togglePieceCenteredAt(sf::Vector2i(cellCenterWinPos));
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

    bool LightsOutGame::handleEvent(const sf::Event & event)
    {
        if (SimpleGameCoordinator::handleExitEvents(event))
        {
            return true;
        }

        if (handleBoardResizeMapEvent(event))
        {
            return true;
        }

        if (event.type != sf::Event::MouseButtonPressed)
        {
            return false;
        }

        const sf::Vector2f clickWindowPos{ sf::Vector2i(event.mouseButton.x, event.mouseButton.y) };

        for (const IPieceUPtr_t & piece : m_board.pieces())
        {
            if (piece->bounds().contains(clickWindowPos))
            {
                handlePieceClickedOn(*piece);
                return true;
            }
        }

        return false;
    }

    void LightsOutGame::handlePieceClickedOn(const IPiece & piece)
    {
        ++m_moveCount;
        playTaggleSfx(piece);
        toggleAdjacentPieces(piece);
        handleIfGameWon();
    }

    void LightsOutGame::handleIfGameWon()
    {
        const std::size_t offCount{ countOffPieces() };
        const std::size_t totalCount{ m_layout.cellCountTotal() };

        if (offCount != totalCount)
        {
            return;
        }

        m_game.endGame(true);
    }

    int LightsOutGame::calcFinalScore()
    {
        const int sizeLength{ static_cast<int>(m_map.front().length()) };
        const int scoreBase{ sizeLength + (sizeLength * sizeLength) };
        const int scoreFinal{ scoreBase - m_moveCount };
        return scoreFinal;
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

    void LightsOutGame::toggleAdjacentPieces(const IPiece & piece)
    {
        for (const sf::Vector2f & centerPos : findAdjacentCellCenters(piece))
        {
            togglePieceCenteredAt(sf::Vector2i(centerPos));
        }
    }

    std::vector<sf::Vector2f>
        LightsOutGame::findAdjacentCellCenters(const IPiece & centerPiece) const
    {
        const BoardPos_t & centerPieceBoardPos{ centerPiece.position() };

        std::vector<sf::Vector2f> cellWindowCenterPositions;
        cellWindowCenterPositions.reserve(9);

        for (const IPieceUPtr_t & adjPiece : m_board.pieces())
        {
            const BoardPos_t adjPieceBoardPos{ adjPiece->position() };
            if (areBoardPositionsAdjacent(centerPieceBoardPos, adjPieceBoardPos))
            {
                cellWindowCenterPositions.push_back(util::center(adjPiece->bounds()));
            }
        }

        return cellWindowCenterPositions;
    }

    void LightsOutGame::playTaggleSfx(const IPiece & piece) const
    {
        if (piece.piece() == Piece::On)
        {
            m_context.audio.play("tap", 0.6f);
        }
        else
        {
            m_context.audio.play("tap");
        }
    }

    void LightsOutGame::togglePieceCenteredAt(const sf::Vector2i & centerWinPos)
    {
        sf::Event event;
        event.type = sf::Event::MouseButtonPressed;

        event.mouseButton =
            sf::Event::MouseButtonEvent{ sf::Mouse::Left, centerWinPos.x, centerWinPos.y };

        m_board.handleEvent(m_context, event);
    }

    bool LightsOutGame::areBoardPositionsAdjacent(
        const BoardPos_t & posA, const BoardPos_t & posB) const
    {
        return ((std::abs(posA.x - posB.x) <= 1) && (std::abs(posA.y - posB.y) <= 1));
    }

    Map_t LightsOutGame::makeMapOfSize(std::size_t size) const
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