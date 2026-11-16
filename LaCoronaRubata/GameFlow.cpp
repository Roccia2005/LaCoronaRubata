#include "GameFlow.h"
#include "LevelCatalog.h"
#include <iterator>

namespace {

constexpr DialogueLine intro[] = {
    {"Piccolo Principe", "Devo tornare sul mio pianeta. La Rosa mi aspetta."},
    {"Volpe", u8"Il ponte \u00e8 rotto. Possiamo prendere una navetta alla stazione."},
    {"Piccolo Principe", "Mi accompagni?"},
    {"Volpe", u8"Volentieri. \u00c8 da un po\u2019 che voglio conoscerla."}
};

constexpr DialogueLine finale[] = {
    {"Volpe", u8"\u00c8 lei la Rosa di cui mi parlavi?"},
    {"Piccolo Principe", u8"S\u00ec. Le ho parlato anche di te."},
    {"Volpe", u8"Ah s\u00ec? E che cosa le hai detto?"},
    {"Piccolo Principe", "Che un giorno saremmo venuti a trovarla insieme."}
};

} // namespace

void GameFlow::update(float dt, float timeSeconds, const PlayerInput& first,
                      const PlayerInput& second, UiAction action)
{
    switch (m_screen) {
    case Screen::MainMenu:
        if (action == UiAction::StartStory) {
            m_dialogueIndex = 0;
            m_screen = Screen::IntroDialogue;
        } else if (action == UiAction::StartTutorial) {
            m_tutorial.emplace(Levels::tutorial());
            m_worldTime = timeSeconds;
            m_screen = Screen::Tutorial;
        }
        break;

    case Screen::IntroDialogue:
        if (action == UiAction::AdvanceDialogue && ++m_dialogueIndex == std::size(intro)) {
            m_story.emplace();
            m_worldTime = timeSeconds;
            m_screen = Screen::Story;
        }
        break;

    case Screen::Story: {
        m_worldTime = timeSeconds;
        if (action == UiAction::RestartRoom) {
            m_story->reset();
            break;
        }

        m_story->update(dt, first, second, m_worldTime);
        if (m_story->finished()) {
            m_dialogueIndex = 0;
            m_screen = Screen::FinalDialogue;
        }
        break;
    }

    case Screen::Tutorial:
        if (action == UiAction::BackToMenu) {
            m_tutorial.reset();
            m_screen = Screen::MainMenu;
        } else {
            m_worldTime = timeSeconds;
            if (action == UiAction::RestartRoom) {
                m_tutorial->reset();
            } else {
                m_tutorial->update(dt, first, second, m_worldTime);
            }
        }
        break;

    case Screen::FinalDialogue:
        if (action == UiAction::AdvanceDialogue && ++m_dialogueIndex == std::size(finale)) {
            m_story.reset();
            m_screen = Screen::MainMenu;
        }
        break;
    }
}

const DialogueLine* GameFlow::dialogueLine() const
{
    if (m_screen == Screen::IntroDialogue) {
        return &intro[m_dialogueIndex];
    }
    if (m_screen == Screen::FinalDialogue) {
        return &finale[m_dialogueIndex];
    }
    return nullptr;
}

const Level* GameFlow::levelToDraw() const
{
    if (m_story) {
        return &m_story->level();
    }
    if (m_tutorial) {
        return &*m_tutorial;
    }
    return nullptr;
}

std::size_t GameFlow::levelNumber() const
{
    return m_story ? m_story->levelNumber() : 0;
}
