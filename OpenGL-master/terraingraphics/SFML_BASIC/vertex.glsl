varying float height;
uniform float heightestPoint;
out vec4 specular;


void main()
{
	vec3 normal, lightDir;
	vec4 diffuse, ambient, globalAmbient;
	float NdotL, NdotHV;
	specular=vec4(0,0,0,0);

	normal = normalize(gl_NormalMatrix * gl_Normal);

	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	globalAmbient = gl_LightModel.ambient * gl_FrontMaterial.ambient;

	vec4 pointEyeSpace=(gl_ModelViewMatrix*gl_Vertex);
	lightDir = normalize(vec3(gl_LightSource[0].position)-pointEyeSpace.xyz);
	NdotL = max(dot(normal, lightDir), 0.0);
	vec4 cameraDir=-pointEyeSpace;	
	//vec3 halfVector=normalize(lightDir + cameraDir).xyz;
	vec3 halfVector=normalize(lightDir + cameraDir.xyz);
	
	if (NdotL > 0.0) {

		// normalize the half-vector, and then compute the
		// cosine (dot product) with the normal
		NdotHV = max(dot(normal, halfVector.xyz),0.0);
		specular = gl_FrontMaterial.specular * gl_LightSource[0].specular *
				pow(NdotHV,gl_FrontMaterial.shininess);
	}
	
	gl_FrontColor =  NdotL * diffuse  + ambient + globalAmbient;
	
	gl_Position = ftransform();









	
	height = gl_Vertex.y / heightestPoint;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

}