#pragma once
#include "Scene.h"
#include <optional>

struct DialogueLine;
struct DialoguePanelResult;
namespace DecorativeShapes { struct CharacterMeshes; }

class DialogueView {
public:
    DialogueView();
    void observeLine(const DialogueLine* line);
    bool updatePose(const DialoguePanelResult& panel, glm::vec2 framebufferSize, double now);
    const Scene& scene() const { return m_scene; }

private:
    struct Figure {
        std::size_t body;
        std::size_t head;
        std::optional<std::size_t> foreground;
        glm::vec2 neckPivot;
    };
    Figure addFigure(const DecorativeShapes::CharacterMeshes& parts);
    void poseFigure(const Figure& figure, glm::vec2 center, float scale,
                    const glm::mat4& headMotion);

    Scene m_scene;
    Figure m_prince;
    Figure m_fox;
    const DialogueLine* m_line = nullptr;
    std::optional<double> m_gestureStart;
};
