#pragma once
#include "Scene.h"
#include "Shader.h"
#include <glm/glm.hpp>
class Renderer {
    const Shader& m_shader;
    glm::mat4 m_projection{1.0f};
public:
    explicit Renderer(const Shader& shader):m_shader(shader){}
    void setRoomViewport(int framebufferWidth, int framebufferHeight, const glm::vec2& roomHalfSize);
    void drawScene(const Scene& scene) const;
    const glm::mat4& projectionMatrix() const { return m_projection; }
};
