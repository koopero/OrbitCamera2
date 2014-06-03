
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

	float zz = pow( (source.a), 1.1 );
	
	pos.z = zz * 6.0 - 6.0 * 0.8;
	pos.xy = pos.xy / ( zz + 0.6 ) ;
	//gl_Position.xyz = pos;
	
	vec4 screenPos = gl_ModelViewProjectionMatrix * pos;
	
	float r = length( screenPos.xy );
	r = pow( r, 1.18 - zz * 0.18 ) / 1.3;
	
	screenPos.xy = normalize( screenPos.xy ) * r;
	
	gl_Position = screenPos;
	//gl_Position.x += z * 0.2;
	//gl_Position.y += z * 0.1;
	//gl_Position.z = z;

	colourMult = vec4( 0.0,0.0,0.0,0.0 );
	colourAdd = mix( colour0, colour1, z ) * source;
	
	
	
	//colourMult.rgb *= 1.0 - colourDetail;
	//colourAdd = source * ( 1.0 - colourDetail ) * colourMult;

	//colourAdd.r += offCentre;
	//colourAdd.a = z;

}
