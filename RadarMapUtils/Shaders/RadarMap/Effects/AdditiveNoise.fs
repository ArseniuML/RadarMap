uniform sampler2DRect texInput;

struct NoiseParams
{
	float Mean;
	float Dev;
};
uniform NoiseParams noiseParams;

vec2 NoiseVal(vec2 coord);

void main()
{	
	float image = texture2DRect(texInput, gl_FragCoord.xy).r;
	gl_FragColor.r = image + noiseParams.Mean + noiseParams.Dev * NoiseVal(gl_FragCoord.xy).r;
}