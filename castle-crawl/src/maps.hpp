#ifndef CASTLECRAWL_MAPS_HPP_INCLUDED
#define CASTLECRAWL_MAPS_HPP_INCLUDED
//
// maps.hpp
//
#include "map.hpp"

namespace castlecrawl
{

    class Maps
    {
      public:
        void load(const Context &);

        Maps_t & maps() { return m_maps; }
        const Maps_t & maps() const { return m_maps; }

      private:
        Maps_t m_maps;
    };

} // namespace castlecrawl

#endif // CASTLECRAWL_MAPS_HPP_INCLUDED
