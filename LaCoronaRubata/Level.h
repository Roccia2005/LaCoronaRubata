#pragma once
#include "LevelDefinition.h"
#include "LevelTypes.h"

class Level {
public:
    explicit Level(const LevelDefinition& definition);
    void update(float dt, const PlayerInput& input1, const PlayerInput& input2,
                float currentTime);
    void reset();
    bool isFinished() const;

    const glm::vec2& roomHalfSize() const { return m_roomHalfSize; }
    float floorY() const { return m_floorY; }
    const std::array<Player, 2>& players() const { return m_players; }
    const std::vector<Obstacle>& obstacles() const { return m_obstacles; }
    const std::vector<Collectible>& coins() const { return m_coins; }
    const std::vector<Pressable>& pressurePlates() const { return m_pressurePlates; }
    const AABB& exitArea() const { return m_exitArea; }
    float doorOpening() const { return m_doorOpening; }
    int score() const { return m_score; }

private:
    glm::vec2 m_roomHalfSize;
    float m_floorY;
    std::array<Player, 2> m_players{};
    std::vector<Obstacle> m_obstacles;
    std::vector<Collectible> m_coins;
    std::vector<Pressable> m_pressurePlates;
    AABB m_exitArea{};
    float m_doorOpening = 0.0f;
    int m_score = 0;

    bool isAtDoor(const Player& player) const;
    void updateDoor(float dt);
    void updatePressurePlates();
    void collectCoins(float currentTime);
    void appendPressurePlateObstacles(std::vector<Obstacle>& solids) const;
};
