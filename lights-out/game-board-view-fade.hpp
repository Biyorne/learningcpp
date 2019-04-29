#ifndef LIGHTS_OUT_GAME_BOARD_VIEW_FADE_HPP_INCLUDED
#define LIGHTS_OUT_GAME_BOARD_VIEW_FADE_HPP_INCLUDED

#include "cell-model.hpp"
#include "cell-view.hpp"
#include "game-board-model.hpp"
#include "game-state-enum.hpp"
#include "window.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

namespace lightsout
{
    class GameBoardViewFade
    {
    public:
        GameBoardViewFade(const sf::Color & ON_COLOR, const GameBoardModel & GAME_BOARD_MODEL);

        void setupOverlay(const GameState STATE, const Window & WINDOW);

        sf::Color calcOffColor(const sf::Color & ON_COLOR) const;

        void update(const float ELAPSED_TIME_SEC, const GameBoardModel & GAME_BOARD);

        void draw(const GameBoardModel & GAME_BOARD, Window & window) const;

    private:
        std::vector<CellView> m_cellViews;
        float m_fadeSpeed;
        sf::Texture m_loseTexture;
        sf::Texture m_winTexture;
        sf::Texture m_helpTexture;
        sf::Sprite m_sprite;
        sf::RectangleShape m_fadeRectangle;
    };

} // namespace lightsout

#endif // LIGHTS_OUT_GAME_BOARD_VIEW_FADE_HPP_INCLUDED
