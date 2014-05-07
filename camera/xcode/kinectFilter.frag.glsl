#version 120
#extension GL_ARB_texture_rectangle: enable

uniform sampler2D kinect;
uniform sampler2DRect buffer;
//uniform sampler2D tex0;

//in vec4 gl_FragCoord;
/*in vec3 gl_FragCoord​;*/

void main()
{
	
	
	
	vec2 sampleCoord = gl_TexCoord[0].xy;
	//vec4 depth = texture2DRect( kinectDepth, sampleCoord );

	float depthCutoff = 20.0 / 255.0;
	float depth = texture2D( kinect, sampleCoord / vec2(640.0,480.0) + vec2(0.01,0.0) ).r;
	vec4 buffer = texture2DRect( buffer, sampleCoord );
	vec4 result = buffer + vec4( depth );
	
	/*
	if ( depth > depthCutoff ) {
		result.r += depth;
		result.b += 0.1;
	} else {
		result.g += 0.1;
	}
	*/
	
	//result.r = 1.0;
	
	
	//gl_FragColor.rgb = vec3( gl_FragCoord.x / 640.0, 0.0, 0.2) + color.xyz;
	gl_FragColor = result;
}