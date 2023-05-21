uniform vec2 vpSize;
uniform sampler2DRect texInput;
struct Element2f
{
	vec2 Origin;
	vec2 Size;
};
uniform Element2f texInputElement;
uniform float flatBrightness;

bool IntersectPlane(vec2 azR, out vec3[2] pos);
bool InsideTriangle(vec3 curPos);

struct Material
{
	float Brightness;
};
float Reflection(Material mat, float cosLia);

float Input(vec3 mwPos)
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

bool AccumulateSignal(in vec3 pos, vec2 azR, inout float signal)
{
	if (pos.y > 0 && InsideTriangle(pos))
	{
		signal = signal + Input(pos);
		return true;
	}
	return false;
}

void main()
{
	vec2 azR = (gl_ProjectionMatrixInverse * vec4(2.0 * gl_FragCoord.xy / vpSize - vec2(1.0), 0.0, 1.0)).xy;
	
	vec3 pos[2]; 
	if (!IntersectPlane(azR, pos)) discard;
	
	float signal = 0;
	bool set1 = AccumulateSignal(pos[0], azR, signal);
	bool set2 = AccumulateSignal(pos[1], azR, signal);
	if (!(set1 || set2)) discard;

	gl_FragColor.r = signal;
}
