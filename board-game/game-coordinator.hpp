#ifndef BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED
#define BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED

#include "animation-player.hpp"
#include "bloom-shader.hpp"
#include "board.hpp"
#include "pieces.hpp"
#include "random.hpp"
#include "resources.hpp"
#include "sound-player.hpp"
#include "types.hpp"
#include "util.hpp"

#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

namespace boardgame
{
    class GameCoordinator
    {
      public:
        explicit GameCoordinator(const std::filesystem::path & mediaDirPath);

        void run();

      private:
        void reset();
        void resetToStartingMapPieces();
        IPieceUPtr_t makePiece(const Piece::Enum piece, const BoardPos_t & boardPos) const;

        void handleEvents();

        // returns true if event processing should continue, if false then process next frame
        bool handleEvent(const sf::Event & event);

        void handleMovingPieces();

        void update(const float elapsedTimeSec);
        void draw();

        float randomPickupSpawnDelaySec() const { return m_context.random.fromTo(4.0f, 8.0f); }

        void updatePickupSpawn(const float elapsedTimeSec);

      private:
        float m_simTimeMult;
        sf::Clock m_frameClock;
        bool m_enableSpecialEffects;

        sf::VideoMode m_videoMode;
        sf::RenderWindow m_window;
        // util::BloomEffectHelper m_bloomWindow;

        MapBase m_map;
        BoardBase m_board;
        Resources m_resources;
        util::Random m_random;
        util::SoundPlayer m_soundPlayer;
        util::AnimationPlayer m_animationPlayer;

        Context m_context;

        bool m_isPlayersTurn;
        sf::Event m_lastUnhandledKeyEvent;
        std::size_t playerWinCount = 0;
        std::size_t villanWinCount = 0;

        static sf::Event makeInvalidEvent()
        {
            sf::Event eventNew;
            eventNew.type = sf::Event::EventType::Count;
            return eventNew;
        }

        static bool isEventValid(const sf::Event & event)
        {
            return (event.type != sf::Event::EventType::Count);
        }

        static bool isEventValidKeypress(const sf::Event & event)
        {
            return (
                isEventValid(event) && (sf::Event::KeyPressed == event.type) &&
                (sf::Keyboard::Unknown != event.key.code));
        }

        // clang-format off
        static inline const MapLayout_t m_mapStrings = {
            "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
            "WWWWWWWWWWWWWWWWWDWWWWWWWWWWWWWW",
            "WWVV W        WW    WW   W   PWW",
            "WW W   W W W   W  W    W   W  WW",
            "WW   W       W    WW W   W  W WW",
            "WW W WW WWW  W W  W  WWW   W  WW",
            "WW W    W      W       W W    WW",
            "WW    W   W W     WW W   W  WWWW",
            "WW W W  W W   WW  W    W       W",
            "WWW    W    W   W    W   W  WWWW",
            "WW   W   W   W    W    W      WW",
            "WW W   W   W    W   W    W  W WW",
            "WW W W W W W W WW W    W   WW WW",
            "WWV  W       W       W      PPWW",
            "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
            "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW" };
        // clang-format on
    };
} // namespace boardgame

#endif // BOARDGAME_GAMECOORDINATOR_HPP_INCLUDED