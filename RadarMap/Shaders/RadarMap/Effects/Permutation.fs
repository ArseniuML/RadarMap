uniform sampler2DRect texNoise;
uniform sampler2DRect texPermutation;

vec2 NoiseVal(vec2 coord)
{	
	vec2 permCoord = vec2
	(
		texture2DRect(texPermutation, vec2(coord.x, 0.5)).r + 0.5,
		texture2DRect(texPermutation, vec2(coord.y, 1.5)).r + 0.5
	);
	
	return texture2DRect(texNoise, permCoord).rg;
}