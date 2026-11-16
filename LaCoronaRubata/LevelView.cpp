#include "LevelView.h"
#include "DecorativeShapes.h"
#include "Level.h"
#include "ShapeGenerator.h"
#include <glm/gtc/matrix_transform.hpp>

namespace {
/// Costruisce una Model Matrix 2D che scala la mesh in coordinate locali e la trasla nella
/// posizione della stanza.
glm::mat4 makeModel(const glm::vec2& position, const glm::vec2& size = glm::vec2(1.0f))
{
    return glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
           glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
}
}

/// Inizializza la vista caricando le geometrie condivise fra le stanze. Richiede un contesto
/// OpenGL attivo.
LevelView::LevelView() : m_geometries(loadGeometries())
{
}

/// Genera le forme, le carica nella Scene e restituisce gli indici delle mesh da riutilizzare
/// per tutte le istanze.
LevelView::LevelGeometries LevelView::loadGeometries()
{
    const auto door = DecorativeShapes::door();
    const auto plate = DecorativeShapes::pressurePlate();
    return {
        m_scene.addGeometry(ShapeGenerator::rectangle(
            1.0f, 1.0f, {107.0f / 255.0f, 112.0f / 255.0f, 107.0f / 255.0f})),
        m_scene.addGeometry(DecorativeShapes::movingWall()),
        m_scene.addGeometry(DecorativeShapes::crystal(GameConfig::COIN_RADIUS)),
        m_scene.addGeometry(DecorativeShapes::ghost()),
        m_scene.addGeometry(DecorativeShapes::crate()),
        {m_scene.addGeometry(DecorativeShapes::man()),
         m_scene.addGeometry(DecorativeShapes::women())},
        m_scene.addGeometry(door.recess),
        {m_scene.addGeometry(door.leaves[0]), m_scene.addGeometry(door.leaves[1])},
        m_scene.addGeometry(door.frame),
        m_scene.addGeometry(plate.base),
        m_scene.addGeometry(plate.releasedPlate),
        m_scene.addGeometry(plate.pressedPlate)
    };
}

/// Sostituisce le istanze della stanza con quelle del livello ricevuto, riusando le geometrie
/// gia' caricate. Registra gli indici delle istanze aggiornate nei frame successivi.
void LevelView::rebuildInstances(const Level& level)
{
    m_scene.clearInstances();
    m_pressurePlates.clear();
    m_movingWallInstances.clear();
    m_ghostInstances.clear();
    m_crateInstances.clear();
    m_coinInstances.clear();

    // L'ordine di inserimento stabilisce la sovrapposizione degli oggetti 2D.
    createDoor(level.exitDoor().area());

    for (const PressurePlate& plate : level.pressurePlates()) {
        createPressurePlate(plate);
    }
    for (std::size_t i = 0; i < m_playerInstances.size(); ++i) {
        m_playerInstances[i] = m_scene.addInstance(m_geometries.players[i]);
    }

    constexpr float floorThickness = GameConfig::FLOOR_THICKNESS;
    const auto floor = m_scene.addInstance(m_geometries.solidRectangle);
    m_scene.instance(floor).modelMatrix = makeModel(
        {0.0f, level.floorY() - floorThickness * 0.5f},
        {level.roomHalfSize().x * 2.0f, floorThickness});

    for (const SolidCollider& platform : level.platforms()) {
        createPlatform(platform);
    }

    // Tutte le pareti mobili condividono la stessa mesh e hanno un'istanza ciascuna.
    for (std::size_t i = 0; i < level.movingWallCount(); ++i) {
        m_movingWallInstances.push_back(m_scene.addInstance(m_geometries.movingWall));
    }

    // I fantasmi condividono la stessa sagoma e hanno ciascuno una propria istanza.
    for (std::size_t i = 0; i < level.ghosts().size(); ++i) {
        m_ghostInstances.push_back(m_scene.addInstance(m_geometries.ghost));
    }

    // Le casse condividono una mesh e hanno ciascuna la propria posizione.
    for (std::size_t i = 0; i < level.crates().size(); ++i) {
        m_crateInstances.push_back(m_scene.addInstance(m_geometries.crate));
    }

    for (const Collectible& coin : level.coins()) {
        const auto index = m_scene.addInstance(m_geometries.crystal, RenderPass::Coins);
        m_scene.instance(index).modelMatrix = makeModel(coin.position);
        m_coinInstances.push_back(index);
    }
}

/// Crea il vano, le due ante e la cornice a partire dall'area di uscita.
/// Conserva gli indici delle ante per aggiornare le loro matrici durante il gioco.
void LevelView::createDoor(const AABB& exitArea)
{
    const auto doorModel = makeModel(exitArea.center, exitArea.halfSize * 2.0f);
    const auto recess = m_scene.addInstance(m_geometries.doorRecess);
    m_scene.instance(recess).modelMatrix = doorModel;
    for (std::size_t i = 0; i < m_doorLeaves.size(); ++i) {
        m_doorLeaves[i] = m_scene.addInstance(m_geometries.doorLeaves[i]);
    }
    const auto frame = m_scene.addInstance(m_geometries.doorFrame);
    m_scene.instance(frame).modelMatrix = doorModel;
}

/// Disegna la piattaforma con il rettangolo condiviso, scalato alle dimensioni del solido.
void LevelView::createPlatform(const SolidCollider& platform)
{
    const auto instance = m_scene.addInstance(m_geometries.solidRectangle);
    m_scene.instance(instance).modelMatrix = makeModel(platform.position, platform.size);
}

/// Posiziona la base e le due varianti del pulsante. Conserva gli indici delle varianti
/// per sincronizzarne la visibilita' con lo stato della pedana.
void LevelView::createPressurePlate(const PressurePlate& plate)
{
    const glm::vec2& size = plate.size();
    const glm::vec2 center = plate.basePosition() + glm::vec2(0.0f, size.y * 0.5f);
    const auto model = makeModel(center, size);

    const auto base = m_scene.addInstance(m_geometries.plateBase);
    m_scene.instance(base).modelMatrix = model;
    const auto released = m_scene.addInstance(m_geometries.plateReleased);
    m_scene.instance(released).modelMatrix = model;
    const auto pressed = m_scene.addInstance(m_geometries.platePressed);
    auto& pressedInstance = m_scene.instance(pressed);
    pressedInstance.modelMatrix = makeModel({center.x, center.y - plate.pressDistance()}, size);
    pressedInstance.visible = false;
    m_pressurePlates.push_back({pressed, released});
}

/// Sincronizza le istanze con lo stato corrente della stanza.
/// Quando cambia il numero del livello, ricostruisce prima le istanze.
void LevelView::sync(const Level& level, std::size_t levelNumber)
{
    if (levelNumber != m_levelNumber) {
        rebuildInstances(level);
        m_levelNumber = levelNumber;
    }

    for (std::size_t i = 0; i < m_playerInstances.size(); ++i) {
        const Player& player = level.players()[i];
        m_scene.instance(m_playerInstances[i]).modelMatrix = makeModel(player.position, player.size);
    }
    updateDoorLeaves(level.exitDoor());

    for (std::size_t i = 0; i < m_pressurePlates.size(); ++i) {
        const auto& instances = m_pressurePlates[i];
        const bool pressed = level.pressurePlates()[i].isPressed();
        m_scene.instance(instances.pressed).visible = pressed;
        m_scene.instance(instances.released).visible = !pressed;
    }

    // Usa il collider corrente affinche' la sagoma disegnata coincida con quella fisica.
    for (std::size_t i = 0; i < m_movingWallInstances.size(); ++i) {
        const SolidCollider wall = level.movingWall(i).collider();
        m_scene.instance(m_movingWallInstances[i]).modelMatrix = makeModel(wall.position, wall.size);
    }

    // La simulazione fornisce la posizione e le dimensioni usate anche per il disegno.
    for (std::size_t i = 0; i < m_ghostInstances.size(); ++i) {
        const AABB bounds = level.ghosts()[i].bounds();
        m_scene.instance(m_ghostInstances[i]).modelMatrix =
            makeModel(bounds.center, bounds.halfSize * 2.0f);
    }

    // Usa il collider aggiornato dalla fisica per mantenere allineata la sagoma.
    for (std::size_t i = 0; i < m_crateInstances.size(); ++i) {
        const SolidCollider crate = level.crates()[i].collider();
        m_scene.instance(m_crateInstances[i]).modelMatrix = makeModel(crate.position, crate.size);
    }

    for (std::size_t i = 0; i < m_coinInstances.size(); ++i) {
        m_scene.instance(m_coinInstances[i]).visible = !level.coins()[i].collected;
    }
}

/// Converte la frazione di apertura in una scala orizzontale per ciascuna anta, mantenendo fisso
/// il relativo cardine.
void LevelView::updateDoorLeaves(const ExitDoor& door)
{
    const AABB& exit = door.area();
    const glm::vec2 doorSize = exit.halfSize * 2.0f;
    const float scaleX = 1.0f - 0.9f * door.opening();
    for (std::size_t i = 0; i < m_doorLeaves.size(); ++i) {
        const float hingeX = DecorativeShapes::DoorMeshes::hingeX[i];
        // T(cardine) * S * T(-cardine) nel sistema locale della porta.
        const glm::vec2 offset{doorSize.x * hingeX * (1.0f - scaleX), 0.0f};
        m_scene.instance(m_doorLeaves[i]).modelMatrix =
            makeModel(exit.center + offset, {doorSize.x * scaleX, doorSize.y});
    }
}
