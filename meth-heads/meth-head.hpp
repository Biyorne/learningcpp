#ifndef METH_HEAD_HPP_INCLUDED
#define METH_HEAD_HPP_INCLUDED

#include "audio.hpp"
#include "cell-content.hpp"
#include "display-constants.hpp"
#include "meth-head-enum.hpp"
#include "random.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <string>

namespace methhead
{
    class MethHead : public sf::Drawable
    {
    public:
        MethHead(
            const DisplayConstants & displayConstants,
            const Motivation motivation,
            const std::string & imagePath,
            const sf::Vector2i & startingCellPos,
            const sf::FloatRect & screenRegion);

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

        void
            act(const DisplayConstants & displayConstants,
                BoardMap_t & gameBoard,
                Audio & audio,
                const Random & random);

        std::size_t getScore() const { return m_score; }

        // TODO move to somewhere I belong...
        static void spawnLoot(BoardMap_t & gameBoard, const Random & random);

    private:
        void moveToward(
            const DisplayConstants & displayConstants,
            BoardMap_t & gameBoard,
            Audio & audio,
            const Random & random,
            const sf::Vector2i & targetCellPos);

        int calcDistance(const sf::Vector2i & from, const sf::Vector2i & to) const
        {
            return (std::abs(to.x - from.x) + std::abs(to.y - from.y));
        }

        struct LootPos
        {
            LootPos(const int lootParam, const sf::Vector2i & cellPos)
                : loot(lootParam)
                , cell_pos(cellPos)
            {}

            int loot;
            sf::Vector2i cell_pos;
        };

        std::vector<LootPos> findAllLoot(const BoardMap_t & gameBoard) const;

        sf::Vector2i findTarget(const BoardMap_t & gameBoard) const;
        sf::Vector2i findTargetLazy(const std::vector<MethHead::LootPos> & allLootPos) const;
        sf::Vector2i findTargetGreedy(const std::vector<MethHead::LootPos> & allLootPos) const;

    private:
        Motivation m_motivation;
        std::size_t m_score;
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Vector2i m_pos;
        sf::Text m_text;
    };
} // namespace methhead

#endif // METH_HEAD_HPP_INCLUDED
