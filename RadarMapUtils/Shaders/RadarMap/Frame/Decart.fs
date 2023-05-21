uniform sampler2DRect texInput;
uniform vec2 texInputSize;
varying vec3 pos;

vec4 texture2DRect_bicubic(sampler2DRect tex, vec2 t);

void main(void)
{
	vec2 azR = 0.5 * ((gl_ProjectionMatrix * vec4(-atan(pos.x, pos.y), length(pos.xyz), 0, 1)).xy + vec2(1.0));
	
	if(all(greaterThan(vec4(azR, -azR), vec4(0, 0, -1, -1))))
		gl_FragColor = vec4(texture2DRect_bicubic(texInput, azR * texInputSize).rgb, 1);
	else
		gl_FragColor = vec4(0, 0, 0, 1);
}