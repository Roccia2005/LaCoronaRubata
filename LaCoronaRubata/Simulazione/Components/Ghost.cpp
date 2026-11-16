#include "Ghost.h"
#include "LevelDefinition.h"
#include <cmath>

Ghost::Ghost(const GhostDefinition& definition)
    : m_startCenter(definition.startCenter),
      m_patrolEndX(definition.patrolEndX),
      m_size(definition.size),
      m_speed(definition.speed)
{
}

void Ghost::update(float dt)
{
    const double patrolLength = std::abs(static_cast<double>(m_patrolEndX) - m_startCenter.x);
    if (dt <= 0.0f || patrolLength == 0.0 || m_speed <= 0.0f) {
        return;
    }

    // Conserva il tratto residuo anche quando un aggiornamento oltrepassa un estremo.
    const double advance = static_cast<double>(dt) * m_speed / patrolLength;
    m_patrolPhase = std::fmod(m_patrolPhase + advance, 2.0);
}

AABB Ghost::bounds() const
{
    const float progress = static_cast<float>(1.0 - std::abs(1.0 - m_patrolPhase));
    const float x = glm::mix(m_startCenter.x, m_patrolEndX, progress);
    return makeAABB({x, m_startCenter.y}, m_size);
}

void Ghost::reset()
{
    m_patrolPhase = 0.0;
}
