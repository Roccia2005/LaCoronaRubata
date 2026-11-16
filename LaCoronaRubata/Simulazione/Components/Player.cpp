#include "Player.h"

/// Riporta il giocatore allo spawn, in piedi e con velocita' nulla.
void Player::reset()
{
    position = spawnPosition;
    velocity = {0.0f, 0.0f};
    onGround = true;
    size.y = GameConfig::PLAYER_HEIGHT;
}

/// Imposta la velocita' orizzontale dai tasti premuti e avvia il salto se c'e' un appoggio.
/// Il salto libera l'appoggio prima che venga valutata la postura richiesta.
void Player::applyInput(const PlayerInput& input)
{
    velocity.x = (static_cast<float>(input.right) - static_cast<float>(input.left))
               * GameConfig::MOVE_SPEED;
    if (input.jumpHeld && onGround) {
        velocity.y = GameConfig::JUMP_SPEED;
        onGround = false;
    }
}

/// Restituisce l'altezza richiesta quando il giocatore si abbassa da appoggiato o torna in piedi.
float Player::requestedHeight(const PlayerInput& input) const
{
    return input.down && onGround
        ? GameConfig::PLAYER_HEIGHT * GameConfig::CROUCH_HEIGHT_RATIO
        : GameConfig::PLAYER_HEIGHT;
}

/// Restituisce il rettangolo di collisione del giocatore nella postura attuale.
AABB Player::bounds() const
{
    return boundsAtHeight(size.y);
}

/// Calcola il rettangolo di una nuova postura mantenendo la quota dei piedi e lo stato attuale.
AABB Player::boundsAtHeight(float height) const
{
    const glm::vec2 center = position + glm::vec2{0.0f, (height - size.y) * 0.5f};
    return makeAABB(center, {GameConfig::PLAYER_HITBOX_WIDTH, height});
}

/// Cambia l'altezza e sposta il centro quanto basta per tenere fermi i piedi.
void Player::setHeight(float height)
{
    position.y = boundsAtHeight(height).center.y;
    size.y = height;
}
