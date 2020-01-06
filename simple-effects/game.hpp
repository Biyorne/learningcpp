#ifndef BULLET_HELL_GAME_HPP_INCLUDED
#define BULLET_HELL_GAME_HPP_INCLUDED

#include "sound-player.hpp"

#include <array>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//
// About vectors
//  Two types of vectors. Euclid uses x and y, polar uses angle and magnitude.
//  All vectors of both kinds have a hypotenuse that is often called the magnitude.
//  A normalized vector (sometimes called a unit vector) has a magnitude of 1.
//  To normalize a vector, simply divide it by its magnitude.
//  Velocity is a change to position. The hypotenuse can be magnitude or speed.

namespace util
{
    inline float right(const sf::FloatRect & rect) { return (rect.left + rect.width); }

    inline float bottom(const sf::FloatRect & rect) { return (rect.top + rect.height); }

    inline void setOrigin2Center(sf::Sprite & sprite)
    {
        sprite.setOrigin(
            (sprite.getLocalBounds().width * 0.5f), (sprite.getLocalBounds().height * 0.5f));
    }

} // namespace util

struct Mover
{
    explicit Mover(
        const sf::Vector2f & pos = sf::Vector2f(0.0f, 0.0f),
        const sf::Vector2f & vel = sf::Vector2f(0.0f, 0.0f),
        const sf::Vector2f & acc = sf::Vector2f(0.0f, 0.0f))
        : position(pos)
        , velocity(vel)
        , acceleration(acc)
    {}

    void update()
    {
        velocity += acceleration;
        position += velocity;
    }

    sf::Vector2f position;     // the result we wanted in the first place
    sf::Vector2f velocity;     // change to position
    sf::Vector2f acceleration; // changes to velocity
};

//

struct Context
{
    Context(const sf::RenderWindow & window, util::Random & rdm, util::SoundPlayer & aud)
        : random(rdm)
        , audio(aud)
        , window_size(window.getSize())
        , window_rect({}, window_size)
    {}

    util::Random & random;
    util::SoundPlayer & audio;
    sf::Vector2f window_size;
    sf::FloatRect window_rect;
};

//

struct EffectBase : public sf::Drawable
{
    explicit EffectBase(const Mover & mov = Mover(), const bool willFen = false)
        : sprite()
        , mover(mov)
        , will_fence(willFen)
        , will_draw_again_add(false)
    {}

    explicit EffectBase(
        const sf::Texture & tex, const Mover & mov = Mover(), const bool willFen = false)
        : EffectBase(mov, willFen)
    {
        sprite.setTexture(tex, true);
        util::setOrigin2Center(sprite);
    }

    virtual ~EffectBase() = default;

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override
    {
        target.draw(sprite, states);

        if (will_draw_again_add)
        {
            target.draw(sprite, sf::BlendAdd);
        }
    }

    virtual void update(const float, const Context & context)
    {
        mover.update();
        sprite.setPosition(mover.position);
        handleFencing(context);
    }

    void handleFencing(const Context & context)
    {
        if (!will_fence)
        {
            return;
        }

        const sf::Vector2f posCorr{ calcFencePosCorrection(
            sprite.getGlobalBounds(), context.window_rect) };

        sprite.move(posCorr);

        if ((posCorr.x > 0.0f) || (posCorr.x < 0.0f))
        {
            mover.velocity.x *= -1.0f;
        }

        if ((posCorr.y > 0.0f) || (posCorr.y < 0.0f))
        {
            mover.velocity.y *= -1.0f;
        }
    }

    static sf::Vector2f
        calcFencePosCorrection(const sf::FloatRect & bounds, const sf::FloatRect & fence)
    {
        sf::Vector2f posAdj(0.0f, 0.0f);

        if (bounds.left < fence.left)
        {
            posAdj.x += (fence.left - bounds.left);
        }

        if (bounds.top < fence.top)
        {
            posAdj.y += (fence.top - bounds.top);
        }

        const float objRight{ util::right(bounds) };
        const float fenceRight{ util::right(fence) };
        if (objRight > fenceRight)
        {
            posAdj.x -= (objRight - fenceRight);
        }

        const float objBottom{ util::bottom(bounds) };
        const float fenceBottom{ util::bottom(fence) };
        if (objBottom > fenceBottom)
        {
            posAdj.y -= (objBottom - fenceBottom);
        }

        return posAdj;
    }

    bool will_draw_again_add;
    bool will_fence;
    sf::Sprite sprite;
    Mover mover;
};

//

struct WallBouncerEffect : public EffectBase
{
    WallBouncerEffect(const sf::Texture & tex, const Mover & mov)
        : EffectBase(tex, mov, true)
    {}

    virtual ~WallBouncerEffect() = default;
};

//

struct RisingFadeEffect : public EffectBase
{
    RisingFadeEffect()
        : EffectBase(Mover({}, { 0.0f, -1.5f }))
    {
        sprite.setScale(0.25f, 0.25f);
    }

    RisingFadeEffect(const sf::Texture & tex, const sf::Vector2f & pos)
        : EffectBase(tex, Mover(pos, { 0.0f, -1.5f }))
    {
        sprite.setScale(0.25f, 0.25f);
    }

    virtual ~RisingFadeEffect() = default;

    void update(const float elapsedTimeSec, const Context & context) override
    {
        EffectBase::update(elapsedTimeSec, context);
        sprite.setColor(sprite.getColor() - sf::Color(0, 0, 0, 8));
    }
};

//

struct TrippyEffect : public EffectBase
{
    TrippyEffect(const Context & context, const sf::Texture & tex)
        : EffectBase(tex)
        , rotate(context.random.fromTo(0.01f, 10.0f))
        , color(makeRandomColor(context))
    {
        sprite.setColor(color);
        sprite.setScale(0.025f, 0.025f);
    }

    virtual ~TrippyEffect() = default;

    void update(const float, const Context & context) override
    {
        sprite.setColor(sprite.getColor() - sf::Color(0, 0, 0, 1));
        sprite.scale(1.1f, 1.1f);
        sprite.rotate(rotate);
        sprite.setPosition(context.window_size * 0.5f);
    }

    sf::Color makeRandomColor(const Context & context)
    {
        return sf::Color(
            static_cast<unsigned char>(context.random.zeroTo<int>(255)),
            static_cast<unsigned char>(context.random.zeroTo<int>(255)),
            static_cast<unsigned char>(context.random.zeroTo<int>(255)));
    }

    float rotate;
    sf::Color color;
};

//

struct TrippyMushrooms : public sf::Drawable
{
    TrippyMushrooms()
        : textures()
        , time_until_sec(0.75f)
        , time_waiting_sec(0.0f)
    {
        for (std::size_t i(0); i < textures.size(); ++i)
        {
            sf::Texture & texture(textures.at(i));

            const std::string path(
                "C:/src/learningcpp/media/image/mushroom-" + std::to_string(i + 1) + ".png");

            if (!texture.loadFromFile(path))
            {
                std::cout << "Unable to load texure: " << path << std::endl;
            }
        }
    }

    void draw(sf::RenderTarget & target, sf::RenderStates states) const override
    {
        for (const auto & effect : effects)
        {
            target.draw(effect, states);
        }
    }

    virtual void update(const float elapsedTimeSec, const Context & context)
    {
        time_waiting_sec += elapsedTimeSec;

        const bool willSpawn(time_waiting_sec > time_until_sec);
        if (willSpawn)
        {
            time_waiting_sec = 0.0f;
            time_until_sec = context.random.fromTo(0.1f, 0.25f);
        }

        for (TrippyEffect & effect : effects)
        {
            effect.will_draw_again_add = true;
            if (effect.sprite.getColor().a > 0)
            {
                effect.update(elapsedTimeSec, context);
            }
        }

        if (willSpawn)
        {
            effects.push_back(TrippyEffect(context, textures.front()));
        }
    }

    std::array<sf::Texture, 4> textures;
    std::vector<TrippyEffect> effects;

    float time_until_sec;
    float time_waiting_sec;
};

//

class Game
{
  public:
    Game();

    void run();

  private:
    void processEvents();
    void update(const float frameTimeSec);
    void render();

    void handleKeyPress(const sf::Keyboard::Key key);

    // void scrollValue(float amount);

    void reset();

  private:
    sf::RenderWindow m_window;
    sf::Texture m_bgTexture;
    sf::Texture m_warnTexture;
    sf::RenderStates m_states;
    bool m_willClear;
    sf::Vector2f m_windowSize;

    sf::Sprite m_bgSprite;
    float m_bgRotateSpeed;
    sf::Music m_music;
    util::Random m_random;
    util::SoundPlayer m_audio;

    Context m_context;
    RisingFadeEffect m_risingEffect;

    TrippyMushrooms m_trippyMushrooms;
};

#endif // BULLET_HELL_GAME_HPP_INCLUDED
