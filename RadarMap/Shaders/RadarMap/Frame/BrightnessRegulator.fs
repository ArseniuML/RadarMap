uniform sampler2DRect texInput;
uniform float brightness;

void main()
{
	float regulatedSignal = brightness * texture2DRect(texInput, gl_FragCoord.xy).r;
	gl_FragColor = vec4(regulatedSignal, regulatedSignal, regulatedSignal, 1);
}