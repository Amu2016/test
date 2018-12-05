
#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;

void main()
{
	gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
}
