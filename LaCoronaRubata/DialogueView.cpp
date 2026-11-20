#include "DialogueView.h"
#include "Config.h"
#include "DecorativeShapes.h"
#include "GameFlow.h"
#include "Gui.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

namespace {

constexpr double duration = 0.45;
constexpr float foxAngle = glm::radians(8.0f);
const glm::vec2 standingSize = glm::vec2(GameConfig::PLAYER_WIDTH, GameConfig::PLAYER_HEIGHT)
                             * GameConfig::PLAYER_RENDER_SCALE;

float gestureIntensity(double elapsed)
{
    if (elapsed <= 0.0 || elapsed >= duration) return 0.0f;
    const double wave = std::sin(glm::pi<double>() * elapsed / duration);
    return static_cast<float>(wave * wave);
}

} // namespace

DialogueView::DialogueView()
    : m_prince(addFigure(DecorativeShapes::littlePrinceParts())),
      m_fox(addFigure(DecorativeShapes::foxParts()))
{
}

DialogueView::Figure DialogueView::addFigure(const DecorativeShapes::CharacterMeshes& parts)
{
    const auto add = [this](const MeshData& mesh) {
        return m_scene.addInstance(m_scene.addGeometry(mesh));
    };
    Figure figure{add(parts.body), add(parts.head), std::nullopt, parts.neckPivot * standingSize};
    if (!parts.foreground.vertices.empty()) figure.foreground = add(parts.foreground);
    return figure;
}

void DialogueView::observeLine(const DialogueLine* line)
{
    if (m_line != line) {
        m_line = line;
        m_gestureStart.reset();
    }
}

void DialogueView::poseFigure(const Figure& figure, glm::vec2 center, float scale,
                              const glm::mat4& headMotion)
{
    auto placement = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f));
    placement = glm::scale(placement, glm::vec3(scale, scale, 1.0f));
    const glm::vec3 proportions(standingSize, 1.0f);
    const auto base = glm::scale(placement, proportions);
    m_scene.instance(figure.body).modelMatrix = base;
    m_scene.instance(figure.head).modelMatrix = headMotion == glm::mat4(1.0f)
        ? base : glm::scale(placement * headMotion, proportions);
    if (figure.foreground) m_scene.instance(*figure.foreground).modelMatrix = base;
}

bool DialogueView::updatePose(const DialoguePanelResult& panel, glm::vec2 framebufferSize, double now)
{
    if (!m_line || panel.viewportSize.x <= 0.0f || panel.viewportSize.y <= 0.0f) return false;
    const auto pixelRatio = framebufferSize / panel.viewportSize;
    const auto available = (panel.bottomRight - panel.topLeft) * pixelRatio;
    if (available.x <= 0.0f || available.y <= 0.0f) return false;
    auto center = (panel.topLeft + panel.bottomRight) * 0.5f * pixelRatio - framebufferSize * 0.5f;
    center.y = -center.y;

    // La corda massima limita lo spostamento di ogni vertice durante la rotazione.
    const float radius = glm::length(standingSize * 0.5f + glm::abs(m_fox.neckPivot));
    const float excursion = 2.0f * radius * std::sin(foxAngle * 0.5f);
    const float slot = standingSize.x + 2.0f * excursion;
    const float separation = slot + 0.25f * standingSize.x;
    const float scale = 0.88f * std::min(available.x / (separation + slot),
                                         available.y / (standingSize.y + excursion));
    // Piedi a -0.5 per entrambe. L'ingombro aggiuntivo e' sopra le orecchie.
    center.y -= excursion * scale * 0.5f;
    const glm::vec2 offset{separation * scale * 0.5f, 0.0f};

    if (!m_gestureStart) m_gestureStart = now;
    const float intensity = gestureIntensity(now - *m_gestureStart);
    glm::mat4 princeMotion(1.0f), foxMotion(1.0f);
    if (intensity > 0.0f) {
        if (m_line->speaker == DialogueSpeaker::Prince) {
            princeMotion = glm::translate(princeMotion, {0.0f, -0.04f * standingSize.y * intensity, 0.0f});
        } else {
            const glm::vec3 pivot(m_fox.neckPivot, 0.0f);
            foxMotion = glm::translate(foxMotion, pivot);
            foxMotion = glm::rotate(foxMotion, foxAngle * intensity, {0.0f, 0.0f, 1.0f});
            foxMotion = glm::translate(foxMotion, -pivot);
        }
    }
    poseFigure(m_prince, center - offset, scale, princeMotion);
    poseFigure(m_fox, center + offset, scale, foxMotion);
    return true;
}
