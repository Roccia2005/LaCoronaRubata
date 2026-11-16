#pragma once
#include <cstddef>
#include <initializer_list>

namespace GameConfig {

inline constexpr char GAME_TITLE[] = "Il Piccolo Principe e la Volpe";
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

struct PlatformBounds {
    float left;
    float right;
    float top;
    float thickness;
};

struct CrystalPosition {
    float x;
    float y;
};

// Livello 1 - Deserto della Volpe.
inline constexpr float ROOM_HALF_WIDTH_L1 = 120.0f;
inline constexpr float ROOM_HALF_HEIGHT_L1 = 76.0f;
inline constexpr float FLOOR_Y_L1 = GameConfig::DEFAULT_FLOOR_Y;
// Il percorso principale slitta a sinistra per lasciare spazio alla salita bonus.
inline constexpr float MAIN_ROUTE_X_OFFSET_L1 = -20.0f;

inline constexpr float PLAYER_1_SPAWN_X_L1 = -92.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float PLAYER_1_SPAWN_FEET_Y_L1 = FLOOR_Y_L1;
inline constexpr float PLAYER_2_SPAWN_X_L1 = -82.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float PLAYER_2_SPAWN_FEET_Y_L1 = FLOOR_Y_L1;

// Due blocchi sospesi conducono alla passerella del fantasma.
inline constexpr float PLATFORM_1_BASE_X_L1 = -15.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float PLATFORM_1_BASE_Y_L1 = -25.0f;
inline constexpr float PLATFORM_1_WIDTH_L1 = 20.0f;
inline constexpr float PLATFORM_1_THICKNESS_L1 = 6.0f;

inline constexpr float PLATFORM_2_BASE_X_L1 = 7.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float PLATFORM_2_BASE_Y_L1 = -24.0f;
inline constexpr float PLATFORM_2_WIDTH_L1 = 18.0f;
inline constexpr float PLATFORM_2_THICKNESS_L1 = 7.0f;

inline constexpr float PLATFORM_3_BASE_X_L1 = 58.5f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float PLATFORM_3_TOP_Y_L1 = -10.0f;
inline constexpr float PLATFORM_3_WIDTH_L1 = 75.0f;
inline constexpr float PLATFORM_3_THICKNESS_L1 = 12.0f;
inline constexpr float PLATFORM_3_BASE_Y_L1 = PLATFORM_3_TOP_Y_L1 - PLATFORM_3_THICKNESS_L1;

// Ripiano della pedana B e piano superiore con l'uscita.
inline constexpr float PLATFORM_4_BASE_X_L1 = -83.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float PLATFORM_4_TOP_Y_L1 = 5.0f;
inline constexpr float PLATFORM_4_WIDTH_L1 = 34.0f;
inline constexpr float PLATFORM_4_THICKNESS_L1 = 6.0f;
inline constexpr float PLATFORM_4_BASE_Y_L1 = PLATFORM_4_TOP_Y_L1 - PLATFORM_4_THICKNESS_L1;

inline constexpr float PLATFORM_5_BASE_X_L1 = 26.5f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float PLATFORM_5_TOP_Y_L1 = 22.0f;
inline constexpr float PLATFORM_5_WIDTH_L1 = 147.0f;
inline constexpr float PLATFORM_5_THICKNESS_L1 = 5.0f;
inline constexpr float PLATFORM_5_BASE_Y_L1 = PLATFORM_5_TOP_Y_L1 - PLATFORM_5_THICKNESS_L1;

inline constexpr float DOOR_X_L1 = 71.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float DOOR_BASE_Y_L1 = PLATFORM_5_TOP_Y_L1;

// I tre muri del piano superiore si affrontano in successione.
inline constexpr float UPPER_WALL_1_X_L1 = -26.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float UPPER_WALL_1_WIDTH_L1 = 8.0f;
inline constexpr float UPPER_WALL_1_HEIGHT_L1 = 14.0f;
inline constexpr float UPPER_WALL_2_X_L1 = -9.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float UPPER_WALL_2_WIDTH_L1 = 10.0f;
inline constexpr float UPPER_WALL_2_HEIGHT_L1 = 25.0f;
inline constexpr float UPPER_WALL_3_X_L1 = 29.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float UPPER_WALL_3_WIDTH_L1 = 10.0f;
inline constexpr float UPPER_WALL_3_HEIGHT_L1 = 36.0f;
inline constexpr float UPPER_CRATE_X_L1 = 2.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float UPPER_CRATE_WIDTH_L1 = 8.0f;
inline constexpr float UPPER_CRATE_HEIGHT_L1 = 6.0f;

// Il muro separa la salita bonus dal piano dell'uscita e lascia passare la cassa al suolo.
inline constexpr float BONUS_WALL_X_L1 = 81.0f;
inline constexpr float BONUS_WALL_BASE_Y_L1 = -24.0f;
inline constexpr float BONUS_WALL_WIDTH_L1 = 10.0f;
inline constexpr float BONUS_WALL_TOP_Y_L1 = 100.0f;
inline constexpr float BONUS_STEP_WIDTH_L1 = 10.0f;
inline constexpr float BONUS_STEP_THICKNESS_L1 = 3.0f;
inline constexpr float BONUS_STEP_LEFT_X_L1 = BONUS_WALL_X_L1
    + (BONUS_WALL_WIDTH_L1 + BONUS_STEP_WIDTH_L1) * 0.5f;
inline constexpr float BONUS_STEP_RIGHT_X_L1 = ROOM_HALF_WIDTH_L1 - BONUS_STEP_WIDTH_L1 * 0.5f;
// Dal pavimento al primo gradino servono l'altezza della cassa e quella del compagno.
inline constexpr float BONUS_FIRST_STEP_TOP_Y_L1 = FLOOR_Y_L1 + 35.0f;
inline constexpr float BONUS_STEP_RISE_L1 = 13.0f;
inline constexpr std::size_t BONUS_STEP_COUNT_L1 = 6;
inline constexpr float BONUS_LAST_STEP_TOP_Y_L1 = BONUS_FIRST_STEP_TOP_Y_L1
    + BONUS_STEP_RISE_L1 * (BONUS_STEP_COUNT_L1 - 1);

// Cristalli: posizione del centro a riposo.
inline constexpr float CRYSTAL_1_X_L1 = 18.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float CRYSTAL_1_Y_L1 = -40.0f;
inline constexpr float CRYSTAL_2_X_L1 = -17.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float CRYSTAL_2_Y_L1 = -11.0f;
inline constexpr float CRYSTAL_3_X_L1 = 9.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float CRYSTAL_3_Y_L1 = -10.0f;
inline constexpr float CRYSTAL_4_X_L1 = 63.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float CRYSTAL_4_Y_L1 = -2.0f;
inline constexpr float CRYSTAL_5_X_L1 = UPPER_WALL_3_X_L1;
inline constexpr float CRYSTAL_5_Y_L1 = PLATFORM_5_TOP_Y_L1 + UPPER_WALL_3_HEIGHT_L1 + 7.0f;
inline constexpr float CRYSTAL_6_X_L1 = 16.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float CRYSTAL_6_Y_L1 = PLATFORM_5_TOP_Y_L1 + 5.0f;
inline constexpr float CRYSTAL_7_X_L1 = BONUS_STEP_RIGHT_X_L1;
inline constexpr float CRYSTAL_7_Y_L1 = BONUS_LAST_STEP_TOP_Y_L1 + 7.0f;

// A e' all'estremita' della pattuglia, B sul ripiano sinistro. Basta tenerne premuta una.
inline constexpr float PRESSURE_PLATE_A_BASE_X_L1 = 86.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float PRESSURE_PLATE_A_BASE_Y_L1 = PLATFORM_3_TOP_Y_L1;
inline constexpr float PRESSURE_PLATE_B_BASE_X_L1 = -87.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float PRESSURE_PLATE_B_BASE_Y_L1 = PLATFORM_4_TOP_Y_L1;
inline constexpr float PRESSURE_PLATE_WIDTH_L1 = 8.0f;
inline constexpr float PRESSURE_PLATE_HEIGHT_L1 = 1.2f;
inline constexpr float PRESSURE_PLATE_PRESS_DISTANCE_L1 = 0.3f;

// La parete scorre verso destra e diventa un gradino per raggiungere B.
inline constexpr float MOVING_WALL_CLOSED_CENTER_X_L1 = -90.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float MOVING_WALL_OPEN_CENTER_X_L1 = -54.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float MOVING_WALL_CENTER_Y_L1 = -13.0f;
inline constexpr float MOVING_WALL_WIDTH_L1 = 20.0f;
inline constexpr float MOVING_WALL_HEIGHT_L1 = 6.0f;
inline constexpr float MOVING_WALL_TRAVEL_SECONDS_L1 = 0.7f;
inline constexpr std::initializer_list<std::size_t> MOVING_WALL_PRESSURE_PLATE_INDICES_L1{0, 1};

inline constexpr float GHOST_START_X_L1 = 36.0f + MAIN_ROUTE_X_OFFSET_L1;
// La pattuglia raggiunge anche il bordo destro della pedana A.
inline constexpr float GHOST_END_X_L1 = PRESSURE_PLATE_A_BASE_X_L1 + PRESSURE_PLATE_WIDTH_L1 * 0.5f;
inline constexpr float GHOST_CENTER_Y_L1 = PLATFORM_3_TOP_Y_L1 + 6.0f;
inline constexpr float GHOST_WIDTH_L1 = 7.0f;
inline constexpr float GHOST_HEIGHT_L1 = 7.0f;
inline constexpr float GHOST_SPEED_L1 = 10.0f;

// La cassa parte a destra e si spinge a sinistra per salire sul primo blocco.
inline constexpr float CRATE_BASE_X_L1 = 65.0f + MAIN_ROUTE_X_OFFSET_L1;
inline constexpr float CRATE_BASE_Y_L1 = FLOOR_Y_L1;
inline constexpr float CRATE_WIDTH_L1 = 8.0f;
inline constexpr float CRATE_HEIGHT_L1 = 11.0f;

// Livello 2 - Stazione sospesa
namespace Station {

inline constexpr float ROOM_HALF_WIDTH = 100.0f;
inline constexpr float ROOM_HALF_HEIGHT = 60.0f;
inline constexpr float FLOOR_Y = GameConfig::DEFAULT_FLOOR_Y;
inline constexpr float SPAWN_TOP_Y = 29.0f;
inline constexpr float PLAYER_1_SPAWN_X = -83.0f;
inline constexpr float PLAYER_2_SPAWN_X = -70.0f;
inline constexpr float EXIT_X = 89.0f;
inline constexpr float EXIT_FLOOR_Y = 36.0f;

inline constexpr float PLATE_A_X = -74.0f;
inline constexpr float PLATE_A_FLOOR_Y = -30.0f;
inline constexpr float PLATE_B_X = 88.0f;
inline constexpr float PLATE_WIDTH = 8.0f;
inline constexpr float PLATE_HEIGHT = 1.2f;
inline constexpr float PLATE_PRESS_DISTANCE = 0.3f;

inline constexpr float GATE_X = -9.0f;
inline constexpr float SERVICE_FLOOR_Y = -13.0f;
inline constexpr float LOWER_GATE_HEIGHT = 21.0f;
inline constexpr float SERVICE_GATE_HEIGHT = 18.0f;
inline constexpr float GATE_WIDTH = 6.0f;
inline constexpr float GATE_TRAVEL_Y = 12.0f;
inline constexpr float GATE_TRAVEL_SECONDS = 0.48f;
inline constexpr float SHAFT_WALL_LEFT_X = -39.0f;
inline constexpr float SHAFT_LEFT_X = -32.0f;
inline constexpr float RECOVERY_FLOOR_END_X = 52.0f;
inline constexpr float RIGHT_STEP_X = 66.0f;
inline constexpr float RIGHT_STEP_LOWER_Y = -32.0f;
inline constexpr float RIGHT_STEP_RAISED_Y = -25.0f;
inline constexpr float RIGHT_STEP_WIDTH = 16.0f;
inline constexpr float RIGHT_STEP_HEIGHT = 4.0f;
inline constexpr float RIGHT_STEP_TRAVEL_SECONDS = 0.7f;

inline constexpr PlatformBounds PLATFORMS[] = {
    // Ingresso, muro della discesa e nicchia con il corridoio basso.
    {-ROOM_HALF_WIDTH, SHAFT_LEFT_X, SPAWN_TOP_Y, 5.0f},
    {SHAFT_WALL_LEFT_X, SHAFT_LEFT_X, SPAWN_TOP_Y, 72.2f},
    {-ROOM_HALF_WIDTH, -90.0f, 24.0f, 17.0f},
    {-90.0f, -66.0f, 24.0f, 24.25f},
    {-66.0f, SHAFT_WALL_LEFT_X, 24.0f, 15.0f},
    {-ROOM_HALF_WIDTH, -55.0f, -5.0f, 5.0f},
    {-87.0f, SHAFT_WALL_LEFT_X, PLATE_A_FLOOR_Y, 5.0f},
    // Gradino fisso sul lato sinistro del pozzo, con spazio per la discesa a destra.
    {SHAFT_LEFT_X, -25.0f, -31.0f, 4.0f},
    // Il divisorio separa il portone basso dal varco di servizio.
    {-12.0f, -6.0f, SERVICE_FLOOR_Y, 15.0f},
    {-12.0f, -6.0f, 86.0f, 82.0f},
    // Il pavimento trattiene chi cade. Sotto il divisorio rimane il passaggio per la cassa.
    {-12.0f, RECOVERY_FLOOR_END_X, SERVICE_FLOOR_Y, 4.0f},
    {RECOVERY_FLOOR_END_X, 57.0f, 22.0f, 56.0f},
    {9.0f, 22.0f, ROOM_HALF_HEIGHT, 38.0f},
    // Salti del percorso alto, risalita da destra e ricongiungimento.
    {13.0f, 24.0f, -2.0f, 4.0f},
    {30.0f, 42.0f, 10.0f, 4.0f},
    {56.0f, 67.0f, -8.0f, 4.0f},
    {75.0f, 88.0f, 7.0f, 4.0f},
    {49.0f, 65.0f, 22.0f, 4.0f},
    {76.0f, ROOM_HALF_WIDTH, EXIT_FLOOR_Y, 5.0f}
};

inline constexpr CrystalPosition CRYSTALS[] = {
    {-94.0f, 0.0f}, {-94.0f, -39.0f}, {-61.0f, -23.0f}, {3.0f, -39.0f},
    {18.0f, 5.0f}, {57.0f, 29.0f}, {83.0f, 14.0f}, {96.0f, 43.0f}
};

inline constexpr float LEFT_GHOST_X = -94.0f;
inline constexpr float LEFT_GHOST_START_Y = -9.0f;
inline constexpr float LEFT_GHOST_END_Y = -42.0f;
inline constexpr float LEFT_GHOST_SPEED = 12.0f;
inline constexpr float SERVICE_GHOST_START_X = 34.0f;
inline constexpr float SERVICE_GHOST_END_X = 48.0f;
inline constexpr float SERVICE_GHOST_Y = 14.0f;
inline constexpr float SERVICE_GHOST_SPEED = 14.0f;
inline constexpr float GHOST_SIZE = 7.0f;

inline constexpr float CRATE_X = 32.0f;
inline constexpr float CRATE_WIDTH = 8.0f;
inline constexpr float CRATE_HEIGHT = 11.0f;

} // namespace Station

// Livello 3 - Il modulo di atterraggio.
namespace LandingModule {

inline constexpr float ROOM_HALF_WIDTH = 100.0f;
inline constexpr float ROOM_HALF_HEIGHT = 60.0f;
inline constexpr float FLOOR_Y = GameConfig::DEFAULT_FLOOR_Y;
inline constexpr float UPPER_FLOOR_Y = 7.0f;
inline constexpr float PLAYER_1_SPAWN_X = -96.0f;
inline constexpr float PLAYER_2_SPAWN_X = -89.0f;
inline constexpr float EXIT_X = 92.0f;

inline constexpr float PLATE_A_X = -57.0f;
inline constexpr float PLATE_B_X = -25.0f;
inline constexpr float PLATE_C_X = 53.0f;
inline constexpr float PLATE_D_X = 49.0f;
inline constexpr float PLATE_WIDTH = 8.0f;
inline constexpr float PLATE_HEIGHT = 1.2f;
inline constexpr float PLATE_PRESS_DISTANCE = 0.3f;

inline constexpr float LOWER_GATE_A_X = -49.0f;
inline constexpr float UPPER_GATE_B_X = -9.0f;
inline constexpr float LOWER_GATE_C_X = 70.0f;
inline constexpr float UPPER_GATE_D_X = 76.0f;
inline constexpr float LOWER_GATE_HEIGHT = 40.0f;
inline constexpr float UPPER_GATE_HEIGHT = 51.0f;
inline constexpr float GATE_WIDTH = 6.0f;
inline constexpr float GATE_TRAVEL_Y = 12.0f;
inline constexpr float GATE_TRAVEL_SECONDS = 0.48f;

inline constexpr float CRATE_SHELF_Y = 17.0f;
inline constexpr float UPPER_CRATE_X = 30.0f;
inline constexpr float LOWER_CRATE_X = 7.0f;
inline constexpr float CRATE_SIZE = 6.0f;

inline constexpr float GHOST_START_X = -6.0f;
inline constexpr float GHOST_END_X = 2.0f;
inline constexpr float GHOST_Y = -39.5f;
inline constexpr float GHOST_SIZE = 7.0f;
inline constexpr float GHOST_SPEED = 10.0f;

inline constexpr PlatformBounds PLATFORMS[] = {
    // Balconata comune e divisorio che termina dopo le ultime due paratie.
    {-ROOM_HALF_WIDTH, -84.0f, UPPER_FLOOR_Y, 5.0f},
    {-66.0f, 83.0f, UPPER_FLOOR_Y, 15.0f},
    // Se scendono entrambi, questi gradini permettono di tornare alla balconata.
    {-ROOM_HALF_WIDTH, -92.0f, -29.0f, 4.0f},
    {-88.0f, -81.0f, -11.0f, 4.0f},
    // Ripiano della cassa alta e tunnel basso da percorrere accucciati.
    {18.0f, 38.0f, CRATE_SHELF_Y, 4.0f},
    {15.0f, 29.0f, -16.0f, 25.6f}
};

inline constexpr CrystalPosition CRYSTALS[] = {
    {-78.0f, 17.0f}, {22.0f, 26.0f}, {62.0f, 17.0f},
    {-68.0f, -39.0f}, {-15.0f, -39.0f}, {62.0f, -39.0f}
};

} // namespace LandingModule

} // namespace LevelConfig

namespace TutorialConfig {

inline constexpr float ROOM_HALF_WIDTH = 100.0f;
inline constexpr float ROOM_HALF_HEIGHT = 60.0f;
inline constexpr float FLOOR_Y = GameConfig::DEFAULT_FLOOR_Y;
inline constexpr float PLAYER_1_SPAWN_X = -91.0f;
inline constexpr float PLAYER_2_SPAWN_X = -77.0f;
inline constexpr float EXIT_X = 91.0f;

inline constexpr float PRACTICE_PLATFORM_X = -60.0f;
inline constexpr float PRACTICE_PLATFORM_WIDTH = 12.0f;
inline constexpr float PRACTICE_PLATFORM_HEIGHT = 7.0f;
inline constexpr float CRYSTAL_HEIGHT = 14.0f;
inline constexpr float LOW_PASSAGE_X = 26.0f;
inline constexpr float LOW_PASSAGE_WIDTH = 14.0f;
inline constexpr float LOW_PASSAGE_CLEARANCE = 4.5f;
inline constexpr float LOW_PASSAGE_THICKNESS = 4.0f;

inline constexpr float CRATE_X = -35.0f;
inline constexpr float CRATE_SIZE = 6.0f;
inline constexpr float PLATE_X = -24.0f;
inline constexpr float PLATE_WIDTH = 10.0f;
inline constexpr float PLATE_HEIGHT = GameConfig::CRATE_MAX_STEP_HEIGHT;
inline constexpr float PLATE_PRESS_DISTANCE = 0.3f;
inline constexpr float WALL_X = -5.0f;
inline constexpr float WALL_WIDTH = 7.0f;
inline constexpr float WALL_HEIGHT = 16.0f;
inline constexpr float WALL_CLEARANCE = 12.0f;
inline constexpr float WALL_TRAVEL_SECONDS = 0.6f;

inline constexpr float GHOST_START_X = 55.0f;
inline constexpr float GHOST_END_X = 75.0f;
inline constexpr float GHOST_CENTER_Y = FLOOR_Y + 6.0f;
inline constexpr float GHOST_SIZE = 8.0f;
inline constexpr float GHOST_SPEED = 14.0f;

} // namespace TutorialConfig
