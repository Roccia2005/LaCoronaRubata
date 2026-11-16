#include "LevelCatalog.h"
#include "Config.h"
#include "LevelDefinition.h"
#include <iterator>
#include <stdexcept>

namespace {

using namespace LevelConfig;

/// Assembla il Deserto della Volpe usando le quote di Config.h.
LevelDefinition foxDesert()
{
    LevelDefinition level{
        {ROOM_HALF_WIDTH_L1, ROOM_HALF_HEIGHT_L1},
        {{{PLAYER_1_SPAWN_X_L1, PLAYER_1_SPAWN_FEET_Y_L1}, {PLAYER_2_SPAWN_X_L1, PLAYER_2_SPAWN_FEET_Y_L1}}},
        {DOOR_X_L1, DOOR_BASE_Y_L1},
        {
            {{PLATFORM_1_BASE_X_L1, PLATFORM_1_BASE_Y_L1}, {PLATFORM_1_WIDTH_L1, PLATFORM_1_THICKNESS_L1}},
            {{PLATFORM_2_BASE_X_L1, PLATFORM_2_BASE_Y_L1}, {PLATFORM_2_WIDTH_L1, PLATFORM_2_THICKNESS_L1}},
            {{PLATFORM_3_BASE_X_L1, PLATFORM_3_BASE_Y_L1}, {PLATFORM_3_WIDTH_L1, PLATFORM_3_THICKNESS_L1}},
            {{PLATFORM_4_BASE_X_L1, PLATFORM_4_BASE_Y_L1}, {PLATFORM_4_WIDTH_L1, PLATFORM_4_THICKNESS_L1}},
            {{PLATFORM_5_BASE_X_L1, PLATFORM_5_BASE_Y_L1}, {PLATFORM_5_WIDTH_L1, PLATFORM_5_THICKNESS_L1}},
            {{UPPER_WALL_1_X_L1, PLATFORM_5_TOP_Y_L1}, {UPPER_WALL_1_WIDTH_L1, UPPER_WALL_1_HEIGHT_L1}},
            {{UPPER_WALL_2_X_L1, PLATFORM_5_TOP_Y_L1}, {UPPER_WALL_2_WIDTH_L1, UPPER_WALL_2_HEIGHT_L1}},
            {{UPPER_WALL_3_X_L1, PLATFORM_5_TOP_Y_L1}, {UPPER_WALL_3_WIDTH_L1, UPPER_WALL_3_HEIGHT_L1}},
            {{BONUS_WALL_X_L1, BONUS_WALL_BASE_Y_L1},
             {BONUS_WALL_WIDTH_L1, BONUS_WALL_TOP_Y_L1 - BONUS_WALL_BASE_Y_L1}}
        },
        {
            {CRYSTAL_1_X_L1, CRYSTAL_1_Y_L1},
            {CRYSTAL_2_X_L1, CRYSTAL_2_Y_L1},
            {CRYSTAL_3_X_L1, CRYSTAL_3_Y_L1},
            {CRYSTAL_4_X_L1, CRYSTAL_4_Y_L1},
            {CRYSTAL_5_X_L1, CRYSTAL_5_Y_L1},
            {CRYSTAL_6_X_L1, CRYSTAL_6_Y_L1},
            {CRYSTAL_7_X_L1, CRYSTAL_7_Y_L1}
        },
        {
            {{PRESSURE_PLATE_A_BASE_X_L1, PRESSURE_PLATE_A_BASE_Y_L1},
             PRESSURE_PLATE_WIDTH_L1,
             PRESSURE_PLATE_HEIGHT_L1,
             PRESSURE_PLATE_PRESS_DISTANCE_L1},
            {{PRESSURE_PLATE_B_BASE_X_L1, PRESSURE_PLATE_B_BASE_Y_L1},
             PRESSURE_PLATE_WIDTH_L1,
             PRESSURE_PLATE_HEIGHT_L1,
             PRESSURE_PLATE_PRESS_DISTANCE_L1}
        },
        FLOOR_Y_L1,
        {
            {{MOVING_WALL_CLOSED_CENTER_X_L1, MOVING_WALL_CENTER_Y_L1},
             {MOVING_WALL_OPEN_CENTER_X_L1, MOVING_WALL_CENTER_Y_L1},
             {MOVING_WALL_WIDTH_L1, MOVING_WALL_HEIGHT_L1},
             MOVING_WALL_TRAVEL_SECONDS_L1,
             MOVING_WALL_PRESSURE_PLATE_INDICES_L1}
        },
        {
            {{GHOST_START_X_L1, GHOST_CENTER_Y_L1}, {GHOST_END_X_L1, GHOST_CENTER_Y_L1},
             {GHOST_WIDTH_L1, GHOST_HEIGHT_L1}, GHOST_SPEED_L1}
        },
        {
            {{CRATE_BASE_X_L1, CRATE_BASE_Y_L1}, {CRATE_WIDTH_L1, CRATE_HEIGHT_L1}},
            {{UPPER_CRATE_X_L1, PLATFORM_5_TOP_Y_L1}, {UPPER_CRATE_WIDTH_L1, UPPER_CRATE_HEIGHT_L1}}
        }
    };
    for (std::size_t step = 0; step < BONUS_STEP_COUNT_L1; ++step) {
        const float x = step % 2 == 0 ? BONUS_STEP_LEFT_X_L1 : BONUS_STEP_RIGHT_X_L1;
        const float top = BONUS_FIRST_STEP_TOP_Y_L1 + static_cast<float>(step) * BONUS_STEP_RISE_L1;
        level.platforms.push_back({
            {x, top - BONUS_STEP_THICKNESS_L1},
            {BONUS_STEP_WIDTH_L1, BONUS_STEP_THICKNESS_L1}
        });
    }
    return level;
}

/// Converte i bordi delle piattaforme nelle posizioni di base usate dalla simulazione.
template<std::size_t PlatformCount, std::size_t CrystalCount>
void loadRoomGeometry(LevelDefinition& level,
                      const PlatformBounds (&platforms)[PlatformCount],
                      const CrystalPosition (&crystals)[CrystalCount])
{
    level.platforms.reserve(PlatformCount);
    for (const auto& platform : platforms) {
        level.platforms.push_back({
            {(platform.left + platform.right) * 0.5f, platform.top - platform.thickness},
            {platform.right - platform.left, platform.thickness}
        });
    }

    level.crystals.reserve(CrystalCount);
    for (const auto& crystal : crystals) {
        level.crystals.push_back({crystal.x, crystal.y});
    }
}

/// Assembla la Stazione sospesa: A comanda il portone basso; B apre la via di ritorno.
LevelDefinition suspendedStation()
{
    using namespace LevelConfig::Station;

    LevelDefinition level{};
    level.roomHalfSize = {ROOM_HALF_WIDTH, ROOM_HALF_HEIGHT};
    level.floorY = FLOOR_Y;
    level.spawnFeet = {{{PLAYER_1_SPAWN_X, SPAWN_TOP_Y}, {PLAYER_2_SPAWN_X, SPAWN_TOP_Y}}};
    level.doorBase = {EXIT_X, EXIT_FLOOR_Y};

    loadRoomGeometry(level, PLATFORMS, CRYSTALS);

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

/// Assembla il Modulo di atterraggio: i due percorsi si aprono a vicenda.
LevelDefinition landingModule()
{
    using namespace LevelConfig::LandingModule;

    LevelDefinition level{};
    level.roomHalfSize = {ROOM_HALF_WIDTH, ROOM_HALF_HEIGHT};
    level.floorY = FLOOR_Y;
    level.spawnFeet = {{{PLAYER_1_SPAWN_X, UPPER_FLOOR_Y}, {PLAYER_2_SPAWN_X, UPPER_FLOOR_Y}}};
    level.doorBase = {EXIT_X, FLOOR_Y};
    loadRoomGeometry(level, PLATFORMS, CRYSTALS);

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
    foxDesert(),
    suspendedStation(),
    landingModule()
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
    room.crystals = {{PRACTICE_PLATFORM_X, FLOOR_Y + CRYSTAL_HEIGHT}};
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
