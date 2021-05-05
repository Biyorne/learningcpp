#ifndef CASTLECRAWL_PIECES_HPP_INCLUDED
#define CASTLECRAWL_PIECES_HPP_INCLUDED
//
// pieces.hpp
//
#include "context.hpp"
#include "map-types.hpp"
#include "tile-image.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>

namespace castlecrawl
{

    struct IPiece : public sf::Drawable
    {
        virtual ~IPiece() override = default;

        virtual char mapChar() const = 0;
        virtual bool isObstacle() const = 0;

        virtual MapPos_t position() const = 0;

        // does NOT change the map!
        virtual void move(Context & context, const sf::Keyboard::Key dir) = 0;

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
        PieceBase();

        virtual ~PieceBase() override = default;

        char mapChar() const override final { return m_mapChar; }
        bool isObstacle() const override final { return m_isObstacle; }
        MapPos_t position() const override final { return m_position; }

        // keep this this ONLY way to move pieces, intentionally NOT changing the map!
        void move(Context & context, const sf::Keyboard::Key dir) override final;

        // does nothing by default
        void handleEvent(Context &, const sf::Event &) override {}

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

      protected:
        void reset(
            Context & context, const MapPos_t & pos, const char mapChar, const bool isObstacle);

      protected:
        char m_mapChar;
        bool m_isObstacle;
        sf::Sprite m_sprite;

      private:
        MapPos_t m_position;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_PIECES_HPP_INCLUDED
