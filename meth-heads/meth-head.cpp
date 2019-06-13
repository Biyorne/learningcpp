//
// meth-head.cpp
//
#include "meth-head.hpp"

#include "utils.hpp"

#include <stdexcept>

namespace methhead
{

    MethHead::MethHead(
        const MethHeadIden iden,
        const std::string & imagePath,
        const sf::Vector2i & startingCellPos,
        const sf::FloatRect & screenRegion)
        : m_iden(iden)
        , m_score(0)
        , m_texture()
        , m_sprite()
        , m_pos(startingCellPos)
    {
        if (!m_texture.loadFromFile(imagePath))
        {
            throw std::runtime_error("Unable to load image: " + imagePath);
        }

        m_sprite.setTexture(m_texture, true);

        setSpriteRegion(m_sprite, screenRegion);
    }

} // namespace methhead
