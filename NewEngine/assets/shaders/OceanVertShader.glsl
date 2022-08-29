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

uniform vec3 modelPosition;
uniform mat4 modelOrientationX;
uniform mat4 modelOrientationY;
uniform mat4 modelOrientationZ;
uniform mat4 modelScale;
//uniform mat4 lightSpaceMatrix;

//uniform bool isShadowPass;

uniform vec2 globalUVRatios;
//uniform vec2 UVoffset;

out vec4 fUVx2;
out vec3 fNormal;
out vec4 fVertPosLightSpace;

void main()
{
	vec4 finalModelPosition = vec4(modelPosition, 1.0);
	finalModelPosition.x += oOffset.x;
	finalModelPosition.y += oOffset.y;
	finalModelPosition.z += oOffset.z;

	mat4 translateMatrix;
	translateMatrix[0] = vec4(1,0,0,0); // who knows if this works
	translateMatrix[1] = vec4(0,1,0,0);
	translateMatrix[2] = vec4(0,0,1,0);
	translateMatrix[3] = vec4(finalModelPosition.x,finalModelPosition.y,finalModelPosition.z,1);

	mat4 model;
	model[0] = vec4(1,0,0,0); // who knows if this works
	model[1] = vec4(0,1,0,0);
	model[2] = vec4(0,0,1,0);
	model[3] = vec4(0,0,0,1);

	model = model * translateMatrix;
	model = model * modelOrientationZ;
	model = model * modelOrientationY;
	model = model * modelOrientationX;
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

	vec3 fragPos = vec3(model * vPosition);
	fUVx2 = vUVx2;

	fUVx2.x = fragPos.z * globalUVRatios.x;
	fUVx2.y = fragPos.x * globalUVRatios.y;

//	fUVx2.x += UVoffset.x;
//	fUVx2.y += UVoffset.y;

	fNormal = mat3(transpose(inverse(model))) * vNormal.xyz;
	fVertPosLightSpace = lightSpace * vec4(fragPos, 1.f);
}