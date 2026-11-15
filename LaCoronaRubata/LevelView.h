#pragma once
#include "Scene.h"
#include <array>
#include <vector>

class Level;
struct AABB;
struct Obstacle;
struct Pressable;

class LevelView {
public:
    LevelView();
    void sync(const Level& level, std::size_t levelNumber);
    const Scene& scene() const { return m_scene; }

private:
    struct PressurePlateInstances {
        std::size_t pressed;
        std::size_t released;
    };
    struct LevelGeometries {
        std::size_t floor;
        std::size_t platform;
        std::size_t grass;
        std::size_t crystal;
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
    std::size_t m_levelNumber = 0;
    std::array<std::size_t, 2> m_playerInstances{};
    std::array<std::size_t, 2> m_doorLeaves{};
    std::vector<PressurePlateInstances> m_pressurePlates;
    std::vector<std::size_t> m_coinInstances;

    LevelGeometries loadGeometries();
    void rebuildInstances(const Level& level);
    void createDoor(const AABB& exitArea);
    void createPlatform(const Obstacle& obstacle);
    void createPressurePlate(const Pressable& plate);
    void updateDoorLeaves(const Level& level);
};
