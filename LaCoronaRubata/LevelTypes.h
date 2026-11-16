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
// Rettangolo solido con posizione al centro e dimensioni complete.
struct SolidCollider {
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
