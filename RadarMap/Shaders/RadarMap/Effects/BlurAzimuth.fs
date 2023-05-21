uniform sampler2DRect texInput;

const int maxKernelSize = 32;
uniform float kernelValue[maxKernelSize];
uniform int kernelSize;

float inputVal(int offset)
{
	return texture2DRect(texInput, gl_FragCoord.xy + vec2(offset, 0)).r;
}

void main()
{	
	float result = 0;
	
	result = result + kernelValue[0] * inputVal(0);
	for (int i = 1; i < kernelSize && i < maxKernelSize; ++i)
	{
		result = result + kernelValue[i] * (inputVal(-i) + inputVal(i));
	}
	
	gl_FragColor.r = result;
}