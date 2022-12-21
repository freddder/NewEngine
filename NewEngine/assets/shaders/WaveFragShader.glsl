#version 330 core

in vec4 fUVx2;
in vec3 fNormal;
in vec4 fVertPosLightSpace;
in vec4 fVertWorldPosition;

struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	//vec4 specular;	// rgb = highlight colour, w = power
	//vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;		// Spot or directional lights
	vec4 extraParam;	// x = lightType, y = inner angle, z = outer angle, w = 0 for off, 1 for on
							// 0 = pointlight
							// 1 = spot light
							// 2 = directional light
};

layout (std140) uniform Lights
{
    sLight theLights[20];
};

layout (std140) uniform Fog
{
	vec4 fogViewOrigin;
	vec4 fogColor;
	float fogDensity;
	float fogGradient;
};

// texture samplers
uniform sampler2D texture_0;
uniform sampler2D shadowMap;

uniform bool useWholeColor;
uniform vec4 wholeColor;

uniform float timer;

float ShadowCalculation(vec4 fragPosLightSpace);
float noise (in vec2 st);
float random (in vec2 st);

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

	// ambient
    vec3 ambient = 0.4 * vertColor.rgb;

	vec3 norm = normalize(fNormal);

	// diffuse 
    vec3 lightDir = normalize(-theLights[0].direction.xyz);
    float diff = max(dot(lightDir, norm), 0.0);
    //vec3 diffuse = theLights[0].diffuse.rgb * diff * vertColor.rgb;
    vec3 diffuse = diff * theLights[0].diffuse.rgb;

	float shadow = ShadowCalculation(fVertPosLightSpace);

	vec2 p = fVertWorldPosition.xz / vec2(5);

	vec2 uv = p*vec2(256/256,1.0);
    uv.x += timer / 2;
    uv.y += timer / 2;

	vec2 uv2 = p*vec2(256/256,1.0);
    uv2.x += timer / 2;
    uv2.y -= timer / 2;
	
	float f = 0.0;
	float f2 = 0.0;

    uv *= 8.0;
    mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );
	f  = 0.5000*noise( uv ); uv = m*uv;
	f += 0.2500*noise( uv ); uv = m*uv;
	f += 0.1250*noise( uv ); uv = m*uv;
	f += 0.0625*noise( uv ); uv = m*uv;

    uv2 *= 8.0;
    mat2 m2 = mat2( 1.6,  1.2, -1.2,  1.6 );
	f2  = 0.5000*noise( uv2 ); uv2 = m2*uv2;
	f2 += 0.2500*noise( uv2 ); uv2 = m2*uv2;
	f2 += 0.1250*noise( uv2 ); uv2 = m2*uv2;
	f2 += 0.0625*noise( uv2 ); uv2 = m2*uv2;

    f = mix(f, f2, 0.5);

	vec3 pixelColor;

	if(f >= 0.7f && shadow < 0.5)
	{
		pixelColor = mix(vertColor.xyz, vec3(f), 0.8f);
	}
	else
	{
		pixelColor = (ambient + (1.0 - shadow) * (diffuse)) * vertColor.xyz;
	}

	float distanceToFogOrigin = length(fVertWorldPosition.xyz - fogViewOrigin.xyz);
	float fFogVisibility = exp(-pow(distanceToFogOrigin * fogDensity, fogGradient));
	fFogVisibility = clamp(fFogVisibility, 0.0, 1.0);
	pixelColor = mix(fogColor.rgb, pixelColor, fFogVisibility);

	gl_FragColor = vec4(pixelColor, 1.f);
	//gl_FragColor = vec4(1.f, 0.f, 0.f, 1.f);
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // check whether current frag pos is in shadow
    float bias = 0.002;
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
	    for(int y = -1; y <= 1; ++y)
	    {
	        float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
	        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
	    }    
	}
	shadow /= 9.0;

    return shadow;
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