#include "ExitDoor.h"
#include "PlayerPhysics.h"
#include <algorithm>

/// Crea una porta chiusa. Costruisce l'area di uscita dal centro del bordo inferiore e dalle
/// dimensioni della porta in GameConfig.
ExitDoor::ExitDoor(const glm::vec2& basePosition)
{
    constexpr glm::vec2 size{GameConfig::DOOR_WIDTH, GameConfig::DOOR_HEIGHT};
    const glm::vec2 center = basePosition + glm::vec2{0.0f, size.y * 0.5f};
    m_area = makeAABB(center, size);
}

/// Verifica se il rettangolo di body si sovrappone all'area di uscita. I bordi a contatto
/// delimitano rettangoli senza area in comune.
bool ExitDoor::overlaps(const AABB& body) const
{
    return PlayerPhysics::intersects(body, m_area);
}

/// Fa avanzare l'apertura di dt secondi se l'area e' occupata, altrimenti fa avanzare la
/// chiusura. Una transizione completa dura GameConfig::DOOR_OPENING_SECONDS.
void ExitDoor::update(float dt, bool occupied)
{
    const float step = dt / GameConfig::DOOR_OPENING_SECONDS;
    m_opening = std::clamp(m_opening + (occupied ? step : -step), 0.0f, 1.0f);
}

/// Riporta subito la porta allo stato chiuso.
void ExitDoor::reset()
{
    m_opening = 0.0f;
}
