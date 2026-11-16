#pragma once
#include "LevelDefinition.h"
#include "LevelTypes.h"
#include "Simulazione/Components/Crate.h"
#include "Simulazione/Components/ExitDoor.h"
#include "Simulazione/Components/Ghost.h"
#include "Simulazione/Components/MovingWall.h"
#include "Simulazione/Components/Player.h"
#include "Simulazione/Components/PressurePlate.h"

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
    const std::vector<SolidCollider>& platforms() const { return m_platforms; }
    const std::vector<Collectible>& coins() const { return m_coins; }
    /// Espone le casse per disegnarle nelle loro posizioni correnti.
    const std::vector<Crate>& crates() const { return m_crates; }
    /// Espone i fantasmi per disegnarli nelle loro posizioni correnti.
    const std::vector<Ghost>& ghosts() const { return m_ghosts; }
    /// Espone le pedane della stanza in sola lettura, nell'ordine definito nella mappa.
    const std::vector<PressurePlate>& pressurePlates() const { return m_pressurePlates; }
    /// Restituisce il numero di pareti mobili della stanza.
    std::size_t movingWallCount() const { return m_movingWalls.size(); }
    /// Espone una parete in sola lettura, tramite il suo indice nella mappa.
    const MovingWall& movingWall(std::size_t index) const { return m_movingWalls.at(index).wall; }
    /// Espone la porta di uscita in sola lettura, compresa la frazione di apertura.
    const ExitDoor& exitDoor() const { return m_exitDoor; }
    int score() const { return m_score; }

private:
    // Raggruppa lo stato della parete e gli indici delle pedane che la comandano.
    struct LinkedMovingWall {
        MovingWall wall;
        std::vector<std::size_t> pressurePlateIndices;
    };

    glm::vec2 m_roomHalfSize;
    float m_floorY;
    std::array<Player, 2> m_players{};
    std::vector<SolidCollider> m_platforms;
    std::vector<Collectible> m_coins;
    std::vector<PressurePlate> m_pressurePlates;
    std::vector<LinkedMovingWall> m_movingWalls;
    std::vector<Ghost> m_ghosts;
    std::vector<Crate> m_crates;
    ExitDoor m_exitDoor;
    int m_score = 0;

    bool isPlayerAtExit(const Player& player) const;
    void updatePressurePlates();
    bool updateMovingWalls(float dt);
    bool updateGhosts(float dt);
    void collectCoins(float currentTime);
};
