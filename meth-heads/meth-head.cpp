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
            const auto & cellPos(random.select(cellPositions));
            board[cellPos].throwIfOccupied();
            board[cellPos].loot = random.rollInteger(1, 100);
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

        // TODO only added to find a bug REMOVE
        for (const sf::Vector2i & cellPos : unoccupiedCellPositions)
        {
            board[cellPos].throwIfOccupied();
        }

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

        assertOrThrow(
            (newCellPos != oldCellPos),
            "MethHead::moveToward() failed because target_cellPos was the same as "
            "current_cellPos.");

        m_pos = newCellPos;

        board[oldCellPos].motivation = Motivation::none;

        Cell & newCell(board[newCellPos]);
        newCell.motivation = getMotivation();

        placeInBounds(m_sprite, board[newCellPos].bounds());
        placeInBounds(m_text, board[newCellPos].bounds());

        audio.playWalk();

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

        // TODO only added to find a bug REMOVE
        for (const LootPos & lootPos : allLootPos)
        {
            const auto & cell(board.find(lootPos.cell_pos)->second);

            assertOrThrow(
                (cell.is_valid),
                "MethHead::pickTarget()'s findAllLoot() failed, because one of the returned "
                "positions was invalid.");

            assertOrThrow(
                (cell.motivation == Motivation::none),
                "MethHead::pickTarget()'s findAllLoot() failed, because one of the returned "
                "positions had a methhead in it.");

            assertOrThrow(
                (cell.isOccupied()),
                "MethHead::pickTarget()'s findAllLoot() failed, because one of the returned "
                "positions was !isOccupied().");

            assertOrThrow(
                (cell.loot > 0),
                "MethHead::pickTarget()'s findAllLoot() failed, because one of the returned "
                "positions had no loot in it.");
        }

        if (allLootPos.empty())
        {
            return { -1, -1 };
        }

        sortAllLoot(allLootPos);

        {
            const auto & cell(board.find(allLootPos.front().cell_pos)->second);

            assertOrThrow(
                (cell.is_valid),
                "***MethHead::pickTarget()'s findAllLoot() failed, because one of the returned "
                "positions was invalid.");

            assertOrThrow(
                (cell.motivation == Motivation::none),
                "***MethHead::pickTarget()'s findAllLoot() failed, because one of the returned "
                "positions had a methhead in it.");

            assertOrThrow(
                (cell.isOccupied()),
                "***MethHead::pickTarget()'s findAllLoot() failed, because one of the returned "
                "positions was !isOccupied().");

            assertOrThrow(
                (cell.loot > 0),
                "***MethHead::pickTarget()'s findAllLoot() failed, because one of the returned "
                "positions had no loot in it.");
        }
        return allLootPos.front().cell_pos;
    }

} // namespace methhead
