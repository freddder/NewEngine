#version 330 core

in vec4 vPosition;
in vec4 vNormal;
in vec4 vUVx2;
in vec4 oOffset;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
	mat4 lightSpace;
	int isShadowPass;
};

uniform vec3 cameraPosition;
uniform vec3 modelPosition;
uniform mat4 modelScale;

out vec4 fUVx2;
out vec3 fNormal;
out vec4 fVertPosLightSpace;
out vec4 fVertWorldPosition;

mat4 rotationY( float angle );
mat4 rotationZ( float angle );

void main()
{
	vec3 finalModelPosition = modelPosition;
	finalModelPosition.x += oOffset.x;
	finalModelPosition.y += oOffset.y;
	finalModelPosition.z += oOffset.z;

	vec3 playerDir = finalModelPosition - cameraPosition;

	float distance2D = sqrt(playerDir.x * playerDir.x + playerDir.y * playerDir.y);
	float theta = atan(playerDir.y, playerDir.x);
	float phi = atan(-playerDir.z, distance2D);

	mat4 model = mat4(1.f);
	model[3] = vec4(finalModelPosition, 1.f);
	
	model = model * rotationZ(-theta) * rotationY(-phi);

	model = model * modelScale;

	mat4 MVP = projection * view * model;

	if(isShadowPass == 1)
	{
		gl_Position = lightSpace * model * vPosition;
	}
	else
	{
		gl_Position = MVP * vPosition;
	}

	fVertWorldPosition = model * vPosition;
	fUVx2 = vUVx2;

	fNormal = mat3(transpose(inverse(model))) * vNormal.xyz;
	fVertPosLightSpace = lightSpace * fVertWorldPosition;
}

mat4 rotationY( float angle ) 
{
	return mat4(	cos(angle),		0,		sin(angle),	0,
			 				0,		1.0,			 0,	0,
					-sin(angle),	0,		cos(angle),	0,
							0, 		0,				0,	1);
}

mat4 rotationZ( float angle ) 
{
	return mat4(	cos(angle),		-sin(angle),	0,	0,
			 		sin(angle),		cos(angle),		0,	0,
							0,				0,		1,	0,
							0,				0,		0,	1);
}