// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
        const std::string & imagePath,
        const sf::Vector2i & boardPos,
        const sf::FloatRect & windowBounds,
        const float waitBetweenActionsSec)
        : m_score(0)
        , m_texture()
        , m_sprite()
        , m_boardPos(boardPos)
        , m_waitBetweenActionsSec(waitBetweenActionsSec)
        , m_elapsedSinceLastActionSec(0.0f)
    {
        if (!m_texture.loadFromFile(imagePath))
        {
            throw std::runtime_error("Unable to load image: " + imagePath);
        }

        m_sprite.setTexture(m_texture, true);
        placeInBounds(m_sprite, windowBounds);
    }

    void MethHeadBase::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
    }

    void MethHeadBase::act(
        const float elapsedSec,
        BoardMap_t & board,
        SoundPlayer & soundPlayer,
        const Random & random,
        AnimationPlayer & animationPlayer)
    {
        m_elapsedSinceLastActionSec += elapsedSec;
        if (m_elapsedSinceLastActionSec > m_waitBetweenActionsSec)
        {
            m_elapsedSinceLastActionSec = 0.0f;
            moveToward(board, soundPlayer, random, animationPlayer, pickTarget(board));
        }
    }

    // TODO make board class and move spawn loot there.
    void MethHeadBase::spawnLoot(BoardMap_t & board, const Random & random)
    {
        std::vector<sf::Vector2i> cellPositions;

        // TODO only added to find a bug REMOVE
        try
        {
            cellPositions = makeUnoccupiedCellPositions(board);
        }
        catch (...)
        {
            std::cout << "makeUnoccupiedCellPositions() threw the execption, but was called by "
                         "spawnLoot()"
                      << std::endl;

            throw;
        }

        if (cellPositions.empty())
        {
            std::cout << "MethHead::spawnLoot() failed to find any unoccupied cells." << std::endl;
        }
        else
        {
            const auto & cellPos(random.from(cellPositions));
            board[cellPos].loot = random.fromTo(1, 100);
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
        BoardMap_t & board,
        SoundPlayer & soundPlayer,
        const Random & random,
        AnimationPlayer & animationPlayer,
        const sf::Vector2i & targetCellPos)
    {
        assertOrThrow(
            (targetCellPos.x >= 0) && (targetCellPos.y >= 0),
            "Target Cell Position is invalid (x or y was negative).");

        const sf::Vector2i oldCellPos(m_boardPos);
        sf::Vector2i newCellPos(m_boardPos);

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

        assertOrThrow(
            (newCellPos != oldCellPos),
            "MethHead::moveToward() failed because target_cellPos was the same as "
            "current_cellPos.");

        m_boardPos = newCellPos;

        board[oldCellPos].motivation = Motivation::none;

        Cell & newCell(board[newCellPos]);

        if (Motivation::none != newCell.motivation)
        {
            soundPlayer.play("");

            animationPlayer.play(
                "", newCell.rectangle.getPosition(), (newCell.rectangle.getSize() * 4.0f));
        }

        newCell.motivation = getMotivation();

        placeInBounds(m_sprite, board[newCellPos].bounds());

        if (newCell.loot > 0)
        {
            assertOrThrow(
                (newCell.is_valid),
                "MethHead::moveToward()'s moved onto loot but that newCellPos is not valid.");

            assertOrThrow(
                (newCell.motivation != Motivation::none),
                "MethHead::moveToward()'s moved onto loot but that newCellPos does not have us on "
                "it?!");

            m_score += static_cast<std::size_t>(newCell.loot);
            newCell.loot = 0;

            soundPlayer.play("coin");
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
