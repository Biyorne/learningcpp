#ifndef CASTLECRAWL_SUMMONIMAGE_HPP_INCLUDED
#define CASTLECRAWL_SUMMONIMAGE_HPP_INCLUDED
//
// summon-image.hpp
//
#include <cstddef>

#include <SFML/Graphics/Rect.hpp>

namespace castlecrawl
{
    // used to index so keep unsigned
    enum class SummonImage : std::size_t
    {
        SnakeBag,
        Snkake1,
        Snkake2,
        Snkake3,
        Snkake4,
        Snkake5,
        Snkake6,
        SpiderWeb,
        Spider1,
        Spider2,
        Spider3,
        Spider4,
        Spider5,
        Spider6,
        GoblinPot,
        Goblin1,
        Goblin2,
        Goblin3,
        Goblin4,
        Goblin5,
        Goblin6,
        BirdMask,
        Bat1,
        Bat2,
        Bat3,
        Bat4,
        Bat5,
        Bat6,
        PixieGoblet,
        Pixie1,
        Pixie2,
        Pixie3,
        Pixie4,
        Pixie5,
        Pixie6,
        SkeletonGrave,
        Skeleton1,
        Skeleton2,
        Skeleton3,
        Skeleton4,
        Skeleton5,
        Skeleton6,
        DemonDoor,
        Demon1,
        Demon2,
        Demon3,
        Demon4,
        Demon5,
        Demon6,
        DragonBreath,
        Dragon1,
        Dragon2,
        Dragon3,
        Dragon4,
        Dragon5,
        Dragon6,
        WizardTomb,
        Wizard1,
        Wizard2,
        Wizard3,
        Wizard4,
        Wizard5,
        Wizard6,
        Count
    };

    inline sf::IntRect summonImageToTileRect(const SummonImage image)
    {
        // clang-format off
        switch (image)
        {
            case SummonImage::SnakeBag:     return sf::IntRect(   0,   0,  64,  64);
            case SummonImage::Snkake1:      return sf::IntRect(  64,   0,  64,  64);
            case SummonImage::Snkake2:      return sf::IntRect( 128,   0,  64,  64);
            case SummonImage::Snkake3:      return sf::IntRect( 192,   0,  64,  64);
            case SummonImage::Snkake4:      return sf::IntRect( 256,   0,  64,  64);
            case SummonImage::Snkake5:      return sf::IntRect( 320,   0,  64,  64);
            case SummonImage::Snkake6:      return sf::IntRect( 384,   0,  64,  64);
            case SummonImage::SpiderWeb:    return sf::IntRect(   0,  64,  64,  64);
            case SummonImage::Spider1:      return sf::IntRect(  64,  64,  64,  64);
            case SummonImage::Spider2:      return sf::IntRect( 128,  64,  64,  64);
            case SummonImage::Spider3:      return sf::IntRect( 192,  64,  64,  64);
            case SummonImage::Spider4:      return sf::IntRect( 256,  64,  64,  64);
            case SummonImage::Spider5:      return sf::IntRect( 320,  64,  64,  64);
            case SummonImage::Spider6:      return sf::IntRect( 384,  64,  64,  64);
            case SummonImage::GoblinPot:    return sf::IntRect(   0, 128,  64,  64);
            case SummonImage::Goblin1:      return sf::IntRect(  64, 128,  64,  64);
            case SummonImage::Goblin2:      return sf::IntRect( 128, 128,  64,  64);
            case SummonImage::Goblin3:      return sf::IntRect( 192, 128,  64,  64);
            case SummonImage::Goblin4:      return sf::IntRect( 256, 128,  64,  64);
            case SummonImage::Goblin5:      return sf::IntRect( 320, 128,  64,  64);
            case SummonImage::Goblin6:      return sf::IntRect( 384, 128,  64,  64);
            case SummonImage::BirdMask:     return sf::IntRect(   0, 192,  64,  64);
            case SummonImage::Bat1:         return sf::IntRect(  64, 192,  64,  64);
            case SummonImage::Bat2:         return sf::IntRect( 128, 192,  64,  64);
            case SummonImage::Bat3:         return sf::IntRect( 192, 192,  64,  64);
            case SummonImage::Bat4:         return sf::IntRect( 256, 192,  64,  64);
            case SummonImage::Bat5:         return sf::IntRect( 320, 192,  64,  64);
            case SummonImage::Bat6:         return sf::IntRect( 384, 192,  64,  64);
            case SummonImage::PixieGoblet:  return sf::IntRect(   0, 256,  64,  64);
            case SummonImage::Pixie1:       return sf::IntRect(  64, 256,  64,  64);
            case SummonImage::Pixie2:       return sf::IntRect( 128, 256,  64,  64);
            case SummonImage::Pixie3:       return sf::IntRect( 192, 256,  64,  64);
            case SummonImage::Pixie4:       return sf::IntRect( 256, 256,  64,  64);
            case SummonImage::Pixie5:       return sf::IntRect( 320, 256,  64,  64);
            case SummonImage::Pixie6:       return sf::IntRect( 384, 256,  64,  64);
            case SummonImage::SkeletonGrave:return sf::IntRect(   0, 320,  64,  64);
            case SummonImage::Skeleton1:    return sf::IntRect(  64, 320,  64,  64);
            case SummonImage::Skeleton2:    return sf::IntRect( 128, 320,  64,  64);
            case SummonImage::Skeleton3:    return sf::IntRect( 192, 320,  64,  64);
            case SummonImage::Skeleton4:    return sf::IntRect( 256, 320,  64,  64);
            case SummonImage::Skeleton5:    return sf::IntRect( 320, 320,  64,  64);
            case SummonImage::Skeleton6:    return sf::IntRect( 384, 320,  64,  64);
            case SummonImage::DemonDoor:    return sf::IntRect(   0, 384,  64,  64);
            case SummonImage::Demon1:       return sf::IntRect(  64, 384,  64,  64);
            case SummonImage::Demon2:       return sf::IntRect( 128, 384,  64,  64);
            case SummonImage::Demon3:       return sf::IntRect( 192, 384,  64,  64);
            case SummonImage::Demon4:       return sf::IntRect( 256, 384,  64,  64);
            case SummonImage::Demon5:       return sf::IntRect( 320, 384,  64,  64);
            case SummonImage::Demon6:       return sf::IntRect( 384, 384,  64,  64);
            case SummonImage::DragonBreath: return sf::IntRect(   0, 448,  64,  64);
            case SummonImage::Dragon1:      return sf::IntRect(  64, 448,  64,  64);
            case SummonImage::Dragon2:      return sf::IntRect( 128, 448,  64,  64);
            case SummonImage::Dragon3:      return sf::IntRect( 192, 448,  64,  64);
            case SummonImage::Dragon4:      return sf::IntRect( 256, 448,  64,  64);
            case SummonImage::Dragon5:      return sf::IntRect( 320, 448,  64,  64);
            case SummonImage::Dragon6:      return sf::IntRect( 384, 448,  64,  64);
            case SummonImage::WizardTomb:   return sf::IntRect( 448,   0,  64,  64);
            case SummonImage::Wizard1:      return sf::IntRect( 448,  64,  64,  64);
            case SummonImage::Wizard2:      return sf::IntRect( 448, 128,  64,  64);
            case SummonImage::Wizard3:      return sf::IntRect( 448, 192,  64,  64);
            case SummonImage::Wizard4:      return sf::IntRect( 448, 256,  64,  64);
            case SummonImage::Wizard5:      return sf::IntRect( 448, 320,  64,  64);
            case SummonImage::Wizard6:      return sf::IntRect( 448, 384,  64,  64);
            case SummonImage::Count:
            default:                         return sf::IntRect(0,0,0,0);
        }
        // clang-format on
    }

} // namespace castlecrawl
#endif // #define CASTLECRAWL_SUMMONIMAGE_HPP_INCLUDED
