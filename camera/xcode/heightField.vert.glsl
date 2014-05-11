
uniform sampler2D image;

uniform float depth0;
uniform float zMid;
uniform float depth1;

uniform float z0;
uniform float z1;


uniform float colourDetail;

uniform vec3 pos0;
uniform vec3 pos1;

uniform vec3 scale0;
uniform vec3 scale1;

uniform vec4 colour0;
uniform vec4 colour1;





varying vec4 colourMult;
varying vec4 colourAdd;

void main()
{
	vec4 source = texture2D( image,  gl_MultiTexCoord0.xy );
	gl_TexCoord[0] = gl_MultiTexCoord0;

	float z = source.a;
	z = ( z - depth0 ) / ( depth1 - depth0 );
	z = clamp( z, 0.0, 1.0 );
	

	//vec3 pos = mix( pos0, pos1, z );
	//vec3 scale = mix( scale0, scale1, z );
	
	vec4 pos = gl_Vertex;

	pos.z = mix(z0,z1,z);
	
	//gl_Position.xyz = pos;
	gl_Position = gl_ModelViewProjectionMatrix * pos;
	//gl_Position.x += z * 0.2;
	//gl_Position.y += z * 0.1;
	//gl_Position.z = z;

	colourMult = mix( colour0, colour1, z );
	colourAdd = vec4( 0.0,0.0,0.0,0.0 );
	
	
	
	//colourMult.rgb *= 1.0 - colourDetail;
	colourAdd = source * ( 1.0 - colourDetail ) * colourMult;
	colourMult *= colourDetail;
	//colourAdd.r += offCentre;
	//colourAdd.a = z;

}
