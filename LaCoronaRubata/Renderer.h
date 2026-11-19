#pragma once
#include "Scene.h"
#include "Shader.h"
#include <glm/glm.hpp>
class Renderer {
    const Shader& m_basicShader;
    const Shader& m_wallShader;
    glm::mat4 m_projection{1.0f};
public:
    Renderer(const Shader& basicShader, const Shader& wallShader)
        : m_basicShader(basicShader), m_wallShader(wallShader) {}
    void setRoomViewport(int framebufferWidth, int framebufferHeight, const glm::vec2& roomHalfSize);
    void drawScene(const Scene& scene) const;
    const glm::mat4& projectionMatrix() const { return m_projection; }
};
