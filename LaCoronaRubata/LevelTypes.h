#pragma once
#include "Config.h"
#include <glm/glm.hpp>

// Dati della simulazione, separati dalle GeometryInstance.
struct AABB { glm::vec2 center; glm::vec2 halfSize; };
// Inline permette di mettere l'implementazione nell'header
inline AABB makeAABB(const glm::vec2& center, const glm::vec2& size)
{
    return { center, size * 0.5f };
}
struct Player {
    glm::vec2 size{GameConfig::PLAYER_WIDTH, GameConfig::PLAYER_HEIGHT};
    glm::vec2 spawnPosition{0.0f};
    glm::vec2 position{0.0f};
    glm::vec2 velocity{0.0f};
    bool onGround = true;

    void reset()
    {
        position = spawnPosition;
        velocity = {0.0f, 0.0f};
        onGround = true;
        size.y = GameConfig::PLAYER_HEIGHT;
    }
};
struct Obstacle {
    glm::vec2 position{0.0f};
    glm::vec2 size{0.0f};
};
struct Collectible {
    glm::vec2 position{0.0f};
    bool collected = false;
};
struct PlayerInput {
    bool left = false;
    bool right = false;
    bool jumpHeld = false;
    bool down = false;
};
struct Pressable {
    glm::vec2 basePosition;
    float width;
    float height;
    float pressDistance;
    bool pressed = false;
    float surfaceY() const
    {
        return basePosition.y + height
            - (pressed ? pressDistance : 0.0f);
    }
};
