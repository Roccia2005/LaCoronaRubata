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

/// Attiva lo shader degli oggetti base e disegna le istanze visibili del passaggio Basic con la
/// proiezione corrente e la loro Model Matrix.
void Renderer::drawScene(const Scene& scene) const
{
    m_shader.use();
    m_shader.setMat4("projection", m_projection);
    for (const GeometryInstance& instance : scene.instances()) {
        if (!instance.visible || instance.pass != RenderPass::Basic) continue;
        m_shader.setMat4("model", instance.modelMatrix);
        scene.geometry(instance.geometryIndex).draw();
    }
    glBindVertexArray(0);
}
