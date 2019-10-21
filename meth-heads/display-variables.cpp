// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "display-variables.hpp"

#include "utils.hpp"

namespace methhead
{
    DisplayVariables::DisplayVariables(const sf::Vector2u & windowSize)
        : m_constants(windowSize)
        , m_lazyScoreRectangle()
        , m_greedyScoreRectangle()
        , m_lootSprite(m_constants.loot_texture)
        , m_lootText(m_constants.default_text)
    {
        m_lootSprite.setColor(sf::Color(255, 255, 255, 127));
        m_lootText.setFillColor(sf::Color::Yellow);
        m_lazyScoreRectangle.setFillColor(m_constants.lazy_color);
        m_greedyScoreRectangle.setFillColor(m_constants.greedy_color);
    }

    void DisplayVariables::update(
        const float,
        const std::size_t lazyScore,
        const std::size_t greedyScore,
        const BoardMap_t & board)
    {
        setScoreBarsHeight(lazyScore, greedyScore);

        for (const auto & [boardPos, cell] : board)
        {
            if (cell.loot <= 0)
            {
                continue;
            }

            placeInBounds(m_lootSprite, cell.bounds());

            m_lootText.setString(std::to_string(cell.loot));
            placeInBounds(m_lootText, cell.bounds());
        }
    }

    void DisplayVariables::draw(
        sf::RenderTarget & target, sf::RenderStates states, const BoardMap_t & board) const
    {
        target.draw(m_lazyScoreRectangle, states);
        target.draw(m_greedyScoreRectangle, states);

        for (const auto & [boardPos, cell] : board)
        {
            target.draw(cell.rectangle);

            if (cell.loot <= 0)
            {
                continue;
            }

            target.draw(m_lootSprite);
            target.draw(m_lootText);
        }
    }

    BoardMap_t DisplayVariables::makeBoard() const
    {
        BoardMap_t board;

        for (std::size_t horiz(0); horiz < m_constants.horiz_cell_count; ++horiz)
        {
            for (std::size_t vert(0); vert < m_constants.vert_cell_count; ++vert)
            {
                const sf::Vector2i boardPos(static_cast<int>(horiz), static_cast<int>(vert));
                const sf::Vector2f windowPos(boardToWindowPos(boardPos));

                const Cell cell(boardPos, windowPos, m_constants.cell_size);

                board.insert(std::make_pair(cell.board_pos, cell));
            }
        }

        return board;
    }

    // TODO Minor optimization: Move some bounds settings to the constructor
    void DisplayVariables::setScoreBarsHeight(
        const std::size_t lazyScore, const std::size_t greedyScore)
    {
        // split the bounds into two halves for lazy vs greedy
        sf::FloatRect bounds(m_constants.score_window_bounds);

        // make the width just under half to put a space between that looks nice
        sf::FloatRect lazyBounds(bounds);
        lazyBounds.width *= 0.475f;

        sf::FloatRect greedyBounds(lazyBounds);
        greedyBounds.left = ((bounds.left + bounds.width) - greedyBounds.width);

        // shrink the height of the loser
        if (lazyScore != greedyScore)
        {
            const auto highScore(std::max(lazyScore, greedyScore));
            const auto lowScore(std::min(lazyScore, greedyScore));

            const float shrinkRatio(static_cast<float>(lowScore) / static_cast<float>(highScore));

            if (lazyScore < greedyScore)
            {
                lazyBounds.height *= shrinkRatio;
            }
            else
            {
                greedyBounds.height *= shrinkRatio;
            }
        }

        // position and size both rectangles
        m_lazyScoreRectangle.setPosition(
            lazyBounds.left, ((bounds.top + bounds.height) - lazyBounds.height));

        m_lazyScoreRectangle.setSize({ lazyBounds.width, lazyBounds.height });

        m_greedyScoreRectangle.setPosition(
            greedyBounds.left, ((bounds.top + bounds.height) - greedyBounds.height));

        m_greedyScoreRectangle.setSize({ greedyBounds.width, greedyBounds.height });
    }

    sf::Vector2f DisplayVariables::boardToWindowPos(const sf::Vector2i & cellPos) const
    {
        const auto bounds(m_constants.board_window_bounds);

        const sf::Vector2f boardPos(bounds.left, bounds.top);

        const sf::Vector2f gridTotalSize(
            (m_constants.cell_size.x * static_cast<float>(m_constants.horiz_cell_count)),
            (m_constants.cell_size.y * static_cast<float>(m_constants.vert_cell_count)));

        const float centerOffsetHoriz((bounds.width - gridTotalSize.x) * 0.5f);
        const float centerOffsetVert((bounds.height - gridTotalSize.y) * 0.5f);
        const sf::Vector2f centerOffset(centerOffsetHoriz, centerOffsetVert);

        sf::Vector2f pos(
            (static_cast<float>(cellPos.x) * m_constants.cell_size.x),
            (static_cast<float>(cellPos.y) * m_constants.cell_size.y));

        pos += (boardPos + centerOffset);
        return pos;
    }

} // namespace methhead
