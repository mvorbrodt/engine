#version 410 core

in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec2 texcoord;

uniform sampler2D texture1;
uniform bool show_normal;
uniform float brightness;

out vec4 FragColor;

void main()
{
	FragColor = show_normal ?
		vec4(normal, 1.0) * vec4(brightness, brightness, brightness, 1.0) :
		texture(texture1, texcoord) * vec4(brightness, brightness, brightness, 1.0);
}
