#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;
enum class UiAction;
struct DialogueLine;

// ============================================================================
// Gui
// Incapsula Dear ImGui e i parametri modificabili dall'utente.
// ============================================================================
class Gui
{
public:
    void initialize(GLFWwindow* window);
    void beginFrame();
    void draw();
    /// Disegna il menu e restituisce l'azione selezionata.
    UiAction drawMenu();
    /// Mostra una battuta e rileva una nuova pressione di Invio o di Continua.
    UiAction drawDialogue(const DialogueLine& line);
    /// Mostra i comandi mentre i due giocatori esplorano la stanza di prova.
    void drawTutorial();
    void render();
    void shutdown();

    const glm::vec4& clearColor() const
    {
        return m_clearColor;
    }

    bool wireframe() const
    {
        return m_wireframe;
    }

private:
    bool m_tutorialSelected = false;
    glm::vec4 m_clearColor = glm::vec4(0.0f, 0.2f, 1.0f, 1.0f);
    bool m_wireframe = false;
};
