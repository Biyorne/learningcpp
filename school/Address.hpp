#ifndef SCHOOL_ADDRESS_HPP_INCLUDED
#define SCHOOL_ADDRESS_HPP_INCLUDED

#include <string>
namespace school
{
    class Address
    {
    public:
        Address(
            const std::string & COUNTRY,
            const std::string & STATE,
            const std::string & CITY,
            const std::string & ZIP,
            const std::string & STREET);

        std::string country() const { return m_country; }

        std::string state() const { return m_state; }

        std::string city() const { return m_city; }

        std::string zip() const { return m_zip; }

        std::string street() const { return m_street; }

        std::string toString() const;

        friend bool operator==(const Address & L, const Address & R);

    private:
        std::string m_country;
        std::string m_state;
        std::string m_city;
        std::string m_zip;
        std::string m_street;
    };

    bool operator==(const Address & L, const Address & R);

    bool operator!=(const Address & L, const Address & R);

} // namespace school
#endif // SCHOOL_ADDRESS_HPP_INCLUDED
