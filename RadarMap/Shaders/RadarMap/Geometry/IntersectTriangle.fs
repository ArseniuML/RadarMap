varying vec3 n2;
varying float h2;

varying vec3 bcn0;
varying vec3 bcn2;
varying float vol;

bool IntersectPlane(vec2 azR, out vec3[2] pos)
{
	vec3 n1 = vec3(cos(azR.x), sin(azR.x), 0);
	float nn12 = dot(n1, n2);

	if ((nn12) == 1) return false;
	float den = 1 - nn12 * nn12;

	float c1 = (- h2 * nn12) / den;
	float c2 = h2 / den;

	vec3 P0 = c1 * n1 + c2 * n2;
	vec3 crs = cross(n1, n2);

	float aa = dot(crs, crs);
	if (aa == 0) return false;

	float bb = dot(P0, crs);
	float cc = dot(P0, P0) - azR.y * azR.y;

	float D = bb * bb - aa * cc;
	if (D < 0) return false;
	float sqrtD = sqrt(D);

	vec2 t = vec2(-bb + sqrtD, -bb - sqrtD) / aa;

	pos[0] = P0 + t[0] * crs;
	pos[1] = P0 + t[1] * crs;

	return true;
}

bool InsideTriangle(vec3 pos)
{
	vec3 barCoord; 
	barCoord.gb = vec2(dot(pos, bcn2), dot(pos, bcn0)) / vol;
	barCoord.r = 1 - barCoord.g - barCoord.b;

	return all(greaterThanEqual(barCoord, vec3(0))) && all(lessThanEqual(barCoord, vec3(1)));
}