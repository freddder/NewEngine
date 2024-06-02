#version 330 core
in vec2 textureCoords;
in float flip;

out vec4 FragColor;

uniform sampler2D texture_0;
uniform vec3 color;
uniform bool testFlip;

void main()
{
	if (testFlip)
	{
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture_0, textureCoords).r);

		if (sampled.a < 0.1)
			discard;

		FragColor = vec4(color, 1.0) * sampled;
	}
	else
	{

		if (flip == 0) {
			FragColor = vec4(1.f, 0.f, 0.f, 1.0);
		}
		else {
			FragColor = vec4(1.0);
		}
	}
}