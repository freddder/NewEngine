#version 330 core

in vec4 vPosition;
in vec4 vNormal;
in vec4 vUVx2;
in vec4 oOffset;

//uniform vec3 modelPosition;
//uniform mat4 modelOrientationX;
//uniform mat4 modelOrientationY;
//uniform mat4 modelOrientationZ;
//uniform mat4 modelScale;

uniform int scrWidth;
uniform int scrHeight;
uniform float swingDegree;

out vec4 fUVx2;

void main()
{
	fUVx2 = vUVx2;

	float finalPosX = (vPosition.x * 100 / float(scrWidth)) + oOffset.x;
	float finalPosY = (vPosition.y * 100 / float(scrHeight)) + oOffset.y;

	finalPosX += sin(radians(gl_InstanceID * 30 + swingDegree)) / 50;

	//gl_Position = vec4((vPosition.x * 100 / scrWidth) + modelPosition.x, (vPosition.y * 100 / scrHeight) + modelPosition.y, 0.f, 1.f);
	gl_Position = vec4(finalPosX, finalPosY, 0.f, 1.f);
}