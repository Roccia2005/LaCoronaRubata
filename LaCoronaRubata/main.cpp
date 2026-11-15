// Platformer 2D - main minimale: GLFW/GLAD, input, update e rendering.
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "LevelView.h"
#include "Shader.h"
#include "Renderer.h"
#include "BackgroundRenderer.h"
#include "CoinRenderer.h"
#include "Game.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

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

struct ShaderSources {
    std::string basicVertex;
    std::string basicFragment;
    std::string backgroundVertex;
    std::string backgroundFragment;
    std::string coinVertex;
};

/// Carica i sorgenti GLSL richiesti all'avvio. Il fragment shader di base viene
/// condiviso fra gli oggetti ordinari e i cristalli.
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

    sources.coinVertex = loadTextFile("shaders/coin.vert");
    if (sources.coinVertex.empty()) {
        return std::nullopt;
    }
    return sources;
}

/// Sincronizza la vista con la simulazione, disegna la stanza e presenta il frame.
/// Se il framebuffer non ha area, salta il disegno.
void renderFrame(GLFWwindow* window, const Game& game, LevelView& levelView,
                 Renderer& renderer, const BackgroundRenderer& background,
                 const CoinRenderer& coinRenderer, float timeSeconds)
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    if (width <= 0 || height <= 0) return;

    const Level& level = game.level();
    levelView.sync(level, game.levelNumber());
    renderer.setRoomViewport(width, height, level.roomHalfSize());

    const std::string title = "La Corona Rubata | Livello " +
        std::to_string(game.levelNumber()) + " | Cristalli: " +
        std::to_string(game.score()) +
        (game.finished() ? " | Fine della prova - R reset" :
                           " | W / Freccia su: salto - R reset");
    glfwSetWindowTitle(window, title.c_str());

    glClearColor(0.06f, 0.11f, 0.20f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Ordine: prima il fondale, poi gli oggetti del mondo.
    background.draw(timeSeconds);
    renderer.drawScene(levelView.scene());
    coinRenderer.draw(levelView.scene(), renderer.projectionMatrix(), timeSeconds);
    glfwSwapBuffers(window);
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
        1100, 700, "La Corona Rubata", nullptr, nullptr);
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
    {
        // Shader e renderer per gli oggetti ordinari
        Shader shader(shaderSources->basicVertex, shaderSources->basicFragment);
        Renderer renderer(shader);

        // Shader e render separato per lo sfondo (fullscreen quad in NDC).
        Shader backgroundShader(shaderSources->backgroundVertex, shaderSources->backgroundFragment);
        BackgroundRenderer background(backgroundShader);

        //Shader e render separato per le monete
        Shader coinShader(shaderSources->coinVertex, shaderSources->basicFragment);
        CoinRenderer coinRenderer(coinShader);
        Game game;
        LevelView levelView;

        bool previousReset = false;
        double previousTime = glfwGetTime();

        // 4. Game loop: tempo, input, simulazione, rendering.
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);

            const double now = glfwGetTime();
            // dt e' il tempo dall'ultimo frame, in secondi. Lo limitiamo con Config.h
            // per ridurre il rischio di attraversare un ostacolo in un solo aggiornamento.
            // Il passo resta variabile. L'eventuale tempo oltre il limite viene scartato.
            const float dt = std::clamp(
                static_cast<float>(now - previousTime), 0.0f, GameConfig::MAX_FRAME_DELTA_SECONDS);
            previousTime = now;

            const auto input1 = readPlayerInput(window, firstPlayerKeys);
            const auto input2 = readPlayerInput(window, secondPlayerKeys);

            const bool restart = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;

            // Rileva il passaggio da tasto rilasciato a premuto.
            if (restart && !previousReset) game.reset();
            previousReset = restart;
            game.update(dt, input1, input2, static_cast<float>(now));

            renderFrame(window, game, levelView, renderer, background, coinRenderer,
                        static_cast<float>(now));
        }
    } // Distruzione automatica degli oggetti locali.

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
