#ifndef METH_HEAD_HPP_INCLUDED
#define METH_HEAD_HPP_INCLUDED

#include "meth-head-enum.hpp"

#include <SFML/Graphics.hpp>

#include <string>

namespace methhead
{

    class MethHead
    {
    public:
        MethHead(
            const MethHeadIden iden,
            const std::string & imagePath,
            const sf::Vector2i & startingCellPos);

    private:
        MethHeadIden m_iden;
        std::size_t m_score;
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Vector2i m_pos;
    };
} // namespace methhead
#endif // METH_HEAD_HPP_INCLUDED
