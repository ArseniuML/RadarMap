uniform vec2 vpSize;
uniform float height;

struct Material
{
	float Brightness;
};
uniform Material material;

float Reflection(Material mat, float cosLia);

void main()
{
	float range = (gl_ProjectionMatrixInverse * vec4(2.0 * gl_FragCoord.xy / vpSize - vec2(1.0), 0.0, 1.0)).y;
	if (range == 0) discard;
	gl_FragColor.r = Reflection(material, height / range);
}
