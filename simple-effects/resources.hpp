#ifndef SIMPLE_EFFECTS_RESOURCES_HPP_INCLUDED
#define SIMPLE_EFFECTS_RESOURCES_HPP_INCLUDED

#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

struct Resources
{
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

        loadTexture("C:/src/learningcpp/media/image/warning.png", warn_texture);
        loadTexture("C:/src/learningcpp/media/image/seamless/brick-wall.jpg", bg_texture);
        loadTexture("C:/src/learningcpp/media/image/rabbit.png", rabbit_texture);
        loadTexture("C:/src/learningcpp/media/image/carrot.png", carrot_texture);
        loadTexture(
            "C:/src/learningcpp/media/image/particle/spirit-recruit-5.png", particle_texture);
    }

    void loadTexture(const std::string & filePath, sf::Texture & texture)
    {
        if (!texture.loadFromFile(filePath))
        {
            std::cerr << "Unable to load texure: " << filePath << std::endl;
        }

        texture.setSmooth(true);
    }

    sf::Texture bg_texture;
    sf::Texture warn_texture;
    sf::Texture rabbit_texture;
    sf::Texture carrot_texture;
    sf::Texture particle_texture;

    sf::Music music;
};
//

#endif // SIMPLE_EFFECTS_RESOURCES_HPP_INCLUDED
