// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "animation-player.hpp"

#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace methhead
{
    AnimationPlayer::AnimationPlayer()
        : m_animations()
        , m_imageCaches()
        , m_fileExtensions(".bmp/.jpg/.jpeg/.png/.tga")
    {
        loadAnimationDirectories();
    }

    void AnimationPlayer::play(
        const Random & random,
        const std::string & name,
        const sf::Vector2f & pos,
        const sf::Vector2f & size,
        const sf::Color & color,
        const float secPerFrame)
    {
        if (m_imageCaches.empty() || name.empty())
        {
            return;
        }

        const std::vector<std::size_t> nameMatchingIndexes(findNameMatchingIndexes(name));
        if (nameMatchingIndexes.empty())
        {
            std::cerr << "AnimationPlayer Error:  .play(\"" << name
                      << "\") called, but none had that name." << std::endl;

            return;
        }

        createAndStartPlaying(random.from(nameMatchingIndexes), pos, size, color, secPerFrame);
    }

    void AnimationPlayer::update(const float elapsedTimeSec)
    {
        for (Animation & anim : m_animations)
        {
            updateAnimation(anim, elapsedTimeSec);
        }
    }

    void AnimationPlayer::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const Animation & anim : m_animations)
        {
            if (!anim.is_finished)
            {
                target.draw(anim.sprite, states);
            }
        }
    }

    void AnimationPlayer::loadAnimationDirectories()
    {
        std::filesystem::recursive_directory_iterator dirIter(
            std::filesystem::current_path(),
            std::filesystem::directory_options::skip_permission_denied);

        for (const std::filesystem::directory_entry & entry : dirIter)
        {
            ParsedDirectoryName parse;

            if (willLoadAnimationDirectory(entry, parse))
            {
                loadAnimationDirectory(entry, parse);
            }
        }

        if (m_imageCaches.empty())
        {
            std::cerr
                << "AnimationPlayer Error:  No valid animation directories were found.  Supported "
                   "image file types: "
                << m_fileExtensions << std::endl;
        }
    }

    bool AnimationPlayer::willLoadAnimationDirectory(
        const std::filesystem::directory_entry & dirEntry, ParsedDirectoryName & parse) const
    {
        parse = parseDirectoryName(dirEntry.path().filename().string());
        return (!parse.name.empty() && (parse.frame_size.x > 0) && (parse.frame_size.y > 0));
    }

    void AnimationPlayer::loadAnimationDirectory(
        const std::filesystem::directory_entry & dirEntry, const ParsedDirectoryName & parse)
    {
        auto imageCache { std::make_unique<ImageCache>() };
        imageCache->animation_name = parse.name;
        imageCache->frame_size = sf::Vector2f(parse.frame_size);

        if (!loadAnimationImages(dirEntry, parse.frame_size, imageCache->images))
        {
            return;
        }

        imageCache->frame_count = 0;
        for (const auto & image : imageCache->images)
        {
            imageCache->frame_count += image.rects.size();
        }

        std::cout << "Loaded Animation: " << imageCache->toString() << std::endl;
        m_imageCaches.push_back(std::move(imageCache));
    }

    bool AnimationPlayer::loadAnimationImages(
        const std::filesystem::directory_entry & dirEntry,
        const sf::Vector2i & frameSize,
        std::vector<Image> & images) const
    {
        std::filesystem::directory_iterator dirIter(
            dirEntry.path(), std::filesystem::directory_options::skip_permission_denied);

        for (const std::filesystem::directory_entry & fileEntry : dirIter)
        {
            if (willLoadAnimationImage(fileEntry))
            {
                if (!loadAnimationImage(fileEntry, frameSize, images))
                {
                    return false;
                }
            }
        }

        if (images.empty())
        {
            std::cerr
                << "AnimationPlayer Error:  Found a directory that is named like an animation "
                   "directory here: \""
                << dirEntry.path().string() << "\", but was unable to load any images from it."
                << std::endl;

            return false;
        }

        // directory iterators do not always go in alphanumeric order, so sort here just in case
        std::sort(std::begin(images), std::end(images), [](const auto & left, const auto & right) {
            return (left.filename < right.filename);
        });

        return true;
    }

    bool AnimationPlayer::willLoadAnimationImage(
        const std::filesystem::directory_entry & fileEntry) const
    {
        if (!fileEntry.is_regular_file())
        {
            return false;
        }

        const std::string fileName(fileEntry.path().filename().string());
        if (fileName.empty())
        {
            return false;
        }

        const std::string fileExt(fileEntry.path().filename().extension().string());
        if ((fileExt.size() != 4) && (fileExt.size() != 5))
        {
            return false;
        }

        return (m_fileExtensions.find(fileExt) < m_fileExtensions.size());
    }

    bool AnimationPlayer::loadAnimationImage(
        const std::filesystem::directory_entry & fileEntry,
        const sf::Vector2i & frameSize,
        std::vector<Image> & images) const
    {
        Image image;

        if (!image.texture.loadFromFile(fileEntry.path().string()))
        {
            std::cerr << "AnimationPlayer Error:  Found a supported file: \""
                      << fileEntry.path().string() << "\", but an error occurred while loading it."
                      << std::endl;

            return false;
        }

        const sf::Vector2i imageSize(image.texture.getSize());

        for (int vert(0); vert < imageSize.y; vert += frameSize.y)
        {
            for (int horiz(0); horiz < imageSize.x; horiz += frameSize.x)
            {
                image.rects.push_back({ sf::Vector2i(horiz, vert), frameSize });
            }
        }

        if (image.rects.empty())
        {
            std::cerr << "AnimationPlayer Error:  Found a supported file: \""
                      << fileEntry.path().string() << "\", but no frame rects could be established."
                      << std::endl;

            return false;
        }

        images.push_back(std::move(image));

        return true;
    }

    void AnimationPlayer::createAndStartPlaying(
        const std::size_t cacheIndex,
        const sf::Vector2f & pos,
        const sf::Vector2f & size,
        const sf::Color & color,
        const float secPerFrame)
    {
        Animation & anim(getAvailableAnimation());

        anim.is_finished = false;
        anim.cache_index = cacheIndex;
        anim.frame_index = 0;
        anim.sec_elapsed = 0.0f;
        anim.sec_per_frame = secPerFrame;

        anim.sprite.setPosition(pos);
        anim.sprite.setColor(color);

        const Image & firstImage(m_imageCaches.at(cacheIndex)->images.at(0));
        anim.sprite.setTexture(firstImage.texture);

        const sf::IntRect rect { firstImage.rects.at(0) };
        anim.sprite.setTextureRect(rect);

        const float horizScale { size.x / static_cast<float>(rect.width) };
        const float vertScale { size.y / static_cast<float>(rect.height) };
        anim.sprite.setScale(horizScale, vertScale);
    }

    AnimationPlayer::ParsedDirectoryName
        AnimationPlayer::parseDirectoryName(const std::string & name) const
    {
        const auto size(name.size());
        const auto dashIndex(name.rfind('-'));
        const auto xIndex(name.rfind('x'));

        if ((0 == dashIndex) || (dashIndex >= size) || (xIndex >= size) || (xIndex <= dashIndex))
        {
            return {};
        }

        try
        {
            const std::string animName(name.substr(0, dashIndex));
            const std::string widthStr(name.substr((dashIndex + 1), (dashIndex - xIndex - 1)));
            const std::string heightStr(name.substr(xIndex + 1));

            const int width(std::stoi(widthStr));
            const int height((heightStr.empty()) ? width : std::stoi(heightStr));

            return { animName, sf::Vector2i(width, height) };
        }
        catch (...)
        {}

        return {};
    }

    AnimationPlayer::Animation & AnimationPlayer::getAvailableAnimation()
    {
        for (Animation & anim : m_animations)
        {
            if (anim.is_finished)
            {
                return anim;
            }
        }

        return m_animations.emplace_back();
    }

    std::vector<std::size_t>
        AnimationPlayer::findNameMatchingIndexes(const std::string & name) const
    {
        std::vector<std::size_t> indexes;

        for (std::size_t i(0); i < m_imageCaches.size(); ++i)
        {
            if (startsWith(m_imageCaches.at(i)->animation_name, name))
            {
                indexes.push_back(i);
            }
        }

        return indexes;
    }

    void AnimationPlayer::updateAnimation(Animation & anim, const float elapsedTimeSec) const
    {
        if (anim.is_finished)
        {
            return;
        }

        anim.sec_elapsed += elapsedTimeSec;

        const std::size_t newFrameIndex(
            static_cast<std::size_t>(anim.sec_elapsed / anim.sec_per_frame));

        if (newFrameIndex == anim.frame_index)
        {
            return;
        }

        const auto & imageCache { m_imageCaches.at(anim.cache_index) };

        if (newFrameIndex > imageCache->frame_count)
        {
            anim.is_finished = true;
            return;
        }

        anim.frame_index = newFrameIndex;

        updateSprite(anim.sprite, *imageCache, anim.frame_index);
    }

    void AnimationPlayer::updateSprite(
        sf::Sprite & sprite, const ImageCache & imageCache, const std::size_t frameIndex) const
    {
        std::size_t frameCounter { 0 };

        for (const Image & image : imageCache.images)
        {
            for (const sf::IntRect & rect : image.rects)
            {
                if (frameIndex == frameCounter)
                {
                    sprite.setTexture(image.texture);
                    sprite.setTextureRect(rect);
                    return;
                }

                ++frameCounter;
            }
        }
    }

    std::string AnimationPlayer::ImageCache::toString() const
    {
        const std::string pad("  ");

        std::ostringstream ss;

        ss << std::setw(14) << std::right << animation_name;
        ss << pad;

        ss << std::setw(3) << std::right << static_cast<int>(frame_size.x);
        ss << "x";
        ss << std::setw(3) << std::left << static_cast<int>(frame_size.y);
        ss << pad;

        ss << "x" << frame_count;
        return ss.str();
    }

} // namespace methhead
