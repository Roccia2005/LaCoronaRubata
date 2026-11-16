#pragma once
#include "LevelTypes.h"

struct PressurePlateDefinition;

class PressurePlate {
public:
    explicit PressurePlate(const PressurePlateDefinition& definition);

    /// Restituisce il centro della base della pedana in coordinate mondo.
    const glm::vec2& basePosition() const { return m_basePosition; }
    /// Restituisce la larghezza e l'altezza della pedana a riposo.
    const glm::vec2& size() const { return m_size; }
    /// Restituisce di quanto scende la superficie quando la pedana viene premuta.
    float pressDistance() const { return m_pressDistance; }
    /// Restituisce true quando la pedana e' abbassata.
    bool isPressed() const { return m_pressed; }

    SolidCollider collider() const;
    SolidCollider releasedCollider() const;
    float update(bool hasSupport, bool releaseBlocked);
    void reset();

private:
    glm::vec2 m_basePosition;
    glm::vec2 m_size;
    float m_pressDistance;
    bool m_pressed = false;

    float surfaceY(bool pressed) const;
    SolidCollider colliderFor(bool pressed) const;
};
