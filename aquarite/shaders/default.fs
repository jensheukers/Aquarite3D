#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specular;    
    float shininess;
}; 

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 texCoord;

uniform vec3 viewPos;
uniform Material material;

uniform DirLight dirLight;
uniform PointLight pointLights[25];

//Determines if object has a texture
uniform bool hasTexture;
// texture samplers
uniform sampler2D objectTexture;

vec3 CalculateDirectionalLight(DirLight dirLight, vec3 norm, vec3 viewDir) {
	// ambient
    vec3 ambient = dirLight.ambient * material.ambientColor;
  	
    // diffuse 
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = dirLight.diffuse * (diff * material.diffuseColor);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dirLight.specular * (spec * material.specular);
	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight pointLight, vec3 norm, vec3 viewDir) {
	// ambient
    vec3 ambient = pointLight.ambient * material.ambientColor;
  	
    // diffuse 
    vec3 lightDir = normalize(pointLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * (diff * material.diffuseColor);
    
    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pointLight.specular * (spec * material.specular);
	return (ambient + diffuse + specular);
}
void main() {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);  
    
	//Directional Lighting
    vec3 result = CalculateDirectionalLight(dirLight, norm, viewDir);
	
	//Point Lights
	for(int i = 0; i < 25; i++) {
		result += CalculatePointLight(pointLights[i], norm, viewDir);
	}
	
	if(hasTexture) {
	    FragColor = texture(objectTexture, texCoord) * vec4(result, 1.0);
	}
	else {
		FragColor = vec4(result, 1.0);
	}
} 