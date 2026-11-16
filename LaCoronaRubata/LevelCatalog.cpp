#include "LevelCatalog.h"
#include "Config.h"
#include "LevelDefinition.h"
#include <iterator>
#include <stdexcept>

namespace {

using namespace LevelConfig;

/// Assembla la definizione della stanza 1 usando i valori di Config.h.
LevelDefinition firstRoom()
{
    return {
        {ROOM_HALF_WIDTH_L1, ROOM_HALF_HEIGHT_L1},
        {{{PLAYER_1_SPAWN_X_L1, PLAYER_1_SPAWN_FEET_Y_L1}, {PLAYER_2_SPAWN_X_L1, PLAYER_2_SPAWN_FEET_Y_L1}}},
        {DOOR_X_L1, DOOR_BASE_Y_L1},
        {
            {{PLATFORM_1_BASE_X_L1, PLATFORM_1_BASE_Y_L1}, {PLATFORM_1_WIDTH_L1, PLATFORM_1_THICKNESS_L1}},
            {{PLATFORM_2_BASE_X_L1, PLATFORM_2_BASE_Y_L1}, {PLATFORM_2_WIDTH_L1, PLATFORM_2_THICKNESS_L1}},
            {{PLATFORM_3_BASE_X_L1, PLATFORM_3_BASE_Y_L1}, {PLATFORM_3_WIDTH_L1, PLATFORM_3_THICKNESS_L1}}
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
            {{PRESSURE_PLATE_BASE_X_L1, PRESSURE_PLATE_BASE_Y_L1},
             PRESSURE_PLATE_WIDTH_L1,
             PRESSURE_PLATE_HEIGHT_L1,
             PRESSURE_PLATE_PRESS_DISTANCE_L1},
            {{PRESSURE_PLATE_2_BASE_X_L1, PRESSURE_PLATE_BASE_Y_L1},
             PRESSURE_PLATE_WIDTH_L1,
             PRESSURE_PLATE_HEIGHT_L1,
             PRESSURE_PLATE_PRESS_DISTANCE_L1}
        },
        FLOOR_Y_L1,
        {
            {{MOVING_WALL_CENTER_X_L1, MOVING_WALL_CLOSED_CENTER_Y_L1},
             {MOVING_WALL_CENTER_X_L1, MOVING_WALL_OPEN_CENTER_Y_L1},
             {MOVING_WALL_WIDTH_L1, MOVING_WALL_HEIGHT_L1},
             MOVING_WALL_TRAVEL_SECONDS_L1,
             MOVING_WALL_PRESSURE_PLATE_INDICES_L1}
        },
        {},
        {
            {{CRATE_BASE_X_L1, CRATE_BASE_Y_L1}, {CRATE_WIDTH_L1, CRATE_HEIGHT_L1}}
        }
    };
}

/// Assembla la definizione della stanza 2 usando i valori di Config.h.
LevelDefinition secondRoom()
{
    return {
        {ROOM_HALF_WIDTH_L2, ROOM_HALF_HEIGHT_L2},
        {{{PLAYER_1_SPAWN_X_L2, PLAYER_1_SPAWN_FEET_Y_L2}, {PLAYER_2_SPAWN_X_L2, PLAYER_2_SPAWN_FEET_Y_L2}}},
        {DOOR_X_L2, DOOR_BASE_Y_L2},
        {
            {{PLATFORM_1_BASE_X_L2, PLATFORM_1_BASE_Y_L2}, {PLATFORM_1_WIDTH_L2, PLATFORM_1_THICKNESS_L2}},
            {{PLATFORM_2_BASE_X_L2, PLATFORM_2_BASE_Y_L2}, {PLATFORM_2_WIDTH_L2, PLATFORM_2_THICKNESS_L2}}
        },
        {
            {CRYSTAL_1_X_L2, CRYSTAL_1_Y_L2},
            {CRYSTAL_2_X_L2, CRYSTAL_2_Y_L2}
        },
        {},
        FLOOR_Y_L2,
        {},
        {
            {{GHOST_START_X_L2, GHOST_CENTER_Y_L2}, GHOST_END_X_L2,
             {GHOST_WIDTH_L2, GHOST_HEIGHT_L2}, GHOST_SPEED_L2}
        }
    };
}

const LevelDefinition levels[] = {
    firstRoom(),
    secondRoom()
};
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

} // namespace Levels
