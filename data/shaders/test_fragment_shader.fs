#version 410 core

in vec2 texCoord;

uniform sampler2D ourTexture;

out vec4 FragColor;

void main()
{
	FragColor = texture(ourTexture, texCoord);
}
