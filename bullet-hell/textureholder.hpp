#ifndef BULLET_HELL_TEXTURE_HOLDER_HPP_INCLUDED
#define BULLET_HELL_TEXTURE_HOLDER_HPP_INCLUDED

#include <cassert>
#include <map>

#include <SFML/Graphics.hpp>

namespace Resources
{

    enum ID
    {
        Landscape,
        Airplane,
        Missile
    };

    template <typename Resource_t, typename Identifier_t>
    class ResourceHolder
    {
      public:
        ResourceHolder()
            : m_textureMap()
        {}

        void load(Identifier_t id, const std::string & filename)
        {
            assert(!filename.empty());

            std::unique_ptr<sf::Texture> textureUPtr(std::make_unique<sf::Texture>());

            const bool loadSuccess(textureUPtr->loadFromFile(filename));
            assert(loadSuccess);

            m_textureMap.insert(std::make_pair(id, std::move(textureUPtr)));
        }

        Resource_t & get(Identifier_t id)
        {
            auto found(m_textureMap.find(id));
            assert(found != std::end(m_textureMap));

            return *(found->second);
        }

        const Resource_t & get(Identifier_t id) const
        {
            auto found(m_textureMap.find(id));
            assert(found != std::end(m_textureMap));

            return *(found->second);
        }

      private:
        std::map<Identifier_t, std::unique_ptr<Resource_t>> m_textureMap;
    };
} // namespace Resources

#endif // BULLET_HELL_TEXTURE_HOLDER_HPP_INCLUDED
