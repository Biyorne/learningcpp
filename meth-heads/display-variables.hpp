#ifndef METH_HEAD_DISPLAY_VARIABLES_HPP_INCLUDED
#define METH_HEAD_DISPLAY_VARIABLES_HPP_INCLUDED

#include "cell.hpp"
#include "display-constants.hpp"
#include "utils.hpp"

#include <SFML/Graphics.hpp>

namespace methhead
{
    class DisplayVariables
    {
    public:
        explicit DisplayVariables(const sf::Vector2u & windowSize);

        const DisplayConstants & constants() const { return m_constants; }

        BoardMap_t makeBoard() const;

    private:
        sf::Vector2f cellPosToWindowPos(const sf::Vector2i & cellPos) const;

    private:
        DisplayConstants m_constants;
    };

} // namespace methhead

#endif // METH_HEAD_DISPLAY_VARIABLES_HPP_INCLUDED
