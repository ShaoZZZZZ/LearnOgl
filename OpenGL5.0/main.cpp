#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"//<>是系统库，“”是用户自定库
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Material.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow*, double xPos, double yPos);
  
#pragma region Camera declare
// Instantiate Camera class初始化Camera类
//Camera camera(glm: :vec3(0.0f,0.0f,3.0f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f));
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::radians(15.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));

#pragma endregion

#pragma region Light Declare
//LightDirectional light = LightDirectional(glm::vec3(10.0f, 10.0f, -5.0f), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
//	glm::vec3(2.0f,0,0));

//LightPoint light = LightPoint(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
//	glm::vec3(5.0f,5.0f,5.0f));
LightSpot light = LightSpot(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(glm::radians(90.0f), glm::radians(0.0f), 0),
	glm::vec3(2.0f, 2.0f, 2.0f));
#pragma endregion

unsigned int  LoadImageToGPU(const char* filename,	GLint internalFormat ,GLenum format,int textureSlot) {
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, TexBuffer);

	int width, height, nrChannel;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannel, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "load image failed." << std::endl;
	}
	stbi_image_free(data);

	return TexBuffer;
}
int main() {

#pragma region Open a window
	glfwInit();
	//设置主要和次要版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//设置核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//开启透明帧缓冲
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGl Game", NULL, NULL);
	if (window == NULL) {
		std::cout << "创建窗口失败" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//关闭光标
	glfwSetCursorPosCallback(window, mouse_callback);


	// Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "Init GLEW failed." << std::endl;
		glfwTerminate();
	}


	//设置视口Viewport
	glViewport(0, 0, 800, 600);
	//glEnable(GL_CULL_FACE);//启动面剔除
	//glCullFace(GL_BACK);//剔除背面
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式
	glEnable(GL_DEPTH_TEST);//打开深度缓存
#pragma endregion

#pragma region Init Sahder
		//GLEW初始化后开始导入shader编译
	Shader* myShader = new Shader("vertexSource.vert", "fragmentSource.frag");
#pragma endregion

#pragma region Init Material
	Material* myMaterial = new Material(myShader,
		LoadImageToGPU("container2.png",GL_RGBA,GL_RGBA,Shader::DIFFUSE),
		LoadImageToGPU("container2_specular.png", GL_RGBA, GL_RGBA, Shader::SPECULAR),
		glm::vec3(1.0f, 1.0f, 1.0f), 
		32.0f);

#pragma endregion

#pragma region Model Data
	//顶点数组
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
	};
#pragma endregion

#pragma region Init and load Models to VAO,VBO
	//创建VAO，VBO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//创建EBO
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  8 * sizeof(float), (void*)0);
	//0号栏位，每三个栏位当成一个资料，不必规范化，每隔3个f长度读取下一个，该属性偏移量为0
	glEnableVertexAttribArray(0);
	//挖颜色值
	//glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(7);
	//UV值
	glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(9);
#pragma endregion

#pragma region Init and Load Texture
	stbi_set_flip_vertically_on_load(true);//翻转纹理
	//创建texture 缓存
	//unsigned int TexBufferA;
	//TexBufferA = LoadImageToGPU("container.jpg",GL_RGB,GL_RGB,0);
	//unsigned int TexBufferB;
	//TexBufferB = LoadImageToGPU("awesomeface.png", GL_RGB, GL_RGB, 3);
#pragma endregion

#pragma region Prepare MVP matrices
	// calculate our transformation matrix here 
	//glm::mat4 trans;
	//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0));//位移
	//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0, 0, 1.0f));//旋转
	//trans = glm::scale(trans, glm::vec3(2.0f, 2.0f, 2.0f));//缩放
	//tips：先缩放，再旋转，最后位移，否则会相互影响，根据矩阵左乘，故上面的顺序相反
	glm::mat4 modelMat;
	glm::mat4 viewMat;
	viewMat = camera.GetViewMatrix();
	glm::mat4 projMat;
	projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
#pragma endregion

	//渲染循环
	while(!glfwWindowShouldClose(window)){

		processInput(window);//检查esc等    

		// 渲染背景颜色/清屏
		glClearColor(0.1f, 0.6f, 0.8f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		


		viewMat = camera.GetViewMatrix();//因为用鼠标，所以此处更新viewMat


		//十次drawCall
		for (int i = 0; i < 10; i++)
		{
			//set Model matrix
			modelMat = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			modelMat = glm::rotate(modelMat, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

			//set View and Projection Matrices here if you want 
			
			// set Material -> shader Program
			myShader->use();
			//set Material -> Texture
		/*	glBindTexture(GL_TEXTURE_2D, TexBufferA);
			glBindTexture(GL_TEXTURE_2D, TexBufferB);*/
			// ste Material -> Uniform
			//glUniform1i(glGetUniformLocation(myShader->ID, "ourTexture"), 0);
			//glUniform1i(glGetUniformLocation(myShader->ID, "ourFace"), 3);
			//glUniformMatrix4fv(glGetUniformLocation(myShader->ID,"transform"), 1, GL_FALSE,glm::value_ptr(trans));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
			glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
			glUniform3f(glGetUniformLocation(myShader->ID, "objColor"), 1.0f, 1.0f,1.0f);
			glUniform3f(glGetUniformLocation(myShader->ID, "ambientColor"), 0.5f, 0.5f, 0.5f);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightPos"), light.position.x,light.position.y,light.position.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightColor"),light.color.x,light.color.y,light.color.z);
			glUniform3f(glGetUniformLocation(myShader->ID, "lightDirUniform"), light.direction.x, light.direction.y, light.direction.z);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightS.cosPhyInner"), light.cosPhyInner);
			glUniform1f(glGetUniformLocation(myShader->ID, "lightS.cosPhyOutter"), light.cosPhyOutter);

			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP.constant"),light.constant );//衰减值
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP.linear"), light.linear);
			//glUniform1f(glGetUniformLocation(myShader->ID, "lightP.quadratic"), light.quadratic);


			glUniform3f(glGetUniformLocation(myShader->ID, "cameraPos"),camera.Position.x, camera.Position.y, camera.Position.z);


			//glUniform3f(glGetUniformLocation(myShader->ID, "material.ambient"), myMaterial->ambient.x, myMaterial->ambient.y, myMaterial->ambient.z);
			myMaterial->shader->SetUniform3f("material.ambient", myMaterial->ambient);
			//myMaterial->shader->SetUniform3f("material.diffuse",myMaterial->diffuse);
			myMaterial->shader->SetUniform1i("material.diffuse", Shader::DIFFUSE);
			myMaterial->shader->SetUniform1i("material.specular", Shader::SPECULAR);
			myMaterial->shader->SetUniform1f("material.shininess", myMaterial->shininess);

			//glUniform3f(glGetUniformLocation(myShader->ID, "material.diffuse"), 0.0f,0.0f,1.0f);
			//glUniform3f(glGetUniformLocation(myShader->ID, "material.specular"), 0.0f,1.0f,0.0f);
			//glUniform1f(glGetUniformLocation(myShader->ID, "material.shininess"), 64.0f);




			//ste Model
			glBindVertexArray(VAO);

			//Drawcall
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		
		// Clean up,prepare for next render loop
		glfwSwapBuffers(window);//交换前后缓存
		glfwPollEvents();//监听事件
		camera.UpdateCameraPos();//更新摄像机坐标
	}

	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	glfwTerminate();//结束后释放资源

	return 0;
}


#pragma region Input Declare


//press esc
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	//控制移动
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.SpeedZ = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.SpeedZ = -1.0f;
	}
	else {
		camera.SpeedZ = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.SpeedX = 1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.SpeedX = -1.0f;
	}
	else {
		camera.SpeedX = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.SpeedY = -1.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.SpeedY = 1.0f;
	}
	else {
		camera.SpeedY = 0;
	}

}


float lastX;
float lastY;
bool firstMouse = true;//鼠标刚进入瞬间会有巨大偏移量

void mouse_callback(GLFWwindow*, double xPos, double yPos) {
	if (firstMouse == true) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float deltaX, deltaY;
	deltaX = xPos - lastX;
	deltaY = yPos - lastY;

	lastX = xPos;   
	lastY = yPos;

	//std::cout << deltaX << std::endl;
	camera.ProcessMouseMovement(deltaX, deltaY);
}
#pragma endregion 