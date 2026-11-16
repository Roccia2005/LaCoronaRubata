#pragma once
#include "LevelTypes.h"

class ExitDoor {
public:
    explicit ExitDoor(const glm::vec2& basePosition);

    /// Restituisce l'area di uscita in coordinate mondo.
    const AABB& area() const { return m_area; }
    /// Restituisce la frazione di apertura, da 0 (chiusa) a 1 (completamente aperta).
    float opening() const { return m_opening; }
    /// Restituisce true quando la porta ha raggiunto l'apertura completa.
    bool isOpen() const { return m_opening >= 1.0f; }
    bool overlaps(const AABB& body) const;

    void update(float dt, bool occupied);
    void reset();

private:
    AABB m_area;
    float m_opening = 0.0f;
};
