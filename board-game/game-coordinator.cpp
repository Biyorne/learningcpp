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
    GameCoordinator::GameCoordinator(const std::string & mediaDirPath)
        : m_enableSpecialEffects(false)
        , m_images(mediaDirPath)
        , m_simTimeMult(1.0f)
        , m_frameClock()
        , m_videoMode(2880u, 1800u, sf::VideoMode::getDesktopMode().bitsPerPixel)
        , m_window(m_videoMode, "bogeyman", sf::Style::Default)
        , m_bloomWindow(m_window)
        , m_random()
        , m_soundPlayer(m_random, (mediaDirPath + "/sfx"))
        , m_animationPlayer(m_random, (mediaDirPath + "/animation"))
        , m_board(m_images, m_random, m_window.getSize())
        , m_collider()
        , m_context(m_images, m_board, m_random, m_soundPlayer, m_animationPlayer)
        , m_boardBaseSprite()
        , m_boardBaseTexture()
        , m_isPickupSpawnEffectRunning(false)
        , m_victimSpawnWaitDurSec(0.0f)
        , m_victimSpawnWaitSoFarSec(0.0f)
        , m_victimSpawnScaler(0.0f)
        , m_victimSpawnBoardPos(0, 0)
        , m_victimSpawnSprite(m_images.tile_texture, Image::coords(Image::Pickup))
    {
        m_window.setFramerateLimit(60);

        m_bloomWindow.isEnabled(m_enableSpecialEffects);

        if (m_enableSpecialEffects)
        {
            m_soundPlayer.loadAll();
            m_animationPlayer.loadAll();
        }

        reset();
        setupBoardBackgroundImage();
    }

    void GameCoordinator::setupBoardBackgroundImage()
    {
        // create
        if (!m_boardBaseTexture.create(m_window.getSize().x, m_window.getSize().y))
        {
            std::ostringstream ss;

            ss << "Error:  GameCoordinator::GameCoordinator() unable to create an "
                  "sf::RenderTexture for the board base of size="
               << m_window.getSize() << ".";

            throw std::runtime_error(ss.str());
        }

        m_boardBaseTexture.clear();

        // draw wood floor
        for (const IPieceUPtr_t & pieceUPtr : m_board.pieces)
        {
            const Image imageEnum{ m_random.from({ Image::WoodFloor1,
                                                   Image::WoodFloor2,
                                                   Image::WoodFloor3,
                                                   Image::WoodFloor4,
                                                   Image::WoodFloor5,
                                                   Image::WoodFloor6 }) };

            m_board.m_boardBaseTexture.draw(
                m_images.makeSprite(imageEnum, util::center(pieceUPtr->)));
        }

        // draw the walls and the shadows on top of them
        for (const auto & [boardPos, cell] : m_board.cells)
        {
            if (cell.piece_enum != Piece::Obstacle)
            {
                continue;
            }

            const sf::Vector2s boardPosS(boardPos);
            const char mapChar{ m_board.map_strings.at(boardPosS.y).at(boardPosS.x) };
            const Image imageEnum{ Image::charToEnum(mapChar) };

            sprite.setTextureRect(Image::coords(imageEnum));

            util::scale(sprite, m_board.cell_size);
            util::setOriginToCenter(sprite);
            sprite.setPosition(cell.window_rect_center);
            m_boardBaseTexture.draw(sprite);

            if ((imageEnum == Image::WallHoriz) && (boardPos.x > 0))
            {
                const char mapCharToTheLeft{
                    m_board.map_strings.at(boardPosS.y).at(boardPosS.x - 1)
                };

                static bool isBlock(const Enum image)
                {
                    return (
                        (image == BlockCorner) || (image == BlockTop) || (image == BlockBottom));
                }

                const Image imageEnumToTheLeft{ Image::charToEnum(mapCharToTheLeft) };

                if (Image::isBlock(imageEnumToTheLeft))
                {
                    sprite.setTextureRect(Image::coords(Image::Shadow));

                    const float horizScale{ m_random.fromTo(1.0f, 1.9f) };
                    const float horizSizeBefore{ sprite.getGlobalBounds().width };

                    sprite.scale(horizScale, 1.0f);
                    sprite.move((horizSizeBefore * (horizScale - 1.0f) * 0.5f), 0.0f);

                    m_boardBaseTexture.draw(sprite);
                }
            }
        }

        m_boardBaseTexture.display();
        m_boardBaseSprite.setTexture(m_boardBaseTexture.getTexture());
    }

    void GameCoordinator::reset()
    {
        m_enableSpecialEffects = false;
        m_bloomWindow.isEnabled(m_enableSpecialEffects);

        m_simTimeMult = 1.0f;
        m_frameClock.restart();

        m_soundPlayer.stopAll();
        m_animationPlayer.stopAll();

        m_board.reset();

        m_victimSpawnWaitDurSec = randomPickupSpawnDelaySec();
        m_victimSpawnWaitSoFarSec = 0.0f;

        loadMap();
    }

    void GameCoordinator::loadMap()
    {
        // setup the walls
        for (auto & [boardPos, cell] : m_board.cells)
        {
            if (cell.piece_enum != Piece::Obstacle)
            {
                continue;
            }

            const sf::Vector2s boardPosS(boardPos);
            const char mapChar{ m_board.map_strings.at(boardPosS.y).at(boardPosS.x) };
            const Image mapImageEnum{ Image::charToEnum(mapChar) };

            m_board.pieces.push_back(
                std::make_unique<WallPiece>(m_context, cell.board_pos, mapImageEnum));

            cell.piece_index = m_board.pieces.size();
        }

        m_board.pieces.push_back(std::make_unique<PlayerPiece>(m_context, BoardPos_t(23, 1)));
        m_board.cellAt(BoardPos_t(23, 0)).piece_enum = Piece::Obstacle;
        m_board.cellAt(BoardPos_t(23, 0)).piece_index = m_board.pieces.size();

        m_board.pieces.push_back(std::make_unique<VillanPiece>(m_context, BoardPos_t(13, 7)));
        m_board.cellAt(BoardPos_t(13, 7)).piece_enum = Piece::Obstacle;
        m_board.cellAt(BoardPos_t(13, 7)).piece_index = m_board.pieces.size();
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
        else if (sf::Keyboard::E == event.key.code)
        {
            m_enableSpecialEffects = !m_enableSpecialEffects;
            m_bloomWindow.isEnabled(m_enableSpecialEffects);

            std::cout << "Special Effets turned " << ((m_enableSpecialEffects) ? "ON" : "OFF")
                      << std::endl;
        }
        else if (sf::Keyboard::B == event.key.code)
        {
            m_bloomWindow.isEnabled(!m_bloomWindow.isEnabled());
        }

        for (IPieceUPtr_t & piece : m_context.board.pieces)
        {
            if (piece->piece() == Piece::Player)
            {
                dynamic_cast<PlayerPiece &>(*piece).nextMovePosAdj(posAdj);
            }
        }
    }

    void GameCoordinator::update(const float elapsedTimeSec)
    {
        for (IPieceUPtr_t & piece : m_context.board.pieces)
        {
            const BoardPos_t oldPos{ piece->boardPos() };
            const BoardPos_t newPos{ piece->update(m_context, elapsedTimeSec) };

            if (newPos != oldPos)
            {
                m_collider.handle(m_context, *piece, newPos);
            }
        }

        if (m_enableSpecialEffects)
        {
            m_animationPlayer.update(elapsedTimeSec);
        }

        updatePickupSpawn(elapsedTimeSec);
    }

    void GameCoordinator::updatePickupSpawn(const float elapsedTimeSec)
    {
        // TODO REMOVE
        // m_victimSpawnWaitSoFarSec = 100.0f;

        if (m_isPickupSpawnEffectRunning)
        {
            m_victimSpawnSprite.setColor(m_victimSpawnSprite.getColor() + sf::Color(0, 0, 0, 1));

            m_victimSpawnSprite.rotate(elapsedTimeSec * 100.0f);

            const float shrinkScale{ 1.0f - elapsedTimeSec };
            m_victimSpawnSprite.scale(shrinkScale, shrinkScale);

            const sf::Vector2f size{ util::size(m_victimSpawnSprite) };
            if ((size.x > m_board.cell_size.x) && (size.y > m_board.cell_size.y))
            {
                return;
            }

            Cell & cell{ m_board.cellAt(m_victimSpawnBoardPos) };
            cell.piece_enum = Piece::Pickup;
            m_board.pieces.push_back(std::make_unique<PickupPiece>(m_context, cell.board_pos));
            cell.piece_index = m_board.pieces.size();

            m_isPickupSpawnEffectRunning = false;
        }
        else
        {
            m_victimSpawnWaitSoFarSec += elapsedTimeSec;

            if (m_victimSpawnWaitSoFarSec < m_victimSpawnWaitDurSec)
            {
                return;
            }

            m_victimSpawnWaitDurSec = randomPickupSpawnDelaySec();
            m_victimSpawnWaitSoFarSec = 0.0f;

            const auto boardPosOpt{ m_board.findRandomEmptyPos(m_context) };
            if (!boardPosOpt)
            {
                m_victimSpawnWaitDurSec = 1.0f;
                return;
            }

            m_victimSpawnBoardPos = *boardPosOpt;

            m_isPickupSpawnEffectRunning = true;

            // make the spawn pos a wall only temporarily to prevent other pieces from moving
            // there while spawn anim runs
            Cell & cell{ m_board.cellAt(m_victimSpawnBoardPos) };
            cell.piece_enum = Piece::Obstacle;

            util::setOriginToCenter(m_victimSpawnSprite);
            util::scaleAndCenterInside(m_victimSpawnSprite, m_board.window_rect);
            m_victimSpawnSprite.setPosition(cell.window_rect_center);
            m_victimSpawnSprite.setColor(sf::Color::Magenta - sf::Color(0, 0, 0, 255));
        }
    }

    void GameCoordinator::draw()
    {
        m_bloomWindow.clear();

        m_bloomWindow.draw(m_boardBaseSprite);

        for (IPieceUPtr_t & piece : m_context.board.pieces)
        {
            m_bloomWindow.draw(*piece);
        }

        if (m_isPickupSpawnEffectRunning)
        {
            m_bloomWindow.draw(m_victimSpawnSprite);
        }

        // if (m_enableSpecialEffects)
        //{
        //    m_animationPlayer.draw(m_window, {});
        //}

        m_bloomWindow.display();
    }
} // namespace boardgame