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

mat4 rotationY( float angle );
mat4 rotationZ( float angle );

float random (in vec2 st);
float noise (in vec2 st);

void main()
{
	vec3 finalModelPosition = modelPosition;
	finalModelPosition += oOffset.xyz;

	vec2 uv = finalModelPosition.zy / vec2(256);
	uv.x += oOffset.w / 4.f; // w being the timer
    uv.y += oOffset.w / 2.f;
	
	float f = 0.0;
    uv *= .7;
    mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );
	f  = 0.5000*noise( uv ); uv = m*uv;
	f += 0.2500*noise( uv ); uv = m*uv;
	f += 0.1250*noise( uv ); uv = m*uv;
	f += 0.0625*noise( uv ); uv = m*uv;

	finalModelPosition.z += f * 7.f;

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

// 2D Random
float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners percentages
    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}