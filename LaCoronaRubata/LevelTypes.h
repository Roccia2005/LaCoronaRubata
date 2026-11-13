#pragma once
#include <glm/glm.hpp>
#include <cstddef>

namespace GameConfig {
constexpr float HALF_HEIGHT = 60.0f;
constexpr float FLOOR_Y = -48.0f;
constexpr float PLAYER_WIDTH = 6.0f;
constexpr float PLAYER_HEIGHT = 7.0f;
constexpr float MOVE_SPEED = 43.0f;
constexpr float JUMP_SPEED = 75.0f; // salto sufficiente per la prima piattaforma
constexpr float GRAVITY = -135.0f;
constexpr float COIN_RADIUS = 3.0f;
constexpr float COIN_BOB_AMPLITUDE = 2.0f; // unita del mondo
constexpr float COIN_BOB_FREQUENCY = 3.0f; // radianti al secondo
constexpr float COIN_PHASE_STEP = 0.8f;
}

// Dati della simulazione, separati dalle GeometryInstance.
struct AABB { glm::vec2 center; glm::vec2 halfSize; };
// Inline permette di mettere l'implementazione nell'header
inline AABB makeAABB(const glm::vec2& center, const glm::vec2& size)
{
    return { center, size * 0.5f };
}
struct Player {
    std::size_t instanceIndex = 0;
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
    std::size_t instanceIndex = 0;
    glm::vec2 position{0.0f};
    glm::vec2 size{0.0f};
};
struct Collectible {
    std::size_t instanceIndex = 0;
    glm::vec2 position{0.0f};
    bool collected = false;
};
struct PlayerInput {
    bool left = false;
    bool right = false;
    bool jumpPressed = false;
    bool down = false;
};
