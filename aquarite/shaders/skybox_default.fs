#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
	vec3 c_TexCoords = TexCoords * vec3(-1.0, -1.0f, -1.0f); // Flip
    FragColor = texture(skybox, c_TexCoords);
}