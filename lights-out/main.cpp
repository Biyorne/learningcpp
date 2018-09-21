#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <vector>

class Cell
{
public:
    Cell(const sf::Vector2f & SIZE_V, const sf::Vector2f & POS_V, const sf::Vector2i & GRID_POS_V)
        : m_isOn(false)
        , m_rectangle()
        , m_gridPosV(GRID_POS_V)
        , m_colorOn(255, 0, 0)
        , m_colorOff(255, 0, 0, 127)
    {
        m_rectangle.setSize(SIZE_V);
        m_rectangle.setPosition(POS_V);
        change();
    }

    bool isOn() const { return m_isOn; }

    bool doesContain(const sf::Vector2f & POSITION_V) const
    {
        return m_rectangle.getGlobalBounds().contains(POSITION_V);
    }

    sf::Vector2i gridPos() const { return m_gridPosV; }

    sf::RectangleShape rectangle() const { return m_rectangle; }

    void change()
    {
        m_isOn = !m_isOn;

        if (m_isOn)
        {
            m_rectangle.setFillColor(m_colorOn);
        }
        else
        {
            m_rectangle.setFillColor(m_colorOff);
        }
    }

private:
    bool m_isOn;
    sf::RectangleShape m_rectangle;
    sf::Vector2i m_gridPosV;
    sf::Color m_colorOn;
    sf::Color m_colorOff;
};

const unsigned int SCREEN_WIDTH(800);
const unsigned int SCREEN_HEIGHT(600);

float screenPad(const float SCREEN_FRACTION)
{
    const float SCREEN_DIMENSION_AVG(static_cast<float>(SCREEN_WIDTH + SCREEN_HEIGHT) * 0.5f);
    return (SCREEN_DIMENSION_AVG * SCREEN_FRACTION);
}

int main()
{
    const sf::Vector2f SCREEN_SIZE_V(sf::Vector2u(SCREEN_WIDTH, SCREEN_HEIGHT));

    const std::size_t CELL_COUNT_HORIZ(3);
    const std::size_t CELL_COUNT_VERT(CELL_COUNT_HORIZ);
    const std::size_t CELL_COUNT(CELL_COUNT_HORIZ * CELL_COUNT_VERT);

    const float CELL_PAD(screenPad(0.01f));

    const float CELL_WIDTH((SCREEN_WIDTH - ((CELL_COUNT_HORIZ + 1) * CELL_PAD)) / CELL_COUNT_HORIZ);
    const float CELL_HEIGHT((SCREEN_HEIGHT - ((CELL_COUNT_VERT + 1) * CELL_PAD)) / CELL_COUNT_VERT);

    const sf::Vector2f CELL_SIZE_V(CELL_WIDTH, CELL_HEIGHT);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Lights Out");

    const sf::Color CELL_OUTLINE_COLOR(sf::Color::Black);

    std::vector<Cell> cells;

    for (std::size_t row(0); row < CELL_COUNT_VERT; ++row)
    {
        const float ROW_FLOAT(static_cast<float>(row));

        for (std::size_t column(0); column < CELL_COUNT_HORIZ; ++column)
        {
            const float COLUMN_FLOAT(static_cast<float>(column));

            const float LEFT((CELL_PAD * (COLUMN_FLOAT + 1.0f)) + (CELL_WIDTH * COLUMN_FLOAT));
            const float TOP((CELL_PAD * (ROW_FLOAT + 1.0f)) + (CELL_HEIGHT * ROW_FLOAT));
            const sf::Vector2f CELL_POSITION_V(LEFT, TOP);

            const sf::Vector2i CELL_GRID_POSITION_V(
                static_cast<int>(column), static_cast<int>(row));

            Cell cell(CELL_SIZE_V, CELL_POSITION_V, CELL_GRID_POSITION_V);

            cells.push_back(cell);
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                const sf::Vector2f MOUSE_POSITION_V(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                // find the cell grid pos (where the mouse was clicked) that is at the center of all
                // to change

                const std::vector<Cell>::const_iterator ITER_TO_CELL_CLICKED(std::find_if(
                    std::begin(cells), std::end(cells), [&MOUSE_POSITION_V](const Cell & CELL) {
                        return CELL.doesContain(MOUSE_POSITION_V);
                    }));

                if (ITER_TO_CELL_CLICKED != std::cend(cells))
                {
                    const sf::Vector2i GRID_POS_OF_CLICKED_CELL(ITER_TO_CELL_CLICKED->gridPos());

                    // make the cell grid positions around and including this cell
                    std::vector<sf::Vector2i> cellGridPosToChange;

                    // for (int row(0); row < CELL_COUNT_VERT; ++row)
                    {

                        for (int column(-1); column <= 1; ++column)
                        {
                            const sf::Vector2i GRID_POS_V(
                                GRID_POS_OF_CLICKED_CELL.x + column, GRID_POS_OF_CLICKED_CELL.y);

                            cellGridPosToChange.push_back(GRID_POS_V);
                        }
                    }

                    // change every cell in the container of grid positions
                    for (const sf::Vector2i & GRID_POS_V : cellGridPosToChange)
                    {
                        auto iterToCellWithGridPos(std::find_if(
                            std::begin(cells), std::end(cells), [&GRID_POS_V](const Cell & CELL) {
                                return (CELL.gridPos() == GRID_POS_V);
                            }));

                        if (iterToCellWithGridPos != std::end(cells))
                        {
                            iterToCellWithGridPos->change();
                        }
                    }
                }
            }
        }

        window.clear(CELL_OUTLINE_COLOR);

        for (const Cell & CELL : cells)
        {
            window.draw(CELL.rectangle());
        }

        window.display();
    }

    return EXIT_SUCCESS;
}
