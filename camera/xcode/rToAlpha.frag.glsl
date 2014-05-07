#version 120

uniform sampler2D tex0;


void main()
{
	vec4 tex = texture2D( tex0, gl_TexCoord[0].xy );
	gl_FragColor.rgb = tex.rgb;
	gl_FragColor.a = tex.r;
}