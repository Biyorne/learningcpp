//
// meth-head.cpp
//
#include "meth-head.hpp"

#include "utils.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>

namespace methhead
{

    MethHead::MethHead(
        const Motivation motivation,
        const std::string & imagePath,
        const sf::Vector2i & startingCellPos,
        const sf::FloatRect & screenRegion)
        : m_motivation(motivation)
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

        std::cout << "Sprite Position: " << m_sprite.getPosition().x << ","
                  << m_sprite.getPosition().y << std::endl;

        std::cout << "Sprite Global Bounds: " << m_sprite.getGlobalBounds().width << " x "
                  << m_sprite.getGlobalBounds().height << std::endl;
    }

    void MethHead::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
    }

    void MethHead::act(std::map<sf::Vector2i, CellContent> & gameBoard)
    {
        // Find target loot
        sf::Vector2i lootPos(-1, -1);
        for (const auto & posContentPair : gameBoard)
        {
            if (posContentPair.second.loot > 0)
            {
                lootPos = posContentPair.first;
                break;
            }
        }

        assert(
            ((lootPos.x >= 0) && (lootPos.y >= 0))
            && "Can't move methhead because no loot on the board.");

        // move toward loot
        const sf::Vector2i currentSelfCellPos(m_pos);
        sf::Vector2i newSelfCellPos(m_pos);

        if (lootPos.x < currentSelfCellPos.x)
        {
            --newSelfCellPos.x;
        }
        else if (lootPos.x > currentSelfCellPos.x)
        {
            ++newSelfCellPos.x;
        }
        else if (lootPos.y < currentSelfCellPos.y)
        {
            --newSelfCellPos.y;
        }
        else if (lootPos.y > currentSelfCellPos.y)
        {
            ++newSelfCellPos.y;
        }

        assert(
            (newSelfCellPos != currentSelfCellPos)
            && "Can't move methhead because methhead started its turn already on top of loot.");

        // change current CellContent motivation to none
        gameBoard[currentSelfCellPos].motivation = Motivation::none;

        // change local m_pos
        m_pos = newSelfCellPos;

        // change new CellContent motivation to ours
        gameBoard[newSelfCellPos].motivation = m_motivation;

        setSpriteRegion(m_sprite, gameBoard[newSelfCellPos].region);

        // update score
        m_score += gameBoard[newSelfCellPos].loot;

        // remove loot from board
        gameBoard[newSelfCellPos].loot = 0;

        // TODO for later
        // Calculate which cell moves self closer to target loot
        // Move sprite
        // If pick up loot, take old new loot off board, put new loot on, update score
    }

} // namespace methhead
