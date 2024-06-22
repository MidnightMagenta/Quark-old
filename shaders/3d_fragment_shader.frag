#version 460 core

uniform sampler2D inTexture;
uniform bool textured;

struct LightSource
{
	vec3 lightSourcePosition;
	vec4 lightSourceColor;
};

layout(std430, binding = 4) buffer lightSources{
	LightSource pointSources[];
};

in vec3 normalsToFrag;
in vec2 texturesToFrag;
in vec4 colorToFrag;
in vec4 transformedVertexToFrag;

out vec4 outColor;

vec4 CalculatePointSourceContribution(LightSource light)
{
	vec3 lightSourceDirection = normalize(light.lightSourcePosition - vec3(transformedVertexToFrag.xyz));
	float diffuseIntensity = max(dot(normalize(normalsToFrag), lightSourceDirection), 0.0);
	vec3 diffuseColor = light.lightSourceColor.xyz * colorToFrag.xyz * diffuseIntensity;
	return vec4(diffuseColor, 1.f);
}

void main()
{
	vec4 lightSourceContributionColor;
	if(pointSources.length() != 0){
		for(int i = 0; i < pointSources.length(); i++){
			lightSourceContributionColor += CalculatePointSourceContribution(pointSources[i]);
		}
		lightSourceContributionColor = normalize(lightSourceContributionColor);
	}
	else {lightSourceContributionColor = vec4(1, 1, 1, 1);};

	if(textured){
		outColor = texture(inTexture, texturesToFrag) * colorToFrag * lightSourceContributionColor;
	}
	else{
		outColor = colorToFrag * lightSourceContributionColor;
	}
}