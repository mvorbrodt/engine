#version 330 core

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D ourTexture1;

out vec4 FragColor;

void main()
{
	FragColor = texture(ourTexture1, texCoord);
}
