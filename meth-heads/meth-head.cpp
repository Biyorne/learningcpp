//
// meth-head.cpp
//
#include "meth-head.hpp"

#include "utils.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>

namespace methhead
{

    MethHeadBase::MethHeadBase(
        const DisplayConstants & displayConstants,
        const Motivation motivation,
        const std::string & imagePath,
        const sf::Vector2i & startingCellPos,
        BoardMap_t & gameBoard)
        : m_score(0)
        , m_texture()
        , m_sprite()
        , m_pos(startingCellPos)
        , m_text()
    {
        if (Motivation::none == motivation)
        {
            throw std::runtime_error("Tried to create meth head with no motivation.");
        }

        if (!m_texture.loadFromFile(imagePath))
        {
            throw std::runtime_error("Unable to load image: " + imagePath);
        }

        m_sprite.setTexture(m_texture, true);
        m_sprite.setColor(sf::Color(255, 255, 255, 127));
        placeInRegion(m_sprite, gameBoard[startingCellPos].region);

        m_text = displayConstants.default_text;

        if (Motivation::lazy == motivation)
        {
            m_text.setString("L");
            m_text.setFillColor(displayConstants.lazy_color);
        }
        else
        {
            m_text.setString("G");
            m_text.setFillColor(displayConstants.greedy_color);
        }

        placeInRegion(m_text, gameBoard[startingCellPos].region);
    }

    void MethHeadBase::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
        target.draw(m_text, states);
    }

    void MethHeadBase::act(
        const DisplayConstants & displayConstants,
        BoardMap_t & gameBoard,
        Audio & audio,
        const Random & random)
    {
        moveToward(displayConstants, gameBoard, audio, random, pickTarget(gameBoard));
    }

    void MethHeadBase::spawnLoot(BoardMap_t & gameBoard, const Random & random)
    {
        // loop over all in gameBoard, and populate a vector<sf::Vector2i> with all valid
        // cellPositions to spawn loot
        std::vector<sf::Vector2i> cellPositions;

        transform_if(
            std::begin(gameBoard),
            std::end(gameBoard),
            std::back_inserter(cellPositions),
            [](const auto & cellPair) {
                return (
                    cellPair.second.is_valid && (Motivation::none == cellPair.second.motivation)
                    && (cellPair.second.loot <= 0));
            },
            [](const auto & cellPair) { return cellPair.first; });

        if (cellPositions.empty())
        {
            return;
        }

        const sf::Vector2i cellPos(random.select(cellPositions));
        gameBoard[cellPos].loot = random.rollInteger(1, 100);
    }

    void MethHeadBase::moveToward(
        const DisplayConstants &,
        BoardMap_t & gameBoard,
        Audio & audio,
        const Random & random,
        const sf::Vector2i & targetCellPos)
    {
        if ((targetCellPos.x < 0) || (targetCellPos.y < 0))
        {
            return;
        }

        const sf::Vector2i oldCellPos(m_pos);
        sf::Vector2i newCellPos(m_pos);

        // TODO make move decision random
        if (targetCellPos.x < oldCellPos.x)
        {
            --newCellPos.x;
        }
        else if (targetCellPos.x > oldCellPos.x)
        {
            ++newCellPos.x;
        }
        else if (targetCellPos.y < oldCellPos.y)
        {
            --newCellPos.y;
        }
        else if (targetCellPos.y > oldCellPos.y)
        {
            ++newCellPos.y;
        }

        if (newCellPos == oldCellPos)
        {
            return;
        }

        m_pos = newCellPos;

        gameBoard[oldCellPos].motivation = Motivation::none;
        gameBoard[newCellPos].motivation = getMotivation();

        placeInRegion(m_sprite, gameBoard[newCellPos].region);
        placeInRegion(m_text, gameBoard[newCellPos].region);

        // audio.playWalk();

        if (gameBoard[newCellPos].loot > 0)
        {
            m_score += static_cast<std::size_t>(gameBoard[newCellPos].loot);
            gameBoard[newCellPos].loot = 0;

            if (Motivation::lazy == getMotivation())
            {
                audio.playCoin1();
            }
            else
            {
                audio.playCoin2();
            }

            spawnLoot(gameBoard, random);
        }
    }

    std::vector<MethHeadBase::LootPos> MethHeadBase::findAllLoot(const BoardMap_t & gameBoard) const
    {
        std::vector<MethHeadBase::LootPos> allLootPos;

        // loop over all cells and make a container of LootPos for all cells where loot > 0
        transform_if(
            std::begin(gameBoard),
            std::end(gameBoard),
            std::back_inserter(allLootPos),
            [](const auto & cellPair) { return (cellPair.second.loot > 0); },
            [](const auto & cellPair) { return LootPos(cellPair.second.loot, cellPair.first); });

        return allLootPos;
    }

    sf::Vector2i MethHeadBase::pickTarget(const BoardMap_t & gameBoard) const
    {
        auto allLootPos = MethHeadBase::findAllLoot(gameBoard);

        if (allLootPos.empty())
        {
            return { -1, -1 };
        }

        sortAllLoot(allLootPos);
        return allLootPos.front().cell_pos;
    }

} // namespace methhead
