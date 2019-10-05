#ifndef METHHEADS_BASE_HPP_INCLUDED
#define METHHEADS_BASE_HPP_INCLUDED

#include "cell.hpp"
#include "random.hpp"
#include "sound-player.hpp"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace methhead
{

    // TODO put in own file
    struct IActor : public sf::Drawable
    {
        virtual ~IActor() = default;

        virtual std::size_t getScore() const = 0;
        virtual Motivation getMotivation() const = 0;
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;

        virtual void act(BoardMap_t & board, SoundPlayer & soundPlayer, const Random & random) = 0;
    };

    using IActorUPtr_t = std::unique_ptr<IActor>;
    using IActorUVec_t = std::vector<IActorUPtr_t>;

    //

    class MethHeadBase : public IActor
    {
    protected:
        MethHeadBase(
            const std::string & imagePath,
            const sf::Vector2i & boardPos,
            const sf::FloatRect & windowBounds);

        virtual ~MethHeadBase() = default;

    public:
        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
        void act(BoardMap_t & board, SoundPlayer & soundPlayer, const Random & random) override;

        std::size_t getScore() const final { return m_score; }

        // TODO make board class and move spawn loot there.
        static void spawnLoot(BoardMap_t & board, const Random & random);

        static std::vector<sf::Vector2i> makeUnoccupiedCellPositions(BoardMap_t & board);

        sf::Vector2i getCellPos() const { return m_boardPos; }

    protected:
        void moveToward(
            BoardMap_t & board,
            SoundPlayer & soundPlayer,
            const Random & random,
            const sf::Vector2i & targetCellPos);

        inline int calcDistance(const sf::Vector2i & from, const sf::Vector2i & to) const
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

        std::vector<LootPos> findAllLoot(const BoardMap_t & board) const;

        sf::Vector2i pickTarget(const BoardMap_t & board) const;

        virtual void sortAllLoot(std::vector<LootPos> & allLoot) const = 0;

    private:
        std::size_t m_score;
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        sf::Vector2i m_boardPos;
    };

    //

    class Lazy : public MethHeadBase
    {
    public:
        Lazy(
            const std::string & imagePath,
            const sf::Vector2i & boardPos,
            const sf::FloatRect & windowBounds)
            : MethHeadBase(imagePath, boardPos, windowBounds)
        {}

        virtual ~Lazy() = default;

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

    //

    class Greedy : public MethHeadBase
    {
    public:
        Greedy(
            const std::string & imagePath,
            const sf::Vector2i & boardPos,
            const sf::FloatRect & windowBounds)
            : MethHeadBase(imagePath, boardPos, windowBounds)
        {}

        virtual ~Greedy() = default;

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

#endif // METHHEADS_BASE_HPP_INCLUDED
