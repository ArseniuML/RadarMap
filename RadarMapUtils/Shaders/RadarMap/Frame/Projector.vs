varying vec3 pos;

void main(void)
{
	pos = gl_Vertex.xyz;
	gl_Position = gl_ModelViewMatrix * gl_Vertex;
}