#pragma once
#include "LevelTypes.h"
#include "Scene.h"
#include <array>
#include <vector>

class Level {
public:
    explicit Level(Scene& scene);
    void update(float dt, float halfWidth, const PlayerInput& input1, const PlayerInput& input2, float currentTime);
    void reset();
    void resize(float halfWidth);
    int score() const { return m_score; }
    std::size_t coinGeometryIndex() const { return m_coinGeometryIndex; }
    const std::vector<Collectible>& coins() const { return m_coins; }
    bool isFinished() const;

private:
    Scene& m_scene; // Level non possiede Scene; la usa soltanto.
    std::array<Player, 2> m_players{};
    std::vector<Obstacle> m_obstacles;
    std::vector<Collectible> m_coins;
    AABB m_exitArea{};
    std::array<std::size_t, 2> m_doorLeaves{};
    float m_doorOpening = 0.0f;
    std::size_t m_floorInstance = 0;
    int m_score = 0;
    std::size_t m_coinGeometryIndex = 0;

    void createPlayer(Player& player, float spawnX, const MeshData& mesh);
    void createDoor();
    bool isAtDoor(const Player& player) const;
    void updateDoor(float dt);
    void updateDoorLeaves();
    void createFloor(std::size_t floorGeo);
    void createObstacles(std::size_t obstacleGeo);
    void createCoins(std::size_t coinGeo);
    void collectCoins(float currentTime);
    void updateModelMatrices();
};
