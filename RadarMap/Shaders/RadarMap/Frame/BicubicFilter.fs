uniform sampler2DRect kernelTex;
uniform int kernelSize;

vec4 cubicFilter(float xValue, vec4 c0, vec4 c1, vec4 c2, vec4 c3)
{
	vec4 h = texture2DRect(kernelTex, vec2(xValue * float(kernelSize), 0.5));
	return h.r*c0 + h.g*c1+ h.b*c2 + h.a*c3;
}

vec4 texture2DRect_bicubic(sampler2DRect tex, vec2 t)
{
	t = t - vec2(0.5);
	vec2 f = fract(t); 
	t = floor(t) + vec2(0.5);
	
	vec4 t0 = cubicFilter(f.x,
							texture2DRect(tex, t + vec2(-1, -1)),
							texture2DRect(tex, t + vec2(0, -1)),
							texture2DRect(tex, t + vec2(1, -1)),
							texture2DRect(tex, t + vec2(2, -1)));
	vec4 t1 = cubicFilter(f.x,
							texture2DRect(tex, t + vec2(-1, 0)),
							texture2DRect(tex, t + vec2(0, 0)),
							texture2DRect(tex, t + vec2(1, 0)),
							texture2DRect(tex, t + vec2(2, 0)));
	vec4 t2 = cubicFilter(f.x,
							texture2DRect(tex, t + vec2(-1, 1)),
							texture2DRect(tex, t + vec2(0, 1)),
							texture2DRect(tex, t + vec2(1, 1)),
							texture2DRect(tex, t + vec2(2, 1)));
	vec4 t3 = cubicFilter(f.x,
							texture2DRect(tex, t + vec2(-1, 2)),
							texture2DRect(tex, t + vec2(0, 2)),
							texture2DRect(tex, t + vec2(1, 2)),
							texture2DRect(tex, t + vec2(2, 2)));
	return cubicFilter(f.y, t0, t1, t2, t3);
}