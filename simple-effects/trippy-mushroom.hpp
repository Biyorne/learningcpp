#ifndef SIMPLE_EFFECTS_TRIPPY_MUSHROOM_HPP_INCLUDED
#define SIMPLE_EFFECTS_TRIPPY_MUSHROOM_HPP_INCLUDED

#include "context.hpp"
#include "effect-base.hpp"

#include <array>

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

#endif // SIMPLE_EFFECTS_TRIPPY_MUSHROOM_HPP_INCLUDED
