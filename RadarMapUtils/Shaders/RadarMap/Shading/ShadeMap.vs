varying vec2 posCrv;
vec2 Curve(vec3 pos); 

void main()
{	
	gl_Position = gl_ModelViewMatrix * gl_Vertex;
	posCrv = Curve(gl_Position.xyz);
}