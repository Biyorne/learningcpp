#ifndef METHHEADS_ANIMATION_HPP_INCLUDED
#define METHHEADS_ANIMATION_HPP_INCLUDED

#include <filesystem>

#include <SFML/Graphics.hpp>

namespace methhead
{
    // Animations are simply directories with one or more images that each contain one or more
    // frames.  The images must be named so that simple alpha-numeric sorting works.  Supported
    // image file types are: bmp, dds, jpg, png, tga, and psd.  Directorty names must be formatted
    // this way:   <name>-<width>x<height>
    //
    //  seizure-inducing-flashes-256x512
    //
    // The last number can be skipped if it's the same.
    //  immorally-suggestive-cartoon-128x
    //
    // If multiple animations directories start with the same name...
    //  addictive-carcinogenic-product-advertisement-for-kids-100x
    //  addictive-carcinogenic-product-advertisement-for-teens-100x
    //  addictive-carcinogenic-product-advertisement-for-liberals-100x
    //  addictive-carcinogenic-product-advertisement-for-conservatives-100x
    //  addictive-carcinogenic-product-advertisement-for-seniors-100x
    //
    // ...then one call to AnimationFactory::make("addictive-carcinogenic-product-advertisement")
    // will pick one for you at random.  You know, if you can't decide since their all legal.

    struct Frame
    {
        std::size_t index;
        sf::IntRect rect;
    };

    class AnimationWarehouse : public sf::Drawable
    {

    public:
    private:
        sf::Sprite m_sprite;
        std::size_t m_frameIndex;
        std::vector<Frame> m_frames;
        std::vector<sf::Texture> m_textures;

        /*
    public:
        Animation(const sf::Vector2f & windowSize)
        {
            const std::size_t count { 2 };
            const sf::Vector2f posAdj(windowSize / static_cast<float>(count));

            sf::Vector2f pos(0.0f, 0.0f);

            m_texture1.loadFromFile("image/head-1.png");
            m_sprite1.setTexture(m_texture1);
            m_sprite1.setPosition(pos);
            pos += posAdj;

            m_texture2.loadFromFile("image/head-2.png");
            m_sprite2.setTexture(m_texture2);
            m_sprite2.setPosition(pos);
            pos += posAdj;
        }

        void draw(sf::RenderTarget & target, sf::RenderStates) const override
        {
            target.draw(m_sprite1);
            target.draw(m_sprite2);
        }

        sf::Texture m_texture1;
        sf::Texture m_texture2;
        sf::Sprite m_sprite1;
        sf::Sprite m_sprite2;
        */
    };

} // namespace methhead

#endif // METHHEADS_ANIMATION_HPP_INCLUDED
