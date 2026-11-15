#include "LevelCatalog.h"
#include "LevelMaps.h"
#include <iterator>
#include <stdexcept>

namespace {
const LevelDefinition levels[] = {
    LevelMaps::firstRoom(),
    LevelMaps::secondRoom()
};
}

namespace Levels {

/// Restituisce una definizione in sola lettura. Gli indici partono da zero.
const LevelDefinition& get(std::size_t index)
{
    if (index >= count()) {
        throw std::out_of_range("Indice livello non valido");
    }
    return levels[index];
}

/// Restituisce il numero di livelli disponibili nel catalogo.
std::size_t count()
{
    return std::size(levels);
}

} // namespace Levels
