#ifndef MARIO_IMAGE_HPP_INCLUDED
#define MARIO_IMAGE_HPP_INCLUDED
//
// mover.hpp
//
#include "context.hpp"
#include "mover.hpp"
#include "util.hpp"

#include <SFML/Graphics.hpp>

namespace mario
{
    struct Image
    {
        enum Enum : std::size_t
        {
            MarioStand = 0,
            MarioRun,
            MarioJump,
            //
            Fireball,
            Coin,
            Mushroom,
            //
            AngryTurtle,
            AngryMushroom,
            AngryBall,
            AngrySpikey,
            //
            Pipe,
            //
            BlockQLight,
            BlockQDark,
            BlockBrick,
            BlockGround,
            BlockRock,
            BlockQSpent,
            //
            CloudBig,
            CloudSmall,
            TreeSmall,
            TreeBig,
            BushBig,
            BushSmall,
            Hill,
            //
            Count
        };

        static void setup()
        {
            const std::string path{ "mario-media/image-tiles.png" };

            if (!texture.loadFromFile(path))
            {
                throw std::runtime_error("Failed to load image: \"" + path + "\".");
            }

            // texture.setSmooth(true);
        }

        // clang-format off
        static sf::IntRect textureRect(const Enum which)
        {
            switch(which)
            {
                case MarioStand:      return sf::IntRect(  0,  0,   16, 32);
                case MarioRun:        return sf::IntRect( 16,  0,   16, 32);
                case MarioJump:       return sf::IntRect( 32,  0,   16, 32);
                case Fireball:        return sf::IntRect( 48,  0,    8,  8);
                case AngryTurtle:     return sf::IntRect( 48, 11,   16, 21);
                case AngryMushroom:   return sf::IntRect( 64,  0,   16, 16);
                case AngryBall:       return sf::IntRect( 64, 16,   16, 16);
                case Coin:            return sf::IntRect( 80,  0,    8, 16);
                case Mushroom:        return sf::IntRect( 80, 16,   16, 16);
                case BlockQLight:     return sf::IntRect( 96,  0,   16, 16);
                case BlockQDark:      return sf::IntRect( 96, 16,   16, 16);
                case AngrySpikey:     return sf::IntRect(112,  0,   16, 16);
                case CloudBig:        return sf::IntRect(  0, 32,   64, 32);
                case CloudSmall:      return sf::IntRect( 64, 32,   32, 32);
                case TreeSmall:       return sf::IntRect( 96, 32,   16, 32);
                case TreeBig:         return sf::IntRect(112, 16,   16, 48);
                case BushBig:         return sf::IntRect(  0, 64,   64, 16);
                case BushSmall:       return sf::IntRect( 64, 64,   32, 16);
                case BlockBrick:      return sf::IntRect( 96, 64,   16, 16);
                case BlockGround:     return sf::IntRect(112, 64,   16, 16);
                case Pipe:            return sf::IntRect(  0, 80,   32, 48);
                case Hill:            return sf::IntRect( 32, 80,   80, 48);
                case BlockQSpent:     return sf::IntRect(112, 80,   16, 16);
                case BlockRock:       return sf::IntRect(112, 96,   16, 16);
                case Count:     throw std::runtime_error("Image::textureRect(Count)");
                default:        throw std::runtime_error("Image::textureRect(enum) switch default");
            };
        }
        // clang-format on

        static sf::Sprite makeSprite(const Context & context, const Enum which)
        {
            sf::Sprite sprite(texture, textureRect(which));
            sprite.setScale(context.block_scale);
            return sprite;
        }

        // clang-format off
        static Mover makeMover(const Enum which)
        {
            switch(which)
            {
                case Count:  throw std::runtime_error("Image::makeMover(Count)");
                //
                case Hill:
                case BlockQSpent:
                case BlockRock:
                case BlockQLight:
                case BlockQDark:
                case BlockBrick:
                case BlockGround:
                case Pipe:
                case TreeSmall:
                case TreeBig:
                case BushBig:
                case BushSmall:     return Mover({0.0f, 0.0f}, {-1.0f, -1.0f});
                //
                case AngryTurtle:
                case AngryMushroom:
                case AngryBall:
                case Mushroom:
                case AngrySpikey:   return Mover({1.0f, 0.0f}, {4.0f, 10.0f});
                //
                case Fireball:      return Mover({1.0f, 1.0f}, {12.0f, 0.0f});
                //
                case Coin:          return Mover({-1.0f, 1.0f}, {10.0f, 0.0f});
                //
                case CloudBig:      return Mover({0.25f, 0.0f}, {2.0f, -1.0f});
                case CloudSmall:    return Mover({0.5f, 0.0f}, {2.0f, -1.0f});
                //
                case MarioStand:
                case MarioRun:
                case MarioJump:
                default:            return Mover({0.0f, 0.0f}, {8.0f, 0.0f});
            };
        }
        // clang-format on

        // clang-format off
        static bool isObstacle(const Enum which)
        {
            switch(which)
            {
                case BlockQSpent:
                case BlockRock:
                case BlockQLight:
                case BlockQDark:
                case BlockBrick:
                case BlockGround:
                case Pipe:
                case AngryTurtle:
                case AngryMushroom:
                case AngryBall:
                case AngrySpikey:   return true;
                //
                case MarioStand:
                case MarioRun:
                case MarioJump:
                case Coin:
                case Fireball:
                case Mushroom:
                case CloudBig:
                case CloudSmall:
                case Hill:
                case TreeSmall:
                case TreeBig:
                case BushBig:
                case BushSmall:
                case Count:
                default:            return false;
            };
        }
        // clang-format on

        // clang-format off
        static bool canMove(const Enum which)
        {
            switch(which)
            {
                case Count:  throw std::runtime_error("Image::makeMover(Count)");
                //
                case Hill:
                case BlockQSpent:
                case BlockRock:
                case BlockQLight:
                case BlockQDark:
                case BlockBrick:
                case BlockGround:
                case Pipe:
                case TreeSmall:
                case TreeBig:
                case BushBig:
                case BushSmall:     return false;
                //
                case AngryTurtle:
                case AngryMushroom:
                case AngryBall:
                case Mushroom:
                case AngrySpikey:
                case Fireball:
                case Coin:
                case CloudBig:
                case CloudSmall:
                case MarioStand:
                case MarioRun:
                case MarioJump:
                default:            return true;
            };
        }
        // clang-format on

        // clang-format off
        static bool canHitObstacles(const Enum which)
        {
            switch(which)
            {
                case AngryTurtle:
                case AngryMushroom:
                case AngryBall:
                case Mushroom:
                case AngrySpikey:
                case Fireball:
                case Coin:
                case MarioStand:
                case MarioRun:
                case MarioJump:     return true;
                //
                case CloudBig:
                case CloudSmall:
                case Hill:
                case BlockQSpent:
                case BlockRock:
                case BlockQLight:
                case BlockQDark:
                case BlockBrick:
                case BlockGround:
                case Pipe:
                case TreeSmall:
                case TreeBig:
                case BushBig:
                case BushSmall:
                case Count:
                default:            return false;
            };
        }
        // clang-format on

        static bool isBackground(const Enum which)
        {
            return (
                (which == CloudBig) || (which == CloudSmall) || (which == TreeSmall) ||
                (which == TreeBig) || (which == BushBig) || (which == BushSmall) ||
                (which == Hill));
        }

        static bool willBounce(const Enum which)
        {
            return ((which == Image::Fireball) || (which == Image::Coin));
        }

        static bool willFall(const Enum which)
        {
            return (canMove(which) && (which != Image::CloudBig) && (which != Image::CloudSmall));
        }

        static inline sf::Texture texture;
    };
} // namespace mario

#endif // MARIO_IMAGE_HPP_INCLUDED