#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform vec2 position;
uniform vec2 scale;

void main()
{
	gl_Position = vec4(scale, 1.0, 1.0) * (vec4(position, 1.0, 1.0) + vec4(aPos, 1.0));
	texCoord = vec2(aTexCoord.x, aTexCoord.y);
}