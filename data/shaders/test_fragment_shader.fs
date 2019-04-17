#version 410 core

in vec3 pos;
in vec2 texcoord;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;

in vec3 lpos;
in mat4 model_cam;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform bool show_normal;
uniform float brightness;

out vec4 FragColor;

void main()
{
	vec3 n = texture(texture2, texcoord).rgb;
	mat3 TBN = mat3(tangent, bitangent, normal);
	n = normalize(mat3(model_cam) * TBN * n);
	vec3 d = normalize(lpos - pos);

	FragColor = texture(texture1, texcoord) * clamp(dot(n, d), 0.25, 1.0);
}
