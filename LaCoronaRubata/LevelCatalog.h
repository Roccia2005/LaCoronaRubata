#pragma once
#include <cstddef>

struct LevelDefinition;

namespace Levels {

const LevelDefinition& get(std::size_t index);
std::size_t count();

} // namespace Levels
