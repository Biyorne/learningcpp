// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "animation.hpp"

#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace methhead
{
    std::string AnimationPlayer::Cache::toString() const
    {
        std::ostringstream ss;

        ss << std::setw(22) << std::left << name;

        std::size_t frameCount(0);
        for (const Image & image : images)
        {
            frameCount += image.rects.size();
        }

        ss << " x" << frameCount;

        ss << " " << static_cast<int>(frame_size.x) << "x" << static_cast<int>(frame_size.y)
           << " frames";

        return ss.str();
    }

    AnimationPlayer::AnimationPlayer(Random & random)
        : m_random(random)
        , m_animations()
        , m_imageCaches()
        , m_supportedFileExtensions(".bmp/.jpg/.jpeg/.png/.tga")
    {}

    void AnimationPlayer::setup()
    {
        try
        {
            loadDirectories();
        }
        catch (const std::exception & ex)
        {
            std::cerr << "AnimationPlayer Error while trying to find and load directories: \""
                      << ex.what() << "\"" << std::endl;
        }

        if (m_imageCaches.empty())
        {
            std::cerr
                << "AnimationPlayer Error:  No valid animation directories were found.  Supported "
                   "image file types: "
                << m_supportedFileExtensions << m_supportedFileExtensions << std::endl;
        }
    }

    void AnimationPlayer::play(
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

        std::vector<std::size_t> nameMatchingIndexes;

        for (std::size_t i(0); i < m_imageCaches.size(); ++i)
        {
            if (startsWith(m_imageCaches.at(i)->name, name))
            {
                nameMatchingIndexes.push_back(i);
            }
        }

        if (nameMatchingIndexes.empty())
        {
            std::cerr << "AnimationPlayer Error:  .play(\"" << name
                      << "\") called, but none had that name." << std::endl;

            return;
        }

        startAnimation(
            findOrMakeAvailableAnimation(),
            m_random.from(nameMatchingIndexes),
            pos,
            size,
            color,
            secPerFrame);
    }

    AnimationPlayer::Animation & AnimationPlayer::findOrMakeAvailableAnimation()
    {
        for (Animation & anim : m_animations)
        {
            if (anim.is_finished_playing)
            {
                return anim;
            }
        }

        return m_animations.emplace_back();
    }

    void AnimationPlayer::startAnimation(
        Animation & anim,
        const std::size_t cacheIndex,
        const sf::Vector2f & pos,
        const sf::Vector2f & size,
        const sf::Color & color,
        const float secPerFrame)
    {
        anim.cache_index = cacheIndex;
        anim.is_finished_playing = false;
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

    void AnimationPlayer::update(const float elapsedTimeSec)
    {
        for (Animation & anim : m_animations)
        {
            if (!anim.is_finished_playing)
            {
                setFrame(anim, elapsedTimeSec);
            }
        }
    }

    void AnimationPlayer::setFrame(Animation & anim, const float elapsedTimeSec)
    {
        const Cache & cache { *m_imageCaches.at(anim.cache_index) };

        anim.sec_elapsed += elapsedTimeSec;

        const std::size_t frameIndexTarget(
            static_cast<std::size_t>(anim.sec_elapsed / anim.sec_per_frame));

        std::size_t frameIndexCounter { 0 };
        for (const Image & image : cache.images)
        {
            for (const sf::IntRect & rect : image.rects)
            {
                if (frameIndexCounter == frameIndexTarget)
                {
                    anim.sprite.setTexture(image.texture);
                    anim.sprite.setTextureRect(rect);
                    return;
                }

                ++frameIndexCounter;
            }
        }

        anim.is_finished_playing = true;
    }

    void AnimationPlayer::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        for (const Animation & anim : m_animations)
        {
            target.draw(anim.sprite, states);
        }
    }

    void AnimationPlayer::loadDirectories()
    {
        std::filesystem::recursive_directory_iterator dirIter(std::filesystem::current_path());

        for (const std::filesystem::directory_entry & entry : dirIter)
        {
            if (!entry.is_directory())
            {
                continue;
            }

            const auto [name, frameSize] = parseDirectoryName(entry.path().filename().string());

            if (name.empty() || (frameSize.x < 1) || (frameSize.y < 1))
            {
                continue;
            }

            try
            {
                loadDirectory(entry.path(), name, frameSize);
            }
            catch (const std::exception & ex)
            {
                std::cerr << "AnimationPlayer Error while trying to load directory: \""
                          << entry.path().string() << "\":  \"" << ex.what() << "\"" << std::endl;
            }
        }
    }

    std::tuple<std::string, sf::Vector2i>
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

    void AnimationPlayer::loadDirectory(
        const std::filesystem::path & path,
        const std::string & name,
        const sf::Vector2i & frameSize)
    {
        auto cacheUPtr = std::make_unique<Cache>();
        cacheUPtr->name = name;
        cacheUPtr->frame_size = sf::Vector2f(frameSize);

        for (const std::filesystem::path & imageFilePath : findImageFiles(path))
        {
            cacheUPtr->images.push_back(loadImage(imageFilePath, frameSize));
        }

        if (cacheUPtr->images.empty())
        {
            throw std::runtime_error(
                "AnimationPlayer Error:  Found a directory named like an animation: \""
                + path.string() + "\", but it could not be loaded as an animation.");
        }

        std::cout << "Loaded: " << cacheUPtr->toString() << std::endl;
        m_imageCaches.push_back(std::move(cacheUPtr));
    }

    std::vector<std::filesystem::path>
        AnimationPlayer::findImageFiles(const std::filesystem::path & dirPath) const
    {
        std::vector<std::filesystem::path> imagePaths;

        std::filesystem::recursive_directory_iterator dirIter(dirPath);
        for (const std::filesystem::directory_entry & entry : dirIter)
        {
            if (!willLoadImageFile(entry))
            {
                continue;
            }

            imagePaths.push_back(entry.path());
        }

        // directory iterators do not always go in alphanumeric order
        std::sort(std::begin(imagePaths), std::end(imagePaths), [](const auto & A, const auto & B) {
            return (A.filename().string() < B.filename().string());
        });

        return imagePaths;
    }

    bool AnimationPlayer::willLoadImageFile(const std::filesystem::directory_entry & entry) const
    {
        if (!entry.is_regular_file())
        {
            return false;
        }

        const std::string fileName(entry.path().filename().string());
        const std::string fileExt(entry.path().filename().extension().string());

        if (fileName.empty() || fileExt.empty())
        {
            return false;
        }

        return (m_supportedFileExtensions.find(fileExt) < m_supportedFileExtensions.size());
    }

    AnimationPlayer::Image AnimationPlayer::loadImage(
        const std::filesystem::path & path, const sf::Vector2i & frameSize) const
    {
        Image image;

        if (!image.texture.loadFromFile(path.string()))
        {
            throw std::runtime_error(
                "AnimationPlayer Error:  Found a supported file: \"" + path.string()
                + "\", but an error occurred while loading it.");
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
            throw std::runtime_error(
                "AnimationPlayer Error:  Found a supported file: \"" + path.string()
                + "\", but no frame rects could be established.");
        }

        return image;
    }

} // namespace methhead
