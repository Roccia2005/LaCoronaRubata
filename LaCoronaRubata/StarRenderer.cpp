#include "StarRenderer.h"
#include "LevelTypes.h"

/// Disegna le istanze visibili del passaggio Stars usando la proiezione della stanza e il tempo
/// in secondi. Le istanze nascoste restano nel conteggio, cosi' la fase delle stelle visibili
/// resta stabile dopo una raccolta.
void StarRenderer::draw(const Scene& scene, const glm::mat4& projection,
                        float timeSeconds) const
{
    m_shader.use();
    m_shader.setMat4("projection", projection);
    m_shader.setFloat("uTime", timeSeconds);
    m_shader.setFloat("uAmplitude", GameConfig::STAR_BOB_AMPLITUDE);
    m_shader.setFloat("uFrequency", GameConfig::STAR_BOB_FREQUENCY);

    std::size_t starIndex = 0;
    for (const GeometryInstance& instance : scene.instances()) {
        if (instance.pass != RenderPass::Stars) continue;
        // L'ordine comprende anche le stelle gia' raccolte, cosi' la fase resta stabile.
        const float phase = static_cast<float>(starIndex++) * GameConfig::STAR_PHASE_STEP;
        if (!instance.visible) continue;
        m_shader.setFloat("uPhase", phase);
        m_shader.setMat4("model", instance.modelMatrix);
        scene.geometry(instance.geometryIndex).draw();
    }
    glBindVertexArray(0);
}
