#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 7) in vec3 aColor;
layout (location = 8) in vec2 aTexCoord;
layout (location = 9) in vec3 aNormal;


//uniform mat4 transform;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;


out vec4 vertexColor;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

void main()
{
   gl_Position = projMat * viewMat * modelMat * vec4(aPos.xyz, 1.0f);
   FragPos = (modelMat * vec4(aPos.xyz, 1.0f)).xyz;
   Normal = mat3(modelMat) * aNormal;
//   vertexColor = vec4(aColor.x,aColor.y,aColor.z,1.0f);    
//   TexCoord = aTexCoord;
	TexCoord = aTexCoord;
gl_PointSize = 20.0f;
}