//
// meth-head.cpp
//
#include "meth-head.hpp"

#include "error-handling.hpp"
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
        BoardMap_t & board)
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
        placeInBounds(m_sprite, board[startingCellPos].bounds());

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

        placeInBounds(m_text, board[startingCellPos].bounds());
    }

    void MethHeadBase::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
        target.draw(m_text, states);
    }

    void MethHeadBase::act(
        const DisplayConstants & displayConstants,
        BoardMap_t & board,
        Audio & audio,
        const Random & random)
    {
        moveToward(displayConstants, board, audio, random, pickTarget(board));
    }

    // TODO make board class and move spawn loot there.
    void MethHeadBase::spawnLoot(BoardMap_t & board, const Random & random)
    {
        const auto cellPositions(makeUnoccupiedCellPositions(board));

        if (cellPositions.empty())
        {
            std::cerr << "MethHead::spawnLoot() failed to find any unoccupied cells." << std::endl;
        }
        else
        {
            board[random.select(cellPositions)].loot = random.rollInteger(1, 100);
        }
    }

    // TODO make board class and move spawn loot there.
    std::vector<sf::Vector2i> MethHeadBase::makeUnoccupiedCellPositions(BoardMap_t & board)
    {
        std::vector<sf::Vector2i> unoccupiedCellPositions;

        transform_if(
            std::begin(board),
            std::end(board),
            std::back_inserter(unoccupiedCellPositions),
            [](const auto & cellPair) { return !cellPair.second.isOccupied(); },
            [](const auto & cellPair) { return cellPair.first; });

        return unoccupiedCellPositions;
    }

    void MethHeadBase::moveToward(
        const DisplayConstants &,
        BoardMap_t & board,
        Audio & audio,
        const Random & random,
        const sf::Vector2i & targetCellPos)
    {
        assertOrThrow(
            (targetCellPos.x >= 0) && (targetCellPos.y >= 0),
            "Target Cell Position is invalid (x or y was negative).");

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

        // assertOrThrow(
        //    (newCellPos != oldCellPos),
        //    "MethHead::moveToward() failed because target_cellPos was the same as "
        //    "current_cellPos.");

        if (oldCellPos == newCellPos)
        {
            return;
        }

        m_pos = newCellPos;

        board[oldCellPos].motivation = Motivation::none;
        board[newCellPos].motivation = getMotivation();

        // placeInBounds(m_sprite, board[newCellPos].bounds());
        // placeInBounds(m_text, board[newCellPos].bounds());

        // audio.playWalk();

        if (board[newCellPos].loot > 0)
        {
            m_score += static_cast<std::size_t>(board[newCellPos].loot);
            board[newCellPos].loot = 0;

            if (Motivation::lazy == getMotivation())
            {
                audio.playCoin1();
            }
            else
            {
                audio.playCoin2();
            }

            spawnLoot(board, random);
        }
    }

    std::vector<MethHeadBase::LootPos> MethHeadBase::findAllLoot(const BoardMap_t & board) const
    {
        std::vector<MethHeadBase::LootPos> allLootPos;
        allLootPos.reserve(16);

        // loop over all cells and make a container of LootPos for all cells where loot > 0
        transform_if(
            std::begin(board),
            std::end(board),
            std::back_inserter(allLootPos),
            [](const auto & cellPair) { return (cellPair.second.loot > 0); },
            [](const auto & cellPair) { return LootPos(cellPair.second.loot, cellPair.first); });

        return allLootPos;
    }

    sf::Vector2i MethHeadBase::pickTarget(const BoardMap_t & board) const
    {
        auto allLootPos = MethHeadBase::findAllLoot(board);

        if (allLootPos.empty())
        {
            return { -1, -1 };
        }

        sortAllLoot(allLootPos);
        return allLootPos.front().cell_pos;
    }

} // namespace methhead
