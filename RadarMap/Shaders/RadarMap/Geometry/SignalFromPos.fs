uniform vec2 vpSize;
uniform sampler2DRect texInput;
struct Element2f
{
	vec2 Origin;
	vec2 Size;
};
uniform Element2f texInputElement;
uniform float flatBrightness;

struct Material
{
	float Brightness;
};
float Reflection(Material mat, float cosLia);

float SignalFromPos(vec3 mwPos)
{
	if (flatBrightness < 0)
	{
		vec2 absPos = (gl_ModelViewMatrixInverse * vec4(mwPos, 1)).xy;
		vec2 texCrd = (absPos - texInputElement.Origin) / texInputElement.Size;
		vec3 inp = texture2DRect(texInput, texCrd).rgb;

		vec3 normal = gl_NormalMatrix * normalize(vec3(inp.y, -inp.z, 8.0 * texInputElement.Size.x));
		float cosLia = dot(normal, -normalize(mwPos));
		
		Material mat; mat.Brightness = inp.x;
		return Reflection(mat, cosLia);
	}
	return flatBrightness;
}