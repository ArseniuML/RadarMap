uniform sampler2DRect texInput;
varying vec3 pos;

vec4 texture2DRect_bicubic(sampler2DRect tex, vec2 t);

void main(void)
{
	gl_FragColor = vec4(texture2DRect_bicubic(texInput, pos.xy).rgb, 1);
}