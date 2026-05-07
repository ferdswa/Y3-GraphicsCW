#version 450 core

layout (location = 0) out vec4 fColour;

in vec2 tex;
in vec3 nor;
in vec3 FragPosWorldSpace;
in vec4 FragPosProjectedLightSpace;

uniform sampler2D shadowMap;
uniform sampler2D Texture;
uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec3 camPos;

uniform vec3 flashLightPos;
uniform vec3 flashLightDir;
uniform vec3 flashLightCol;

uniform int type;

float shadowOnFragment(vec4 FragPosProjectedLightSpace){
	vec3 ndc = FragPosProjectedLightSpace.xyz / FragPosProjectedLightSpace.w;
	vec3 ss = (ndc+1)*0.5;

	float fragDepth = ss.z;
	float shadow = 0.f;
	float litDepth = texture(shadowMap, ss.xy).r;

	vec3 Nnor = normalize(nor);
	vec3 NtoLight = normalize(-lightDirection);
	float bias = max(0.05 *(1.0-dot(Nnor, NtoLight)),0.005);

	shadow = fragDepth > (litDepth+bias) ? 1.0 : 0.0;

	return shadow;
}

float CalculateDirectionalIllumination(){
	float amb = 0.1;
	
	vec3 nNor = normalize(nor);
	vec3 nto_light = normalize(-1.0 * lightDirection);
	float diffuse = max(0,dot(nNor, nto_light));

	vec3 nfrom_light = normalize(lightDirection);
	vec3 nreflight = reflect(nfrom_light, nNor);
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);
	float specular = max(0,pow(dot(NcamDirection, nreflight),128));
	//float shadow = shadowOnFragment(FragPosProjectedLightSpace);
	//float phong = amb + ((1-shadow) * (diffuse+specular));
	float phong = amb + diffuse + specular;
	return phong;
}

float CalculatePositionalIllumination(){
	float amb = 0.1;
	
	vec3 nNor = normalize(nor);
	vec3 nto_light = normalize(flashLightPos - FragPosWorldSpace);
	float diffuse = max(0,dot(nNor, nto_light));

	vec3 nfrom_light = -1 * nto_light;
	vec3 nreflight = reflect(nfrom_light, nNor);
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);
	float specular = max(0,pow(dot(NcamDirection, nreflight),128));

	float d = length(FragPosWorldSpace - flashLightPos);

	float atten = 1/(1 + (0.05*d) + (0.002*pow(d,2)));

	float phong = (amb + diffuse + specular) * atten;
	return phong;
}

float CalculateSpotIllumination(){
	float amb = 0.1;
	
	vec3 nNor = normalize(nor);
	vec3 nto_light = normalize(flashLightPos - FragPosWorldSpace);
	float diffuse = max(0,dot(nNor, nto_light));

	vec3 nfrom_light = -1 * nto_light;
	vec3 nreflight = reflect(nfrom_light, nNor);
	vec3 camDirection = camPos - FragPosWorldSpace;
	vec3 NcamDirection = normalize(camDirection);
	float specular = max(0,pow(dot(NcamDirection, nreflight),128));

	float d = length(FragPosWorldSpace - flashLightPos);

	float atten = 1/(1 + (0.05*d) + (0.002*pow(d,2)));

	float phi = cos(radians(15));
	float phi2 = cos(radians(5));
	vec3 NSpotDir = normalize(flashLightDir);
	float theta = dot(nfrom_light, NSpotDir);

	float phong;
	if(theta>phi2){
		phong = 2*(amb + diffuse + specular) * atten;
	}
	else if(theta>phi){
		phong = (amb + diffuse + specular) * atten;
	} else {
		phong = amb * atten;
	}
	
	return phong;
}

void main()
{
	if(type == 0){
		float phong = CalculateDirectionalIllumination();
		float phong2 = CalculateSpotIllumination();
		vec4 tColour=texture(Texture, tex);
		vec3 iCol = vec3(tColour.x, tColour.y, tColour.z);
		vec3 iCol2 = vec3(tColour.x, tColour.y, tColour.z);
		iCol = phong * iCol * lightColour;
		iCol2 = phong2 * iCol * flashLightCol;
		iCol = iCol + iCol2;
		fColour = vec4(iCol, tColour.a);
	}else{
		fColour = texture(Texture, tex);
	}
}