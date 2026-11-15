#pragma once
#include "Config.h"
#include <glm/glm.hpp>
#include <array>
#include <vector>

struct PlatformDefinition {
    glm::vec2 basePosition; // Centro del bordo inferiore, in coordinate mondo.
    glm::vec2 size;         // Larghezza e spessore.
};

struct PressurePlateDefinition {
    glm::vec2 basePosition; // Centro della base, alla quota di appoggio.
    float width;
    float height;         // Altezza della superficie a riposo rispetto alla base.
    float pressDistance;  // Di quanto scende la superficie quando viene premuta.
};

struct LevelDefinition {
    glm::vec2 roomHalfSize; // Semidimensioni in coordinate mondo, centrate sull'origine.
    std::array<glm::vec2, 2> spawnFeet; // Posizione dei piedi allo spawn.
    glm::vec2 doorBase;                // Centro del bordo inferiore della porta.
    std::vector<PlatformDefinition> platforms;
    std::vector<glm::vec2> crystals;
    std::vector<PressurePlateDefinition> pressurePlates;
    float floorY = GameConfig::DEFAULT_FLOOR_Y;
};
