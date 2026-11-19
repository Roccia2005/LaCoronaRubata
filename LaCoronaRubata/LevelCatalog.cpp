#include "LevelCatalog.h"
#include "Config.h"
#include "LevelDefinition.h"
#include <iterator>
#include <stdexcept>

namespace {

/// Converte i bordi delle piattaforme nelle posizioni di base usate dalla simulazione.
template<std::size_t PlatformCount, std::size_t StarCount>
void loadRoomGeometry(LevelDefinition& level,
                      const LevelConfig::PlatformBounds (&platforms)[PlatformCount],
                      const LevelConfig::StarPosition (&stars)[StarCount])
{
    level.platforms.reserve(PlatformCount);
    for (const auto& platform : platforms) {
        level.platforms.push_back({
            {(platform.left + platform.right) * 0.5f, platform.top - platform.thickness},
            {platform.right - platform.left, platform.thickness}
        });
    }

    level.stars.reserve(StarCount);
    for (const auto& star : stars) {
        level.stars.push_back({star.x, star.y});
    }
}

/// Assembla il livello 1 usando le quote di Config.h.
LevelDefinition level1()
{
    using namespace LevelConfig::Level1;

    LevelDefinition level{
        {ROOM_HALF_WIDTH, ROOM_HALF_HEIGHT},
        {{{PLAYER_1_SPAWN_X, PLAYER_1_SPAWN_FEET_Y}, {PLAYER_2_SPAWN_X, PLAYER_2_SPAWN_FEET_Y}}},
        {DOOR_X, DOOR_BASE_Y},
        {}, // Piattaforme caricate da PLATFORMS.
        {}, // Stelle caricate da STARS.
        {
            {{PRESSURE_PLATE_A_BASE_X, PRESSURE_PLATE_A_BASE_Y},
             PRESSURE_PLATE_WIDTH,
             PRESSURE_PLATE_HEIGHT,
             PRESSURE_PLATE_PRESS_DISTANCE},
            {{PRESSURE_PLATE_B_BASE_X, PRESSURE_PLATE_B_BASE_Y},
             PRESSURE_PLATE_WIDTH,
             PRESSURE_PLATE_HEIGHT,
             PRESSURE_PLATE_PRESS_DISTANCE}
        },
        FLOOR_Y,
        {
            {{MOVING_WALL_CLOSED_CENTER_X, MOVING_WALL_CENTER_Y},
             {MOVING_WALL_OPEN_CENTER_X, MOVING_WALL_CENTER_Y},
             {MOVING_WALL_WIDTH, MOVING_WALL_HEIGHT},
             MOVING_WALL_TRAVEL_SECONDS,
             MOVING_WALL_PRESSURE_PLATE_INDICES}
        },
        {
            {{GHOST_START_X, GHOST_CENTER_Y}, {GHOST_END_X, GHOST_CENTER_Y},
             {GHOST_WIDTH, GHOST_HEIGHT}, GHOST_SPEED}
        },
        {
            {{CRATE_BASE_X, CRATE_BASE_Y}, {CRATE_WIDTH, CRATE_HEIGHT}},
            {{UPPER_CRATE_X, PLATFORM_5_TOP_Y}, {UPPER_CRATE_WIDTH, UPPER_CRATE_HEIGHT}}
        }
    };
    loadRoomGeometry(level, PLATFORMS, STARS);
    return level;
}

/// Nel livello 2, A comanda il portone basso e B apre la via di ritorno.
LevelDefinition level2()
{
    using namespace LevelConfig::Level2;

    LevelDefinition level{};
    level.roomHalfSize = {ROOM_HALF_WIDTH, ROOM_HALF_HEIGHT};
    level.floorY = FLOOR_Y;
    level.spawnFeet = {{{PLAYER_1_SPAWN_X, SPAWN_TOP_Y}, {PLAYER_2_SPAWN_X, SPAWN_TOP_Y}}};
    level.doorBase = {EXIT_X, EXIT_FLOOR_Y};

    loadRoomGeometry(level, PLATFORMS, STARS);

    level.pressurePlates = {
        {{PLATE_A_X, PLATE_A_FLOOR_Y}, PLATE_WIDTH, PLATE_HEIGHT, PLATE_PRESS_DISTANCE},
        {{PLATE_B_X, FLOOR_Y}, PLATE_WIDTH, PLATE_HEIGHT, PLATE_PRESS_DISTANCE}
    };
    const float lowerGateCenter = FLOOR_Y + LOWER_GATE_HEIGHT * 0.5f;
    const float serviceGateCenter = SERVICE_FLOOR_Y + SERVICE_GATE_HEIGHT * 0.5f;
    level.movingWalls = {
        {{GATE_X, lowerGateCenter}, {GATE_X, lowerGateCenter + GATE_TRAVEL_Y},
         {GATE_WIDTH, LOWER_GATE_HEIGHT}, GATE_TRAVEL_SECONDS, {0}},
        {{GATE_X, serviceGateCenter}, {GATE_X, serviceGateCenter + GATE_TRAVEL_Y},
         {GATE_WIDTH, SERVICE_GATE_HEIGHT}, GATE_TRAVEL_SECONDS, {1}},
        {{RIGHT_STEP_X, RIGHT_STEP_LOWER_Y}, {RIGHT_STEP_X, RIGHT_STEP_RAISED_Y},
         {RIGHT_STEP_WIDTH, RIGHT_STEP_HEIGHT}, RIGHT_STEP_TRAVEL_SECONDS, {1}}
    };
    level.ghosts = {
        {{LEFT_GHOST_X, LEFT_GHOST_START_Y}, {LEFT_GHOST_X, LEFT_GHOST_END_Y},
         {GHOST_SIZE, GHOST_SIZE}, LEFT_GHOST_SPEED},
        {{SERVICE_GHOST_START_X, SERVICE_GHOST_Y}, {SERVICE_GHOST_END_X, SERVICE_GHOST_Y},
         {GHOST_SIZE, GHOST_SIZE}, SERVICE_GHOST_SPEED}
    };
    level.crates = {{{CRATE_X, FLOOR_Y}, {CRATE_WIDTH, CRATE_HEIGHT}}};
    return level;
}

/// Nel livello 3 i due percorsi si aprono a vicenda.
LevelDefinition level3()
{
    using namespace LevelConfig::Level3;

    LevelDefinition level{};
    level.roomHalfSize = {ROOM_HALF_WIDTH, ROOM_HALF_HEIGHT};
    level.floorY = FLOOR_Y;
    level.spawnFeet = {{{PLAYER_1_SPAWN_X, UPPER_FLOOR_Y}, {PLAYER_2_SPAWN_X, UPPER_FLOOR_Y}}};
    level.doorBase = {EXIT_X, FLOOR_Y};
    loadRoomGeometry(level, PLATFORMS, STARS);

    level.pressurePlates = {
        {{PLATE_A_X, UPPER_FLOOR_Y}, PLATE_WIDTH, PLATE_HEIGHT, PLATE_PRESS_DISTANCE},
        {{PLATE_B_X, FLOOR_Y}, PLATE_WIDTH, PLATE_HEIGHT, PLATE_PRESS_DISTANCE},
        {{PLATE_C_X, UPPER_FLOOR_Y}, PLATE_WIDTH, PLATE_HEIGHT, PLATE_PRESS_DISTANCE},
        {{PLATE_D_X, FLOOR_Y}, PLATE_WIDTH, PLATE_HEIGHT, PLATE_PRESS_DISTANCE}
    };
    const float lowerGateCenter = FLOOR_Y + LOWER_GATE_HEIGHT * 0.5f;
    const float upperGateCenter = UPPER_FLOOR_Y + UPPER_GATE_HEIGHT * 0.5f;
    // Ogni pedana comanda la paratia sul percorso opposto.
    level.movingWalls = {
        {{LOWER_GATE_A_X, lowerGateCenter}, {LOWER_GATE_A_X, lowerGateCenter + GATE_TRAVEL_Y},
         {GATE_WIDTH, LOWER_GATE_HEIGHT}, GATE_TRAVEL_SECONDS, {0}},
        {{UPPER_GATE_B_X, upperGateCenter}, {UPPER_GATE_B_X, upperGateCenter + GATE_TRAVEL_Y},
         {GATE_WIDTH, UPPER_GATE_HEIGHT}, GATE_TRAVEL_SECONDS, {1}},
        {{LOWER_GATE_C_X, lowerGateCenter}, {LOWER_GATE_C_X, lowerGateCenter + GATE_TRAVEL_Y},
         {GATE_WIDTH, LOWER_GATE_HEIGHT}, GATE_TRAVEL_SECONDS, {2}},
        {{UPPER_GATE_D_X, upperGateCenter}, {UPPER_GATE_D_X, upperGateCenter + GATE_TRAVEL_Y},
         {GATE_WIDTH, UPPER_GATE_HEIGHT}, GATE_TRAVEL_SECONDS, {3}}
    };
    level.crates = {
        {{UPPER_CRATE_X, CRATE_SHELF_Y}, {CRATE_SIZE, CRATE_SIZE}},
        {{LOWER_CRATE_X, FLOOR_Y}, {CRATE_SIZE, CRATE_SIZE}}
    };
    level.ghosts = {
        {{GHOST_START_X, GHOST_Y}, {GHOST_END_X, GHOST_Y}, {GHOST_SIZE, GHOST_SIZE}, GHOST_SPEED}
    };
    return level;
}

const LevelDefinition levels[] = {
    level1(),
    level2(),
    level3()
};

/// Compone una stanza di prova con gli stessi oggetti della campagna.
LevelDefinition tutorialRoom()
{
    using namespace TutorialConfig;

    LevelDefinition room{};
    room.roomHalfSize = {ROOM_HALF_WIDTH, ROOM_HALF_HEIGHT};
    room.floorY = FLOOR_Y;
    room.spawnFeet = {{{PLAYER_1_SPAWN_X, FLOOR_Y}, {PLAYER_2_SPAWN_X, FLOOR_Y}}};
    room.doorBase = {EXIT_X, FLOOR_Y};
    room.platforms = {
        {{PRACTICE_PLATFORM_X, FLOOR_Y}, {PRACTICE_PLATFORM_WIDTH, PRACTICE_PLATFORM_HEIGHT}},
        {{LOW_PASSAGE_X, FLOOR_Y + LOW_PASSAGE_CLEARANCE}, {LOW_PASSAGE_WIDTH, LOW_PASSAGE_THICKNESS}}
    };
    room.stars = {{PRACTICE_PLATFORM_X, FLOOR_Y + STAR_HEIGHT}};
    room.pressurePlates = {{{PLATE_X, FLOOR_Y}, PLATE_WIDTH, PLATE_HEIGHT, PLATE_PRESS_DISTANCE}};
    const float wallCenterY = FLOOR_Y + WALL_HEIGHT * 0.5f;
    room.movingWalls = {
        {{WALL_X, wallCenterY}, {WALL_X, wallCenterY + WALL_CLEARANCE},
         {WALL_WIDTH, WALL_HEIGHT}, WALL_TRAVEL_SECONDS, {0}}
    };
    room.ghosts = {{{GHOST_START_X, GHOST_CENTER_Y}, {GHOST_END_X, GHOST_CENTER_Y},
                    {GHOST_SIZE, GHOST_SIZE}, GHOST_SPEED}};
    room.crates = {{{CRATE_X, FLOOR_Y}, {CRATE_SIZE, CRATE_SIZE}}};
    return room;
}
}

namespace Levels {

/// Restituisce una definizione in sola lettura. Gli indici partono da zero.
const LevelDefinition& get(std::size_t index)
{
    if (index >= count()) {
        throw std::out_of_range("Indice livello non valido");
    }
    return levels[index];
}

/// Restituisce il numero di livelli disponibili nel catalogo.
std::size_t count()
{
    return std::size(levels);
}

const LevelDefinition& tutorial()
{
    static const LevelDefinition definition = tutorialRoom();
    return definition;
}

} // namespace Levels
