#pragma once
#include "LevelTypes.h"

struct Player {
    // Dimensioni della sagoma grafica. Per le collisioni usare bounds().
    glm::vec2 size{GameConfig::PLAYER_WIDTH, GameConfig::PLAYER_HEIGHT};
    glm::vec2 spawnPosition{0.0f};
    glm::vec2 position{0.0f};
    glm::vec2 velocity{0.0f};
    bool onGround = true;

    void reset();
    void applyInput(const PlayerInput& input);
    float requestedHeight(const PlayerInput& input) const;
    AABB bounds() const;
    AABB boundsAtHeight(float height) const;
    void setHeight(float height);
};
