#version 330 core

in vec4 vPosition;
in vec4 vNormal;
in vec4 vUVx2;
in vec4 oOffset;

uniform mat4 view;
uniform mat4 projection;
//uniform mat4 model;

uniform vec3 modelPosition;
uniform mat4 modelOrientationX;
uniform mat4 modelOrientationY;
uniform mat4 modelOrientationZ;
uniform mat4 modelScale;
uniform mat4 lightSpaceMatrix;

uniform bool isShadowPass;

uniform int isTextureAnimated;
uniform int numCols;
uniform int numRows;
uniform int spriteId;
uniform vec2 UVoffset;
uniform vec3 globalShiftingRatios;

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

	if(isShadowPass)
	{
		gl_Position = lightSpaceMatrix * model * vPosition;
	}
	else
	{
		gl_Position = MVP * vPosition;
	}

	vec3 fragPos = vec3(model * vPosition);

	if(isTextureAnimated == 1)
	{
		vec2 newUV = vec2(vUVx2.x / float(numCols), vUVx2.y / float(numRows));

		float addU = (float(1) / float(numCols)) * (spriteId % numCols);
		float addV = (float(1) / float(numRows)) * (numRows - (spriteId / numCols));

		fUVx2 = vec4(newUV.x + addU, newUV.y - addV, 0, 0);
	}
	else if(isTextureAnimated == 2)
	{
		fUVx2 = vec4(vUVx2.x + UVoffset.x, vUVx2.y + UVoffset.y, vUVx2.z, vUVx2.w);
		//fUVx2 = vec4((model * vPosition).x * 0.35 + UVoffset.x, (model * vPosition).z * 0.35 + UVoffset.y, vUVx2.z, vUVx2.w);
	}
	else if(isTextureAnimated == 3)
	{
		fUVx2 = vec4((model * vPosition).x * globalShiftingRatios.x + UVoffset.x, (model * vPosition).z * globalShiftingRatios.z + UVoffset.y, vUVx2.z, vUVx2.w);
	}
	else
	{
		fUVx2 = vUVx2;
	}

	fNormal = mat3(transpose(inverse(model))) * vNormal.xyz;
	//fVertPosLightSpace = lightSpaceMatrix * model * vPosition;
	fVertPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.f);
}