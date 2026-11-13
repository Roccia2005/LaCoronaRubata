#pragma once
#include "Level.h"

class Game {
public:
    explicit Game(Scene& scene);
    void update(float dt, float halfWidth, const PlayerInput& input1,
                const PlayerInput& input2, float currentTime);
    void reset();
    void resize(float halfWidth);
    int score() const { return m_level.score(); }
    std::size_t coinGeometryIndex() const { return m_level.coinGeometryIndex(); }
    const std::vector<Collectible>& coins() const { return m_level.coins(); }

private:
    Level m_level;
};
