#version 410 core

in vec3 normal;
in vec3 tangent;
in vec2 texcoord;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	FragColor = texture(ourTexture, texcoord);
}
