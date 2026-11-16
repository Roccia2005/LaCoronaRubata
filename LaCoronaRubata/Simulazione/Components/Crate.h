#pragma once
#include "LevelTypes.h"
#include <cstddef>
#include <vector>

struct CrateDefinition;

class Crate {
public:
    /// Posiziona la cassa alla quota di base definita dalla mappa.
    explicit Crate(const CrateDefinition& definition);

    /// Restituisce il solido corrente, con centro e dimensioni complete.
    SolidCollider collider() const;
    /// Restituisce il rettangolo di collisione usato per pedane e appoggi.
    AABB bounds() const;

    /// Applica la gravita' e ferma la discesa sulla prima superficie attraversata.
    void fall(float dt, float floorY, const std::vector<SolidCollider>& solids,
              std::size_t ownSolidIndex);
    /// Applica la spinta orizzontale consentita da solidi, confini e altro giocatore.
    void push(float distance, float roomHalfWidth, const std::vector<SolidCollider>& solids,
              std::size_t ownSolidIndex, const AABB& otherPlayer);
    /// Segue lo spostamento verticale della superficie di una pedana.
    void moveWithSupport(float deltaY);
    /// Ripristina posizione e velocita' iniziali.
    void reset();

private:
    glm::vec2 m_spawnCenter;
    glm::vec2 m_position;
    glm::vec2 m_size;
    float m_velocityY = 0.0f;
    bool m_onGround = false;
};
