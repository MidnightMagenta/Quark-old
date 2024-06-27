#version 460 core

#define Q_POINT 0
#define Q_DIRECTIONAL 1

uniform sampler2D inTexture;
uniform bool textured;

struct LightSource
{
	int lightType;
	float constant;
    float linear;
    float quadratic;  

	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	float shininess;
	vec3 specular;
	vec3 diffuse;
	vec3 ambient;
};

layout(std430, binding = 4) buffer lightSources{
	LightSource sources[];
};

in vec3 f_normals;
in vec2 f_textures;
in vec4 f_color;
in vec4 f_transformedVertices;
in vec3 f_cameraPosition;
in Material f_material;

out vec4 outColor;

vec3 CalculatePointSource(LightSource source)
{
	vec3 lightDirection = normalize(source.position.xyz - f_transformedVertices.xyz);
	vec3 viewDir = normalize(f_cameraPosition - f_transformedVertices.xyz);
	//diffuse
	float diffuse = max(dot(normalize(f_normals), lightDirection), 0.0);
	//specular
	vec3 reflectionDirection = reflect(-lightDirection, normalize(f_normals));
	float specular = pow(max(dot(viewDir, reflectionDirection), 0.0), f_material.shininess);
	//attenuation
	float dist = length(source.position - f_transformedVertices.xyz);
	float attentuation = 1.0f / (source.constant + source.linear * dist + source.quadratic * (dist * dist));
	//combine
	vec3 ambient_F = source.ambient * f_material.ambient;
	vec3 diffuse_F = diffuse * source.diffuse * f_material.diffuse;
	vec3 specular_F = specular * source.specular * f_material.specular;

	return specular_F + diffuse_F + ambient_F;
}

void main()
{
	vec3 lightResult;
	if(sources.length() != 0){
		for(int i = 0; i < sources.length(); i++){
			if(sources[i].lightType == Q_POINT) { lightResult += CalculatePointSource(sources[i]); }
		}
	}
	else {lightResult = vec3(1.f, 1.f, 1.f);}

	if(textured){
		outColor = texture(inTexture, f_textures) * f_color * vec4(lightResult, 1.f);
	}
	else{
		outColor = f_color * vec4(lightResult, 1.f);
	}
}