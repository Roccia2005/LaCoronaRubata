#include "PressurePlate.h"
#include "LevelDefinition.h"

/// Crea una pedana rilasciata con la posizione, le dimensioni e la corsa definite nella mappa.
PressurePlate::PressurePlate(const PressurePlateDefinition& definition)
    : m_basePosition(definition.basePosition),
      m_size(definition.width, definition.height),
      m_pressDistance(definition.pressDistance)
{
}

/// Restituisce il rettangolo di collisione corrispondente allo stato attuale della pedana.
SolidCollider PressurePlate::collider() const
{
    return colliderFor(m_pressed);
}

/// Calcola il rettangolo che la pedana occuperebbe da rilasciata. Level lo usa per verificare
/// se c'e' spazio per la risalita prima di modificare lo stato.
SolidCollider PressurePlate::releasedCollider() const
{
    return colliderFor(false);
}

/// Abbassa la pedana se un giocatore e' appoggiato. Quando l'appoggio termina, la rialza se
/// releaseBlocked e' false. Restituisce lo spostamento verticale della superficie in unita'
/// mondo, negativo in discesa, positivo in salita e zero se la quota resta invariata.
float PressurePlate::update(bool hasSupport, bool releaseBlocked)
{
    if (hasSupport == m_pressed || (!hasSupport && releaseBlocked)) {
        return 0.0f;
    }

    const float previousSurfaceY = surfaceY(m_pressed);
    m_pressed = hasSupport;
    return surfaceY(m_pressed) - previousSurfaceY;
}

/// Ripristina lo stato rilasciato e il relativo collider durante il reset del livello.
void PressurePlate::reset()
{
    m_pressed = false;
}

/// Calcola la quota Y della superficie in coordinate mondo per lo stato indicato da pressed.
float PressurePlate::surfaceY(bool pressed) const
{
    return m_basePosition.y + m_size.y - (pressed ? m_pressDistance : 0.0f);
}

/// Costruisce il collider per lo stato richiesto, dalla base fissa alla superficie di appoggio.
/// Restituisce un SolidCollider con centro e dimensioni complete.
SolidCollider PressurePlate::colliderFor(bool pressed) const
{
    const float height = surfaceY(pressed) - m_basePosition.y;
    return {
        m_basePosition + glm::vec2{0.0f, height * 0.5f},
        {m_size.x, height}
    };
}
