#version 400

in vec3 VertexPosition;
in vec3 VertexColor;
in vec3 VertexNormal;

out float ColorFactor;
out vec3 Color;

uniform vec3 LightDirection;
uniform mat4 RotationMatrix;
uniform mat4 ProjModelViewMatrix;

void main()
{
	Color = VertexColor;
	ColorFactor = max(dot(VertexNormal, LightDirection), 0.0);	

	gl_Position = ProjModelViewMatrix * vec4(VertexPosition, 1.0);
}
