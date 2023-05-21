uniform sampler2DRect zBoundsMap;
uniform float height;

struct Grid
{
	vec2 Origin;
	vec2 ElementSize;
};
uniform Grid grid;

uniform vec2 reqAzimuthBounds[2];
uniform vec2 reqRangeBounds2;

void main()
{
	vec2 zBounds = texture2DRect(zBoundsMap, gl_FragCoord.xy).rg;
	vec3 cellCenter = vec3(grid.Origin + gl_FragCoord.xy * grid.ElementSize, 
		height - 0.5 * (zBounds.y + zBounds.x));
	vec3 halfCellSize = 0.5 * vec3(grid.ElementSize, zBounds.y - zBounds.x);

	vec4 gridCoord = vec4(cellCenter.xy - halfCellSize.xy, cellCenter.xy + halfCellSize.xy);
	vec2 corner[4] = 	{gridCoord.xy, gridCoord.zy, gridCoord.zw, gridCoord.xw};
	
	if (!all(bvec2 
	(
		any(greaterThanEqual
		(
			vec4
			(
				dot(corner[0], reqAzimuthBounds[0]), 
				dot(corner[1], reqAzimuthBounds[0]), 
				dot(corner[2], reqAzimuthBounds[0]), 
				dot(corner[3], reqAzimuthBounds[0])
			), 
			vec4(0)
		)),
		any(greaterThanEqual
		(
			vec4
			(
				dot(corner[0], reqAzimuthBounds[1]),
				dot(corner[1], reqAzimuthBounds[1]), 
				dot(corner[2], reqAzimuthBounds[1]), 
				dot(corner[3], reqAzimuthBounds[1])
			), 
			vec4(0)
		))
	)))
	{
		gl_FragColor.r = 0;
		return;		
	}

	vec3 minRangeVec= abs(cellCenter) - halfCellSize;
	minRangeVec = minRangeVec * greaterThan(minRangeVec, vec3(0));
	vec3 maxRangeVec = abs(cellCenter) + halfCellSize;
	vec2 rangeBounds2 = vec2(dot(minRangeVec, minRangeVec), dot(maxRangeVec, maxRangeVec));
	
	gl_FragColor.r = (rangeBounds2.x > reqRangeBounds2.y || rangeBounds2.y < reqRangeBounds2.x) ? 0 : 1;
}

