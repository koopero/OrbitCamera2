#version 120

uniform sampler2D image;

varying vec4 colourMult;
varying vec4 colourAdd;

void main()
{
	vec4 tex = texture2D( image, gl_TexCoord[0].xy );
	
	
	gl_FragColor.rgb = tex.rgb;
	gl_FragColor.a = tex.a;
	gl_FragColor = gl_FragColor * colourMult + colourAdd;
	
	//gl_FragColor.rgb = source.rgb;
}