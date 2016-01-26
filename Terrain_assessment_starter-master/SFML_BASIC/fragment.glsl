uniform sampler2D Seatex;
uniform sampler2D Rocktex;
uniform sampler2D Snowtex;
varying float height;

void main()
{
    vec4 Seacolor  = texture2D(Seatex,gl_TexCoord[0].st);
	vec4 Rockcolor = texture2D(Rocktex,gl_TexCoord[0].st);
    vec4 Snowcolor = texture2D(Snowtex,gl_TexCoord[0].st);

	if (height > 7.5)
	{
	    gl_FragColor = Snowcolor;
	}
	if (height > 0 && height < 0.25)
	{
	    gl_FragColor = Seacolor;
	}
	if (height > 0.25 && height < 7.5)
	{
	    gl_FragColor = Rockcolor;
	}

}