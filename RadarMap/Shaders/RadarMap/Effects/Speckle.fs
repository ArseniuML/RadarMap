uniform sampler2DRect texInput;
uniform int numLooks;

vec2 NoiseVal(vec2 coord);

void main()
{	
	float image = texture2DRect(texInput, gl_FragCoord.xy).r;
	gl_FragColor.r = image * NoiseVal(gl_FragCoord.xy).g / (float)numLooks;
}