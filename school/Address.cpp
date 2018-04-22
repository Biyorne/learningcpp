#include "Address.hpp"
#include <sstream>
#include <tuple>

namespace school
{
    Address::Address(
        const std::string & COUNTRY,
        const std::string & STATE,
        const std::string & CITY,
        const std::string & ZIP,
        const std::string & STREET)
        : m_country(COUNTRY)
        , m_state(STATE)
        , m_city(CITY)
        , m_zip(ZIP)
        , m_street(STREET)
    {}

    std::string Address::toString() const
    {
        std::stringstream ss;
        ss << m_country << " " << m_state << " " << m_city << " " << m_zip << " " << m_street;
        return ss.str();
    }

    bool operator==(const Address & L, const Address & R)
    {
        return (
            std::tie(L.m_country, L.m_state, L.m_city, L.m_zip, L.m_street)
            == std::tie(R.m_country, R.m_state, R.m_city, R.m_zip, R.m_street));
    }

    bool operator!=(const Address & L, const Address & R) { return !(L == R); }

} // namespace school
