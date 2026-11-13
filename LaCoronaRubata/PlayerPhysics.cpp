#include "PlayerPhysics.h"
#include <algorithm>
#include <cmath>

using namespace GameConfig;

// Due rettangoli AABB si intersecano solo se si sovrappongono su X e Y.
// Cambiare posizione, non è un metodo della classe PlayerPhysics, perche' non serve accedere a membri privati.
bool PlayerPhysics::intersects(const AABB& a, const AABB& b)
{
    return std::abs(a.center.x - b.center.x) < a.halfSize.x + b.halfSize.x &&
           std::abs(a.center.y - b.center.y) < a.halfSize.y + b.halfSize.y;
}

namespace {
using PlayerPhysics::intersects;

// Collisione verticale tra player e ostacoli
void verticalCollision(Player& player, const std::vector<Obstacle>& obstacles)
{
    const float halfPlayerHeight = player.size.y * 0.5f;
    const float verticalVelocity = player.velocity.y;
    for (const Obstacle& obstacle : obstacles) {
        const AABB playerBox{ player.position, {player.size.x * 0.5f, halfPlayerHeight} };
        const AABB obstacleBox{ obstacle.position, obstacle.size * 0.5f };
        if (!intersects(playerBox, obstacleBox)) {
            continue;
        }
        if (verticalVelocity <= 0.0f) {
            // Discesa: appoggia i piedi sulla sommita' della piattaforma.
            player.position.y = obstacle.position.y + obstacle.size.y * 0.5f + halfPlayerHeight;
            player.onGround = true;
        }
        else {
            // Salita: la testa urta il lato inferiore dell'ostacolo.
            player.position.y = obstacle.position.y - obstacle.size.y * 0.5f - halfPlayerHeight;
        }
        player.velocity.y = 0.0f;
    }
}
void horizontalCollision(Player& player, const std::vector<Obstacle>& obstacles)
{
    const float horizontalVelocity = player.velocity.x;
    if (horizontalVelocity == 0.0f) {
        return;
    }

    const float halfPlayerWidth = player.size.x * 0.5f;
    for (const Obstacle& obstacle : obstacles) {
        const AABB playerBox{ player.position, {halfPlayerWidth, player.size.y * 0.5f} };
        const AABB obstacleBox{ obstacle.position, obstacle.size * 0.5f };
        if (!intersects(playerBox, obstacleBox)) {
            continue;
        }
        if (horizontalVelocity > 0.0f) {
            // Movimento verso destra: blocca il lato destro del player.
            player.position.x = obstacle.position.x - obstacle.size.x * 0.5f - halfPlayerWidth;
        }
        else {
            // Movimento verso sinistra: blocca il lato sinistro del player.
            player.position.x = obstacle.position.x + obstacle.size.x * 0.5f + halfPlayerWidth;
        }
        player.velocity.x = 0.0f;
    }
}

struct VerticalBounds {
    float feet;
    float head;
};

VerticalBounds verticalBounds(const Player& player)
{
    const float halfHeight = player.size.y * 0.5f;
    return {player.position.y - halfHeight, player.position.y + halfHeight};
}

void resolvePlayerVerticalContact(Player& upper, Player& lower,
                                  VerticalBounds upperBefore, VerticalBounds lowerBefore)
{
    constexpr float contactTolerance = 0.0001f;
    const float gapBefore = upperBefore.feet - lowerBefore.head;
    const float halfWidthSum = (upper.size.x + lower.size.x) * 0.5f;
    if (gapBefore < -contactTolerance ||
        std::abs(upper.position.x - lower.position.x) >= halfWidthSum) {
        return;
    }

    const float upperFeet = verticalBounds(upper).feet;
    auto lowerNow = verticalBounds(lower);
    if (upperFeet > lowerNow.head) {
        return;
    }

    // Se manca spazio sopra la testa, conserva la postura abbassata.
    const float previousHeight = lowerBefore.head - lowerBefore.feet;
    if (lower.size.y > previousHeight + contactTolerance) {
        lower.size.y = previousHeight;
        lower.position.y = lowerNow.feet + previousHeight * 0.5f;
        lowerNow = verticalBounds(lower);
    }

    if (upperFeet > lowerNow.head) {
        return;
    }

    if (!lower.onGround && lowerNow.head > lowerBefore.head + contactTolerance) {
        // In caso di urto dal basso, ferma il salto di chi sta sotto.
        float contactY = upperFeet;
        if (!upper.onGround && upperFeet < upperBefore.feet) {
            // I due si muovono l'uno verso l'altro: fermali alla quota d'incontro.
            const float distanceBefore = std::max(gapBefore, 0.0f);
            const float penetration = lowerNow.head - upperFeet;
            const float contactFraction = distanceBefore > 0.0f ? distanceBefore / (distanceBefore + penetration) : 0.0f;
            contactY = lowerBefore.head + (lowerNow.head - lowerBefore.head) * contactFraction;
            upper.position.y += contactY - upperFeet;
            upper.velocity.y = 0.0f;
            upper.onGround = true;
        }
        lower.position.y += contactY - lowerNow.head;
        lower.velocity.y = 0.0f;
    } else if (upper.velocity.y <= 0.0f && upperFeet <= upperBefore.feet) {
        upper.position.y += lowerNow.head - upperFeet;
        upper.velocity.y = 0.0f;
        upper.onGround = true;
    }
}

// Aggiorna il personaggio in quattro fasi:
// 1) input e salto; 2) movimento orizzontale; 3) gravita'; 4) collisioni verticali.
// La separazione degli assi rende piu' facile capire la risposta alle collisioni.
void updatePlayer(Player& player, float dt, float halfWidth, const PlayerInput& input,
                  const std::vector<Obstacle>& obstacles)
{
    const float halfPlayerWidth = player.size.x * 0.5f;

    // FASE 1: input. Due tasti opposti si annullano.
    player.velocity.x = (float(input.right) - float(input.left)) * MOVE_SPEED;

    // Il salto e' consentito solo se il personaggio e' appoggiato.
    if (input.jumpPressed && player.onGround) {
        player.velocity.y = JUMP_SPEED;
        player.onGround = false;
    }

    // Mantieni i piedi alla stessa quota quando cambia la postura.
    const bool crouching = input.down && player.onGround;
    const float newHeight = crouching ? PLAYER_HEIGHT * 0.5f : PLAYER_HEIGHT;
    if (newHeight != player.size.y) {
        player.position.y += (newHeight - player.size.y) * 0.5f;
        player.size.y = newHeight;
    }
    const float halfPlayerHeight = player.size.y * 0.5f;

    // FASE 2: movimento orizzontale e collisioni laterali.
    player.position.x += player.velocity.x * dt;
    player.position.x = std::clamp(
        player.position.x,
        -halfWidth + halfPlayerWidth,
        halfWidth - halfPlayerWidth
    );
    horizontalCollision(player, obstacles);

    // FASE 3: gravita' e integrazione di Eulero semi-implicito.
    player.velocity.y += GRAVITY * dt;
    player.position.y += player.velocity.y * dt;
    player.onGround = false; // verra' aggiornato dalle collisioni.

    // FASE 4a: collisione con il pavimento.
    if (player.position.y - halfPlayerHeight <= FLOOR_Y) {
        player.position.y = FLOOR_Y + halfPlayerHeight;
        player.velocity.y = 0.0f;
        player.onGround = true;
    }

    verticalCollision(player, obstacles);
}
} // namespace

void PlayerPhysics::updatePlayers(Player& first, Player& second,
                                  const PlayerInput& input1, const PlayerInput& input2,
                                  float dt, float halfWidth, const std::vector<Obstacle>& obstacles)
{
    const auto firstBefore = verticalBounds(first);
    const auto secondBefore = verticalBounds(second);

    updatePlayer(first, dt, halfWidth, input1, obstacles);
    updatePlayer(second, dt, halfWidth, input2, obstacles);
    resolvePlayerVerticalContact(first, second, firstBefore, secondBefore);
    resolvePlayerVerticalContact(second, first, secondBefore, firstBefore);
}
