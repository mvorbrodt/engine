#version 450 core

in vec4 color;
in vec2 texcoord;

uniform sampler2D texture1;

out vec4 FragColor;

void main()
{
	//FragColor = vec4(texture(texture1, texcoord).xyz, 1.0) * color;
	FragColor = color;
}
