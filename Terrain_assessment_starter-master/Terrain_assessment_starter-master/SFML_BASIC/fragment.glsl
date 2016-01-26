uniform sampler2D Seatex;
uniform sampler2D Rocktex;
uniform sampler2D Snowtex;

void main()
{
    vec4 Seacolor  = texture2D(Seatex,gl_TexCoord[0].st);
	vec4 Rockcolor = texture2D(Rocktex,gl_TexCoord[0].st);
    vec4 Snowcolor = texture2D(Snowtex,gl_TexCoord[0].st);
	gl_FragColor = Seacolor;
}