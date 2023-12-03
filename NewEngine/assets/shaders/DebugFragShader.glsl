#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D testTexture;

// depth map stuff
//uniform sampler2D depthMap;
//uniform float near_plane;
//uniform float far_plane;

// required when using a perspective projection matrix
//float LinearizeDepth(float depth)
//{
//    float z = depth * 2.0 - 1.0; // Back to NDC 
//    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
//}

uniform float timer;
uniform float threshold;

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

void main()
{
    // -------------------------- UI Texture testing --------------------------
    vec4 color = texture(testTexture, TexCoords);

    if (color.a < 0.1)
			discard;

    FragColor = color; // orthographic

    return;
    // ------------------------------------------------------------------------

    // -------------------------- Shadow depth testing --------------------------
    //float depthValue = texture(depthMap, TexCoords).r;
    ////FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
    //FragColor = vec4(vec3(depthValue), 1.0); // orthographic
    // --------------------------------------------------------------------------

    // -------------------------- Noise testing --------------------------
    // 256 = resolution
    vec2 p = gl_FragCoord.xy / vec2(256);

	vec2 uv = p*vec2(256/256,1.0);
    //uv.x += timer / 2;
    //uv.y += timer / 2;
	
	float f = 0.0;

    uv *= .7;
    mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );
	f  = 0.5000*noise( uv ); uv = m*uv;
	f += 0.2500*noise( uv ); uv = m*uv;
	f += 0.1250*noise( uv ); uv = m*uv;
	f += 0.0625*noise( uv ); uv = m*uv;
	//f += 0.03125*noise( uv ); uv = m*uv;
	//f += 0.015625*noise( uv ); uv = m*uv;

    //vec2 uv2 = p*vec2(256/256,1.0);
    //uv2.x += timer / 2;
    //uv2.y -= timer / 2;
    //
    //float f2 = 0.0;
    //
    //uv2 *= 8.0;
    //mat2 m2 = mat2( 1.6,  1.2, -1.2,  1.6 );
	//f2  = 0.5000*noise( uv2 ); uv2 = m2*uv2;
	//f2 += 0.2500*noise( uv2 ); uv2 = m2*uv2;
	//f2 += 0.1250*noise( uv2 ); uv2 = m2*uv2;
	//f2 += 0.0625*noise( uv2 ); uv2 = m2*uv2;
    //
    //f = mix(f, f2, 0.5);

    //f = 0.5 + 0.5*f;
	
    //f *= smoothstep( 0.0, 0.005, abs(p.x-0.6) );

    //if(f >= threshold)
    //{
    //    FragColor = vec4(vec3(f), 1.f);
    //}
    //else
    //{
    //    FragColor = vec4(vec3(0), 1.f);
    //}

    FragColor = vec4(vec3(f), 1.f);
    // -------------------------------------------------------------------


}