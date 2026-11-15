#pragma once
#include "Level.h"
#include <cstddef>

class Game {
public:
    Game();
    void update(float dt, const PlayerInput& input1, const PlayerInput& input2,
                float currentTime);
    void reset();
    const Level& level() const { return m_level; }
    int score() const { return m_level.score(); }
    std::size_t levelNumber() const { return m_levelIndex + 1; }
    bool finished() const { return m_finished; }

private:
    Level m_level;
    std::size_t m_levelIndex = 0;
    bool m_finished = false;
};
