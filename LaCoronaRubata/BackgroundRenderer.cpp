#include "BackgroundRenderer.h"

/// Carica nella GPU un rettangolo in coordinate NDC che copre tutto il viewport. Conserva un
/// riferimento allo shader, che deve restare in vita per tutta la durata del renderer.
BackgroundRenderer::BackgroundRenderer(const Shader& shader)
    : m_shader(shader)
{
    // Due triangoli in coordinate normalizzate NDC [-1,+1].
    // Non occorrono Model Matrix ne' matrice di proiezione.
    const float vertices[] = {
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/// Rilascia il VAO e il VBO dello sfondo. Il contesto OpenGL deve essere ancora attivo.
BackgroundRenderer::~BackgroundRenderer()
{
    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
}

/// Disegna lo sfondo nel viewport corrente con il programma dedicato. timeSeconds, in secondi,
/// pilota gli effetti animati dello shader.
void BackgroundRenderer::draw(float timeSeconds) const
{
    // Usa il programma shader dello sfondo, diverso da quello del gioco.
    m_shader.use();
    m_shader.setFloat("uTime", timeSeconds);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
