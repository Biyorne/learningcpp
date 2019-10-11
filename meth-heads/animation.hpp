#ifndef METHHEADS_ANIMATION_HPP_INCLUDED
#define METHHEADS_ANIMATION_HPP_INCLUDED
//
// animation.hpp
//
#include "random.hpp"
#include "utils.hpp"

#include <filesystem>
#include <memory>
#include <tuple>
#include <vector>

#include <SFML/Graphics.hpp>

namespace methhead
{
    // Animations are simply directories with one or more images that each contain one or more
    // frames.  The images must be named so that simple alpha-numeric sorting works.  Supported
    // image file types are: bmp, jpg, png, and tga.  Directorty names must be formatted
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
    // ...then one call to play("addictive-carcinogenic-product-advertisement")
    // will pick one for you at random.  You know, if you can't decide since their all legal.
    //
    class AnimationPlayer : public sf::Drawable
    {
        // a single image with one rect per frame
        struct Image
        {
            sf::Texture texture;
            std::vector<sf::IntRect> rects;
        };

        // the name and images of an animation
        struct Cache
        {
            std::string toString() const;

            std::string name;
            sf::Vector2f frame_size;
            std::vector<Image> images;
        };

        using CacheUPtr_t = std::unique_ptr<Cache>;

        // a visiable animation made unique by its pos, size, and time elapsed
        struct Animation
        {
            sf::Sprite sprite;
            std::size_t cache_index = 0;
            float sec_elapsed = 0.0f;
            float sec_per_frame = 0.0f;
            bool is_finished_playing = false;
        };

    public:
        AnimationPlayer(Random & random);

        void setup();

        void play(
            const std::string & name,
            const sf::Vector2f & pos,
            const sf::Vector2f & size,
            const sf::Color & color = sf::Color::White,
            const float secPerFrame = m_defaultSecPerFrame);

        void update(const float elapsedTimeSec);
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

    private:
        void loadDirectories();
        std::tuple<std::string, sf::Vector2i> parseDirectoryName(const std::string & name) const;

        void loadDirectory(
            const std::filesystem::path & path,
            const std::string & name,
            const sf::Vector2i & frameSize);

        std::vector<std::filesystem::path> findImageFiles(const std::filesystem::path & path) const;

        bool willLoadImageFile(const std::filesystem::directory_entry & entry) const;

        Image loadImage(const std::filesystem::path & path, const sf::Vector2i & frameSize) const;

        Animation & findOrMakeAvailableAnimation();

        void startAnimation(
            Animation & anim,
            const std::size_t cacheIndex,
            const sf::Vector2f & pos,
            const sf::Vector2f & size,
            const sf::Color & color,
            const float secPerFrame);

        void setFrame(Animation & anim, const float elapsedTimeSec);

    private:
        Random & m_random;
        std::vector<Animation> m_animations;
        std::vector<CacheUPtr_t> m_imageCaches;
        std::string m_fileExtensions;

        static inline float m_defaultSecPerFrame = 0.05f;
    };

} // namespace methhead

#endif // METHHEADS_ANIMATION_HPP_INCLUDED

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
