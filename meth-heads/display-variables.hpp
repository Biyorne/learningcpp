#ifndef METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED
#define METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED

#include "cell.hpp"
#include "display-constants.hpp"

#include <SFML/Graphics.hpp>

namespace methhead
{
    class DisplayVariables
    {
    public:
        explicit DisplayVariables(const sf::Vector2u & windowSize);
        virtual ~DisplayVariables() = default;

        void update(
            const float elapsedMs,
            const std::size_t lazyScore,
            const std::size_t greedyScore,
            const BoardMap_t & board);

        void draw(
            sf::RenderTarget & target, sf::RenderStates states, const BoardMap_t & board) const;

        const DisplayConstants constants() const { return m_constants; }

        BoardMap_t makeBoard() const;

    private:
        void setScoreBarsHeight(const std::size_t lazyScore, const std::size_t greedyScore);

        sf::Vector2f boardToWindowPos(const sf::Vector2i & cellPos) const;

    private:
        DisplayConstants m_constants;
        sf::RectangleShape m_lazyScoreRectangle;
        sf::RectangleShape m_greedyScoreRectangle;
        sf::Sprite m_lootSprite;
        sf::Text m_lootText;
    };

} // namespace methhead

#endif // METHHEADS_DISPLAY_VARIABLES_HPP_INCLUDED
