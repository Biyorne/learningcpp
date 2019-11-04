#ifndef METHHEADS_CELL_HPP_INCLUDED
#define METHHEADS_CELL_HPP_INCLUDED

#include <map>

#include <SFML/Graphics.hpp>

namespace methhead
{
    // TODO in the end of all cleanup this should have a different home...
    //
    // also... TODO In the end, after cleaning up everything, there should be no more need for
    // "none" none/invalid options in enums is not always code smell, it is not always wrong, but on
    // the other hand, always adding none/invalid to your enums as a default/habit just in case it
    // is needed IS ALWAYS A BAD HABIT, becauase any enum that has a none/invalid option that is not
    // needed is often a huge source of errors and mess.
    enum class Motivation
    {
        none,
        lazy,
        greedy
    };

    class Cell
    {
    public:
        Cell();

        Cell(
            const sf::Vector2i & boardPos,
            const sf::Vector2f & windowPos,
            const sf::Vector2f & size);

        inline sf::FloatRect bounds() const { return rectangle.getGlobalBounds(); }

        inline bool isOccupied() const { return (Motivation::none != motivation) || (loot > 0); }

        sf::Vector2i board_pos;
        sf::RectangleShape rectangle;
        Motivation motivation; // TODO is this the right way to track where actors are?
        int loot;
        bool is_valid; // TODO this should go away

        static inline sf::Color background_color { 32, 32, 32 };
        static inline sf::Color line_color { 220, 220, 220 };
    };

    using BoardMap_t = std::map<sf::Vector2i, Cell>;

} // namespace methhead

#endif // METHHEADS_CELL_HPP_INCLUDED
