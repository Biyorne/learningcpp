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
        const BoardPos_t & boardPos,
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

    void MethHeadBase::update(
        const float elapsedSec,
        SoundPlayer & soundPlayer,
        const Random & random,
        AnimationPlayer & animationPlayer)
    {
        m_elapsedSinceLastActionSec += elapsedSec;

        if (m_elapsedSinceLastActionSec > m_waitBetweenActionsSec)
        {
            m_elapsedSinceLastActionSec = 0.0f;
            moveToward(soundPlayer, random, animationPlayer, pickTarget());
        }
    }

    void MethHeadBase::moveToward(
        SoundPlayer & soundPlayer,
        const Random & random,
        AnimationPlayer & animationPlayer,
        const BoardPos_t & targetBoardPos)
    {
        //   assertOrThrow(
        //       (targetBoardPos.x >= 0) && (targetBoardPos.y >= 0),
        //       "Target Cell Position is invalid (x or y was negative).");
        //
        //   const BoardPos_t oldCellPos(m_boardPos);
        //   BoardPos_t newCellPos(m_boardPos);
        //
        //   // TODO make move decision random
        //   if (targetBoardPos.x < oldCellPos.x)
        //   {
        //       --newCellPos.x;
        //   }
        //   else if (targetBoardPos.x > oldCellPos.x)
        //   {
        //       ++newCellPos.x;
        //   }
        //   else if (targetBoardPos.y < oldCellPos.y)
        //   {
        //       --newCellPos.y;
        //   }
        //   else if (targetBoardPos.y > oldCellPos.y)
        //   {
        //       ++newCellPos.y;
        //   }
        //
        //   assertOrThrow(
        //       (newCellPos != oldCellPos),
        //       "MethHead::moveToward() failed because target_cellPos was the same as "
        //       "current_cellPos.");
        //
        //   m_boardPos = newCellPos;
        //
        //   board[oldCellPos].motivation = Motivation::none;
        //
        //   Cell & newCell(board[newCellPos]);
        //
        //   const bool didBumpIntoOtherActor { (Motivation::none != newCell.motivation) };
        //
        //   newCell.motivation = motivation();
        //
        //   placeInBounds(m_sprite, board[newCellPos].bounds());
        //
        //   if (newCell.loot > 0)
        //   {
        //       m_score += static_cast<int>(newCell.loot);
        //       newCell.loot = 0;
        //
        //       soundPlayer.play("coin", random);
        //       spawnLoot(board, random);
        //
        //       return true;
        //   }
        //   else if (didBumpIntoOtherActor)
        //   {
        //       soundPlayer.play(random.from({ "punch", "ouch" }), random);
        //
        //       const sf::Vector2f spriteSize { m_sprite.getGlobalBounds().width,
        //                                       m_sprite.getGlobalBounds().height };
        //
        //       const sf::Vector2f animPos { m_sprite.getPosition() + (spriteSize * 0.5f) };
        //
        //       animationPlayer.play(random, "explode", animPos, (spriteSize * 4.0f));
        //   }
        //
        //   return false;
    }

    BoardPos_t MethHeadBase::pickTarget() const
    {
        //   auto allLootPos = MethHeadBase::findAllLoot(board);
        //
        //   if (allLootPos.empty())
        //   {
        //       return { -1, -1 };
        //   }
        //
        //   sortAllLoot(allLootPos);
        //
        //   return allLootPos.front().cell_pos;
        return m_boardPos;
    }

} // namespace methhead
