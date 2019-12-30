#ifndef BULLET_HELL_TEXTURE_HOLDER_HPP_INCLUDED
#define BULLET_HELL_TEXTURE_HOLDER_HPP_INCLUDED

#include <cassert>
#include <map>

#include <SFML/Graphics.hpp>

namespace Textures
{

    enum ID
    {
        Landscape,
        Airplane,
        Missile
    };

    class TextureHolder
    {

      public:
        TextureHolder()
            : m_textureMap()
        {}

        void load(const Textures::ID id, const std::string & filename)
        {
            assert(!filename.empty());

            std::unique_ptr<sf::Texture> textureUPtr(std::make_unique<sf::Texture>());

            const bool loadSuccess(textureUPtr->loadFromFile(filename));
            assert(loadSuccess);

            m_textureMap.insert(std::make_pair(id, std::move(textureUPtr)));
        }

        sf::Texture & get(const Textures::ID id)
        {
            const auto found(m_textureMap.find(id));
            assert(found != std::end(m_textureMap));

            return *(found->second);
        }

      private:
        std::map<Textures::ID, std::unique_ptr<sf::Texture>> m_textureMap;
    };
} // namespace Textures

#endif // BULLET_HELL_TEXTURE_HOLDER_HPP_INCLUDED
