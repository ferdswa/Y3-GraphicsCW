#version 450 core

layout (location = 0) out vec4 fColour;

in vec2 tex;
in vec3 nor;
in vec3 FragPosWorldSpace;

uniform sampler2D Texture;
uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec3 camPos;

uniform int type;

void main()
{
	if(type == 0){
		float amb = 0.1;
	
		vec3 nNor = normalize(nor);
		vec3 nto_light = normalize(-1.0 * lightDirection);
		float diffuse = max(0,dot(nNor, nto_light));

		vec3 nfrom_light = normalize(lightDirection);
		vec3 nreflight = reflect(nfrom_light, nNor);
		vec3 camDirection = camPos - FragPosWorldSpace;
		vec3 NcamDirection = normalize(camDirection);
		float specular = max(0,pow(dot(NcamDirection, nreflight),128));
		float phong = amb + diffuse + specular;
	
		vec4 tColour=texture(Texture, tex);
		vec3 iCol = vec3(tColour.x, tColour.y, tColour.z);
		iCol = phong * iCol * lightColour;
		fColour = vec4(iCol, tColour.a);
	}else{
		fColour = texture(Texture, tex);
	}
}