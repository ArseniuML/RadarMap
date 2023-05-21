uniform sampler2DRect shadeMapTex;
uniform vec2 shadeMapSize;
uniform float height;

float k = 0.26318945;
int step[11] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

float Shade(vec3 pos, float D)
{
	return 1;
	
	float hRange = length(pos.xy);
	float cotanEl = - hRange / pos.z;
	
	float shadeHRange = height * cotanEl;
	float shadeCoord =  (shadeHRange - shadeMapSize.x) / shadeMapSize.y + 1.0;
	
	vec2 sh = texture2DRect(shadeMapTex, vec2(gl_FragCoord.x, shadeCoord)).rg;
	if (hRange <= sh.r) return 1;
	
	float el1 = atan(cotanEl);

	int p, p1, p2;
	for (int i = 2; i < 11; ++i)
	{
		p2 = step[i];
		float hr = texture2DRect(shadeMapTex, vec2(gl_FragCoord.x, shadeCoord + p2)).r;
		
		if (hRange <= hr | hr < 0 | i == 10)
		{
			for (int j = i - 2; j >= 0; --j)
			{
				p = p1 + step[j];
				float hr = texture2DRect(shadeMapTex, vec2(gl_FragCoord.x, shadeCoord + p)).r;
				if (hRange <= hr | hr < 0)
				{
					p2 = p;
				}
				else
				{
					p1 = p;
				}			
			}
			
			float el2 = atan((shadeMapSize.x + (shadeCoord + float(p2)) * shadeMapSize.y) / height);
			float Dq = texture2DRect(shadeMapTex, vec2(gl_FragCoord.x, shadeCoord + float(p1))).g;
			
			float w = abs(el2 - el1) * sqrt(k * Dq * D / (D - Dq)) + 1.0;
			return 1 / (w * w);
		}
		p1 = p2;
	}
		
	return 0;
}