#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNor;
layout (location = 3) in vec3 aTan;
layout (location = 4) in vec3 aBiTan;

uniform mat4 Projection;
uniform mat4 Camera;
uniform mat4 Model;
uniform vec3 Light;

out vec3 pos;
out vec2 texcoord;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;

out vec3 lpos;
out mat3 model;
out mat4 model_cam;

void main()
{
	model = mat3(Model);
	model_cam = Camera * Model;
	pos = (Camera * Model * vec4(aPos, 1.0)).xyz;
	lpos = (Camera * vec4(Light, 1.0)).xyz;
	gl_Position = Projection * Camera * Model * vec4(aPos, 1.0);

	normal = aNor;
	tangent = aTan;
	bitangent = aBiTan;
	texcoord = aTex;
}
