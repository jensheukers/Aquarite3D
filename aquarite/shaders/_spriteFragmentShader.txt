#version 330 core
out vec4 FragColor;

in vec2 texCoord;

// texture samplers
uniform sampler2D objectTexture;

void main()
{
	FragColor = texture(objectTexture, texCoord);
}