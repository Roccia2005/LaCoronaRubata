#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <iostream>
namespace {
/// Compila il sorgente GLSL per lo stadio kind e restituisce l'handle dello shader. Se fallisce,
/// stampa il log, elimina lo shader e lancia std::runtime_error.
GLuint compileShader(GLenum kind,const std::string& src) {
    GLuint id=glCreateShader(kind);
    const char* ptr=src.c_str();
    glShaderSource(id,1,&ptr,nullptr);
    glCompileShader(id);
    GLint ok=0;glGetShaderiv(id,GL_COMPILE_STATUS,&ok);
    if(!ok) {
        char log[4096]={};glGetShaderInfoLog(id,sizeof(log),nullptr,log);
        std::cerr<<log<<'\n';glDeleteShader(id);
        throw std::runtime_error("Compilazione shader fallita");
    }
    return id;
}
}
/// Compila i sorgenti vertex e fragment e li collega in un programma. Segnala gli errori di
/// compilazione o link con un'eccezione.
Shader::Shader(const std::string& vs,const std::string& fs) {
    GLuint v=compileShader(GL_VERTEX_SHADER,vs);
    GLuint f=compileShader(GL_FRAGMENT_SHADER,fs);
    m_program=glCreateProgram();glAttachShader(m_program,v);glAttachShader(m_program,f);
    glLinkProgram(m_program);glDeleteShader(v);glDeleteShader(f);
    GLint ok=0;glGetProgramiv(m_program,GL_LINK_STATUS,&ok);
    if(!ok) {char log[4096]={};glGetProgramInfoLog(m_program,sizeof(log),nullptr,log);std::cerr<<log<<'\n';throw std::runtime_error("Link shader fallito");}
}
/// Rilascia il programma GLSL posseduto dall'oggetto mentre il contesto OpenGL e' ancora attivo.
Shader::~Shader() {if(m_program)glDeleteProgram(m_program);}
/// Rende attivo il programma per le successive scritture delle uniform e chiamate di disegno.
void Shader::use() const {glUseProgram(m_program);}
/// Imposta la uniform matriciale identificata da name. Chiamare dopo avere attivato il programma
/// con use().
void Shader::setMat4(const char* name,const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(m_program,name),1,GL_FALSE,glm::value_ptr(value));
}

/// Imposta la uniform float identificata da name. Chiamare dopo avere attivato il programma con
/// use().
void Shader::setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(m_program, name), value);
}
