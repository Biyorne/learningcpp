#ifndef METHHEADS_BASE_HPP_INCLUDED
#define METHHEADS_BASE_HPP_INCLUDED

#include "animation-player.hpp"
#include "display-constants.hpp"
#include "random.hpp"
#include "sound-player.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace methhead
{
    // TODO in the end of all cleanup this should have a different home...
    //
    // also... TODO In the end, after cleaning up everything, there should be no more need for
    // "none" none/invalid options in enums is not always code smell, it is not always wrong, but on
    // the other hand, always adding none/invalid to your enums as a default/habit just in case it
    // is needed IS ALWAYS A BAD HABIT, becauase any enum that has a none/invalid option that is not
    // needed is often a huge source of errors and mess.
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

        virtual void update(
            const float elapsedMs,
            SoundPlayer & soundPlayer,
            const Random & random,
            AnimationPlayer & animationPlayer)
            = 0;
    };

    using IActorUPtr_t = std::unique_ptr<IActor>;
    using IActorUVec_t = std::vector<IActorUPtr_t>;

    //

    struct IPickup : public sf::Drawable
    {
        virtual ~IPickup() = default;

        virtual void changeActor(IActor & actor) = 0;

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;

        virtual void update(
            const float elapsedMs,
            SoundPlayer & soundPlayer,
            const Random & random,
            AnimationPlayer & animationPlayer)
            = 0;

        virtual BoardPos_t boardPos() const = 0;

        virtual int value() const = 0;
    };

    using IPickupUPtr_t = std::unique_ptr<IPickup>;
    using IPickupUVec_t = std::vector<IPickupUPtr_t>;

    //

    class Loot : public IPickup
    {
    public:
        Loot(
            const std::string & imagePath,
            const BoardPos_t & boardPos,
            const sf::FloatRect & windowBounds,
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
            placeInBounds(m_sprite, windowBounds);
            m_sprite.setColor(sf::Color(255, 255, 255, 127));

            m_text.setFillColor(sf::Color::Yellow);
            m_text.setString(std::to_string(m_value));
            placeInBounds(m_text, windowBounds);
        }

        virtual ~Loot() = default;

        void changeActor(IActor & actor) override { actor.score(actor.score() + m_value); }

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override
        {
            target.draw(m_sprite, states);
            target.draw(m_text, states);
        }

        void update(const float, SoundPlayer &, const Random &, AnimationPlayer &) override {}

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
            const sf::FloatRect & windowBounds,
            const float waitBetweenActionsSec = 0.333f);

        virtual ~MethHeadBase() = default;

    public:
        void draw(sf::RenderTarget & target, sf::RenderStates states) const final;

        void update(
            const float elapsedSec,
            SoundPlayer & soundPlayer,
            const Random & random,
            AnimationPlayer & animationPlayer) override;

        int score() const final { return m_score; }
        void score(const int value) final { m_score = value; }

        BoardPos_t boardPos() const final { return m_boardPos; }

    protected:
        void moveToward(
            SoundPlayer & soundPlayer,
            const Random & random,
            AnimationPlayer & animationPlayer,
            const BoardPos_t & targetBoardPos);

        static inline int calcDistance(const BoardPos_t & from, const BoardPos_t & to)
        {
            return (std::abs(to.x - from.x) + std::abs(to.y - from.y));
        }

        BoardPos_t pickTarget() const;

        virtual void sortPickups(IPickupUVec_t & pickups) const = 0;

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
            const sf::FloatRect & windowBounds)
            : MethHeadBase(imagePath, boardPos, windowBounds)
        {}

        virtual ~Lazy() = default;

        Motivation motivation() const final { return Motivation::lazy; }

    private:
        void sortPickups(IPickupUVec_t & pickups) const final
        {
            std::sort(
                std::begin(pickups),
                std::end(pickups),
                [&](const IPickupUPtr_t & left, const IPickupUPtr_t & right) {
                    const int distLeft(calcDistance(boardPos(), left->boardPos()));
                    const int distRight(calcDistance(boardPos(), right->boardPos()));

                    return (distLeft < distRight);
                });
        }
    };

    //

    class Greedy : public MethHeadBase
    {
    public:
        Greedy(
            const std::string & imagePath,
            const BoardPos_t & boardPos,
            const sf::FloatRect & windowBounds)
            : MethHeadBase(imagePath, boardPos, windowBounds)
        {}

        virtual ~Greedy() = default;

        Motivation motivation() const final { return Motivation::greedy; }

    private:
        void sortPickups(IPickupUVec_t & pickups) const final
        {
            std::sort(
                std::begin(pickups),
                std::end(pickups),
                [&](const IPickupUPtr_t & left, const IPickupUPtr_t & right) {
                    return (left->value() > right->value());
                });
        }
    };

} // namespace methhead

#endif // METHHEADS_BASE_HPP_INCLUDED
