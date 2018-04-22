#ifndef SCHOOL_CLASS_HPP_INCLUDED
#define SCHOOL_CLASS_HPP_INCLUDED

#include "Address.hpp"
#include "Date.hpp"
#include "Employee.hpp"
#include "TimeOfDay.hpp"
#include <string>
namespace school
{
	class Class
	{
	public:
		Class(const std::string & NAME, const Employee & INSTRUCTOR, const Date & DATE, const Address & LOCATION, const TimeOfDay & MEETINGTIME);

		std::string name() const { return m_name; }

		Employee instructor() const { return m_instructor; }

		Date date() const { return m_date; }

		TimeOfDay meetingTime() const { return m_meetingTime; }

		Address location() const { return m_location; }

		std::string toString() const;

	private:
		std::string m_name;
		Employee m_instructor;
		Date m_date;
		TimeOfDay m_meetingTime;
		Address m_location;
	};
}
#endif //SCHOOL_CLASS_HPP_INCLUDED
