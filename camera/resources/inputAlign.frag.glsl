#version 120

uniform sampler2D tex0;
void main()
{
	vec4 tex = texture2D( tex0, gl_TexCoord[0].xy );
	gl_FragColor.rgb = tex.rgb;
	gl_FragColor.b = 1.0 - tex.a;
	
	gl_FragColor.a = 1.0;
}