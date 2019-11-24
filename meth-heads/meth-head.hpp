#ifndef METHHEADS_BASE_HPP_INCLUDED
#define METHHEADS_BASE_HPP_INCLUDED

#include "display-constants.hpp"
#include "random.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>
#include <vector>

namespace methhead
{
    struct ActorContext;

    // TODO in the end of all cleanup this should have a different home...
    //
    // also... TODO In the end, after cleaning up everything, there should be no more need for
    // "none" none/invalid options in enums is not always code smell, it is not always wrong,
    // but on the other hand, always adding none/invalid to your enums as a default/habit just
    // in case it is needed IS ALWAYS A BAD HABIT, becauase any enum that has a none/invalid
    // option that is not needed is often a huge source of errors and mess.
    enum class Motivation
    {
        none,
        lazy,
        greedy
    };

    //

    struct IActor : public sf::Drawable
    {
        virtual ~IActor() = default;

        virtual int score() const = 0;
        virtual void score(const int value) = 0;

        virtual Motivation motivation() const = 0;

        virtual BoardPos_t boardPos() const = 0;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;

        // returns true if the boardPos changed
        virtual bool update(const float elapsedMs, const ActorContext & context) = 0;
    };

    using IActorUPtr_t = std::unique_ptr<IActor>;

    //

    struct IPickup : public sf::Drawable
    {
        virtual ~IPickup() = default;

        virtual void changeActor(IActor & actor) = 0;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;

        virtual BoardPos_t boardPos() const = 0;

        virtual int value() const = 0;
    };

    using IPickupUPtr_t = std::unique_ptr<IPickup>;

    //

    struct ActorContext
    {
        ActorContext(
            const Random & rand,
            const std::vector<IPickupUPtr_t> & pups,
            const std::vector<IActorUPtr_t> & acts,
            const DisplayConstants & disp)
            : random(rand)
            , actors(acts)
            , pickups(pups)
            , display(disp)
        {}

        const Random & random;
        const std::vector<IActorUPtr_t> & actors;
        const std::vector<IPickupUPtr_t> & pickups;
        const DisplayConstants & display;

        // TODO Nell:
        //  What is the same about these two functions?
        //  What is different?
        //  What opportunity or other-way-to-write the code does that reveal?
        //  Would that be simpler/more-complex/faster/slower/better/worse?
        bool isActorAtBoardPos(const BoardPos_t & posToCheck) const
        {
            // clang-format off
            //const auto foundIter =
            //    std::find(
            //                std::begin(actors),
            //                std::end(actors),
            //                [&](const IActorUPtr_t & actor)
            //                {
            //                    return (actor->boardPos() == posToCheck);
            //                }
            //             );
            // clang-format on

            const auto foundIter =
                std::find_if(std::begin(actors), std::end(actors), [&](const IActorUPtr_t & actor) {
                    return (actor->boardPos() == posToCheck);
                });

            return (foundIter != std::end(actors));
        }
        //
        bool isPickupAtBoardPos(const BoardPos_t & posToCheck) const
        {
            // clang-format off
            //const auto foundIter =
            //    std::find(
            //                std::begin(pickups),
            //                std::end(pickups),
            //                [&](const IPickupUPtr_t & pickup)
            //                {
            //                    return (pickup->boardPos() == posToCheck);
            //                }
            //             );
            // clang-format on

            const auto foundIter = std::find_if(
                std::begin(pickups), std::end(pickups), [&](const IPickupUPtr_t & pickup) {
                    return (pickup->boardPos() == posToCheck);
                });

            return (foundIter != std::end(pickups));
        }

        bool isEitherAtBoardPos(const BoardPos_t & posToCheck) const
        {
            return (isActorAtBoardPos(posToCheck) || (isPickupAtBoardPos(posToCheck)));
        }
    };

    //

    class Loot : public IPickup
    {
      public:
        Loot(
            const std::string & imagePath,
            const BoardPos_t & boardPos,
            const sf::FloatRect & windowRect,
            const int value,
            const DisplayConstants & displayConstants)
            : m_texture()
            , m_sprite()
            , m_value(value)
            , m_boardPos(boardPos)
            , m_text(displayConstants.default_text)
        {
            if (!m_texture.loadFromFile(imagePath))
            {
                throw std::runtime_error("Unable to load image: " + imagePath);
            }

            m_sprite.setTexture(m_texture, true);
            fit(m_sprite, windowRect);
            m_sprite.setColor(sf::Color(255, 255, 255, 127));

            m_text.setFillColor(sf::Color::Yellow);
            m_text.setString(std::to_string(m_value));
            fit(m_text, windowRect);
        }

        virtual ~Loot() = default;

        void changeActor(IActor & actor) override { actor.score(actor.score() + m_value); }

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            target.draw(m_sprite, states);
            target.draw(m_text, states);
        }

        BoardPos_t boardPos() const final { return m_boardPos; }

        int value() const final { return m_value; }

      private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        int m_value;
        BoardPos_t m_boardPos;
        sf::Text m_text;
    };

    //

    class MethHeadBase : public IActor
    {
      protected:
        MethHeadBase(
            const std::string & imagePath,
            const BoardPos_t & boardPos,
            const sf::FloatRect & windowRect,
            const float waitBetweenActionsSec = 0.333f);

        virtual ~MethHeadBase() = default;

      public:
        void draw(sf::RenderTarget & target, sf::RenderStates states) const final;

        // returns true if the boardPos changed
        bool update(const float elapsedSec, const ActorContext & context) override;

        inline int score() const final { return m_score; }
        inline void score(const int value) final { m_score = value; }

        inline BoardPos_t boardPos() const final { return m_boardPos; }

      protected:
        bool isTimeToMove(const float elapsedSec);

        // returns true if position changed
        bool move(const ActorContext & context);

        inline int walkDistanceTo(const BoardPos_t & to) const
        {
            const BoardPos_t posDiff(to - m_boardPos);
            return (std::abs(posDiff.x) + std::abs(posDiff.y));
        }

        // returns the current position if there are no pickups
        virtual BoardPos_t findMostDesiredPickupBoardPos(const ActorContext & context) const = 0;

        std::vector<BoardPos_t> makeAllBoardPosMovesToward(const BoardPos_t & targetBoardPos) const;

      private:
        int m_score;
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        BoardPos_t m_boardPos;

        float m_waitBetweenActionsSec;
        float m_elapsedSinceLastActionSec;
    };

    //

    class Lazy : public MethHeadBase
    {
      public:
        Lazy(
            const std::string & imagePath,
            const BoardPos_t & boardPos,
            const sf::FloatRect & windowRect)
            : MethHeadBase(imagePath, boardPos, windowRect)
        {}

        virtual ~Lazy() = default;

        inline Motivation motivation() const final { return Motivation::lazy; }

      private:
        BoardPos_t findMostDesiredPickupBoardPos(const ActorContext & context) const final
        {
            if (context.pickups.empty())
            {
                return boardPos();
            }

            std::size_t mostDesiredIndex(0);
            int mostDesiredDistance(std::numeric_limits<int>::max());

            for (std::size_t i(0); i < context.pickups.size(); ++i)
            {
                const IPickup & pickup(*context.pickups.at(i));
                const int distance{ walkDistanceTo(pickup.boardPos()) };

                if (distance < mostDesiredDistance)
                {
                    mostDesiredDistance = distance;
                    mostDesiredIndex = i;
                }
            }

            assert(mostDesiredDistance > 0);

            return context.pickups.at(mostDesiredIndex)->boardPos();
        }
    };

    //

    class Greedy : public MethHeadBase
    {
      public:
        Greedy(
            const std::string & imagePath,
            const BoardPos_t & boardPos,
            const sf::FloatRect & windowRect)
            : MethHeadBase(imagePath, boardPos, windowRect)
        {}

        virtual ~Greedy() = default;

        inline Motivation motivation() const final { return Motivation::greedy; }

      private:
        BoardPos_t findMostDesiredPickupBoardPos(const ActorContext & context) const final
        {
            if (context.pickups.empty())
            {
                return boardPos();
            }

            std::vector<std::pair<int, BoardPos_t>> valuePositions;
            for (const IPickupUPtr_t & pickup : context.pickups)
            {
                valuePositions.push_back({ pickup->value(), pickup->boardPos() });
            }

            assert(valuePositions.size() == context.pickups.size());
            assert(!valuePositions.empty());

            std::sort(std::begin(valuePositions), std::end(valuePositions));

            assert(valuePositions.front().first <= valuePositions.back().first);

            return valuePositions.back().second;
        }
    };
} // namespace methhead

#endif // METHHEADS_BASE_HPP_INCLUDED