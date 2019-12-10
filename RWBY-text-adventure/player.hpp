#ifndef RWBY_TEXT_ADVENTURE_PLAYER_HPP_INCLUDED
#define RWBY_TEXT_ADVENTURE_PLAYER_HPP_INCLUDED

#include <cstdlib>

namespace rwby
{
    enum class Gender
    {
        NonBinary,
        Male,
        Female,
    };

    class Player
    {
    public:
        Player()
            : m_firstName("Default")
            , m_lastName("McDefault")
            , m_gender(Gender::NonBinary)
        {}

        void introSpeech()
        {
            std::cout << "\"Greetings. I am Professor Ozpin, the headmaster of Beacon Academy.\""
                      << std::endl;

            std::cout << "\nThe white haired man glances up at you from a stack of papers on his "
                         "desk and a warm smile "
                         "graces his lips. Despite his welcoming demeanor, his eyes "
                         "betray a deep melancholy behind the small dark lenses of his glasses. "
                         "However, before you can ponder the reason, he speaks to you again."
                      << std::endl;
            std::cout
                << "\"Before I can allow you to begin your journey as a student here at Beacon, "
                << "I must ask you to fill in a few... unfortunately illegible fields on your "
                   "admission form.\""
                << std::endl;
            std::cout
                << "\nHe shuffles through his doccuments, a few badly stained with what appears to "
                   "be "
                   "hot cocoa, then slowly slides you the aforementioned form along with a pen.";
        }

        void characterNaming()
        {
            std::cout << "As you look over the form, you see what information was missing..."
                      << std::endl;
            std::cout << "\nName: " << std::endl;

            std::string tempPlayerName;
            std::cin >> tempPlayerName;
            m_firstName = tempPlayerName;

            std::cout << "\nThank you, " << m_firstName << "." << std::endl;
        }

        void characterGender()
        {
            std::cout << "\nGender: (M)ale, (F)emale, (N)onbinary: " << std::endl;
            char genderTemp;
            std::cin >> genderTemp;

            if ('M' == genderTemp)
            {
                m_gender = Gender::Male;
            }
            else if ('F' == genderTemp)
            {
                m_gender = Gender::Female;
            }
            else
            {
                m_gender = Gender::NonBinary;
            }

            std::cout << "\nSo you are " << m_gender << "?" << std::endl;
        }

    private:
        std::string m_firstName;
        std::string m_lastName;
        Gender m_gender;
    };
} // namespace rwby

#endif // RWBY_TEXT_ADVENTURE_PLAYER_HPP_INCLUDED
