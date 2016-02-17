uniform sampler2D grassTex;
uniform sampler2D waterTex;
uniform sampler2D snowTex;
varying float height;
in vec4 specular;
void main()
{
	vec4 color;
	vec4 specularlight = vec4(0,0,0,0);
	if(height < 0.1f)
	{
		//water
		color = texture2D(waterTex,gl_TexCoord[0].st);
		specularlight = specular;
	}
	else if(height < 0.15f){
		//water, grass
		//the mix area of water and grass
		float localheight = height - 0.1f; 
		float percent = localheight / 0.05f;
		vec4 c1 = texture2D(waterTex,gl_TexCoord[0].st);
		vec4 c2 = texture2D(grassTex,gl_TexCoord[0].st);
		specularlight = specular;
		color = mix(c1, c2, percent);

	}
	else if(height < 0.6f){
		//grass
		color = texture2D(grassTex,gl_TexCoord[0].st);
	}
	else if(height < 0.75f){
		//grass, rock
		//the mix area of grass and rock
		float localheight = height - 0.6f;
		float percent = localheight / 0.15f;
		vec4 c1 = texture2D(grassTex,gl_TexCoord[0].st);
		vec4 c2 = texture2D(snowTex,gl_TexCoord[0].st);
		
		color = mix(c1, c2, percent) ;
	}
	else
	{
		//rock
		color = texture2D(snowTex,gl_TexCoord[0].st);
	}

	gl_FragColor = color * (gl_Color +specularlight);
}