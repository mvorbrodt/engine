#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;
layout (location = 2) in vec2 aTex;

uniform mat4 Projection;
uniform mat4 Camera;
uniform mat4 Model;

out vec4 color;
out vec2 texcoord;

void main()
{
	gl_Position = Projection * Camera * Model * vec4(aPos, 1.0);

	color = aCol;
	texcoord = aTex;
}
