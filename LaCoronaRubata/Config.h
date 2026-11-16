#pragma once
#include <cstddef>
#include <initializer_list>

namespace GameConfig {

inline constexpr float DEFAULT_FLOOR_Y = -48.0f;
inline constexpr float PLAYER_WIDTH = 6.0f;
inline constexpr float PLAYER_HITBOX_WIDTH = 5.0f;
inline constexpr float PLAYER_HEIGHT = 7.0f;
inline constexpr float MOVE_SPEED = 43.0f;
inline constexpr float JUMP_SPEED = 75.0f; // salto sufficiente per la prima piattaforma
inline constexpr float GRAVITY = -135.0f;
inline constexpr float CRATE_MAX_STEP_HEIGHT = 1.5f;
inline constexpr float COIN_RADIUS = 3.0f;
inline constexpr float COIN_BOB_AMPLITUDE = 2.0f; // unita del mondo
inline constexpr float COIN_BOB_FREQUENCY = 3.0f; // radianti al secondo
inline constexpr float COIN_PHASE_STEP = 0.8f;

inline constexpr float CROUCH_HEIGHT_RATIO = 0.5f;
inline constexpr float MAX_FRAME_DELTA_SECONDS = 0.03f;
inline constexpr float DOOR_WIDTH = 12.0f;
inline constexpr float DOOR_HEIGHT = 16.0f;
inline constexpr float DOOR_OPENING_SECONDS = 0.35f;
inline constexpr float FLOOR_THICKNESS = 6.0f;
inline constexpr int POINTS_PER_CRYSTAL = 10;

} // namespace GameConfig

namespace LevelConfig {

// Livello 1
inline constexpr float ROOM_HALF_WIDTH_L1 = 100.0f;
inline constexpr float ROOM_HALF_HEIGHT_L1 = 60.0f;
inline constexpr float FLOOR_Y_L1 = GameConfig::DEFAULT_FLOOR_Y;

inline constexpr float PLAYER_1_SPAWN_X_L1 = -92.0f;
inline constexpr float PLAYER_1_SPAWN_FEET_Y_L1 = FLOOR_Y_L1;
inline constexpr float PLAYER_2_SPAWN_X_L1 = -78.0f;
inline constexpr float PLAYER_2_SPAWN_FEET_Y_L1 = FLOOR_Y_L1;

inline constexpr float DOOR_X_L1 = 75.0f;
inline constexpr float DOOR_BASE_Y_L1 = FLOOR_Y_L1;

// Piattaforme: posizione della base in coordinate mondo e dimensioni.
inline constexpr float PLATFORM_1_BASE_X_L1 = -43.0f;
inline constexpr float PLATFORM_1_BASE_Y_L1 = FLOOR_Y_L1;
inline constexpr float PLATFORM_1_WIDTH_L1 = 10.0f;
inline constexpr float PLATFORM_1_THICKNESS_L1 = 15.0f;

inline constexpr float PLATFORM_2_BASE_X_L1 = 0.0f;
inline constexpr float PLATFORM_2_BASE_Y_L1 = FLOOR_Y_L1;
inline constexpr float PLATFORM_2_WIDTH_L1 = 12.0f;
inline constexpr float PLATFORM_2_THICKNESS_L1 = 22.0f;

inline constexpr float PLATFORM_3_BASE_X_L1 = 48.0f;
inline constexpr float PLATFORM_3_BASE_Y_L1 = FLOOR_Y_L1;
inline constexpr float PLATFORM_3_WIDTH_L1 = 10.0f;
inline constexpr float PLATFORM_3_THICKNESS_L1 = 18.0f;

// Cristalli: posizione del centro a riposo.
inline constexpr float CRYSTAL_1_X_L1 = -68.0f;
inline constexpr float CRYSTAL_1_Y_L1 = -27.0f;
inline constexpr float CRYSTAL_2_X_L1 = -43.0f;
inline constexpr float CRYSTAL_2_Y_L1 = -21.0f;
inline constexpr float CRYSTAL_3_X_L1 = -18.0f;
inline constexpr float CRYSTAL_3_Y_L1 = -23.0f;
inline constexpr float CRYSTAL_4_X_L1 = 0.0f;
inline constexpr float CRYSTAL_4_Y_L1 = -13.0f;
inline constexpr float CRYSTAL_5_X_L1 = 24.0f;
inline constexpr float CRYSTAL_5_Y_L1 = -28.0f;
inline constexpr float CRYSTAL_6_X_L1 = 48.0f;
inline constexpr float CRYSTAL_6_Y_L1 = -17.0f;
inline constexpr float CRYSTAL_7_X_L1 = 77.0f;
inline constexpr float CRYSTAL_7_Y_L1 = -28.0f;

// Pedane: una prima del muro e una oltre il passaggio, con le stesse dimensioni.
inline constexpr float PRESSURE_PLATE_BASE_X_L1 = -60.0f;
inline constexpr float PRESSURE_PLATE_2_BASE_X_L1 = -18.0f;
inline constexpr float PRESSURE_PLATE_BASE_Y_L1 = FLOOR_Y_L1;
inline constexpr float PRESSURE_PLATE_WIDTH_L1 = 10.0f;
inline constexpr float PRESSURE_PLATE_HEIGHT_L1 = 1.5f;
inline constexpr float PRESSURE_PLATE_PRESS_DISTANCE_L1 = 0.3f;

// Parete mobile: stessa X per entrambe le posizioni, apertura verso l'alto.
inline constexpr float MOVING_WALL_CENTER_X_L1 = -28.0f;
inline constexpr float MOVING_WALL_WIDTH_L1 = 8.0f;
inline constexpr float MOVING_WALL_HEIGHT_L1 = 18.0f;
inline constexpr float MOVING_WALL_TRAVEL_Y_L1 = 14.0f;
inline constexpr float MOVING_WALL_CLOSED_CENTER_Y_L1 = FLOOR_Y_L1 + MOVING_WALL_HEIGHT_L1 * 0.5f;
inline constexpr float MOVING_WALL_OPEN_CENTER_Y_L1 = MOVING_WALL_CLOSED_CENTER_Y_L1 + MOVING_WALL_TRAVEL_Y_L1;
inline constexpr float MOVING_WALL_TRAVEL_SECONDS_L1 = 0.6f;
inline constexpr std::initializer_list<std::size_t> MOVING_WALL_PRESSURE_PLATE_INDICES_L1{0, 1};

// Cassa di prova, sul pavimento prima della prima pedana.
inline constexpr float CRATE_BASE_X_L1 = -70.0f;
inline constexpr float CRATE_BASE_Y_L1 = FLOOR_Y_L1;
inline constexpr float CRATE_WIDTH_L1 = 6.0f;
inline constexpr float CRATE_HEIGHT_L1 = 6.0f;

// Livello 2
inline constexpr float ROOM_HALF_WIDTH_L2 = 100.0f;
inline constexpr float ROOM_HALF_HEIGHT_L2 = 60.0f;
inline constexpr float FLOOR_Y_L2 = GameConfig::DEFAULT_FLOOR_Y;

inline constexpr float PLAYER_1_SPAWN_X_L2 = 84.0f;
inline constexpr float PLAYER_1_SPAWN_FEET_Y_L2 = FLOOR_Y_L2;
inline constexpr float PLAYER_2_SPAWN_X_L2 = 70.0f;
inline constexpr float PLAYER_2_SPAWN_FEET_Y_L2 = FLOOR_Y_L2;

inline constexpr float DOOR_X_L2 = -75.0f;
inline constexpr float DOOR_BASE_Y_L2 = FLOOR_Y_L2;

// Piattaforme: posizione della base in coordinate mondo e dimensioni.
inline constexpr float PLATFORM_1_BASE_X_L2 = 30.0f;
inline constexpr float PLATFORM_1_BASE_Y_L2 = FLOOR_Y_L2;
inline constexpr float PLATFORM_1_WIDTH_L2 = 10.0f;
inline constexpr float PLATFORM_1_THICKNESS_L2 = 8.0f;

inline constexpr float PLATFORM_2_BASE_X_L2 = -25.0f;
inline constexpr float PLATFORM_2_BASE_Y_L2 = FLOOR_Y_L2;
inline constexpr float PLATFORM_2_WIDTH_L2 = 12.0f;
inline constexpr float PLATFORM_2_THICKNESS_L2 = 12.0f;

// Cristalli: posizione del centro a riposo.
inline constexpr float CRYSTAL_1_X_L2 = 30.0f;
inline constexpr float CRYSTAL_1_Y_L2 = -32.0f;
inline constexpr float CRYSTAL_2_X_L2 = -25.0f;
inline constexpr float CRYSTAL_2_Y_L2 = -28.0f;

// Fantasma: pattuglia il corridoio tra le due piattaforme.
inline constexpr float GHOST_START_X_L2 = -14.0f;
inline constexpr float GHOST_END_X_L2 = 18.0f;
inline constexpr float GHOST_CENTER_Y_L2 = FLOOR_Y_L2 + 6.0f;
inline constexpr float GHOST_WIDTH_L2 = 8.0f;
inline constexpr float GHOST_HEIGHT_L2 = 8.0f;
inline constexpr float GHOST_SPEED_L2 = 16.0f;

} // namespace LevelConfig
