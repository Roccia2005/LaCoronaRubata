#pragma once
#include <cstddef>

struct LevelDefinition;

namespace Levels {

const LevelDefinition& get(std::size_t index);
std::size_t count();
/// Definizione della stanza di prova accessibile dal menu.
const LevelDefinition& tutorial();

} // namespace Levels
