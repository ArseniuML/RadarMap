vec2 curve(vec3 pos) 
{
	return vec2(-atan(pos.x, pos.y), length(pos));
}

void main()
{	
	vec2 crvPos = curve((gl_ModelViewMatrix * gl_Vertex).xyz);
	gl_Position = gl_ProjectionMatrix * vec4(crvPos, 0, 1);
}