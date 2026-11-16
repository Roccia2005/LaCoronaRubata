#include "PlayerPhysics.h"
#include "Crate.h"
#include "Player.h"
#include <algorithm>
#include <cmath>

using namespace GameConfig;

/// Verifica se due AABB si sovrappongono su entrambi gli assi. Il semplice contatto sul bordo
/// restituisce false.
bool PlayerPhysics::intersects(const AABB& a, const AABB& b)
{
    return std::abs(a.center.x - b.center.x) < a.halfSize.x + b.halfSize.x &&
           std::abs(a.center.y - b.center.y) < a.halfSize.y + b.halfSize.y;
}

namespace {
using PlayerPhysics::intersects;
constexpr float contactTolerance = 0.001f;

/// Ignora le sovrapposizioni numeriche al semplice contatto sul bordo.
bool overlapsHorizontally(const AABB& first, const AABB& second)
{
    return std::abs(first.center.x - second.center.x) <
           first.halfSize.x + second.halfSize.x - contactTolerance;
}

/// Un giocatore puo' passare di lato quando i suoi piedi sono sopra l'altro corpo.
bool overlapsVertically(const AABB& first, const AABB& second)
{
    return std::abs(first.center.y - second.center.y) <
           first.halfSize.y + second.halfSize.y - contactTolerance;
}

/// Corregge le compenetrazioni verticali con i solidi. Azzera la velocita' verticale e segna
/// l'appoggio quando il giocatore atterra sulla superficie.
void verticalCollision(Player& player, const std::vector<SolidCollider>& solids)
{
    const float halfPlayerHeight = player.bounds().halfSize.y;
    const float verticalVelocity = player.velocity.y;
    for (const SolidCollider& solid : solids) {
        const AABB playerBox = player.bounds();
        const AABB solidBox{ solid.position, solid.size * 0.5f };
        // Il contatto di lato durante una spinta non e' un atterraggio sulla cassa.
        if (!overlapsHorizontally(playerBox, solidBox) || !intersects(playerBox, solidBox)) {
            continue;
        }
        if (verticalVelocity <= 0.0f) {
            // Discesa: appoggia i piedi sulla superficie del solido.
            player.position.y = solid.position.y + solid.size.y * 0.5f + halfPlayerHeight;
            player.onGround = true;
        }
        else {
            // Salita: la testa urta il lato inferiore del solido.
            player.position.y = solid.position.y - solid.size.y * 0.5f - halfPlayerHeight;
        }
        player.velocity.y = 0.0f;
    }
}
/// Limita lo spostamento al bordo di un corpo incontrato nella direzione di marcia.
float limitHorizontalDistance(const AABB& body, float distance, const AABB& obstacle)
{
    const float direction = distance > 0.0f ? 1.0f : -1.0f;
    const float centerGap = (obstacle.center.x - body.center.x) * direction;
    if (centerGap <= 0.0f || !overlapsVertically(body, obstacle)) {
        return distance;
    }
    const float gap = std::max(0.0f, centerGap - body.halfSize.x - obstacle.halfSize.x);
    return direction * std::min(std::abs(distance), gap);
}

/// Calcola la corsa possibile verso il mondo fisso prima di considerare giocatori e casse.
float distanceAgainstFixedSolids(const Player& player, float dt, float roomHalfWidth,
                                 const std::vector<SolidCollider>& solids, std::size_t firstCrate)
{
    const AABB body = player.bounds();
    const float limit = roomHalfWidth - body.halfSize.x;
    float distance = std::clamp(body.center.x + player.velocity.x * dt, -limit, limit)
                   - body.center.x;
    for (std::size_t i = 0; i < firstCrate; ++i) {
        const SolidCollider& solid = solids[i];
        distance = limitHorizontalDistance(body, distance, makeAABB(solid.position, solid.size));
    }
    return distance;
}

/// Ferma il giocatore contro il compagno e tenta di spingere le casse raggiunte lungo il tragitto.
void moveHorizontally(Player& player, const Player& other, float distance, float dt,
                      float roomHalfWidth, std::vector<SolidCollider>& solids,
                      std::vector<Crate>& crates)
{
    const AABB body = player.bounds();
    const AABB otherBody = other.bounds();
    distance = limitHorizontalDistance(body, distance, otherBody);
    const std::size_t firstCrate = solids.size() - crates.size();
    for (std::size_t i = 0; i < crates.size(); ++i) {
        Crate& crate = crates[i];
        const float gap = limitHorizontalDistance(body, distance, crate.bounds());
        if (std::abs(gap) < std::abs(distance)) {
            crate.push(distance - gap, roomHalfWidth, solids, firstCrate + i, otherBody);
            solids[firstCrate + i] = crate.collider();
            distance = limitHorizontalDistance(body, distance, crate.bounds());
        }
    }
    player.position.x += distance;
    if (std::abs(distance - player.velocity.x * dt) > contactTolerance) {
        player.velocity.x = 0.0f;
    }
}

/// Risolve l'incontro frontale allo stesso istante per entrambi. Quando procedono
/// nella stessa direzione, muove chi e' davanti prima di chi lo segue.
void movePlayersHorizontally(Player& first, Player& second, float dt, float roomHalfWidth,
                             std::vector<SolidCollider>& solids, std::vector<Crate>& crates)
{
    Player& left = first.position.x <= second.position.x ? first : second;
    Player& right = &left == &first ? second : first;
    const std::size_t firstCrate = solids.size() - crates.size();
    float leftDistance = distanceAgainstFixedSolids(left, dt, roomHalfWidth, solids, firstCrate);
    float rightDistance = distanceAgainstFixedSolids(right, dt, roomHalfWidth, solids, firstCrate);

    if (leftDistance > 0.0f && rightDistance < 0.0f &&
        overlapsVertically(left.bounds(), right.bounds())) {
        const float gap = std::max(0.0f, right.position.x - left.position.x
                          - left.bounds().halfSize.x - right.bounds().halfSize.x);
        const float fraction = std::min(1.0f, gap / (leftDistance - rightDistance));
        leftDistance *= fraction;
        rightDistance *= fraction;
    }

    if (leftDistance > 0.0f && rightDistance > 0.0f) {
        moveHorizontally(right, left, rightDistance, dt, roomHalfWidth, solids, crates);
        moveHorizontally(left, right, leftDistance, dt, roomHalfWidth, solids, crates);
    } else {
        moveHorizontally(left, right, leftDistance, dt, roomHalfWidth, solids, crates);
        moveHorizontally(right, left, rightDistance, dt, roomHalfWidth, solids, crates);
    }
}

struct VerticalBounds {
    float feet;
    float head;
};

/// Restituisce le quote dei piedi e della testa a partire dal centro e dall'altezza attuale del
/// giocatore.
VerticalBounds verticalBounds(const Player& player)
{
    const float halfHeight = player.bounds().halfSize.y;
    return {player.position.y - halfHeight, player.position.y + halfHeight};
}

/// Usa le quote prima e dopo il movimento per risolvere l'appoggio sulla testa e gli urti dal
/// basso. Se rialzarsi causerebbe una sovrapposizione, ripristina l'altezza precedente.
void resolvePlayerVerticalContact(Player& upper, Player& lower,
                                  VerticalBounds upperBefore, VerticalBounds lowerBefore)
{
    constexpr float contactTolerance = 0.0001f;
    const float gapBefore = upperBefore.feet - lowerBefore.head;
    if (gapBefore < -contactTolerance ||
        !overlapsHorizontally(upper.bounds(), lower.bounds())) {
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
        lower.setHeight(previousHeight);
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

/// Applica la postura richiesta quando lo spazio sopra la testa permette il rialzo.
/// Controlla solo la parte aggiunta alla sagoma, cosi' l'appoggio ai piedi resta valido.
void updatePlayerPosture(Player& player, const Player& other, const PlayerInput& input,
                         const std::vector<SolidCollider>& solids)
{
    const float height = player.requestedHeight(input);
    if (height == player.size.y) {
        return;
    }

    if (height > player.size.y) {
        const AABB current = player.bounds();
        const AABB taller = player.boundsAtHeight(height);
        const float head = current.center.y + current.halfSize.y;
        const float tallerHead = taller.center.y + taller.halfSize.y;
        const AABB headroom = makeAABB({current.center.x, (head + tallerHead) * 0.5f},
                                      {current.halfSize.x * 2.0f, tallerHead - head});
        const AABB otherBody = other.bounds();
        if (overlapsHorizontally(headroom, otherBody) && intersects(headroom, otherBody)) {
            return;
        }
        const bool blocked = std::any_of(solids.begin(), solids.end(),
            [&](const SolidCollider& solid) {
                return intersects(headroom, makeAABB(solid.position, solid.size));
            });
        if (blocked) {
            return;
        }
    }

    player.setHeight(height);
}

/// Applica gravita' e collisioni verticali dopo aver risolto il movimento orizzontale.
void moveVertically(Player& player, float dt, float floorY, const std::vector<SolidCollider>& solids)
{
    const float halfPlayerHeight = player.bounds().halfSize.y;

    // Gravita' e integrazione di Eulero semi-implicito.
    player.velocity.y += GRAVITY * dt;
    player.position.y += player.velocity.y * dt;
    player.onGround = false; // verra' aggiornato dalle collisioni.

    // Collisione con il pavimento.
    if (player.position.y - halfPlayerHeight <= floorY) {
        player.position.y = floorY + halfPlayerHeight;
        player.velocity.y = 0.0f;
        player.onGround = true;
    }

    verticalCollision(player, solids);
}
} // namespace

/// Fa cadere le casse, aggiorna i giocatori e risolve il contatto verticale reciproco.
/// I collider delle casse vengono aggiunti in coda all'elenco dei solidi della stanza.
void PlayerPhysics::updatePlayers(Player& first, Player& second,
                                  const PlayerInput& input1, const PlayerInput& input2,
                                  float dt, float roomHalfWidth, float floorY,
                                  std::vector<SolidCollider>& solids, std::vector<Crate>& crates)
{
    if (!crates.empty()) {
        const std::size_t firstCrate = solids.size();
        for (const Crate& crate : crates) {
            solids.push_back(crate.collider());
        }
        // Durante la caduta, anche la testa di un giocatore puo' sostenere una cassa.
        const AABB firstBody = first.bounds();
        const AABB secondBody = second.bounds();
        solids.push_back({firstBody.center, firstBody.halfSize * 2.0f});
        solids.push_back({secondBody.center, secondBody.halfSize * 2.0f});
        for (std::size_t i = 0; i < crates.size(); ++i) {
            crates[i].fall(dt, floorY, solids, firstCrate + i);
            solids[firstCrate + i] = crates[i].collider();
        }
        solids.resize(firstCrate + crates.size());
    }
    const auto firstBefore = verticalBounds(first);
    const auto secondBefore = verticalBounds(second);

    first.applyInput(input1);
    second.applyInput(input2);
    updatePlayerPosture(first, second, input1, solids);
    updatePlayerPosture(second, first, input2, solids);
    movePlayersHorizontally(first, second, dt, roomHalfWidth, solids, crates);
    moveVertically(first, dt, floorY, solids);
    moveVertically(second, dt, floorY, solids);
    resolvePlayerVerticalContact(first, second, firstBefore, secondBefore);
    resolvePlayerVerticalContact(second, first, secondBefore, firstBefore);
}
