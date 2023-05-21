bool InsideTriangle(vec3 pos, vec3 bcn0, vec3 bcn2, float vol)
{
	vec3 barCoord; 
	barCoord.gb = vec2(dot(pos, bcn2), dot(pos, bcn0)) / vol;
	barCoord.r = 1 - barCoord.g - barCoord.b;

	return all(greaterThanEqual(barCoord, vec3(0))) && all(lessThanEqual(barCoord, vec3(1)));
}

bool IntersectLines(vec2 n1, float h1, vec2 n2, float h2, out vec2 pos)
{
	float den = n1.y * n2.x - n1.x * n2.y;
	if (den == 0) return false;

	pos = vec2(-n2.y * h1 + n1.y * h2, n2.x * h1 - n1.x * h2) / den;
	return true;
}