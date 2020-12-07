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

inline std::size_t playStage(const Stage & stage)
{
    std::cout << "\n\n" << stage.description << std::endl;

    for (std::size_t i { 0 }; i < stage.choices.size(); ++i)
    {
        std::cout << "(" << (i + 1) << ") " << stage.choices.at(i).description << std::endl;
    }

    std::size_t choice { stage.id };
    std::cin >> choice;
    --choice;

    if (!std::cin)
    {
        std::cin.clear();
        std::string line;
        std::getline(std::cin, line);

        std::cout << "Invalid input \"" << line << "\" -not a choice.  Staying here.\n";
        return stage.id;
    }

    if (choice < stage.choices.size())
    {
        const std::size_t nextStageId { stage.choices.at(choice).id };
        std::cout << "Transition to stage " << nextStageId << std::endl;
        return nextStageId;
    }
    else
    {
        std::cout << "Invalid choice, staying here." << std::endl;
        return stage.id;
    }
}

int main()
{
    // game layout
    const Stage jungle { 0,
                         "You are in a jungle. Do you go:",
                         { Transition { 1, "left" }, Transition { 2, "right" } } };

    const Stage jungleWin { 1, "You escaped the jungle! You win!", {} };

    const Stage jungleLose { 2, "You are eaten by a grue. You lose.", {} };

    const std::vector<Stage> stages = { jungle, jungleWin, jungleLose };

    // game engine
    std::size_t currentStageId { jungle.id };

    while (!stages.at(currentStageId).choices.empty())
    {
        currentStageId = playStage(stages.at(currentStageId));
    }

    std::cout << stages.at(currentStageId).description << '\n' << "Game Over." << std::endl;

    return EXIT_SUCCESS;
}
