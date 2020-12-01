#include <cstdlib>
#include <iostream>
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

    bool isValidChoice(const std::size_t choice) const
    {
        return ((choice <= (choices.size())) && (choice != 0));
    }

    std::size_t changeStage(const std::size_t choice) const
    {
        if (!isValidChoice(choice))
        {
            std::cout << "Invalid choice, staying here." << std::endl;
            return id;
        }

        return choices.at(choice - 1).id;
    }
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

        std::size_t number { '\0' };
        std::cin >> number;
        std::cout << "\n\n";

        const std::size_t nextStageId { jungle.changeStage(number) };
        std::cout << "Transition to stage " << nextStageId << std::endl;
    }

    return EXIT_SUCCESS;
}

/*
 *
 * You are int eh jungle.
 * (1) "to go left"
 * (2) "to go right"
 * */
