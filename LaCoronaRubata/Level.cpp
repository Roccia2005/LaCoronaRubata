#include "Level.h"
#include "DecorativeShapes.h"
#include "PlayerPhysics.h"
#include "ShapeGenerator.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

using namespace GameConfig;

namespace {
// T * S: i vertici locali rimangono nei VBO, cambia la Model Matrix.
glm::mat4 makeModel(const glm::vec2& pos, float sx = 1.0f, float sy = 1.0f)
{
    return glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f)) *
           glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, 1.0f));
}
} // namespace

Level::Level(Scene& scene) : m_scene(scene)
{
    const auto floorGeo = m_scene.addGeometry(
        ShapeGenerator::rectangle(1.0f, 1.0f, {107/255.0f, 112/255.0f, 107/255.0f}));
    const auto obstacleGeo = m_scene.addGeometry(
        DecorativeShapes::platformRock());
    const auto coinGeo = m_scene.addGeometry(
        DecorativeShapes::crystal(COIN_RADIUS));
    createDoor();
    createPlayer(m_players[0], -92.0f, DecorativeShapes::man());
    createPlayer(m_players[1], -78.0f, DecorativeShapes::women());
    createFloor(floorGeo);
    createObstacles(obstacleGeo);
    m_coinGeometryIndex = coinGeo;
    createCoins(coinGeo);
    updateModelMatrices();
}

void Level::createPlayer(Player& player, float spawnX, const MeshData& mesh)
{
    player.spawnPosition = {spawnX, FLOOR_Y + player.size.y * 0.5f};
    const auto geometryIndex = m_scene.addGeometry(mesh);
    player.instanceIndex = m_scene.addInstance(geometryIndex);
    player.reset();
}

void Level::createDoor()
{
    constexpr float width = 12.0f;
    constexpr float height = 16.0f;
    constexpr float x = 75.0f;

    const glm::vec2 doorSize{width, height};
    const glm::vec2 doorCenter{x, FLOOR_Y + doorSize.y * 0.5f};
    m_exitArea = makeAABB(doorCenter, doorSize);

    const auto meshes = DecorativeShapes::door();
    const auto recess = m_scene.addInstance(m_scene.addGeometry(meshes.recess));
    const auto model = makeModel(doorCenter, doorSize.x, doorSize.y);
    m_scene.instance(recess).modelMatrix = model;
    for (std::size_t i = 0; i < m_doorLeaves.size(); ++i) {
        m_doorLeaves[i] = m_scene.addInstance(m_scene.addGeometry(meshes.leaves[i]));
    }
    const auto frame = m_scene.addInstance(m_scene.addGeometry(meshes.frame));
    m_scene.instance(frame).modelMatrix = model;
    updateDoorLeaves();
}

bool Level::isAtDoor(const Player& player) const
{
    return PlayerPhysics::intersects(makeAABB(player.position, player.size), m_exitArea);
}

void Level::updateDoor(float dt)
{
    constexpr float openingDuration = 0.35f;
    const bool shouldOpen = isAtDoor(m_players[0]) || isAtDoor(m_players[1]);
    const float step = dt / openingDuration;
    const float opening = std::clamp(m_doorOpening + (shouldOpen ? step : -step),
                                     0.0f, 1.0f);
    if (opening != m_doorOpening) {
        m_doorOpening = opening;
        updateDoorLeaves();
    }
}

void Level::updateDoorLeaves()
{
    const glm::vec2 doorSize = m_exitArea.halfSize * 2.0f;
    const float scaleX = 1.0f - 0.9f * m_doorOpening;
    for (std::size_t i = 0; i < m_doorLeaves.size(); ++i) {
        const float hingeX = DecorativeShapes::DoorMeshes::hingeX[i];
        // Equivale a T(cardine) * S * T(-cardine) in coordinate locali.
        const glm::vec2 offset{doorSize.x * hingeX * (1.0f - scaleX), 0.0f};
        m_scene.instance(m_doorLeaves[i]).modelMatrix =
            makeModel(m_exitArea.center + offset, doorSize.x * scaleX, doorSize.y);
    }
}

void Level::createFloor(std::size_t floorGeo)
{
    m_floorInstance = m_scene.addInstance(floorGeo);
}

void Level::createObstacles(std::size_t obstacleGeo)
{
    // Ogni elemento: {centro X, larghezza, altezza}.
    const std::vector<std::array<float, 3>> specs = {
        {{-43.0f, 10.0f, 15.0f}},
        {{  0.0f, 12.0f, 22.0f}},
        {{ 48.0f, 10.0f, 18.0f}}
    };
    const auto grassGeo = m_scene.addGeometry(DecorativeShapes::grassStrip());
    for (const auto& spec : specs) {
        Obstacle o;
        o.instanceIndex = m_scene.addInstance(obstacleGeo);
        o.position = {spec[0], FLOOR_Y + spec[2] * 0.5f};
        o.size = {spec[1], spec[2]};
        m_scene.instance(o.instanceIndex).modelMatrix =
            makeModel(o.position, o.size.x, o.size.y);
        // Il prato e' decorativo: non modifica la AABB dell'ostacolo.
        const std::size_t grass = m_scene.addInstance(grassGeo);
        m_scene.instance(grass).modelMatrix =
            makeModel({o.position.x, FLOOR_Y + o.size.y - 0.45f}, o.size.x, 1.3f);
        m_obstacles.push_back(o);
    }
}

void Level::createCoins(std::size_t coinGeo)
{
    const std::vector<glm::vec2> positions = {
        {-68.0f, -27.0f}, {-43.0f, -21.0f}, {-18.0f, -23.0f},
        {0.0f, -13.0f}, {24.0f, -28.0f}, {48.0f, -17.0f}, {77.0f, -28.0f}
    };
    for (const glm::vec2& pos : positions) {
        Collectible coin;
        coin.instanceIndex = m_scene.addInstance(coinGeo);
        coin.position = pos;
        coin.collected = false;
        m_scene.instance(coin.instanceIndex).modelMatrix = makeModel(pos);
        m_coins.push_back(coin);
    }
}

void Level::update(float dt, float halfWidth, const PlayerInput& input1, const PlayerInput& input2, float currentTime)
{
    PlayerPhysics::updatePlayers(m_players[0], m_players[1], input1, input2,
                                 dt, halfWidth, m_obstacles);

    updateModelMatrices();
    updateDoor(dt);
    collectCoins(currentTime);
}

void Level::resize(float halfWidth)
{
    // Il terreno si allarga con la finestra; il bordo superiore resta a FLOOR_Y.
    m_scene.instance(m_floorInstance).modelMatrix =
        makeModel({0.0f, FLOOR_Y - 3.0f}, 2.0f * halfWidth, 6.0f);
}

void Level::updateModelMatrices()
{
    for (const Player& player : m_players) {
        m_scene.instance(player.instanceIndex).modelMatrix =
            makeModel(player.position, player.size.x, player.size.y);
    }
}

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
            m_scene.instance(coin.instanceIndex).visible = false;
            m_score += 10;
        }
    }
}

bool Level::isFinished() const
{
    return m_doorOpening >= 1.0f && isAtDoor(m_players[0]) && isAtDoor(m_players[1]);
}

void Level::reset()
{
    m_doorOpening = 0.0f;
    updateDoorLeaves();
    for (Player& player : m_players) {
        player.reset();
    }
    m_score = 0;
    for (Collectible& coin : m_coins) {
        coin.collected = false;
        m_scene.instance(coin.instanceIndex).visible = true;
    }
    updateModelMatrices();
}
