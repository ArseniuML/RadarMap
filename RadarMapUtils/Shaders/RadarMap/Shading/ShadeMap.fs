uniform vec2 vpSize;

vec3 Decart(vec2 pos);
float Distance(vec3 curPos);

void main()
{	
	vec2 crvPos = (gl_ProjectionMatrixInverse * vec4(2.0 * gl_FragCoord.xy / vpSize - vec2(1.0), 0.0, 1.0)).xy;
	vec3 pos = Decart(crvPos);
	
	float d = Distance(pos); if (d < 0) discard;
	pos = d * pos;
	
	gl_FragColor.rg = vec2((3000+ pos.z) / 2000, d);
	gl_FragDepth = (d - gl_DepthRange.near) / gl_DepthRange.diff;
}
