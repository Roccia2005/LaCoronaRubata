#pragma once
#include "Game.h"
#include <cstddef>
#include <optional>
#include <string_view>

enum class Screen {
    MainMenu,
    IntroDialogue,
    Story,
    Tutorial,
    FinalDialogue
};

enum class UiAction {
    None,
    StartStory,
    StartTutorial,
    AdvanceDialogue,
    BackToMenu,
    RestartRoom
};

enum class DialogueSpeaker { Prince, Fox };
const char* speakerName(DialogueSpeaker speaker);

struct DialogueLine {
    DialogueSpeaker speaker;
    std::string_view text;
};

class GameFlow {
public:
    /// Consuma un'azione e aggiorna soltanto la schermata attiva.
    void update(float dt, float timeSeconds, const PlayerInput& first,
                const PlayerInput& second, UiAction action);

    Screen screen() const { return m_screen; }

    /// Restituisce la battuta da mostrare, se c'e' un dialogo in corso.
    const DialogueLine* dialogueLine() const;
    /// Espone la stanza da disegnare, compresa l'ultima stanza durante l'epilogo.
    const Level* levelToDraw() const;

    /// Numero della stanza restituita da levelToDraw(). Il tutorial vale 0.
    std::size_t levelNumber() const;
    /// Tempo usato dalla simulazione e dall'animazione delle stelle della stanza.
    float worldTime() const { return m_worldTime; }

private:
    Screen m_screen = Screen::MainMenu;
    std::optional<Game> m_story;
    std::optional<Level> m_tutorial;
    std::size_t m_dialogueIndex = 0;
    float m_worldTime = 0.0f;
};
