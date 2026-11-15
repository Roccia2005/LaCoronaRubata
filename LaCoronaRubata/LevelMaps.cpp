#include "LevelMaps.h"
#include "Config.h"

namespace LevelMaps {

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
             PRESSURE_PLATE_PRESS_DISTANCE_L1}
        },
        FLOOR_Y_L1
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
        FLOOR_Y_L2
    };
}

} // namespace LevelMaps
