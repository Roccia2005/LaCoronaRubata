#pragma once
#include "LevelTypes.h"

struct GhostDefinition;

class Ghost {
public:
    /// Inizializza il fantasma al punto di partenza della pattuglia.
    explicit Ghost(const GhostDefinition& definition);

    /// Avanza di dt secondi lungo il percorso e inverte il moto agli estremi.
    void update(float dt);

    /// Restituisce centro e semidimensioni del fantasma in coordinate mondo.
    AABB bounds() const;

    /// Riporta il fantasma alla posizione iniziale e ripristina la direzione di partenza.
    void reset();

private:
    glm::vec2 m_startCenter;
    float m_patrolEndX;
    glm::vec2 m_size;
    float m_speed;
    // Fase del ciclo, da 0 a 1 all'andata e da 1 a 2 al ritorno.
    double m_patrolPhase = 0.0;
};
