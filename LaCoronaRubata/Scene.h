#pragma once
#include "Geometry.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <cstddef>
// Una istanza NON contiene una copia dei vertici: conserva un indice e una matrice.
enum class RenderPass { Basic, MovingWall, Stars };

struct GeometryInstance {
    std::size_t geometryIndex=0;
    glm::mat4 modelMatrix{1.0f};
    bool visible=true;
    RenderPass pass = RenderPass::Basic;
    bool moving = false;
};
class Scene {
    std::vector<std::unique_ptr<Geometry>> m_geometries;
    std::vector<GeometryInstance> m_instances;
public:
    std::size_t addGeometry(const MeshData& mesh);
    std::size_t addInstance(std::size_t geometryIndex, RenderPass pass = RenderPass::Basic);
    GeometryInstance& instance(std::size_t index);
    const Geometry& geometry(std::size_t index) const;
    const std::vector<GeometryInstance>& instances() const;
    // Rimuove le istanze. Gli indici delle geometrie restano validi.
    void clearInstances();
    void clear();
};
