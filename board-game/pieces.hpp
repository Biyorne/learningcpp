#ifndef BOARDGAME_PIECES_HPP_INCLUDED
#define BOARDGAME_PIECES_HPP_INCLUDED
//
// pieces.hpp
//
//
#include "context.hpp"
#include "keys.hpp"
#include "map.hpp"
#include "tile-image.hpp"

#include <list>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include <SFML/Graphics.hpp>

namespace boardgame
{

    struct IPiece : public sf::Drawable
    {
        virtual ~IPiece() = default;

        virtual char mapChar() const = 0;
        virtual bool isObstacle() const = 0;

        virtual MapPos_t position() const = 0;
        virtual void move(const sf::Keyboard::Key dir) = 0;

        virtual void handleEvent(Context &, const sf::Event &) = 0;
        void draw(sf::RenderTarget &, sf::RenderStates) const override = 0;
    };

    //

    // using IPieceOpt_t = std::optional<std::reference_wrapper<IPiece>>;
    // using IPieceUPtr_t = std::unique_ptr<IPiece>;

    // ALL pieces must derive from this
    class PieceBase : public IPiece
    {
      public:
        PieceBase(const MapPos_t & pos, const char mapChar = ' ', const bool isObstacle = true);

        virtual ~PieceBase() = default;

        char mapChar() const override final { return m_mapChar; }
        bool isObstacle() const override final { return m_isObstacle; }
        MapPos_t position() const override final { return m_position; }

        // keep this this ONLY way to move pieces, intentionally NOT changing the map!
        void move(const sf::Keyboard::Key dir) override final
        {
            m_position = keys::moveIfDir(m_position, dir);
        }

        // does nothing by default
        void handleEvent(Context &, const sf::Event &) override {}

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override = 0;

      protected:
        char m_mapChar;
        bool m_isObstacle;

      private:
        MapPos_t m_position;
    };

} // namespace boardgame

#endif // BOARDGAME_PIECES_HPP_INCLUDED
