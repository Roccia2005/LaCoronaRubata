#include "Renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

/// Inquadra l'intera stanza con proiezione ortografica e centra un viewport proporzionato nel
/// framebuffer. width e height sono in pixel, roomHalfSize e' in unita' mondo.
void Renderer::setRoomViewport(int width, int height, const glm::vec2& roomHalfSize)
{
    const double roomWidth = 2.0 * roomHalfSize.x;
    const double roomHeight = 2.0 * roomHalfSize.y;
    const double scale = std::min(width / roomWidth, height / roomHeight);
    const int viewportWidth = std::max(1, static_cast<int>(std::lround(roomWidth * scale)));
    const int viewportHeight = std::max(1, static_cast<int>(std::lround(roomHeight * scale)));
    glViewport((width - viewportWidth) / 2, (height - viewportHeight) / 2,
               viewportWidth, viewportHeight);

    m_projection = glm::ortho(-roomHalfSize.x, roomHalfSize.x,
                              -roomHalfSize.y, roomHalfSize.y, -1.0f, 1.0f);
}

/// Disegna oggetti base e muri mobili nello stesso ordine delle istanze.
void Renderer::drawScene(const Scene& scene) const
{
    const Shader* active = nullptr;
    for (const GeometryInstance& instance : scene.instances()) {
        if (!instance.visible || instance.pass == RenderPass::Stars) continue;
        const bool wall = instance.pass == RenderPass::MovingWall;
        const Shader& shader = wall ? m_wallShader : m_basicShader;
        if (active != &shader) {
            shader.use();
            shader.setMat4("projection", m_projection);
            active = &shader;
        }
        shader.setMat4("model", instance.modelMatrix);
        if (wall) shader.setFloat("uMoving", instance.moving ? 1.0f : 0.0f);
        scene.geometry(instance.geometryIndex).draw();
    }
    glBindVertexArray(0);
}
