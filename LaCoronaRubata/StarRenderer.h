#pragma once
#include "Shader.h"
#include "Scene.h"
#include <glm/glm.hpp>

// Disegna soltanto le stelle con il vertex shader animato.
// Le Geometry e le istanze restano proprieta' della Scene.
class StarRenderer {
public:
    explicit StarRenderer(const Shader& shader) : m_shader(shader) {}
    void draw(const Scene& scene, const glm::mat4& projection, float timeSeconds) const;
private:
    const Shader& m_shader;
};
