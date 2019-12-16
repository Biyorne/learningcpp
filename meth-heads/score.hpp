#ifndef METHHEADS_SCORE_HPP_INCLUDED
#define METHHEADS_SCORE_HPP_INCLUDED

#include "display-constants.hpp"

#include <SFML/Graphics.hpp>

namespace methhead
{
    struct SimContext;

    //

    class Score : public sf::Drawable
    {
      public:
        Score();

        void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
        void update(const SimContext & context, const float elapsedMs);

      private:
        void updateScoreBars(const SimContext & context);
        void updateScores(const SimContext & context);
        void printStatus(const SimContext & context, const float elapsedSincePrevUpdateTimeSec);

      private:
        sf::Clock m_updateClock;
        float m_updateIntervalTimeSec;
        std::size_t m_framesPerUpdateCheck;
        std::size_t m_framesSinceUpdate;
        sf::VertexArray m_barQuadVerts;
        int m_lazyScore;
        int m_greedyScore;
    };
} // namespace methhead

#endif // METHHEADS_SCORE_HPP_INCLUDED