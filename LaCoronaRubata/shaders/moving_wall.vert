#version 330 core
layout(location = 0) in vec2 aPosition;
uniform mat4 model;
uniform mat4 projection;
out vec2 localPosition;
flat out vec2 wallSize;

void main()
{
    localPosition = aPosition;
    wallSize = vec2(length(model[0].xyz), length(model[1].xyz));
    gl_Position = projection * model * vec4(aPosition, 0.0, 1.0);
}
