#version 400

in vec3 Color;
in float ColorFactor;

layout (location = 0) out vec4 FragColor;

void main()
{
	FragColor = ColorFactor * vec4(Color, 1.0);
}
