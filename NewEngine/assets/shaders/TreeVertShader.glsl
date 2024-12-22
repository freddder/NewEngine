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

uniform vec3 modelPosition;
uniform mat4 modelOrientationX;
uniform mat4 modelOrientationY;
uniform mat4 modelOrientationZ;
uniform mat4 modelScale;

uniform float timer;
uniform float windSpeed;

out vec4 fUVx2;
out vec3 fNormal;
out vec4 fVertPosLightSpace;
out vec4 fVertWorldPosition;

float random (in vec2 st);
float noise (in vec2 st);

void main()
{
	vec4 finalModelPosition = vec4(modelPosition, 1.0);
	finalModelPosition += oOffset;

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

	// How zoomed into noise (resolution)
	// Use xz coords to sample noise
	vec2 uvSample = (finalModelPosition.xz * 50.f) / vec2(256);
	uvSample.x += timer / (1.f / windSpeed);
	uvSample.y += timer / (1.f / windSpeed);

	// f is the noise generated value with the sample
	float f;
    mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );
	f  = 0.5000 * noise(uvSample); uvSample = m * uvSample;
	f += 0.2500 * noise(uvSample); uvSample = m * uvSample;
	f += 0.1250 * noise(uvSample); uvSample = m * uvSample;
	f += 0.0625 * noise(uvSample); uvSample = m * uvSample;

	// Im going to assume all the vertices that will be moved to be between a specific y level
	vec4 newVertPos = vPosition;
	if (newVertPos.y > 0.1)
	{
		float shakeMultiplier = .75f;
		newVertPos.x += (f - 0.5f) * shakeMultiplier;
		newVertPos.z += (f - 0.5f) * shakeMultiplier;
	}

	mat4 MVP = projection * view * model;

	if(isShadowPass == 1)
	{
		gl_Position = lightSpace * model * newVertPos;
	}
	else
	{
		gl_Position = MVP * newVertPos;
	}

	fVertWorldPosition = model * newVertPos;
	fUVx2 = vUVx2;

	fNormal = mat3(transpose(inverse(model))) * vNormal.xyz;
	fVertPosLightSpace = lightSpace * fVertWorldPosition;
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