#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform vec2 scale;
uniform vec2 position;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4((aPos.x + position.x) * scale.x, (aPos.y + position.y) * scale.y, 0.0, 1.0); 
}  