#include "game-board-view-fade.hpp"

namespace lightsout
{
    GameBoardViewFade::GameBoardViewFade(
        const sf::Color & ON_COLOR, const GameBoardModel & GAME_BOARD_MODEL)
        : m_cellViews()
        , m_fadeSpeed(500.0f)
        , m_loseTexture()
        , m_winTexture()
        , m_helpTexture()
        , m_sprite()
        , m_fadeRectangle()
    {
        const sf::Color OFF_COLOR(calcOffColor(ON_COLOR));
        for (const CellModel & CELL_MODEL : GAME_BOARD_MODEL.cells())
        {
            const CellView CELL_VIEW(ON_COLOR, OFF_COLOR, CELL_MODEL.gridPos(), CELL_MODEL.isOn());

            m_cellViews.push_back(CELL_VIEW);
        }

        m_loseTexture.setSmooth(true);
        m_winTexture.setSmooth(true);
        m_helpTexture.setSmooth(true);

        if (!m_loseTexture.loadFromFile("image\\lose.png"))
        {
            std::cerr << "Failed to load image: image\\lose.png" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!m_winTexture.loadFromFile("image\\win.png"))
        {
            std::cerr << "Failed to load image: image\\win.png" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!m_helpTexture.loadFromFile("image\\help.png"))
        {
            std::cerr << "Failed to load image: image\\help.png" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void GameBoardViewFade::setupOverlay(const GameState STATE, const Window & WINDOW)
    {
        if (STATE == GameState::Help)
        {
            m_sprite.setTexture(m_helpTexture);
        }
        else if (STATE == GameState::Win)
        {
            m_sprite.setTexture(m_winTexture);
        }
        else if (STATE == GameState::Lose)
        {
            m_sprite.setTexture(m_loseTexture);
        }
        else
        {
            m_sprite = sf::Sprite();
        }

        WINDOW.spriteFitHorizontalAndCenter(m_sprite);

        m_fadeRectangle.setSize(WINDOW.size());

        if (STATE == GameState::Play)
        {
            m_fadeRectangle.setFillColor(sf::Color::Transparent);
        }
        else
        {
            m_fadeRectangle.setFillColor(sf::Color(0, 0, 0, 140));
        }
    }

    sf::Color GameBoardViewFade::calcOffColor(const sf::Color ON_COLOR) const
    {
        sf::Color offColor(ON_COLOR);
        offColor.r = (ON_COLOR.r / 2);
        offColor.g = (ON_COLOR.g / 2);
        offColor.b = (ON_COLOR.b / 2);
        return offColor;
    }

    void GameBoardViewFade::update(const float ELAPSED_TIME_SEC, const GameBoardModel & GAME_BOARD)
    {
        for (const CellModel & CELL_MODEL : GAME_BOARD.cells())
        {
            GridPos_t GRID_POSITION_V(CELL_MODEL.gridPos());

            auto iterToFoundCellView(std::find_if(
                std::begin(m_cellViews),
                std::end(m_cellViews),
                [&GRID_POSITION_V](const CellView & CELL_VIEW) {
                    return (CELL_VIEW.gridPosition() == GRID_POSITION_V);
                }));

            if (std::end(m_cellViews) != iterToFoundCellView)
            {
                iterToFoundCellView->setIsOn(CELL_MODEL.isOn());
                iterToFoundCellView->updateCurrentColor(ELAPSED_TIME_SEC, m_fadeSpeed);
            }
        }
    }

    void GameBoardViewFade::draw(const GameBoardModel & GAME_BOARD, Window & window) const
    {
        for (const CellModel & CELL_MODEL : GAME_BOARD.cells())
        {
            GridPos_t GRID_POSITION_V(CELL_MODEL.gridPos());

            auto iterToFoundCellView(std::find_if(
                std::begin(m_cellViews),
                std::end(m_cellViews),
                [&GRID_POSITION_V](const CellView & CELL_VIEW) {
                    return (CELL_VIEW.gridPosition() == GRID_POSITION_V);
                }));

            if (std::end(m_cellViews) != iterToFoundCellView)
            {
                const sf::Color CURRENT_COLOR(iterToFoundCellView->currentColor());
                window.drawRectangle(CELL_MODEL.region(), CURRENT_COLOR);
            }
        }
        window.draw(m_fadeRectangle);
        window.draw(m_sprite);
    }
} // namespace lightsout
