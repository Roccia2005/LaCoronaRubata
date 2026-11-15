#include "Geometry.h"
/// Carica nella GPU i vertici di mesh e configura il VAO per il formato x, y, r, g, b. Memorizza
/// la primitiva e il numero di vertici da disegnare.
Geometry::Geometry(const MeshData& mesh) {
    m_vertexCount=static_cast<GLsizei>(mesh.vertices.size()/5);
    m_primitive=mesh.primitive;
    glGenVertexArrays(1,&m_vao);
    glGenBuffers(1,&m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
    glBufferData(GL_ARRAY_BUFFER,mesh.vertices.size()*sizeof(float),mesh.vertices.data(),GL_STATIC_DRAW);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,5*sizeof(float),nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,5*sizeof(float),reinterpret_cast<void*>(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}
/// Rilascia il VBO e il VAO posseduti dalla geometria. Il contesto OpenGL deve essere ancora
/// attivo.
Geometry::~Geometry() {
    if(m_vbo) glDeleteBuffers(1,&m_vbo);
    if(m_vao) glDeleteVertexArrays(1,&m_vao);
}
/// Associa il VAO ed emette una draw call con la primitiva e il numero di vertici della mesh. Il
/// renderer deve avere gia' impostato programma e uniform.
void Geometry::draw() const {
    glBindVertexArray(m_vao);
    glDrawArrays(m_primitive,0,m_vertexCount);
}
