#version 120
#define PI 3.1415926535897932384626433832795

uniform sampler2D tex0;
uniform vec4 multColour;

void main()
{
	vec4 tex = texture2D( tex0, gl_TexCoord[0].xy );

	float blurRad = 0.009;
	
	vec4 blur = vec4( 0.0, 0.0, 0.0, 0.0 );
	
	for ( int i = 0; i < 8; i ++ ) {
		float a = i / 4.0 * PI;
		vec2 offset = vec2( sin( a ), cos( a ) ) * blurRad;
		vec4 blurSample = texture2D( tex0, gl_TexCoord[0].xy + offset );
		blur += blurSample / 8.0;
	}
	gl_FragColor.rgb = blur.rgb;
	gl_FragColor.a = blur.a;
	
	//gl_FragColor *= multColour;
}