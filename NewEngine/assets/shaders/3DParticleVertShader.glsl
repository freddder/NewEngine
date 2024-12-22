#version 330 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vNormal;
layout (location = 2) in vec4 vUVx2;
layout (location = 3) in vec4 oOffset;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
	mat4 lightSpace;
	int isShadowPass;
};

uniform vec3 cameraPosition;
uniform vec3 modelPosition;
uniform vec3 modelScale;

out vec4 fUVx2;
out vec3 fNormal;
out vec4 fVertPosLightSpace;
out vec4 fVertWorldPosition;

void main()
{
	vec3 finalModelPosition = modelPosition;
	finalModelPosition += oOffset.xyz;

	vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
	vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);

	vec3 finalVertPos = 
		finalModelPosition
		+ cameraRight * vPosition.x * modelScale.x
		+ cameraUp * vPosition.y * modelScale.y;

	mat4 VP = projection * view;

	gl_Position = VP * vec4(finalVertPos, 1.f);

	fVertWorldPosition = vec4(finalVertPos, 1);

	fUVx2 = vUVx2;

	// TODO: figure out how to get the correct normal without a model matrix
	//fNormal = mat3(transpose(inverse(model))) * vNormal.xyz;
	fNormal = vec3(1);
	fVertPosLightSpace = lightSpace * fVertWorldPosition;
}