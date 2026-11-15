#include "Level.h"
#include "PlayerPhysics.h"
#include <algorithm>
#include <cmath>

using namespace GameConfig;

namespace {

    Obstacle makePlateObstacle(const Pressable& plate)
    {
        const float height = plate.surfaceY() - plate.basePosition.y;
        return {
            plate.basePosition + glm::vec2{0.0f, height * 0.5f},
            {plate.width, height}
        };
    }

    bool isSupportedBy(const AABB& body, const AABB& solid)
    {
        constexpr float contactTolerance = 0.001f;

        const float feetY = body.center.y - body.halfSize.y;
        const float topY = solid.center.y + solid.halfSize.y;
        const bool overlapsX =
            std::abs(body.center.x - solid.center.x)
            < body.halfSize.x + solid.halfSize.x;

        return overlapsX && std::abs(feetY - topY) <= contactTolerance;
    }
}

/// Costruisce lo stato iniziale della stanza a partire dalla mappa. Imposta confini, spawn, area
/// di uscita, ostacoli e cristalli.
Level::Level(const LevelDefinition& definition)
    : m_roomHalfSize(definition.roomHalfSize), m_floorY(definition.floorY)
{
    for (std::size_t i = 0; i < m_players.size(); ++i) {
        Player& player = m_players[i];
        player.spawnPosition = definition.spawnFeet[i] + glm::vec2(0.0f, player.size.y * 0.5f);
        player.reset();
    }

    constexpr glm::vec2 doorSize{DOOR_WIDTH, DOOR_HEIGHT};
    const glm::vec2 doorCenter = definition.doorBase + glm::vec2(0.0f, doorSize.y * 0.5f);
    m_exitArea = makeAABB(doorCenter, doorSize);

    for (const auto& platform : definition.platforms) {
        const glm::vec2 center = platform.basePosition + glm::vec2(0.0f, platform.size.y * 0.5f);
        m_obstacles.push_back({center, platform.size});
    }
    for (const auto& position : definition.crystals) {
        m_coins.push_back({position});
    }
    for (const auto& plate : definition.pressurePlates) {
        m_pressurePlates.push_back({
            plate.basePosition, plate.width, plate.height, plate.pressDistance
        });
    }
}

/// Avanza la fisica dei giocatori di dt secondi, poi aggiorna la porta e verifica la raccolta.
/// currentTime e' il tempo in secondi usato per calcolare l'oscillazione dei cristalli.
void Level::update(float dt, const PlayerInput& input1, const PlayerInput& input2,
                   float currentTime)
{
    auto solids = m_obstacles;
    solids.reserve(solids.size() + m_pressurePlates.size());
    appendPressurePlateObstacles(solids);

    PlayerPhysics::updatePlayers(m_players[0], m_players[1], input1, input2,
        dt, m_roomHalfSize.x, m_floorY, solids);
    updateDoor(dt);
    updatePressurePlates();
    collectCoins(currentTime);
}

/// Restituisce true quando il rettangolo del giocatore si sovrappone all'area di uscita.
bool Level::isAtDoor(const Player& player) const
{
    return PlayerPhysics::intersects(makeAABB(player.position, player.size), m_exitArea);
}

/// Fa avanzare l'apertura se almeno un giocatore occupa l'uscita, altrimenti la fa diminuire. Il
/// valore resta fra 0 e 1 e una transizione completa dura 0.35 secondi.
void Level::updateDoor(float dt)
{
    const bool shouldOpen = isAtDoor(m_players[0]) || isAtDoor(m_players[1]);
    const float step = dt / DOOR_OPENING_SECONDS;
    m_doorOpening = std::clamp(m_doorOpening + (shouldOpen ? step : -step), 0.0f, 1.0f);
}

void Level::updatePressurePlates()
{
    for (Pressable& plate : m_pressurePlates) {
        const Obstacle solid = makePlateObstacle(plate);
        const AABB plateBox = makeAABB(solid.position, solid.size);

        std::array<bool, 2> supported{};
        for (std::size_t i = 0; i < m_players.size(); ++i) {
            const Player& player = m_players[i];
            supported[i] = player.onGround && player.velocity.y <= 0.0f
                && isSupportedBy(makeAABB(player.position, player.size), plateBox);
        }

        const bool shouldPress = supported[0] || supported[1];
        if (shouldPress == plate.pressed) {
            continue;
        }

        if (!shouldPress) {
            // Prima di risalire, verifica che ci sia spazio.
            Pressable released = plate;
            released.pressed = false;
            const Obstacle raised = makePlateObstacle(released);
            const AABB raisedBox = makeAABB(raised.position, raised.size);

            const bool blocked = std::any_of(
                m_players.begin(), m_players.end(), [&](const Player& player) {
                    return PlayerPhysics::intersects(
                        makeAABB(player.position, player.size), raisedBox);
                });

            if (blocked) {
                continue;
            }
        }

        const float previousSurfaceY = plate.surfaceY();
        plate.pressed = shouldPress;
        const float deltaY = plate.surfaceY() - previousSurfaceY;

        for (std::size_t i = 0; i < m_players.size(); ++i) {
            if (supported[i]) {
                m_players[i].position.y += deltaY;
            }
        }
    }
}

/// Controlla il contatto del primo giocatore con i cristalli ancora disponibili, alla loro
/// posizione oscillante. Ogni nuova raccolta assegna 10 punti.
void Level::collectCoins(float currentTime)
{
    const AABB playerBox{m_players[0].position, m_players[0].size * 0.5f};
    for (std::size_t i = 0; i < m_coins.size(); ++i) {
        Collectible& coin = m_coins[i];
        if (coin.collected) continue;
        // Stessa sinusoide del vertex shader: collisione e immagine coincidono.
        glm::vec2 animatedPosition = coin.position;
        const float phase = static_cast<float>(i) * COIN_PHASE_STEP;
        animatedPosition.y += COIN_BOB_AMPLITUDE *
            std::sin(COIN_BOB_FREQUENCY * currentTime + phase);
        const AABB coinBox{animatedPosition, {COIN_RADIUS, COIN_RADIUS}};
        if (PlayerPhysics::intersects(playerBox, coinBox)) {
            coin.collected = true;
            m_score += POINTS_PER_CRYSTAL;
        }
    }
}

/// Restituisce true quando la porta e' completamente aperta ed entrambi i giocatori sono
/// nell'area di uscita.
bool Level::isFinished() const
{
    return m_doorOpening >= 1.0f && isAtDoor(m_players[0]) && isAtDoor(m_players[1]);
}

/// Riporta i giocatori agli spawn, chiude la porta, azzera il punteggio e rende di nuovo
/// raccoglibili i cristalli.
void Level::reset()
{
    m_doorOpening = 0.0f;
    for (Player& player : m_players) {
        player.reset();
    }
    m_score = 0;
    for (Collectible& coin : m_coins) {
        coin.collected = false;
    }
}

void Level::appendPressurePlateObstacles(std::vector<Obstacle>& solids) const
{
    for (const Pressable& plate : m_pressurePlates) {
        solids.push_back(makePlateObstacle(plate));
    }
}