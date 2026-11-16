#pragma once
#include "LevelTypes.h"
#include <vector>

struct Player;
class Crate;

namespace PlayerPhysics {
bool intersects(const AABB& a, const AABB& b);

/// In coda a solids devono esserci i collider di tutte le casse, nell'ordine di crates.
void updatePlayers(Player& first, Player& second,
                   const PlayerInput& input1, const PlayerInput& input2,
                   float dt, float roomHalfWidth, float floorY,
                   std::vector<SolidCollider>& solids, std::vector<Crate>& crates);
}
