#pragma once
#include "Scene.h"
#include <array>
#include <optional>
#include <vector>

class ExitDoor;
class Level;
class PressurePlate;
struct AABB;
struct SolidCollider;

class LevelView {
public:
    LevelView();
    void sync(const Level& level, std::size_t levelNumber, float timeSeconds, bool playable);
    const Scene& scene() const { return m_scene; }

private:
    struct PressurePlateInstances {
        std::size_t pressed;
        std::size_t released;
    };
    struct LevelGeometries {
        std::size_t solidRectangle;
        std::size_t movingWall;
        std::size_t star;
        std::size_t snake;
        std::size_t crate;
        std::array<std::size_t, 2> players;
        std::size_t doorRecess;
        std::array<std::size_t, 2> doorLeaves;
        std::size_t doorFrame;
        std::size_t plateBase;
        std::size_t plateReleased;
        std::size_t platePressed;
    };

    Scene m_scene;
    LevelGeometries m_geometries;
    // Vuoto prima di caricare una stanza. Il numero 0 e' riservato al tutorial.
    std::optional<std::size_t> m_loadedLevelNumber;
    std::array<std::size_t, 2> m_playerInstances{};
    std::array<std::size_t, 2> m_doorLeaves{};
    std::vector<PressurePlateInstances> m_pressurePlates;
    // Indici delle istanze grafiche, nello stesso ordine delle pareti mobili in Level.
    std::vector<std::size_t> m_movingWallInstances;
    // Indici delle istanze dei fantasmi, nello stesso ordine di Level::ghosts().
    std::vector<std::size_t> m_ghostInstances;
    // Indici delle istanze delle casse, nello stesso ordine di Level::crates().
    std::vector<std::size_t> m_crateInstances;
    std::vector<std::size_t> m_starInstances;

    LevelGeometries loadGeometries();
    void rebuildInstances(const Level& level);
    void createDoor(const AABB& exitArea);
    void createPlatform(const SolidCollider& platform);
    void createPressurePlate(const PressurePlate& plate);
    void updateDoorLeaves(const ExitDoor& door);
};
