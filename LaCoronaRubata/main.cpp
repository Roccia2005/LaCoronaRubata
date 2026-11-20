// Platformer 2D - main minimale: GLFW/GLAD, input, update e rendering.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Config.h"
#include "LevelView.h"
#include "DialogueView.h"
#include "Shader.h"
#include "Renderer.h"
#include "BackgroundRenderer.h"
#include "StarRenderer.h"
#include "GameFlow.h"
#include "Gui.h"
#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <utility>

/// Legge il file di testo usato come sorgente GLSL. Se l'apertura fallisce, scrive il percorso
/// su stderr e restituisce una stringa vuota.
std::string loadTextFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "File shader non trovato: " << path << '\n';
        return "";
    }
    std::ostringstream out;
    out << file.rdbuf();
    return out.str();
}

namespace {

struct PlayerKeys {
    int left;
    int right;
    int jump;
    int down;
};

constexpr PlayerKeys firstPlayerKeys{GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S};
constexpr PlayerKeys secondPlayerKeys{GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN};

/// Legge lo stato dei quattro tasti associati al giocatore, incluso il salto tenuto.
PlayerInput readPlayerInput(GLFWwindow* window, const PlayerKeys& keys)
{
    return {
        glfwGetKey(window, keys.left) == GLFW_PRESS,
        glfwGetKey(window, keys.right) == GLFW_PRESS,
        glfwGetKey(window, keys.jump) == GLFW_PRESS,
        glfwGetKey(window, keys.down) == GLFW_PRESS
    };
}

bool isPlayable(Screen screen)
{
    return screen == Screen::Story || screen == Screen::Tutorial;
}

struct ShaderSources {
    std::string movingWallVertex;
    std::string movingWallFragment;
    std::string basicVertex;
    std::string basicFragment;
    std::string backgroundVertex;
    std::string backgroundFragment;
    std::string starVertex;
};

/// Carica i sorgenti GLSL richiesti all'avvio. Il fragment shader di base viene
/// condiviso fra gli oggetti ordinari e le stelle.
std::optional<ShaderSources> loadShaderSources()
{
    ShaderSources sources;
    sources.basicVertex = loadTextFile("shaders/basic.vert");
    sources.basicFragment = loadTextFile("shaders/basic.frag");
    if (sources.basicVertex.empty() || sources.basicFragment.empty()) {
        return std::nullopt;
    }

    sources.backgroundVertex = loadTextFile("shaders/background.vert");
    sources.backgroundFragment = loadTextFile("shaders/background.frag");
    if (sources.backgroundVertex.empty() || sources.backgroundFragment.empty()) {
        return std::nullopt;
    }

    sources.movingWallVertex = loadTextFile("shaders/moving_wall.vert");
    sources.movingWallFragment = loadTextFile("shaders/moving_wall.frag");
    if (sources.movingWallVertex.empty() || sources.movingWallFragment.empty()) {
        return std::nullopt;
    }

    sources.starVertex = loadTextFile("shaders/star.vert");
    if (sources.starVertex.empty()) {
        return std::nullopt;
    }
    return sources;
}

void drawBackground(const BackgroundRenderer& background, float timeSeconds)
{
    glClearColor(0.06f, 0.11f, 0.20f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    background.draw(timeSeconds);
}

/// Disegna fondale e stanza prima dei pannelli dell'interfaccia.
void renderWorld(int width, int height, const GameFlow& flow, LevelView& levelView,
                 Renderer& renderer, const BackgroundRenderer& background,
                 const StarRenderer& starRenderer, float timeSeconds)
{
    const Level* level = flow.levelToDraw();
    if (level) {
        const Screen screen = flow.screen();
        levelView.sync(*level, flow.levelNumber(), flow.worldTime(), isPlayable(screen));
        renderer.setRoomViewport(width, height, level->roomHalfSize());
    } else {
        glViewport(0, 0, width, height);
    }

    drawBackground(background, timeSeconds);
    if (level) {
        renderer.drawScene(levelView.scene());
        starRenderer.draw(levelView.scene(), renderer.projectionMatrix(), flow.worldTime());
    }
}

/// Mostra nel titolo il livello e il punteggio durante la campagna.
void updateWindowTitle(GLFWwindow* window, const GameFlow& flow)
{
    std::string title = GameConfig::GAME_TITLE;
    if (flow.screen() == Screen::Story) {
        title += " | Livello " + std::to_string(flow.levelNumber()) + " | Punti: "
              + std::to_string(flow.levelToDraw()->score()) + " | R reset";
    } else if (flow.screen() == Screen::Tutorial) {
        title += " | Tutorial";
    }
    glfwSetWindowTitle(window, title.c_str());
}

} // namespace

/// Inizializza finestra, OpenGL e gioco. A ogni frame legge gli input, aggiorna la simulazione e
/// disegna la vista della stanza. Rilascia le risorse GPU prima di distruggere il contesto.
int main()
{
    // 1. Finestra e contesto OpenGL 3.3 Core.
    if (!glfwInit()) {
        std::cerr << "Errore inizializzazione GLFW\n";
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        1100, 700, GameConfig::GAME_TITLE, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // 2. GLAD deve essere caricato dopo la creazione del contesto.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Errore inizializzazione GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    const auto shaderSources = loadShaderSources();
    if (!shaderSources) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Le risorse OpenGL vengono distrutte prima della finestra e del contesto.
    try {
        // Shader e renderer per gli oggetti ordinari
        Shader shader(shaderSources->basicVertex, shaderSources->basicFragment);
        Shader wallShader(shaderSources->movingWallVertex, shaderSources->movingWallFragment);
        Renderer renderer(shader, wallShader);

        // Shader e render separato per lo sfondo (fullscreen quad in NDC).
        Shader backgroundShader(shaderSources->backgroundVertex, shaderSources->backgroundFragment);
        BackgroundRenderer background(backgroundShader);

        // Shader e renderer dedicati alle stelle.
        Shader starShader(shaderSources->starVertex, shaderSources->basicFragment);
        StarRenderer starRenderer(starShader);
        GameFlow flow;
        LevelView levelView;
        DialogueView dialogueView;
        Gui gui;
        gui.initialize(window);

        bool previousReset = false;
        bool previousEscape = false;
        UiAction pendingAction = UiAction::None;
        double previousTime = glfwGetTime();

        // 4. Game loop: tempo, input, simulazione, rendering.
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            const double now = glfwGetTime();
            // dt e' il tempo dall'ultimo frame, in secondi. Lo limitiamo con Config.h
            // per ridurre il rischio di attraversare un ostacolo in un solo aggiornamento.
            // Il passo resta variabile. L'eventuale tempo oltre il limite viene scartato.
            const float dt = std::clamp(
                static_cast<float>(now - previousTime), 0.0f, GameConfig::MAX_FRAME_DELTA_SECONDS);
            previousTime = now;

            const Screen screen = flow.screen();
            const bool playing = isPlayable(screen);
            const bool escape = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
            const bool restart = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;

            // Applica al massimo un'azione per frame. I pulsanti vengono letti dopo il disegno.
            UiAction action = std::exchange(pendingAction, UiAction::None);
            if (escape && !previousEscape) {
                if (screen == Screen::Tutorial) {
                    action = UiAction::BackToMenu;
                } else {
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                    break;
                }
            } else if (playing && restart && !previousReset) {
                action = UiAction::RestartRoom;
            }
            previousEscape = escape;
            previousReset = restart;

            const PlayerInput input1 = playing ? readPlayerInput(window, firstPlayerKeys) : PlayerInput{};
            const PlayerInput input2 = playing ? readPlayerInput(window, secondPlayerKeys) : PlayerInput{};
            flow.update(dt, static_cast<float>(now), input1, input2, action);
            const DialogueLine* line = flow.dialogueLine();
            dialogueView.observeLine(line);
            updateWindowTitle(window, flow);

            int width = 0, height = 0;
            glfwGetFramebufferSize(window, &width, &height);
            if (width <= 0 || height <= 0) {
                continue;
            }

            gui.beginFrame();
            if (line) {
                const auto panel = gui.drawDialogue(*line);
                pendingAction = panel.action;
                const glm::vec2 framebufferSize(width, height);
                renderer.setRoomViewport(width, height, framebufferSize * 0.5f);
                drawBackground(background, static_cast<float>(now));
                if (dialogueView.updatePose(panel, framebufferSize, now)) {
                    renderer.drawScene(dialogueView.scene());
                }
            } else {
                renderWorld(width, height, flow, levelView, renderer, background, starRenderer,
                            static_cast<float>(now));
                if (flow.screen() == Screen::MainMenu) {
                    pendingAction = gui.drawMenu();
                } else if (flow.screen() == Screen::Tutorial) {
                    gui.drawTutorial();
                }
            }
            gui.render();
            glfwSwapBuffers(window);
        }
        gui.shutdown();
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
