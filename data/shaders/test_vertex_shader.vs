#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aColor;

uniform mat4 Projection;
uniform mat4 ModelView;

out vec3 ourColor;
out vec2 texCoord;

void main()
{
	gl_Position = Projection * ModelView * vec4(aPos, 1.0);
	ourColor = aColor;
	texCoord = aTex;
}
