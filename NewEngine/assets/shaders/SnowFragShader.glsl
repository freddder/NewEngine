#version 330 core

in vec4 fUVx2;
in vec3 fNormal;
in vec4 fVertPosLightSpace;

// texture samplers
uniform sampler2D texture_0;

uniform bool useWholeColor;
uniform vec4 wholeColor;

void main()
{

	vec4 vertColor;

	if(useWholeColor)
	{
		vertColor = wholeColor;
	}
	else
	{
		vertColor = texture(texture_0, vec2(fUVx2.x, fUVx2.y));

		if(vertColor.a < 0.1)
			discard;
	}

	gl_FragColor = vec4(vertColor.rgb, 1.f);
}