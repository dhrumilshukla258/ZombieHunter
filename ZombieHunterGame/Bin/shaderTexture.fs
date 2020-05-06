#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform vec3 blood;
void main()
{
	vec4 b = vec4(blood, 1);
	FragColor = texture(texture1, TexCoord) * b;
}