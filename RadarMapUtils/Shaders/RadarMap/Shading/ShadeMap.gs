varying in vec2 posCrv[3];

varying out vec3 E[3];
varying out vec3 T;
varying out vec3 Q;

float cross2(vec2 a, vec2 b) 
{
	return a.x * b.y - a.y * b.x;
}

bool intersect(vec3 L, vec3 M, out vec2 I)
{
	float den = cross2(L.xy, M.xy);	if (den == 0) return false;
	I = vec2(cross2(L.yz, M.yz), -cross2(L.xz, M.xz)) / den;
	return true;
}

vec2 Curve(vec3 pos);

void main(void)
{
	E[0] = (gl_PositionIn[1] - gl_PositionIn[0]).xyz;
	E[1] = (gl_PositionIn[2] - gl_PositionIn[1]).xyz;
	E[2] = (gl_PositionIn[2] - gl_PositionIn[0]).xyz;
	
	vec3 E2 = vec3(dot(E[0], E[0]), dot(E[1], E[1]), dot(E[2], E[2]));
	if (any(lessThan(E2, vec3(0.000001)))) return;
	
	T = - gl_PositionIn[0].xyz;
	Q = cross(T, E[0]);
		
	vec2 edgeCrv[3];
	edgeCrv[0] = posCrv[1] - posCrv[0];
	edgeCrv[1] = posCrv[2] - posCrv[1];
	edgeCrv[2] = posCrv[0] - posCrv[2];
	
	vec2 median[3];
	median[0] = Curve(0.5 * (gl_PositionIn[0] + gl_PositionIn[1]).xyz);
	median[1] = Curve(0.5 * (gl_PositionIn[1] + gl_PositionIn[2]).xyz);
	median[2] = Curve(0.5 * (gl_PositionIn[2] + gl_PositionIn[0]).xyz);

	float frontFaceCrv = sign(cross2(edgeCrv[0], -edgeCrv[2]));
	vec3 lineEq[3];
	for (int i = 0; i < 3; ++i)
	{
		if (frontFaceCrv * cross2(edgeCrv[i], median[i] - posCrv[i]) < 0)
			lineEq[i] = vec3(edgeCrv[i].y, -edgeCrv[i].x, cross2(edgeCrv[i], posCrv[i] + 1.2 * (median[i] - posCrv[i])));
		else
			lineEq[i] = vec3(edgeCrv[i].y, -edgeCrv[i].x, cross2(edgeCrv[i], posCrv[i]));
	}
	
	vec2 vPos[3];
	if (intersect(lineEq[0], lineEq[1], vPos[0]) &&
		intersect(lineEq[1], lineEq[2], vPos[1]) &&
		intersect(lineEq[2], lineEq[0], vPos[2]))
	{
		gl_Position = gl_ProjectionMatrix * vec4(vPos[0], 0, 1); EmitVertex();
		gl_Position = gl_ProjectionMatrix * vec4(vPos[1], 0, 1); EmitVertex();
		gl_Position = gl_ProjectionMatrix * vec4(vPos[2], 0, 1); EmitVertex();
		EndPrimitive();		
	}
}	