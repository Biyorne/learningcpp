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
    class MethHeadBase : public sf::Drawable
    {
    protected:
        MethHeadBase(
            const DisplayConstants & displayConstants,
            const Motivation motivation,
            const std::string & imagePath,
            const sf::Vector2i & startingCellPos,
            BoardMap_t & gameBoard);

    public:
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

        void
            act(const DisplayConstants & displayConstants,
                BoardMap_t & gameBoard,
                Audio & audio,
                const Random & random);

        std::size_t getScore() const { return m_score; }

        // TODO move to somewhere I belong...
        static void spawnLoot(BoardMap_t & gameBoard, const Random & random);

        virtual Motivation getMotivation() const = 0;

    protected:
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

        sf::Vector2i pickTarget(const BoardMap_t & gameBoard) const;

        virtual void sortAllLoot(std::vector<LootPos> & allLoot) const = 0;

        sf::Vector2i getCellPos() const { return m_pos; }

    private:
        std::size_t m_score;
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Vector2i m_pos;
        sf::Text m_text;
    };

    class Lazy : public MethHeadBase
    {
    public:
        Lazy(
            const DisplayConstants & displayConstants,
            const std::string & imagePath,
            const sf::Vector2i & startingCellPos,
            BoardMap_t & gameBoard)
            : MethHeadBase(displayConstants, getMotivation(), imagePath, startingCellPos, gameBoard)
        {
            gameBoard[startingCellPos].motivation = getMotivation();
        }

        Motivation getMotivation() const final { return Motivation::lazy; }

    private:
        void sortAllLoot(std::vector<LootPos> & allLoot) const final
        {
            std::sort(
                std::begin(allLoot), std::end(allLoot), [&](const LootPos & A, const LootPos & B) {
                    const auto distToA(calcDistance(getCellPos(), A.cell_pos));
                    const auto distToB(calcDistance(getCellPos(), B.cell_pos));

                    if (distToA != distToB)
                    {
                        return (distToA < distToB);
                    }
                    else
                    {
                        return (A.loot > B.loot);
                    }
                });
        }
    };

    class Greedy : public MethHeadBase
    {
    public:
        Greedy(
            const DisplayConstants & displayConstants,
            const std::string & imagePath,
            const sf::Vector2i & startingCellPos,
            BoardMap_t & gameBoard)
            : MethHeadBase(displayConstants, getMotivation(), imagePath, startingCellPos, gameBoard)
        {
            gameBoard[startingCellPos].motivation = getMotivation();
        }

        Motivation getMotivation() const final { return Motivation::greedy; }

    private:
        void sortAllLoot(std::vector<LootPos> & allLoot) const final
        {
            std::sort(
                std::begin(allLoot), std::end(allLoot), [&](const LootPos & A, const LootPos & B) {
                    if (A.loot != B.loot)
                    {
                        return (A.loot > B.loot);
                    }
                    else
                    {
                        const auto distToA(calcDistance(getCellPos(), A.cell_pos));
                        const auto distToB(calcDistance(getCellPos(), B.cell_pos));
                        return (distToA < distToB);
                    }
                });
        }
    };

} // namespace methhead

#endif // METH_HEAD_HPP_INCLUDED
