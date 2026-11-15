#include "Game.h"
#include "LevelCatalog.h"

/// Crea la partita inizializzando lo stato della prima stanza dalle definizioni dei livelli.
Game::Game() : m_level(Levels::get(0))
{
}

/// Fa avanzare la stanza di dt secondi con le azioni dei due giocatori. Se raggiungono l'uscita,
/// carica la stanza successiva oppure segna la partita come conclusa.
void Game::update(float dt, const PlayerInput& input1, const PlayerInput& input2,
                  float currentTime)
{
    if (m_finished) {
        return;
    }
    m_level.update(dt, input1, input2, currentTime);

    if (!m_level.isFinished()) {
        return;
    }
    const auto nextIndex = m_levelIndex + 1;
    if (nextIndex == Levels::count()) {
        m_finished = true;
        return;
    }

    m_level = Level(Levels::get(nextIndex));
    m_levelIndex = nextIndex;
}

/// Ripristina lo stato iniziale della stanza corrente e riattiva la partita anche dopo il
/// completamento.
void Game::reset()
{
    m_level.reset();
    m_finished = false;
}
