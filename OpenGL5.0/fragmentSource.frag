#version 330 core
//in vec4 vertexColor;
//in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct LightDirectional{
	 vec3 color;
	 vec3 pos;
	 vec3 dirToLight;
};
struct Material{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct LightPoint{
	float constant;
	float linear;
	float quadratic;
};

struct LightSpot{
	float cosPhyInner;
	float cosPhyOutter;
};

uniform LightDirectional  lightD;
uniform Material material;
uniform LightPoint lightP;
uniform LightSpot lightS;
//uniform sampler2D ourTexture;
//uniform sampler2D ourFace;
uniform vec3 objColor;
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 lightDirUniform;
uniform vec3 cameraPos;
out vec4 FragColor;
void main()
{	float dist = length(lightPos - FragPos);
	float attenuation = 1.0 / (lightP.constant + lightP.linear * dist + lightP.quadratic * (dist * dist));

   //FragColor = vertexColor; 
   //FragColor = texture(ourTexture, TexCoord) * (texture(ourFace,TexCoord)*2);
    vec3 lightDir = normalize(lightPos - FragPos);
	vec3 reflectVec = reflect(-lightDir,Normal);
	vec3 cameraVec = normalize(cameraPos - FragPos);

	//specular
	float specularAmount = pow(max(dot(reflectVec,cameraVec),0),material.shininess);
	vec3 specular =  texture(material.specular,TexCoord).rgb * specularAmount * lightColor;

	//diffuse
   vec3 diffuse = texture(material.diffuse , TexCoord ).rgb *  max(dot(lightDir ,Normal),0.0f) * lightColor * objColor;
	//vec3 diffuse = texture(material.diffuse,TexCoord).rgb;
   //ambient
   vec3 ambient = texture(material.diffuse , TexCoord ).rgb * ambientColor;

	//
	float cosTheta =  dot(normalize(FragPos - lightPos) , -1 * lightDirUniform);

	float spotRatio;

	if(cosTheta > lightS.cosPhyInner){
		// inside     
		spotRatio =1.0f;
	}
	else if(cosTheta > lightS.cosPhyOutter){
		// middle
		spotRatio = 1 -(cosTheta - lightS.cosPhyInner) / (lightS.cosPhyOutter - lightS.cosPhyInner);
	}else{
		// outside
		spotRatio =0.0f;
	}
	FragColor = vec4 ((ambient*0.7f  + (diffuse + specular) * spotRatio ) * objColor ,1.0);
//	if(cosTheta > lightS.cosPhy){
//		//inner
//		   FragColor = vec4 ((ambient*0.7f  + (diffuse + specular) ) * objColor ,1.0);
//	}else{
//		//outside
//		   FragColor = vec4 ((ambient*0.7f ) * objColor ,1.0);
//	}

   //FragColor = vec4 (objColor * ambientColor,1.0);
  // FragColor = vec4 ((ambient*0.7f  + (diffuse + specular) *  attenuation) * objColor ,1.0);

	


} 