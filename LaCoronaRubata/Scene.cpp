#include "Scene.h"
#include <stdexcept>

/// Crea una geometria GPU posseduta dalla scena e restituisce l'indice da usare per creare
/// istanze.
std::size_t Scene::addGeometry(const MeshData& mesh)
{
    m_geometries.push_back(std::make_unique<Geometry>(mesh));
    return m_geometries.size() - 1;
}

/// Aggiunge un'istanza visibile con matrice identita' e il passaggio di rendering richiesto.
/// Restituisce il suo indice e segnala un indice mesh non valido con std::out_of_range.
std::size_t Scene::addInstance(std::size_t geometryIndex, RenderPass pass)
{
    if (geometryIndex >= m_geometries.size()) {
        throw std::out_of_range("geometryIndex non valido");
    }
    m_instances.push_back({geometryIndex, glm::mat4(1.0f), true, pass});
    return m_instances.size() - 1;
}

/// Restituisce un riferimento modificabile all'istanza. Un indice fuori intervallo genera
/// std::out_of_range.
GeometryInstance& Scene::instance(std::size_t index)
{
    return m_instances.at(index);
}

/// Restituisce la geometria condivisa in sola lettura. Un indice fuori intervallo genera
/// std::out_of_range.
const Geometry& Scene::geometry(std::size_t index) const
{
    return *m_geometries.at(index);
}

/// Espone le istanze in sola lettura, nell'ordine di inserimento usato dai renderer.
const std::vector<GeometryInstance>& Scene::instances() const
{
    return m_instances;
}

/// Rimuove tutte le istanze. Gli indici delle istanze decadono e le geometrie restano
/// disponibili.
void Scene::clearInstances()
{
    m_instances.clear();
}

/// Svuota la scena e distrugge le geometrie GPU. Richiede che il contesto OpenGL sia attivo.
void Scene::clear()
{
    clearInstances();
    m_geometries.clear();
}
