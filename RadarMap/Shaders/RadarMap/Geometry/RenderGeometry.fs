uniform vec2 vpSize;

bool IntersectPlane(vec2 azR, out vec3[2] pos);
bool InsideTriangle(vec3 curPos);
float SignalFromPos(vec3 mwPos);

bool AccumulateSignal(in vec3 pos, vec2 azR, inout float signal)
{
	if (pos.y > 0 && pos.z < 0 && InsideTriangle(pos))
	{
		signal = signal + SignalFromPos(pos);
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