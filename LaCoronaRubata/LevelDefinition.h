#pragma once
#include "Config.h"
#include <glm/glm.hpp>
#include <array>
#include <cstddef>
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

struct MovingWallDefinition {
    glm::vec2 closedCenter; // Centro della parete chiusa, in coordinate mondo.
    glm::vec2 openCenter;   // Centro della parete aperta, in coordinate mondo.
    glm::vec2 size;         // Larghezza e altezza complete.
    float travelSeconds;   // Durata di una corsa completa.
    // Indici a partire da zero in LevelDefinition::pressurePlates.
    // Basta che una delle pedane elencate sia premuta per comandare l'apertura.
    std::vector<std::size_t> pressurePlateIndices;
};

struct GhostDefinition {
    glm::vec2 startCenter; // Centro iniziale e quota fissa della pattuglia.
    float patrolEndX;      // Secondo estremo della pattuglia orizzontale.
    glm::vec2 size;        // Dimensioni complete della sagoma.
    float speed;           // Velocita' in unita' mondo al secondo.
};

struct CrateDefinition {
    glm::vec2 basePosition; // Centro del bordo inferiore allo spawn.
    glm::vec2 size;         // Larghezza e altezza in unita' mondo.
};

struct LevelDefinition {
    glm::vec2 roomHalfSize; // Semidimensioni in coordinate mondo, centrate sull'origine.
    std::array<glm::vec2, 2> spawnFeet; // Posizione dei piedi allo spawn.
    glm::vec2 doorBase;                // Centro del bordo inferiore della porta.
    std::vector<PlatformDefinition> platforms;
    std::vector<glm::vec2> crystals;
    std::vector<PressurePlateDefinition> pressurePlates;
    float floorY = GameConfig::DEFAULT_FLOOR_Y;
    std::vector<MovingWallDefinition> movingWalls{};
    std::vector<GhostDefinition> ghosts{};
    std::vector<CrateDefinition> crates{};
};
