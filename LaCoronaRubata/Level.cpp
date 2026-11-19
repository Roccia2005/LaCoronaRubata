#include "Level.h"
#include "Simulazione/Components/PlayerPhysics.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

using namespace GameConfig;

namespace {

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

    /// Verifica se il rettangolo della parete attraversa un corpo durante il
    /// movimento. Il semplice contatto sui bordi non blocca la corsa.
    bool wallPathIntersects(const SolidCollider& wall, const glm::vec2& nextPosition,
                            const AABB& body)
    {
        const glm::vec2 delta = nextPosition - wall.position;
        if (delta == glm::vec2(0.0f)) {
            return false;
        }

        // Allargare il corpo di mezzo collider consente di seguire solo il centro
        // della parete. I valori tra 0 e 1 rappresentano le frazioni della corsa del frame.
        const glm::vec2 halfSize = body.halfSize + wall.size * 0.5f;
        const glm::vec2 offset = wall.position - body.center;
        float enter = 0.0f;
        float leave = 1.0f;

        for (glm::length_t axis = 0; axis < 2; ++axis) {
            if (delta[axis] == 0.0f) {
                if (std::abs(offset[axis]) >= halfSize[axis]) {
                    return false;
                }
                continue;
            }

            // Interseca gli intervalli di sovrapposizione sui due assi.
            const float first = (-halfSize[axis] - offset[axis]) / delta[axis];
            const float last = (halfSize[axis] - offset[axis]) / delta[axis];
            enter = std::max(enter, std::min(first, last));
            leave = std::min(leave, std::max(first, last));
            if (enter >= leave) {
                return false;
            }
        }
        return true;
    }
}

/// Costruisce lo stato iniziale della stanza a partire dalla mappa. Imposta confini, spawn, area
/// di uscita, piattaforme, stelle, pedane, pareti mobili, fantasmi e casse.
Level::Level(const LevelDefinition& definition)
    : m_roomHalfSize(definition.roomHalfSize), m_floorY(definition.floorY),
      m_exitDoor(definition.doorBase)
{
    for (std::size_t i = 0; i < m_players.size(); ++i) {
        Player& player = m_players[i];
        player.spawnPosition = definition.spawnFeet[i] + glm::vec2(0.0f, player.size.y * 0.5f);
        player.reset();
    }

    for (const auto& platform : definition.platforms) {
        const glm::vec2 center = platform.basePosition + glm::vec2(0.0f, platform.size.y * 0.5f);
        m_platforms.push_back({center, platform.size});
    }
    for (const auto& position : definition.stars) {
        m_stars.push_back({position});
    }
    for (const auto& plate : definition.pressurePlates) {
        m_pressurePlates.emplace_back(plate);
    }

    // Valida i collegamenti una volta sola, dopo aver caricato tutte le pedane.
    for (const auto& wall : definition.movingWalls) {
        const bool invalidLink = std::any_of(
            wall.pressurePlateIndices.begin(), wall.pressurePlateIndices.end(),
            [&](std::size_t index) { return index >= m_pressurePlates.size(); });
        if (invalidLink) {
            throw std::out_of_range("Indice pedana non valido per la parete mobile");
        }

        m_movingWalls.push_back({
            MovingWall(wall.closedCenter, wall.openCenter, wall.size, wall.travelSeconds),
            wall.pressurePlateIndices
        });
    }
    for (const GhostDefinition& ghost : definition.ghosts) {
        m_ghosts.emplace_back(ghost);
    }
    for (const CrateDefinition& crate : definition.crates) {
        m_crates.emplace_back(crate);
    }
}

/// Avanza la fisica dei giocatori e delle casse, poi aggiorna la stanza.
/// Il contatto con un fantasma o con una parete in chiusura resetta la stanza.
/// currentTime e' il tempo in secondi usato per calcolare l'oscillazione delle stelle.
void Level::update(float dt, const PlayerInput& input1, const PlayerInput& input2,
                   float currentTime)
{
    auto solids = m_platforms;
    solids.reserve(solids.size() + m_pressurePlates.size() + m_movingWalls.size() + m_crates.size());
    for (const PressurePlate& plate : m_pressurePlates) {
        solids.push_back(plate.collider());
    }
    // I giocatori incontrano le pareti nella posizione di inizio frame.
    for (const LinkedMovingWall& link : m_movingWalls) {
        solids.push_back(link.wall.collider());
    }
    // La fisica aggiorna i collider delle casse in coda, nello stesso ordine di m_crates.
    for (const Crate& crate : m_crates) {
        solids.push_back(crate.collider());
    }

    PlayerPhysics::updatePlayers(m_players[0], m_players[1], input1, input2,
        dt, m_roomHalfSize.x, m_floorY, solids, m_crates);
    m_exitDoor.update(dt, isPlayerAtExit(m_players[0]) || isPlayerAtExit(m_players[1]));
    updatePressurePlates();
    if (updateMovingWalls(dt) || updateGhosts(dt)) {
        reset();
        return;
    }
    collectStars(currentTime);
}

/// Restituisce true quando il rettangolo del giocatore si sovrappone all'area di uscita.
bool Level::isPlayerAtExit(const Player& player) const
{
    return m_exitDoor.overlaps(player.bounds());
}

/// Per ogni pedana rileva giocatori e casse appoggiati e controlla lo spazio per la risalita.
/// Applica agli oggetti appoggiati lo spostamento verticale restituito dalla pedana.
void Level::updatePressurePlates()
{
    for (PressurePlate& plate : m_pressurePlates) {
        const SolidCollider solid = plate.collider();
        const AABB plateBox = makeAABB(solid.position, solid.size);

        std::array<bool, 2> supported{};
        for (std::size_t i = 0; i < m_players.size(); ++i) {
            const Player& player = m_players[i];
            supported[i] = player.onGround && player.velocity.y <= 0.0f
                && isSupportedBy(player.bounds(), plateBox);
        }

        const auto crateOnPlate = [&](const Crate& crate) {
            return isSupportedBy(crate.bounds(), plateBox);
        };
        const bool hasSupport = supported[0] || supported[1]
            || std::any_of(m_crates.begin(), m_crates.end(), crateOnPlate);
        bool releaseBlocked = false;
        if (plate.isPressed() && !hasSupport) {
            // Prima di risalire, verifica che ci sia spazio.
            const SolidCollider raised = plate.releasedCollider();
            const AABB raisedBox = makeAABB(raised.position, raised.size);

            releaseBlocked = std::any_of(
                m_players.begin(), m_players.end(), [&](const Player& player) {
                    return PlayerPhysics::intersects(player.bounds(), raisedBox);
                }) || std::any_of(m_crates.begin(), m_crates.end(), [&](const Crate& crate) {
                    return PlayerPhysics::intersects(crate.bounds(), raisedBox);
                });
        }

        const float deltaY = plate.update(hasSupport, releaseBlocked);
        if (deltaY == 0.0f) {
            continue;
        }

        for (std::size_t i = 0; i < m_players.size(); ++i) {
            if (supported[i]) {
                m_players[i].position.y += deltaY;
            }
        }
        for (Crate& crate : m_crates) {
            if (crateOnPlate(crate)) {
                crate.moveWithSupport(deltaY);
            }
        }
    }
}

/// Legge lo stato appena aggiornato delle pedane. Basta una pedana premuta per aprire la parete.
/// La parete si ferma contro una cassa. Se colpisce un giocatore in chiusura, segnala il reset.
bool Level::updateMovingWalls(float dt)
{
    for (LinkedMovingWall& link : m_movingWalls) {
        link.moving = false;
        const bool activated = std::any_of(
            link.pressurePlateIndices.begin(), link.pressurePlateIndices.end(),
            [&](std::size_t index) { return m_pressurePlates[index].isPressed(); });

        // Simula il passo su una copia prima di cambiare la posizione effettiva.
        MovingWall next = link.wall;
        next.update(dt, activated);

        const SolidCollider current = link.wall.collider();
        const glm::vec2 nextPosition = next.position();
        const bool blocked = std::any_of(
            m_players.begin(), m_players.end(), [&](const Player& player) {
                return wallPathIntersects(current, nextPosition, player.bounds());
            });
        if (blocked && !activated) {
            return true;
        }
        if (!blocked && std::none_of(m_crates.begin(), m_crates.end(), [&](const Crate& crate) {
                return wallPathIntersects(current, nextPosition, crate.bounds());
            })) {
            link.wall = next;
            link.moving = current.position != nextPosition && link.wall.isBetweenStops();
        }
    }
    return false;
}

/// Fa avanzare la pattuglia e segnala il contatto con uno dei due giocatori.
bool Level::updateGhosts(float dt)
{
    for (Ghost& ghost : m_ghosts) {
        ghost.update(dt);
        const AABB ghostBounds = ghost.bounds();
        const bool touchesPlayer = std::any_of(
            m_players.begin(), m_players.end(), [&](const Player& player) {
                return PlayerPhysics::intersects(player.bounds(), ghostBounds);
            });
        if (touchesPlayer) {
            return true;
        }
    }
    return false;
}

/// Controlla il contatto del primo giocatore con le stelle ancora disponibili, alla loro
/// posizione oscillante. Ogni nuova raccolta assegna 10 punti.
void Level::collectStars(float currentTime)
{
    const AABB playerBox = m_players[0].bounds();
    for (std::size_t i = 0; i < m_stars.size(); ++i) {
        Collectible& star = m_stars[i];
        if (star.collected) continue;
        // Stessa sinusoide del vertex shader: collisione e immagine coincidono.
        glm::vec2 animatedPosition = star.position;
        const float phase = static_cast<float>(i) * STAR_PHASE_STEP;
        animatedPosition.y += STAR_BOB_AMPLITUDE *
            std::sin(STAR_BOB_FREQUENCY * currentTime + phase);
        const AABB starBox{animatedPosition, {STAR_RADIUS, STAR_RADIUS}};
        if (PlayerPhysics::intersects(playerBox, starBox)) {
            star.collected = true;
            m_score += POINTS_PER_STAR;
        }
    }
}

/// Restituisce true quando la porta e' completamente aperta ed entrambi i giocatori sono
/// nell'area di uscita.
bool Level::isFinished() const
{
    return m_exitDoor.isOpen() && isPlayerAtExit(m_players[0]) && isPlayerAtExit(m_players[1]);
}

/// Ripristina giocatori, porta, pedane, pareti, fantasmi, casse, punteggio e stelle.
void Level::reset()
{
    m_exitDoor.reset();
    for (Player& player : m_players) {
        player.reset();
    }
    for (PressurePlate& plate : m_pressurePlates) {
        plate.reset();
    }
    for (LinkedMovingWall& link : m_movingWalls) {
        link.wall.reset();
        link.moving = false;
    }
    for (Ghost& ghost : m_ghosts) {
        ghost.reset();
    }
    for (Crate& crate : m_crates) {
        crate.reset();
    }
    m_score = 0;
    for (Collectible& star : m_stars) {
        star.collected = false;
    }
}
