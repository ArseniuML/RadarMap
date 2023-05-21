varying in vec2 posCrv[3];

varying out vec3 n2;
varying out float h2;

varying out vec3 bcn0;
varying out vec3 bcn2;
varying out float vol;

vec2 Curve(vec3 pos);
bool InsideTriangle(vec3 pos, vec3 bcn0, vec3 bcn2, float vol);
bool IntersectLines(vec2 n1, float h1, vec2 n2, float h2, out vec2 pos);

void main(void)
{
	vec3 E[3];	
	E[0] = (gl_PositionIn[1] - gl_PositionIn[0]).xyz;
	E[1] = (gl_PositionIn[2] - gl_PositionIn[1]).xyz;
	E[2] = (gl_PositionIn[0] - gl_PositionIn[2]).xyz;
	
	vec3 EL2 = vec3(dot(E[0], E[0]), dot(E[1], E[1]), dot(E[2], E[2]));
	if (any(lessThan(EL2, vec3(0.000001)))) return;
	
	vec3 crs = cross(E[0], -E[2]);
	n2 = normalize(crs);
	h2 = dot(n2, gl_PositionIn[0].xyz);

	bcn0 = cross(-gl_PositionIn[0].xyz, E[0]);
	bcn2 = cross(-gl_PositionIn[0].xyz, E[2]);
	vol = dot(-gl_PositionIn[0].xyz, crs);

	if (!InsideTriangle(n2, bcn0, bcn2, vol))
	{
		vec2 nrmCrv[3];
		nrmCrv[0] = posCrv[1] - posCrv[0];
		nrmCrv[1] = posCrv[2] - posCrv[1];
		nrmCrv[2] = posCrv[0] - posCrv[2];

		nrmCrv[0] = normalize(vec2(-nrmCrv[0].y, nrmCrv[0].x));
		nrmCrv[1] = normalize(vec2(-nrmCrv[1].y, nrmCrv[1].x));
		nrmCrv[2] = normalize(vec2(-nrmCrv[2].y, nrmCrv[2].x));

		float sf = -sign(dot(nrmCrv[0], posCrv[2] - posCrv[0]));
		nrmCrv[0] = nrmCrv[0] * sf;
		nrmCrv[1] = nrmCrv[1] * sf;
		nrmCrv[2] = nrmCrv[2] * sf;
		
		vec2 median[3];
		median[0] = Curve(0.5 * (gl_PositionIn[0] + gl_PositionIn[1]).xyz);
		median[1] = Curve(0.5 * (gl_PositionIn[1] + gl_PositionIn[2]).xyz);
		median[2] = Curve(0.5 * (gl_PositionIn[2] + gl_PositionIn[0]).xyz);

		float hCrv[3];
		hCrv[0] = max(dot(nrmCrv[0], posCrv[0]), dot(nrmCrv[0], median[0]));
		hCrv[1] = max(dot(nrmCrv[1], posCrv[1]), dot(nrmCrv[1], median[1]));
		hCrv[2] = max(dot(nrmCrv[2], posCrv[2]), dot(nrmCrv[2], median[2]));

		vec2 vPos[3];
		if (IntersectLines(nrmCrv[0], hCrv[0], nrmCrv[1], hCrv[1], vPos[1]) &&
			IntersectLines(nrmCrv[1], hCrv[1], nrmCrv[2], hCrv[2], vPos[2]) &&
			IntersectLines(nrmCrv[2], hCrv[2], nrmCrv[0], hCrv[0], vPos[0]))
		{
			gl_Position = gl_ProjectionMatrix * vec4(vPos[0], 0, 1); EmitVertex();
			gl_Position = gl_ProjectionMatrix * vec4(vPos[1], 0, 1); EmitVertex();
			gl_Position = gl_ProjectionMatrix * vec4(vPos[2], 0, 1); EmitVertex();

			EndPrimitive();		
		}
	}
	else
	{
		vec4 cn = vec4
		(
			min(posCrv[0].x, min(posCrv[1].x, posCrv[2].x)),
			abs(h2),
			max(posCrv[0].x, max(posCrv[1].x, posCrv[2].x)),
			max(posCrv[0].y, max(posCrv[1].y, posCrv[2].y))
		);
		float gb = 0.05 * (cn.w - cn.y);
		cn.yw = cn.yw + 0.05 * vec2(-gb, gb);
		
		gl_Position = gl_ProjectionMatrix * vec4(cn.xy, 0, 1); EmitVertex();
		gl_Position = gl_ProjectionMatrix * vec4(cn.zy, 0, 1); EmitVertex();
		gl_Position = gl_ProjectionMatrix * vec4(cn.xw, 0, 1); EmitVertex();
		EndPrimitive();
		
		gl_Position = gl_ProjectionMatrix * vec4(cn.xw, 0, 1); EmitVertex();
		gl_Position = gl_ProjectionMatrix * vec4(cn.zy, 0, 1); EmitVertex();
		gl_Position = gl_ProjectionMatrix * vec4(cn.zw, 0, 1); EmitVertex();
		EndPrimitive();
	}
}	
