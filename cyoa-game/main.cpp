#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

struct Transition
{
    Transition(const std::size_t pId, const std::string & pDesc)
        : id { pId }
        , description { pDesc }
    { }

    std::size_t id;
    std::string description;
};

struct Stage
{
    Stage(
        const std::size_t pId, const std::string & pDesc, const std::vector<Transition> & pChoices)
        : id { pId }
        , description { pDesc }
        , choices { pChoices }
    { }

    std::size_t id;
    std::string description;
    std::vector<Transition> choices;
};

int main()
{
    const Stage jungle { 0,
                         "You are in a jungle. Do you go:",
                         { Transition { 1, "left" }, Transition { 2, "right" } } };

    const Stage jungleWin { 1, "You escaped the jungle! You win!", {} };

    const Stage jungleLose { 2, "You are eaten by a grue. You lose.", {} };

    while (true)
    {
        std::cout << jungle.description << std::endl;

        if (jungle.choices.empty())
        {
            break;
        }

        for (std::size_t i { 0 }; i < jungle.choices.size(); ++i)
        {
            std::cout << "(" << (i + 1) << ") " << jungle.choices.at(i).description << std::endl;
        }

        std::size_t number { jungle.id };
        std::cin >> number;
        --number;

        if (number < jungle.choices.size())
        {
            const std::size_t nextStageId { jungle.choices.at(number).id };
            std::cout << "Transition to stage " << nextStageId << std::endl;
        }
        else
        {
            std::cout << "Invalid choice, staying here." << std::endl;
        }

        std::cout << "\n\n";
    }

    return EXIT_SUCCESS;
}
