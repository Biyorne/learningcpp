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
        if (Motivation::none == m_motivation)
        {
            throw std::runtime_error("Tried to create meth head with no motivation.");
        }

        if (!m_texture.loadFromFile(imagePath))
        {
            throw std::runtime_error("Unable to load image: " + imagePath);
        }

        m_sprite.setTexture(m_texture, true);
        setSpriteRegion(m_sprite, screenRegion);
    }

    void MethHead::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_sprite, states);
    }

    void MethHead::act(BoardMap_t & gameBoard, Audio & audio)
    {
        moveToward(gameBoard, audio, findTarget(gameBoard));
    }

    void MethHead::moveToward(
        BoardMap_t & gameBoard, Audio & audio, const sf::Vector2i & targetCellPos)
    {
        // bail if the target position is invalid or the same as where we already are
        if ((targetCellPos.x < 0) || (targetCellPos.y < 0) || (targetCellPos == m_pos))
        {
            return;
        }

        // where we were
        const sf::Vector2i oldCellPos(m_pos);
        CellContent & oldCellContent(gameBoard[oldCellPos]);

        // where we are moving to
        sf::Vector2i newCellPos(oldCellPos);
        if (oldCellPos.x < targetCellPos.x)
        {
            ++newCellPos.x;
        }
        else if (oldCellPos.x > targetCellPos.x)
        {
            --newCellPos.x;
        }
        else if (oldCellPos.y < targetCellPos.y)
        {
            ++newCellPos.y;
        }
        else if (oldCellPos.y > targetCellPos.y)
        {
            --newCellPos.y;
        }

        CellContent & newCellContent(gameBoard[newCellPos]);

        // do everything it takes to actually move
        audio.playWalk();
        m_pos = newCellPos;
        oldCellContent.motivation = Motivation::none;
        newCellContent.motivation = m_motivation;
        methhead::setSpriteRegion(m_sprite, newCellContent.region);

        // handle loot
        if (newCellContent.loot > 0)
        {
            if (Motivation::lazy == m_motivation)
            {
                audio.playCoin1();
            }
            else
            {
                audio.playCoin2();
            }

            m_score += static_cast<std::size_t>(newCellContent.loot);
            newCellContent.loot = 0;

            // TODO place new loot on the board
        }
    }

    void MethHead::actOldBroken(BoardMap_t & gameBoard)
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
        m_score += static_cast<std::size_t>(gameBoard[newSelfCellPos].loot);

        // remove loot from board
        gameBoard[newSelfCellPos].loot = 0;

        // TODO for later
        // Calculate which cell moves self closer to target loot
        // Move sprite
        // If pick up loot, take old new loot off board, put new loot on, update score
    }

    std::vector<MethHead::LootPos> MethHead::findAllLoot(const BoardMap_t & gameBoard) const
    {
        std::vector<MethHead::LootPos> allLootPos;

        // loop over all cells and make a container of LootPos for all cells where loot > 0
        transform_if(
            std::begin(gameBoard),
            std::end(gameBoard),
            std::back_inserter(allLootPos),
            [](const auto & cellPair) { return (cellPair.second.loot > 0); },
            [](const auto & cellPair) { return LootPos(cellPair.second.loot, cellPair.first); });

        return allLootPos;
    }

    sf::Vector2i MethHead::findTarget(const BoardMap_t & gameBoard) const
    {
        if (Motivation::lazy == m_motivation)
        {
            return findTargetLazy(findAllLoot(gameBoard));
        }
        else
        {
            return findTargetGreedy(findAllLoot(gameBoard));
        }
    }

    sf::Vector2i
        MethHead::findTargetLazy(const std::vector<MethHead::LootPos> & allLootPosParam) const
    {
        auto allLootPos = allLootPosParam;

        // bail if empty
        if (allLootPos.empty())
        {
            return { -1, -1 };
        }

        // sort by distance ascending (<)
        std::sort(
            std::begin(allLootPos), std::end(allLootPos), [&](const auto & A, const auto & B) {
                const auto distToA(calcDistance(m_pos, A.cell_pos));
                const auto distToB(calcDistance(m_pos, B.cell_pos));

                if (distToA != distToB)
                {
                    return (distToA < distToB);
                }
                else
                {
                    return (A.loot > B.loot);
                }
            });

        return allLootPos.front().cell_pos;
    }

    sf::Vector2i
        MethHead::findTargetGreedy(const std::vector<MethHead::LootPos> & allLootPosParam) const
    {
        auto allLootPos = allLootPosParam;

        // bail if empty
        if (allLootPos.empty())
        {
            return { -1, -1 };
        }

        // sort by distance ascending (<)
        std::sort(
            std::begin(allLootPos), std::end(allLootPos), [&](const auto & A, const auto & B) {
                if (A.loot != B.loot)
                {
                    return (A.loot > B.loot);
                }
                else
                {
                    const auto distToA(calcDistance(m_pos, A.cell_pos));
                    const auto distToB(calcDistance(m_pos, B.cell_pos));
                    return (distToA < distToB);
                }
            });

        return allLootPos.front().cell_pos;
    }

} // namespace methhead
