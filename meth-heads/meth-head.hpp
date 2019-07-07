#ifndef METH_HEAD_HPP_INCLUDED
#define METH_HEAD_HPP_INCLUDED

#include "cell-content.hpp"
#include "meth-head-enum.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <string>

namespace methhead
{

    class MethHead : public sf::Drawable
    {
    public:
        MethHead(
            const Motivation motivation,
            const std::string & imagePath,
            const sf::Vector2i & startingCellPos,
            const sf::FloatRect & screenRegion);

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

        void act(std::map<sf::Vector2i, CellContent> & gameBoard);

        std::size_t getScore() const { return m_score; }

    private:
        Motivation m_motivation;
        std::size_t m_score;
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Vector2i m_pos;
    };
} // namespace methhead
#endif // METH_HEAD_HPP_INCLUDED
