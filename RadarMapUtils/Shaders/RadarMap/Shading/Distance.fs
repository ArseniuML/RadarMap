varying vec3 E[3];
varying vec3 T;
varying vec3 Q;

float Distance(vec3 pos)
{
	vec3 P = cross(pos, E[2]);
	float DV = dot(P, E[0]);
	
	vec3 barCoord; 
	barCoord.gb = vec2(dot(P, T), dot(Q, pos)) / DV;
	barCoord.r = 1 - barCoord.g - barCoord.b;
	
	if (all(greaterThanEqual(barCoord, vec3(0))) & all(lessThanEqual(barCoord.gb, vec2(1))))
		return dot(Q, E[2]) / DV;
	
	return -1;
}