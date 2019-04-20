#version 450 core

layout (location = 0) in vec3 Point;
layout (location = 1) in vec4 Color;
layout (location = 2) in vec2 Texcoord;
layout (location = 3) in vec3 Normal;
layout (location = 4) in vec3 Tangent;
layout (location = 5) in vec3 Bitangent;

layout (std140) uniform Matrices
{
	mat4 Projection;
	mat4 Camera;
};

layout (std140) uniform Lights
{
	vec3 Light;
};

layout (std430) buffer MatricesV2
{
	mat4 ProjectionV2;
	mat4 CameraV2;
};

uniform mat4 Model;

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
	pos = (Camera * Model * vec4(Point, 1.0)).xyz;
	lpos = (Camera * vec4(Light, 1.0)).xyz;
	gl_Position = ProjectionV2 * CameraV2 * Model * vec4(Point, 1.0);
	gl_PointSize = gl_Position.z / 5;

	normal = Normal;
	tangent = Tangent;
	bitangent = Bitangent;
	texcoord = Texcoord;
}
