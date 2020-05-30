#ifndef SIMPLE_EFFECTS_RESOURCES_HPP_INCLUDED
#define SIMPLE_EFFECTS_RESOURCES_HPP_INCLUDED

#include <filesystem>
#include <iostream>
#include <memory>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <SFML/Graphics/Texture.hpp>
struct Resources
{
    using ImageVec_t = std::vector<std::unique_ptr<sf::Texture>>;
    using ImageVecIter_t = ImageVec_t::iterator;

    Resources()
    {
        // try this music:  "C:/src/learningcpp/media/music/trippy-shpongle.ogg"
        if (!music.openFromFile(""))
        {
            std::cout << "Unable to load music: trippy-shpongle" << std::endl;
        }
        else
        {
            music.setVolume(10.0f);
            music.play();
        }

        //
        const std::filesystem::path tileImagePath("C:/src/learningcpp/media/image/seamless");

        for (const std::filesystem::directory_entry & entry :
             std::filesystem::directory_iterator(tileImagePath))
        {
            if (!(entry.is_regular_file()))
            {
                continue;
            }

            std::unique_ptr<sf::Texture> textureUPtr(std::make_unique<sf::Texture>());

            if (!loadTexture(entry.path().string(), *textureUPtr))
            {
                std::cout << "ERROR LOADING: " << entry.path().filename().string() << std::endl;
                continue;
            }

            const sf::Vector2f localSize(textureUPtr->getSize());
            const float localDimmMaxSize(std::max(localSize.x, localSize.y));

            const float minDimmSize(100.0f);

            if (localDimmMaxSize < minDimmSize)
            {
                std::cout << "Background Image too small: " << entry.path().filename().string()
                          << ": local_dimm_max=" << localDimmMaxSize << " while min=" << minDimmSize
                          << std::endl;

                continue;
            }

            textureUPtr->setRepeated(true);
            seamless_textures.push_back(std::move(textureUPtr));
        }

        std::cout << "loaded this many images: " << seamless_textures.size() << std::endl;

        seamless_iter = std::begin(seamless_textures);

        //

        loadTexture("C:/src/learningcpp/media/image/warning.png", warn_texture);
        // loadTexture("C:/src/learningcpp/media/image/seamless/wood-dark-thin-1.jpg", bg_texture);
        loadTexture("C:/src/learningcpp/media/image/rabbit.png", rabbit_texture);
        loadTexture("C:/src/learningcpp/media/image/carrot.png", carrot_texture);
        loadTexture("C:/src/learningcpp/media/image/particle/fire-cloud.png", particle_texture);
        loadTexture("C:/src/learningcpp/media/image/ruby.jpg", exploder_texture);
        loadTexture("C:/src/learningcpp/media/image/backdrop/effluvium2.jpg", backdrop_texture);
        loadTexture(
            "C:/src/learningcpp/media/image/backdrop/effluvium-highlight.png", highlight_texture);

        if (!font.loadFromFile("C:/src/learningcpp/media/font/gentium-plus/gentium-plus.ttf"))
        {
            std::cerr << "Unable to load font: gentium-plus.ttf" << std::endl;
        }
    }

    bool loadTexture(const std::string & filePath, sf::Texture & texture)
    {
        if (texture.loadFromFile(filePath))
        {
            texture.setSmooth(true);
            return true;
        }
        else
        {
            std::cerr << "Unable to load texure: " << filePath << std::endl;
            return false;
        }
    }

    sf::Texture & bgTextureNext()
    {
        if (seamless_textures.empty())
        {
            seamless_iter = std::end(seamless_textures);
            return default_empty_texture;
        }

        if (std::end(seamless_textures) == seamless_iter)
        {
            seamless_iter = std::begin(seamless_textures);
        }
        else
        {
            seamless_iter++;
        }

        return bgTexture();
    }

    sf::Texture & bgTexturePrev()
    {
        if (seamless_textures.empty())
        {
            seamless_iter = std::end(seamless_textures);
            return default_empty_texture;
        }

        if (std::begin(seamless_textures) == seamless_iter)
        {
            std::advance(seamless_iter, (seamless_textures.size() - 1));
        }
        else
        {
            seamless_iter--;
        }

        return bgTexture();
    }

    sf::Texture & bgTexture()
    {
        if (seamless_textures.empty())
        {
            seamless_iter = std::end(seamless_textures);
            return default_empty_texture;
        }

        if (std::end(seamless_textures) == seamless_iter)
        {
            seamless_iter = std::begin(seamless_textures);
        }

        if (std::end(seamless_textures) == seamless_iter)
        {
            return default_empty_texture;
        }

        return **seamless_iter;
    }

    ImageVec_t seamless_textures;
    ImageVecIter_t seamless_iter{ std::end(seamless_textures) };

    sf::Texture bg_texture;
    sf::Texture warn_texture;
    sf::Texture rabbit_texture;
    sf::Texture carrot_texture;
    sf::Texture particle_texture;
    sf::Texture exploder_texture;
    sf::Texture backdrop_texture;
    sf::Texture highlight_texture;
    sf::Texture default_empty_texture;

    sf::Font font;
    sf::Music music;

    // std::vector<std::unique_ptr<sf::Texture>>::iterator
    //
};
//

#endif // SIMPLE_EFFECTS_RESOURCES_HPP_INCLUDED
