#include "Class.hpp"
#include <sstream>
namespace school
{

		Class::Class(
			const std::string & NAME,
			const Employee & INSTRUCTOR,
			const Date & DATE,
			const Address & LOCATION,
			const TimeOfDay & MEETINGTIME)
			: m_name(NAME)
			, m_instructor(INSTRUCTOR)
			, m_date(DATE)
			, m_meetingTime(MEETINGTIME)
			, m_location(LOCATION)
		{}

		std::string Class::toString() const
		{
			std::stringstream ss;
			ss << "Class " << m_name << " taught by " << m_instructor.toString() << " first meeting on "
				<< m_date.toString() << " at " << m_meetingTime.toString() << " at "
				<< m_location.toString() << ".";
			return ss.str();
		}
				
}
