#version 410 core

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

uniform mat4 Model;

out vec4 color;
out vec2 texcoord;

void main()
{
	gl_Position = Projection * Camera * Model * vec4(Point, 1.0);

	color = Color;
	texcoord = Texcoord;
}
