#pragma once
#include <cstddef>
#include <initializer_list>

namespace GameConfig {

inline constexpr char GAME_TITLE[] = "Il Piccolo Principe e la Volpe";
inline constexpr float DEFAULT_FLOOR_Y = -48.0f;
inline constexpr float PLAYER_WIDTH = 6.0f;
inline constexpr float PLAYER_HITBOX_WIDTH = 5.0f;
inline constexpr float PLAYER_HEIGHT = 7.0f;
inline constexpr float PLAYER_RENDER_SCALE = 1.08f;
inline constexpr float MOVE_SPEED = 43.0f;
inline constexpr float JUMP_SPEED = 75.0f; // salto sufficiente per la prima piattaforma
inline constexpr float GRAVITY = -135.0f;
inline constexpr float CRATE_MAX_STEP_HEIGHT = 1.5f;
inline constexpr float STAR_RADIUS = 3.0f;
inline constexpr float STAR_BOB_AMPLITUDE = 2.0f; // unita del mondo
inline constexpr float STAR_BOB_FREQUENCY = 3.0f; // radianti al secondo
inline constexpr float STAR_PHASE_STEP = 0.8f;
inline constexpr float STAR_ROTATION_SPEED = 1.25f; // radianti al secondo

inline constexpr float CROUCH_HEIGHT_RATIO = 0.5f;
inline constexpr float MAX_FRAME_DELTA_SECONDS = 0.03f;
inline constexpr float DOOR_WIDTH = 12.0f;
inline constexpr float DOOR_HEIGHT = 16.0f;
inline constexpr float DOOR_OPENING_SECONDS = 0.35f;
inline constexpr float FLOOR_THICKNESS = 6.0f;
inline constexpr int POINTS_PER_STAR = 10;

} // namespace GameConfig

namespace LevelConfig {

struct PlatformBounds {
    float left;
    float right;
    float top;
    float thickness;
};

struct StarPosition {
    float x;
    float y;
};

namespace Level1 {

inline constexpr float ROOM_HALF_WIDTH = 120.0f;
inline constexpr float ROOM_HALF_HEIGHT = 76.0f;
inline constexpr float FLOOR_Y = GameConfig::DEFAULT_FLOOR_Y;
// Il percorso principale slitta a sinistra per lasciare spazio alla salita bonus.
inline constexpr float MAIN_ROUTE_X_OFFSET = -20.0f;

inline constexpr float PLAYER_1_SPAWN_X = -92.0f + MAIN_ROUTE_X_OFFSET;
inline constexpr float PLAYER_1_SPAWN_FEET_Y = FLOOR_Y;
inline constexpr float PLAYER_2_SPAWN_X = -82.0f + MAIN_ROUTE_X_OFFSET;
inline constexpr float PLAYER_2_SPAWN_FEET_Y = FLOOR_Y;

// Quote condivise con pedane, fantasma e uscita.
inline constexpr float PLATFORM_3_TOP_Y = -10.0f;
inline constexpr float PLATFORM_4_TOP_Y = 5.0f;
inline constexpr float PLATFORM_5_TOP_Y = 22.0f;

inline constexpr float DOOR_X = 71.0f + MAIN_ROUTE_X_OFFSET;
inline constexpr float DOOR_BASE_Y = PLATFORM_5_TOP_Y;

inline constexpr float UPPER_WALL_1_HEIGHT = 14.0f;
inline constexpr float UPPER_WALL_2_HEIGHT = 25.0f;
inline constexpr float UPPER_WALL_3_X = 29.0f + MAIN_ROUTE_X_OFFSET;
inline constexpr float UPPER_WALL_3_HEIGHT = 36.0f;
inline constexpr float UPPER_CRATE_X = 2.0f + MAIN_ROUTE_X_OFFSET;
inline constexpr float UPPER_CRATE_WIDTH = 8.0f;
inline constexpr float UPPER_CRATE_HEIGHT = 6.0f;

// Il muro separa la salita bonus dal piano dell'uscita e lascia passare la cassa al suolo.
inline constexpr float BONUS_WALL_RIGHT_X = 86.0f;
inline constexpr float BONUS_WALL_WIDTH = 10.0f;
inline constexpr float BONUS_WALL_BASE_Y = -24.0f;
inline constexpr float BONUS_WALL_TOP_Y = 100.0f;
inline constexpr float BONUS_STEP_WIDTH = 10.0f;
inline constexpr float BONUS_STEP_THICKNESS = 3.0f;
// Dal pavimento al primo gradino servono l'altezza della cassa e quella del compagno.
inline constexpr float BONUS_FIRST_STEP_TOP_Y = FLOOR_Y + 35.0f;
inline constexpr float BONUS_STEP_RISE = 13.0f;

constexpr PlatformBounds bonusStep(std::size_t step)
{
    const float left = step % 2 == 0 ? BONUS_WALL_RIGHT_X : ROOM_HALF_WIDTH - BONUS_STEP_WIDTH;
    const float top = BONUS_FIRST_STEP_TOP_Y + static_cast<float>(step) * BONUS_STEP_RISE;
    return {left, left + BONUS_STEP_WIDTH, top, BONUS_STEP_THICKNESS};
}

inline constexpr PlatformBounds BONUS_LAST_STEP = bonusStep(5);

inline constexpr PlatformBounds PLATFORMS[] = {
    // Due blocchi sospesi conducono alla passerella del fantasma.
    {-25.0f + MAIN_ROUTE_X_OFFSET, -5.0f + MAIN_ROUTE_X_OFFSET, -19.0f, 6.0f},
    {-2.0f + MAIN_ROUTE_X_OFFSET, 16.0f + MAIN_ROUTE_X_OFFSET, -17.0f, 7.0f},
    {21.0f + MAIN_ROUTE_X_OFFSET, 96.0f + MAIN_ROUTE_X_OFFSET, PLATFORM_3_TOP_Y, 12.0f},
    // Ripiano della pedana B e piano superiore con l'uscita.
    {-100.0f + MAIN_ROUTE_X_OFFSET, -66.0f + MAIN_ROUTE_X_OFFSET, PLATFORM_4_TOP_Y, 6.0f},
    {-47.0f + MAIN_ROUTE_X_OFFSET, 100.0f + MAIN_ROUTE_X_OFFSET, PLATFORM_5_TOP_Y, 5.0f},
    // I tre muri del piano superiore si affrontano in successione.
    {-30.0f + MAIN_ROUTE_X_OFFSET, -22.0f + MAIN_ROUTE_X_OFFSET,
     PLATFORM_5_TOP_Y + UPPER_WALL_1_HEIGHT, UPPER_WALL_1_HEIGHT},
    {-14.0f + MAIN_ROUTE_X_OFFSET, -4.0f + MAIN_ROUTE_X_OFFSET,
     PLATFORM_5_TOP_Y + UPPER_WALL_2_HEIGHT, UPPER_WALL_2_HEIGHT},
    {UPPER_WALL_3_X - 5.0f, UPPER_WALL_3_X + 5.0f,
     PLATFORM_5_TOP_Y + UPPER_WALL_3_HEIGHT, UPPER_WALL_3_HEIGHT},
    {BONUS_WALL_RIGHT_X - BONUS_WALL_WIDTH, BONUS_WALL_RIGHT_X,
     BONUS_WALL_TOP_Y, BONUS_WALL_TOP_Y - BONUS_WALL_BASE_Y},
    // I gradini alternano il lato a ogni salita.
    bonusStep(0), bonusStep(1), bonusStep(2), bonusStep(3), bonusStep(4), BONUS_LAST_STEP
};

// Stelle: posizione del centro a riposo.
inline constexpr StarPosition STARS[] = {
    {18.0f + MAIN_ROUTE_X_OFFSET, -40.0f},
    {-17.0f + MAIN_ROUTE_X_OFFSET, -11.0f},
    {9.0f + MAIN_ROUTE_X_OFFSET, -10.0f},
    {63.0f + MAIN_ROUTE_X_OFFSET, -2.0f},
    {UPPER_WALL_3_X, PLATFORM_5_TOP_Y + UPPER_WALL_3_HEIGHT + 7.0f},
    {16.0f + MAIN_ROUTE_X_OFFSET, PLATFORM_5_TOP_Y + 5.0f},
    {(BONUS_LAST_STEP.left + BONUS_LAST_STEP.right) * 0.5f, BONUS_LAST_STEP.top + 7.0f}
};

// A e' all'estremita' della pattuglia, B sul ripiano sinistro. Basta tenerne premuta una.
inline constexpr float PRESSURE_PLATE_A_BASE_X = 86.0f + MAIN_ROUTE_X_OFFSET;
inline constexpr float PRESSURE_PLATE_A_BASE_Y = PLATFORM_3_TOP_Y;
inline constexpr float PRESSURE_PLATE_B_BASE_X = -87.0f + MAIN_ROUTE_X_OFFSET;
inline constexpr float PRESSURE_PLATE_B_BASE_Y = PLATFORM_4_TOP_Y;
inline constexpr float PRESSURE_PLATE_WIDTH = 8.0f;
inline constexpr float PRESSURE_PLATE_HEIGHT = 1.2f;
inline constexpr float PRESSURE_PLATE_PRESS_DISTANCE = 0.3f;

// La parete scorre verso destra e diventa un gradino per raggiungere B.
inline constexpr float MOVING_WALL_CLOSED_CENTER_X = -90.0f + MAIN_ROUTE_X_OFFSET;
inline constexpr float MOVING_WALL_OPEN_CENTER_X = -54.0f + MAIN_ROUTE_X_OFFSET;
inline constexpr float MOVING_WALL_CENTER_Y = -13.0f;
inline constexpr float MOVING_WALL_WIDTH = 20.0f;
inline constexpr float MOVING_WALL_HEIGHT = 6.0f;
inline constexpr float MOVING_WALL_TRAVEL_SECONDS = 0.7f;
inline constexpr std::initializer_list<std::size_t> MOVING_WALL_PRESSURE_PLATE_INDICES{0, 1};

inline constexpr float GHOST_START_X = 36.0f + MAIN_ROUTE_X_OFFSET;
// La pattuglia raggiunge anche il bordo destro della pedana A.
inline constexpr float GHOST_END_X = PRESSURE_PLATE_A_BASE_X + PRESSURE_PLATE_WIDTH * 0.5f;
inline constexpr float GHOST_CENTER_Y = PLATFORM_3_TOP_Y + 6.0f;
inline constexpr float GHOST_WIDTH = 7.0f;
inline constexpr float GHOST_HEIGHT = 7.0f;
inline constexpr float GHOST_SPEED = 10.0f;

// La cassa parte a destra e si spinge a sinistra per salire sul primo blocco.
inline constexpr float CRATE_BASE_X = 65.0f + MAIN_ROUTE_X_OFFSET;
inline constexpr float CRATE_BASE_Y = FLOOR_Y;
inline constexpr float CRATE_WIDTH = 8.0f;
inline constexpr float CRATE_HEIGHT = 11.0f;

} // namespace Level1

namespace Level2 {

inline constexpr float ROOM_HALF_WIDTH = 100.0f;
inline constexpr float ROOM_HALF_HEIGHT = 60.0f;
inline constexpr float FLOOR_Y = GameConfig::DEFAULT_FLOOR_Y;
inline constexpr float SPAWN_TOP_Y = 29.0f;
inline constexpr float PLAYER_1_SPAWN_X = -83.0f;
inline constexpr float PLAYER_2_SPAWN_X = -70.0f;
inline constexpr float EXIT_X = 89.0f;
inline constexpr float EXIT_FLOOR_Y = 36.0f;

inline constexpr float PLATE_A_X = -74.0f;
inline constexpr float PLATE_A_FLOOR_Y = -28.0f;
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
    // Il ripiano sale per mantenere la stessa altezza libera nel corridoio sottostante.
    {-87.0f, SHAFT_WALL_LEFT_X, PLATE_A_FLOOR_Y, 3.0f},
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

inline constexpr StarPosition STARS[] = {
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

} // namespace Level2

namespace Level3 {

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

inline constexpr StarPosition STARS[] = {
    {-78.0f, 17.0f}, {22.0f, 26.0f}, {62.0f, 17.0f},
    {-68.0f, -39.0f}, {-15.0f, -39.0f}, {62.0f, -39.0f}
};

} // namespace Level3

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
inline constexpr float STAR_HEIGHT = 14.0f;
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
