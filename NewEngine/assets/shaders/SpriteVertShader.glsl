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

uniform int numCols;
uniform int numRows;
uniform int spriteId;

out vec4 fUVx2;
out vec3 fNormal;
out vec4 fVertPosLightSpace;
out vec4 fVertWorldPosition;

void main()
{
	vec4 finalModelPosition = vec4(modelPosition, 1.0);
	finalModelPosition.x += oOffset.x;
	finalModelPosition.y += oOffset.y;
	finalModelPosition.z += oOffset.z;

	mat4 translateMatrix;
	translateMatrix[0] = vec4(1,0,0,0);
	translateMatrix[1] = vec4(0,1,0,0);
	translateMatrix[2] = vec4(0,0,1,0);
	translateMatrix[3] = vec4(finalModelPosition.x,finalModelPosition.y,finalModelPosition.z,1);

	mat4 model;
	model[0] = vec4(1,0,0,0);
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

	fVertWorldPosition = model * vPosition;

	vec2 newUV = vec2(vUVx2.x / float(numCols), vUVx2.y / float(numRows));

	float addU = (float(1) / float(numCols)) * (spriteId % numCols);
	float addV = (float(1) / float(numRows)) * (numRows - (spriteId / numCols));

	fUVx2 = vec4(newUV.x + addU, newUV.y - addV, 0, 0);

	fNormal = mat3(transpose(inverse(model))) * vNormal.xyz;
	fVertPosLightSpace = lightSpace * fVertWorldPosition;
}