#ifndef METHHEADS_SETTINGS_HPP_INCLUDED
#define METHHEADS_SETTINGS_HPP_INCLUDED
//
// settings.hpp
//
#include "nameof.hpp"

#include <array>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

namespace methhead
{
    class Settings
    {
      public:
        enum Enum
        {
            None = sf::Keyboard::KeyCount,
            SpecialEffects = sf::Keyboard::Num0,
            BoardPositionOptimization = sf::Keyboard::Num1
        };

        struct Entry
        {
            bool isUsed() const noexcept { return (key != sf::Keyboard::KeyCount); }

            bool is_enabled{ false };
            sf::Keyboard::Key key{ sf::Keyboard::KeyCount };
            Enum enumeration{ static_cast<Enum>(sf::Keyboard::KeyCount) };
        };

        Settings()
        {
            setupEntry(Enum::SpecialEffects, false);
            setupEntry(Enum::BoardPositionOptimization, true);
        }

        template <typename T>
        bool query(const T & findBy) const
        {
            return m_entrys[getIndex(findBy)].is_enabled;
        }

        template <typename T>
        void flip(const T & findBy)
        {
            Entry & entry = m_entrys[getIndex(findBy)];
            if (!entry.isUsed())
            {
                return;
            }

            entry.is_enabled = !entry.is_enabled;

            std::cout << "Keypress '" << nameof::nameof_enum(entry.key)
                      << ((entry.is_enabled) ? "' enabled " : "' disabled ")
                      << nameof::nameof_enum(entry.enumeration) << "." << std::endl;
        }

        void printAll() const
        {
            for (const Entry & entry : m_entrys)
            {
                if (!entry.isUsed())
                {
                    continue;
                }

                std::cout << "Setting " << nameof::nameof_enum(entry.enumeration)
                          << " (key=" << nameof::nameof_enum(entry.key) << ")"
                          << " is " << ((entry.is_enabled) ? "enabled" : "disabled") << std::endl;
            }
        }

      private:
        template <typename T>
        std::size_t getIndex(const T & findBy) const
        {
            const std::size_t index{ static_cast<std::size_t>(findBy) };

            if (index < m_entrys.size())
            {
                return index;
            }
            else
            {
                return (m_entrys.size() - 1);
            }
        }

        void setupEntry(const Enum enumeration, const bool isEnabled)
        {
            Entry & entry = m_entrys[getIndex(enumeration)];

            entry.enumeration = enumeration;
            entry.key = static_cast<sf::Keyboard::Key>(enumeration);
            entry.is_enabled = isEnabled;
        }

      private:
        std::array<Entry, static_cast<std::size_t>(sf::Keyboard::KeyCount + 1)> m_entrys;
    };
} // namespace methhead

#endif // METHHEADS_SETTINGS_HPP_INCLUDED