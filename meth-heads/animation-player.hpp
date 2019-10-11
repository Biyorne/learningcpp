#ifndef METHHEADS_ANIMATION_HPP_INCLUDED
#define METHHEADS_ANIMATION_HPP_INCLUDED
//
// animation-player.hpp
//
#include "random.hpp"

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
        struct ParsedDirectoryName
        {
            std::string name;
            sf::Vector2i frame_size;
        };

        struct Image
        {
            std::string filename;
            sf::Texture texture;
            std::vector<sf::IntRect> rects;
        };

        struct ImageCache
        {
            std::string toString() const;

            std::string animation_name;
            sf::Vector2f frame_size;
            std::size_t frame_count = 0;
            std::vector<Image> images;
        };

        struct Animation
        {
            bool is_finished = false;
            sf::Sprite sprite;
            std::size_t cache_index = 0;
            std::size_t frame_index = 0;
            float sec_elapsed = 0.0f;
            float sec_per_frame = 0.0f;
        };

    public:
        AnimationPlayer();

        void play(
            const Random & random,
            const std::string & name,
            const sf::Vector2f & pos,
            const sf::Vector2f & size,
            const sf::Color & color = sf::Color::White,
            const float secPerFrame = m_defaultSecPerFrame);

        void update(const float elapsedTimeSec);

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

    private:
        void loadAnimationDirectories();

        bool willLoadAnimationDirectory(
            const std::filesystem::directory_entry & dirEntry, ParsedDirectoryName & parse) const;

        void loadAnimationDirectory(
            const std::filesystem::directory_entry & dirEntry, const ParsedDirectoryName & parse);

        //

        bool loadAnimationImages(
            const std::filesystem::directory_entry & dirEntry,
            const sf::Vector2i & frameSize,
            std::vector<Image> & images) const;

        bool willLoadAnimationImage(const std::filesystem::directory_entry & fileEntry) const;

        bool loadAnimationImage(
            const std::filesystem::directory_entry & fileEntry,
            const sf::Vector2i & frameSize,
            std::vector<Image> & images) const;

        //

        void createAndStartPlaying(
            const std::size_t cacheIndex,
            const sf::Vector2f & pos,
            const sf::Vector2f & size,
            const sf::Color & color,
            const float secPerFrame);

        ParsedDirectoryName parseDirectoryName(const std::string & name) const;

        Animation & getAvailableAnimation();

        void updateAnimation(Animation & anim, const float elapsedTimeSec) const;

        void updateSprite(
            sf::Sprite & sprite, const ImageCache & imageCache, const std::size_t frameIndex) const;

        std::vector<std::size_t> findNameMatchingIndexes(const std::string & name) const;

    private:
        std::vector<Animation> m_animations;
        std::vector<std::unique_ptr<ImageCache>> m_imageCaches;
        std::string m_fileExtensions;

        static inline float m_defaultSecPerFrame = 0.15f;
    };

} // namespace methhead

#endif // METHHEADS_ANIMATION_HPP_INCLUDED
