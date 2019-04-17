#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec3 aTan;
layout (location = 3) in vec3 aBiTan;
layout (location = 4) in vec2 aTex;

uniform mat4 Projection;
uniform mat4 Camera;
uniform mat4 Model;

out vec3 normal;
out vec3 tangent;
out vec3 bitangent;
out vec2 texcoord;

void main()
{
	gl_Position = Projection * Camera * Model * vec4(aPos, 1.0);

	normal = aNor;
	tangent = aTan;
	bitangent = aBiTan;
	texcoord = aTex;
}
