// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "score.hpp"

#include "meth-head.hpp"
#include "sim-context.hpp"
#include "utils.hpp"

#include <cassert>

namespace methhead
{
    Score::Score()
        : m_updateClock()
        , m_updateIntervalTimeSec(1.0f)
        , m_framesPerUpdateCheck(30)
        , m_framesSinceUpdate(0)
        , m_barQuadVerts(sf::Quads)
        , m_lazyScore(0)
        , m_greedyScore(0)
    {}

    void Score::draw(sf::RenderTarget & target, sf::RenderStates states) const
    {
        target.draw(m_barQuadVerts, states);
    }

    void Score::update(const SimContext & context, const float)
    {
        ++m_framesSinceUpdate;

        // only bother checking if it's time to display status every 100th frame
        if ((m_framesSinceUpdate % m_framesPerUpdateCheck) != 0)
        {
            return;
        }

        const float elapsedSincePrevTimeSec{ m_updateClock.getElapsedTime().asSeconds() };
        if (elapsedSincePrevTimeSec < m_updateIntervalTimeSec)
        {
            return;
        }

        updateScores(context);
        updateScoreBars(context);
        printStatus(context, elapsedSincePrevTimeSec);

        m_updateClock.restart();
        m_framesSinceUpdate = 0;
    }

    void Score::updateScoreBars(const SimContext & context)
    {
        sf::FloatRect bothRect(context.display.score_rect);
        const float bothRight{ bothRect.left + bothRect.width };
        const float bothBottom{ bothRect.top + bothRect.height };

        // split the rect into two halves for lazy vs greedy

        // make the width just under half to put a space between that looks nice
        sf::FloatRect lazyRect(bothRect);
        lazyRect.width *= 0.475f;

        sf::FloatRect greedyRect(lazyRect);
        greedyRect.left = (bothRight - greedyRect.width);

        // shrink the height of the loser
        if (m_lazyScore != m_greedyScore)
        {
            const auto highScore{ std::max(m_lazyScore, m_greedyScore) };
            const auto lowScore{ std::min(m_lazyScore, m_greedyScore) };
            const float shrinkRatio{ static_cast<float>(lowScore) / static_cast<float>(highScore) };

            if (m_lazyScore < m_greedyScore)
            {
                lazyRect.height *= shrinkRatio;
            }
            else
            {
                greedyRect.height *= shrinkRatio;
            }
        }

        m_barQuadVerts.clear();

        const sf::Vector2f lazyPos{ lazyRect.left, (bothBottom - lazyRect.height) };
        const sf::Vector2f lazySize{ lazyRect.width, lazyRect.height };
        appendQuadVerts(lazyPos, lazySize, m_barQuadVerts, context.display.lazy_color);

        const sf::Vector2f greedyPos{ greedyRect.left, (bothBottom - greedyRect.height) };
        const sf::Vector2f greedySize{ greedyRect.width, greedyRect.height };
        appendQuadVerts(greedyPos, greedySize, m_barQuadVerts, context.display.greedy_color);
    }

    void Score::updateScores(const SimContext & context)
    {
        m_lazyScore = 0;
        m_greedyScore = 0;

        for (const IActorUPtr_t & actorUPtr : context.actors)
        {
            if (actorUPtr->motivation() == Motivation::lazy)
            {
                m_lazyScore = actorUPtr->score();
            }
            else
            {
                m_greedyScore = actorUPtr->score();
            }

            if ((m_lazyScore > 0) && (m_greedyScore > 0))
            {
                break;
            }
        }
    }

    void Score::printStatus(const SimContext & context, const float elapsedSincePrevUpdateTimeSec)
    {
        const std::size_t fpsCount(static_cast<std::size_t>(
            static_cast<float>(m_framesSinceUpdate) / elapsedSincePrevUpdateTimeSec));

        const std::size_t lazyActorCount{ static_cast<std::size_t>(std::count_if(
            std::begin(context.actors), std::end(context.actors), [](const IActorUPtr_t & actor) {
                return (actor->motivation() == Motivation::lazy);
            })) };

        const std::size_t greedyActorCount{ context.actors.size() - lazyActorCount };

        const float lazyWinPercent{ 100.0f - (100.0f * (static_cast<float>(m_greedyScore) /
                                                        static_cast<float>(m_lazyScore))) };

        std::cout.imbue(std::locale("")); // this is only to put commas in the big

        const std::string sep{ ", " };

#ifdef WIN32
        std::cout << "Win";
#else
        std::cout << "Mac";
#endif

        std::cout << sep;

#ifdef NDEBUG
        std::cout << "Rel";
#else
        std::cout << "Dbg";
#endif

        std::cout << sep;

        if (context.is_mode_normal)
        {
            std::cout << "Nml";
        }
        else
        {
            std::cout << "Tst";
        }

        std::cout << sep << fpsCount << sep;
        std::cout << sep << lazyActorCount << "l";
        std::cout << sep << greedyActorCount << "g";
        std::cout << sep << context.pickups.size() << "p";
        std::cout << sep << static_cast<int>(lazyWinPercent) << '%';
        std::cout << std::endl;
    }
} // namespace methhead