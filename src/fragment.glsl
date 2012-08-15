#version 400

in vec3 LightIntensity;
in vec3 Color;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

subroutine vec4 ColoringType();
subroutine uniform ColoringType colorModel;

uniform sampler2D Tex1;

subroutine(ColoringType)
vec4 PlainColor()
{
	return vec4(Color, 1.0);
}

subroutine(ColoringType)
vec4 ShadeColor()
{
	return vec4(LightIntensity, 1.0);
}

subroutine(ColoringType)
vec4 ShadeTexColor()
{
	vec4 texColor = texture(Tex1, TexCoord);
	return texColor * vec4(LightIntensity, 1.0);
}

void main()
{
	FragColor = colorModel();
	//float a = (LightIntensity.x);
	//float b = (LightIntensity.y);
	//float c = (LightIntensity.z);
	//FragColor = vec4(a, 0, 0, 1.0);
}


