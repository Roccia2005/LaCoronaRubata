#pragma once
#include "LevelTypes.h"
#include <vector>

namespace PlayerPhysics {
bool intersects(const AABB& a, const AABB& b);

void updatePlayers(Player& first, Player& second,
                   const PlayerInput& input1, const PlayerInput& input2,
                   float dt, float halfWidth, const std::vector<Obstacle>& obstacles);
}
