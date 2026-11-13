#include "Game.h"
#include <iostream>

Game::Game(Scene& scene) : m_level(scene)
{
}

void Game::update(float dt, float halfWidth, const PlayerInput& input1,
                  const PlayerInput& input2, float currentTime)
{
    m_level.update(dt, halfWidth, input1, input2, currentTime);

    if (m_level.isFinished()) {
        std::cout << "Entrambi i giocatori sono alla porta\n"; // Breakpoint qui
    }
}

void Game::reset()
{
    m_level.reset();
}

void Game::resize(float halfWidth)
{
    m_level.resize(halfWidth);
}
