#ifndef METHHEADS_BASE_HPP_INCLUDED
#define METHHEADS_BASE_HPP_INCLUDED

#include "display-constants.hpp"
#include "enums.hpp"
#include "random.hpp"
#include "scoped-board-position.hpp"
#include "sim-context.hpp"
#include "utils.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>
#include <vector>

namespace methhead
{
    struct MediaShared
    {
        explicit MediaShared(
            const sf::Color & col = sf::Color::Transparent, const std::string & imgPath = "")
            : color(col)
            , texture()
        {
            if (!imgPath.empty())
            {
                if (!texture.loadFromFile(imgPath))
                {
                    std::cout << "Failed to load image: \"" << imgPath << "\"" << std::endl;
                }
            }
        }

        sf::Color color;
        sf::Texture texture;
    };

    //

    struct MediaSpecific : public sf::Drawable
    {
        MediaSpecific(const SimContext & context, const MediaShared & mediaShared)
            : text(context.display.default_text)
            , sprite(mediaShared.texture)
        {}

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            target.draw(sprite, states);
            target.draw(text, states);
        }

        sf::Text text;
        sf::Sprite sprite;
    };

    //

    struct IPickup : public sf::Drawable
    {
        virtual ~IPickup() = default;

        virtual Piece piece() const = 0;
        virtual int scoreChange() const = 0;
        virtual float speedChangeMult() const = 0;
        virtual BoardPos_t boardPos() const = 0;
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;
    };

    using IPickupUPtr_t = std::unique_ptr<IPickup>;

    //

    struct Loot
        : public IPickup
        , public ScopedBoardPosition
    {
        explicit Loot(const SimContext & context)
            : ScopedBoardPosition(context)
            , m_loot(context.random.fromTo(1, 99))
            , m_media(context, m_mediaShared)
        {}

        virtual ~Loot() = default;

        Piece piece() const override { return Piece::Loot; }
        int scoreChange() const override { return m_loot; }
        float speedChangeMult() const override { return 1.0f; }
        BoardPos_t boardPos() const final { return ScopedBoardPosition::get(); }

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            m_media.draw(target, states);
        }

      private:
        int m_loot;
        MediaSpecific m_media;
        static inline MediaShared m_mediaShared{ sf::Color(253, 173, 57), "image/loot.png" };
    };

    //

    struct IActor : public sf::Drawable
    {
        virtual ~IActor() = default;

        virtual int score() const = 0;
        virtual Piece piece() const = 0;
        virtual BoardPos_t boardPos() const = 0;

        virtual void pickTarget(const SimContext & context) = 0;
        virtual void pickup(const SimContext & context, const IPickup & pickup) = 0;
        virtual bool update(const SimContext & context, const float elapsedMs) = 0;

        // void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;
    };

    using IActorUPtr_t = std::unique_ptr<IActor>;

    //

    class ActorBase
        : public IActor
        , public ScopedBoardPosition
    {
      protected:
        explicit ActorBase(const SimContext & context);

      public:
        virtual ~ActorBase() = default;

        inline int score() const final { return m_score; }
        inline BoardPos_t boardPos() const final { return ScopedBoardPosition::get(); }

        void pickup(const SimContext &, const IPickup & pickup) override;
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;
        bool update(const SimContext & context, const float elapsedSec) override;

      private:
        bool isTimeToMove(const float elapsedSec);

        inline int walkDistanceTo(const BoardPos_t & to) const
        {
            return walkDistance(boardPos(), to);
        }

        bool move(const SimContext & context);

      protected:
        BoardPos_t m_targetBoardPos;

      private:
        int m_score;
        float m_moveDelaySec;
        float m_turnDelaySoFarSec;

        static inline const float s_turnDelayDefaultSec{ 0.3f };
    };

    //

    struct Lazy : public ActorBase
    {
        explicit Lazy(const SimContext & context)
            : ActorBase(context)
        {}

        virtual ~Lazy() = default;

        inline Motivation motivation() const final { return Motivation::lazy; }

      private:
        void pickTarget(const SimContext & context) final
        {
            const BoardPos_t currPos{ boardPos() };

            if (context.pickups.empty())
            {
                m_targetBoardPos = currPos;
                return;
            }

            const auto foundIter = std::min_element(
                std::begin(context.pickups),
                std::end(context.pickups),
                [&](const IPickupUPtr_t & left, const IPickupUPtr_t & right) {
                    return (
                        walkDistance(currPos, left->boardPos()) <
                        walkDistance(currPos, right->boardPos()));
                });

            m_targetBoardPos = (*foundIter)->boardPos();
        }
    };

    //

    struct Greedy : public ActorBase
    {
        explicit Greedy(const SimContext & context)
            : ActorBase(context)
        {}

        virtual ~Greedy() = default;

        inline Motivation motivation() const final { return Motivation::greedy; }

      private:
        void pickTarget(const SimContext & context) final
        {
            if (context.pickups.empty())
            {
                m_targetBoardPos = boardPos();
                return;
            }

            const auto foundIter = std::max_element(
                std::begin(context.pickups),
                std::end(context.pickups),
                [&](const IPickupUPtr_t & left, const IPickupUPtr_t & right) {
                    return (left->value() < right->value());
                });

            m_targetBoardPos = (*foundIter)->boardPos();
        }
    };
} // namespace methhead

#endif // METHHEADS_BASE_HPP_INCLUDED