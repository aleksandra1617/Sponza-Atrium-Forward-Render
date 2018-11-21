#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

//layout(location = 2) reserved for tex coordinates

layout(location = 3) in mat4 model_xform;

layout(location = 7) in vec3 ambientCol;
layout(location = 8) in vec3 diffusedCol;
layout(location = 9) in vec3 specularCol;

uniform per_frame_ublock
{
	mat4 projection_view_xform;
	vec3 spotLights[5]; 
	vec3 pointLights[20];
	vec3 camPos;
};

out vec4 varying_normal;

out vec3 varying_position;
out vec3 varying_ambCol;
out vec3 varying_diffCol;
out vec3 varying_specCol;

void main(void)
{
	vec4 normal_colour = vec4(vertex_normal, 0);
	varying_normal = model_xform * normal_colour;
	varying_position = mat4x3(model_xform) * vec4(vertex_position, 1.0);

	varying_ambCol = ambientCol;
	varying_diffCol = diffusedCol;
	varying_specCol = specularCol;

	gl_Position = projection_view_xform * model_xform * vec4(vertex_position, 1.0);
}
