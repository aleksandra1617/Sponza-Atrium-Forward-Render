#version 430

layout(location = 0) out vec4 fragment_colour; 

in vec3 varying_position;
in vec4 varying_normal;

in vec3 varying_ambCol;
in vec3 varying_diffCol;
in vec3 varying_specCol;

uniform per_frame_ublock
{
	mat4 projection_view_xform;
	vec3 spotLights[5];
	vec3 pointLights[20];
	vec3 camPos;
};

uniform float shininess;


float CalculateAttenuation(vec3 vertPos, vec3 lightPosition)
{
	//Attenuation constants
	const float ATTENUATION_CONSTANT = 1;
	const float ATTENUATION_LINEAR = 0.0;
	const float ATTENUATION_QUADRATIC = 0.0001;

	float d = distance(lightPosition, vertPos);
	float fd = 1.0 / (ATTENUATION_CONSTANT
		+ d * ATTENUATION_LINEAR
		+ d * d * ATTENUATION_QUADRATIC);

	return fd;
}

void main(void)
{
	vec3 vp = varying_position;
	vec3 Normal = normalize(vec3(varying_normal));

	//DIFFUSED LIGHT
	vec3 diffTexColouring = varying_diffCol;
	vec3 L = normalize(pointLights[1] - vp);	// Diff intensity
	vec3 diffused = diffTexColouring*max(dot(L, Normal), 0.0); //kd*(L.N)

	//SPECULAR LIGHT
	vec3 specTexColouring = varying_specCol;
	vec3 r = reflect( -L, Normal);
	vec3 Rv = normalize( camPos - vp ); //Cam pos - vp
	vec3 specular = specTexColouring * pow(max( dot(r,Rv), 0.0 ), 64);

	//ATTENUATION 
	float fd = CalculateAttenuation(vp, spotLights[0]);

	//PHONG
	vec3 phong = varying_ambCol * fd * (diffused + specular);

    fragment_colour = vec4(phong, 1.0);
}
