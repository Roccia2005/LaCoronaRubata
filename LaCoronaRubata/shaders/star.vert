#version 330 core
// Shader dedicato esclusivamente alle stelle.
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

uniform mat4 projection;
uniform mat4 model;
uniform float uTime;
uniform float uAmplitude;
uniform float uFrequency;
uniform float uPhase;
out vec3 vColor;

void main()
{
    // Aggiunge l'oscillazione in unita' del mondo alla traslazione della Model Matrix.
    mat4 animatedModel = model;
    animatedModel[3].y += uAmplitude * sin(uFrequency * uTime + uPhase);
    gl_Position = projection * animatedModel * vec4(aPosition, 0.0, 1.0);
    vColor = aColor;
}
