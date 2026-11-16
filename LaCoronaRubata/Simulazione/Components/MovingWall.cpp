#include "MovingWall.h"
#include <algorithm>

MovingWall::MovingWall(const glm::vec2& closedCenter, const glm::vec2& openCenter,
                       const glm::vec2& size, float travelSeconds)
    : m_closedCenter(closedCenter),
      m_openCenter(openCenter),
      m_size(size),
      m_travelSeconds(travelSeconds)
{
}

glm::vec2 MovingWall::position() const
{
    return glm::mix(m_closedCenter, m_openCenter, m_progress);
}

SolidCollider MovingWall::collider() const
{
    return {position(), m_size};
}

void MovingWall::update(float dt, bool activated)
{
    if (dt <= 0.0f) {
        return;
    }

    const float step = m_travelSeconds > 0.0f ? dt / m_travelSeconds : 1.0f;
    m_progress = std::clamp(m_progress + (activated ? step : -step), 0.0f, 1.0f);
}

void MovingWall::reset()
{
    m_progress = 0.0f;
}
