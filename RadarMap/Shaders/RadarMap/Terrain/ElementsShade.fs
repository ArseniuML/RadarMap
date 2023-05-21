uniform sampler2DRect zCoordMap;
uniform sampler2DRect elevationMap;

struct Grid
{
	vec2 Origin;
	vec2 ElementSize;
};
uniform Grid grid;
uniform vec3 boundPlane;
uniform float height;

void main()
{
	float elevation = texture2DRect(elevationMap, gl_FragCoord.xy).r;
	if (elevation == -2)
	{
		gl_FragColor.r = 0;
		return;
	}
	
	else if (elevation >=0)
	{
		gl_FragColor.r = 2;
		return;
	}
	
	vec4 gridCoord = vec4
	(
		grid.Origin + floor(gl_FragCoord.xy) * grid.ElementSize,
		grid.Origin + ceil(gl_FragCoord.xy) * grid.ElementSize
	);
	
	float zCoord = -(height - texture2DRect(zCoordMap, gl_FragCoord.xy).g);
	
	vec3 corner[4] = 
	{
		vec3(gridCoord.xy, zCoord),
		vec3(gridCoord.xw, zCoord),
		vec3(gridCoord.zy, zCoord),
		vec3(gridCoord.zw, zCoord),
	};	
	
	bool inner = false;
	for(int j = 0; j < 4; ++j) 
	{
		if (dot(corner[j], boundPlane) >= 0)
		{
			inner = true;
			break;
		}
	}
	gl_FragColor.r = inner ? 1 : 0;
}