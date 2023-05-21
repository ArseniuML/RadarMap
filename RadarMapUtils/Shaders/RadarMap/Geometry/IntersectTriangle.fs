varying vec3 face;
varying float facew;
varying vec2 facezw2;

varying vec3 E[3];
varying vec3 T;
varying vec3 Q;

bool IntersectPlane(vec2 azR, out vec3[2] pos)
{
	vec2 cv1 = vec2(tan(azR.x), azR.y * azR.y);
	vec2 cv2 = vec2(1 + cv1[0] * cv1[0], -face.x * cv1[0] + face.y);
	float a = cv2[0] * facezw2[0] + cv2[1] * cv2[1];
	if (a == 0) return false;

	vec2 bc;
	float discr, sqrtd;
	vec2 x, y, z;
	
	if (abs(cv2[1]) < abs(face.z))
	{
		vec2 bc = vec2(cv2[1] * facew, facezw2[1] - facezw2[0] * cv1[1]);
		
		discr = bc[0] * bc[0] - a * bc[1]; 
		if (discr < 0) return false;
		sqrtd = sqrt(discr);
		
		y = vec2(-bc[0] - sqrtd, -bc[0] + sqrtd) / vec2(a);
		if (face.z == 0) return false;
		z = -(cv2[1] * y + vec2(facew)) / face.z;
	}
	else
	{
		bc = vec2(cv2[0] * face.z * facew, cv2[0] * facezw2[1] - cv2[1] * cv2[1] * cv1[1]);
		
		discr = bc[0] * bc[0] - a * bc[1]; 
		if (discr < 0) return false;
		sqrtd = sqrt(discr);
		
		z = vec2(-bc[0] - sqrtd, -bc[0] + sqrtd) / vec2(a);
		if (cv2[1] == 0) return false;
		y = -(face.z * z + vec2(facew)) / cv2[1];
	} 
	
	x = -cv1[0] * y;
	 
	pos[0] = vec3(x[0], y[0], z[0]); 
	pos[1] = vec3(x[1], y[1], z[1]);

	return true;
}

bool InsideTriangle(vec3 pos)
{
	vec3 P = cross(pos, E[2]);
	float den = dot(P, E[0]);
	if (den == 0) return false;

	vec3 barCoord; 
	barCoord.gb = vec2(dot(P, T), dot(Q, pos)) / den;
	barCoord.r = 1 - barCoord.g - barCoord.b;

	return all(greaterThanEqual(barCoord, vec3(0))) && all(lessThanEqual(barCoord, vec3(1)));
}