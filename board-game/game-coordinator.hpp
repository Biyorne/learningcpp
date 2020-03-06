#ifndef BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED
#define BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED
//
// game-coordinator.hpp
//
#include "animation-player.hpp"
#include "bloom-shader.hpp"
#include "board.hpp"
#include "context.hpp"
#include "keys.hpp"
#include "pieces.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "settings.hpp"
#include "sound-player.hpp"
#include "types.hpp"
#include "util.hpp"

#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace boardgame
{
    class SimpleGame
    {
      public:
        SimpleGame();
        virtual ~SimpleGame() = default;

        virtual void reset(const GameConfig & config, const Map_t & map);
        virtual void switchToMap(const Map_t & map);
        virtual void run();

      protected:
        virtual void openWindow();
        virtual void handleEvents();
        virtual void handleEvent(const sf::Event & event);
        virtual void update(const float elapsedTimeSec);
        virtual void draw();

      protected:
        Map_t m_map;
        GameConfig m_config;
        sf::RenderWindow m_window;

        util::Random m_random;
        util::SoundPlayer m_soundPlayer;
        util::AnimationPlayer m_animationPlayer;

        SimpleLayout m_layout;
        SimpleMedia m_media;
        SimpleBoard m_board;
        SimpleGameInPlay m_game;
        Context m_context;
    };

    //

    class LightsOutGame : public SimpleGame
    {
      public:
        LightsOutGame() = default;
        virtual ~LightsOutGame() = default;

        void reset(const GameConfig & configOld, const Map_t & mapOrig) override
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

            SimpleGame::reset(configNew, mapToUse);

            m_soundPlayer.load({ "tap-1-a.ogg", "spawn-huge.ogg" });
        }

        void handleEvent(const sf::Event & event) override
        {
            if (sf::Event::KeyPressed == event.type)
            {
                const auto numberOpt{ keys::toNumberOpt<std::size_t>(event.key.code) };
                if (numberOpt)
                {
                    const Map_t newMap{ makeMapOfSize(numberOpt.value()) };
                    if (!newMap.empty())
                    {
                        switchToMap(newMap);
                        return;
                    }
                }
            }

            SimpleGame::handleEvent(event);
        }

      private:
        static Map_t makeMapOfSize(std::size_t size)
        {
            if (size < 2)
            {
                size = 2;
            }
            else if (size > 9)
            {
                size = 9;
            }

            Map_t map;
            const std::string row(size, 'C');
            for (std::size_t vert(0); vert < size; ++vert)
            {
                map.push_back(row);
            }

            return map;
        }
    };
} // namespace boardgame

#endif // BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED