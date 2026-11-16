#include "Crate.h"
#include "LevelDefinition.h"
#include "PlayerPhysics.h"
#include <algorithm>
#include <cmath>

namespace {
constexpr float contactTolerance = 0.001f;

/// Limita lo spostamento sul primo lato incontrato, lasciando libero l'appoggio ai piedi.
float horizontalLimit(const AABB& body, float distance, const AABB& obstacle)
{
    if (std::abs(body.center.y - obstacle.center.y) >=
        body.halfSize.y + obstacle.halfSize.y - contactTolerance) {
        return distance;
    }

    const float direction = distance > 0.0f ? 1.0f : -1.0f;
    const float gap = (obstacle.center.x - body.center.x) * direction
                    - body.halfSize.x - obstacle.halfSize.x;
    if (gap < -contactTolerance) {
        return distance;
    }
    return direction * std::min(std::abs(distance), std::max(gap, 0.0f));
}
}

Crate::Crate(const CrateDefinition& definition)
    : m_spawnCenter(definition.basePosition + glm::vec2{0.0f, definition.size.y * 0.5f}),
      m_position(m_spawnCenter),
      m_size(definition.size)
{
}

SolidCollider Crate::collider() const
{
    return {m_position, m_size};
}

AABB Crate::bounds() const
{
    return makeAABB(m_position, m_size);
}

void Crate::fall(float dt, float floorY, const std::vector<SolidCollider>& solids,
                 std::size_t ownSolidIndex)
{
    if (dt <= 0.0f) {
        return;
    }
    const AABB box = bounds();
    const float bottom = box.center.y - box.halfSize.y;
    float supportY = floorY;
    for (std::size_t i = 0; i < solids.size(); ++i) {
        if (i == ownSolidIndex) {
            continue;
        }
        const AABB solid = makeAABB(solids[i].position, solids[i].size);
        const float top = solid.center.y + solid.halfSize.y;
        if (top <= bottom + contactTolerance &&
            std::abs(box.center.x - solid.center.x) < box.halfSize.x + solid.halfSize.x) {
            supportY = std::max(supportY, top);
        }
    }

    m_velocityY += GameConfig::GRAVITY * dt;
    const float nextBottom = bottom + m_velocityY * dt;
    m_onGround = nextBottom <= supportY;
    m_position.y = std::max(nextBottom, supportY) + box.halfSize.y;
    if (m_onGround) {
        m_velocityY = 0.0f;
    }
}

void Crate::push(float distance, float roomHalfWidth, const std::vector<SolidCollider>& solids,
                 std::size_t ownSolidIndex, const AABB& otherPlayer)
{
    const AABB box = bounds();
    const float limit = roomHalfWidth - box.halfSize.x;
    const float target = std::clamp(m_position.x + distance, -limit, limit);
    const float requested = target - m_position.x;
    const auto distanceAt = [&](const AABB& body) {
        float allowed = horizontalLimit(body, requested, otherPlayer);
        for (std::size_t i = 0; i < solids.size(); ++i) {
            if (i != ownSolidIndex) {
                allowed = horizontalLimit(body, allowed, makeAABB(solids[i].position, solids[i].size));
            }
        }
        return allowed;
    };

    float allowed = distanceAt(box);
    if (m_onGround && std::abs(allowed) < std::abs(requested)) {
        // Una cassa appoggiata puo' superare il rialzo di una pedana se ha spazio sopra.
        const float step = GameConfig::CRATE_MAX_STEP_HEIGHT;
        const AABB headroom = makeAABB(
            box.center + glm::vec2{0.0f, box.halfSize.y + step * 0.5f}, {m_size.x, step});
        bool blockedAbove = PlayerPhysics::intersects(headroom, otherPlayer);
        for (std::size_t i = 0; i < solids.size() && !blockedAbove; ++i) {
            if (i != ownSolidIndex) {
                blockedAbove = PlayerPhysics::intersects(
                    headroom, makeAABB(solids[i].position, solids[i].size));
            }
        }
        if (!blockedAbove) {
            AABB raised = box;
            raised.center.y += step;
            const float raisedDistance = distanceAt(raised);
            if (std::abs(raisedDistance) > std::abs(allowed)) {
                m_position.y = raised.center.y;
                allowed = raisedDistance;
                m_onGround = false;
            }
        }
    }
    m_position.x += allowed;
}

void Crate::moveWithSupport(float deltaY)
{
    m_position.y += deltaY;
}

void Crate::reset()
{
    m_position = m_spawnCenter;
    m_velocityY = 0.0f;
    m_onGround = false;
}
