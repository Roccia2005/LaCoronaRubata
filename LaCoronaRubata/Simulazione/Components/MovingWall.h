#pragma once
#include "LevelTypes.h"

class MovingWall {
public:
    /// Crea la parete in posizione chiusa. I centri e le dimensioni complete sono in
    /// coordinate mondo. travelSeconds indica la durata di una corsa completa.
    MovingWall(const glm::vec2& closedCenter, const glm::vec2& openCenter,
               const glm::vec2& size, float travelSeconds);

    /// Restituisce il centro attuale, interpolato tra le posizioni chiusa e aperta.
    glm::vec2 position() const;

    /// Restituisce il solido nella posizione attuale, con larghezza e altezza complete.
    SolidCollider collider() const;

    /// Avanza di dt secondi verso la posizione aperta se il comando e' attivo, altrimenti
    /// verso quella chiusa. Con durata zero o negativa, il movimento e' immediato.
    /// Un dt zero o negativo lascia la parete nella posizione attuale.
    void update(float dt, bool activated);

    /// Riporta la parete e il suo collider alla posizione chiusa.
    void reset();

private:
    glm::vec2 m_closedCenter;
    glm::vec2 m_openCenter;
    glm::vec2 m_size;
    float m_travelSeconds;
    float m_progress = 0.0f;
};
