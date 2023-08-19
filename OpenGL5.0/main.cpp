#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"//<>是系统库，“”是用户自定库
void processInput(GLFWwindow* window);


int main() {
	glfwInit();
	//设置主要和次要版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//设置核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGl Game", NULL, NULL);
	if (window == NULL) {
		std::cout << "创建窗口失败" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cout << "Init GLEW failed." << std::endl;
		glfwTerminate();
	}
	//GLEW初始化后开始导入shader编译
	Shader* myShader = new Shader("vertexSource.txt", "fragmentSource.txt");

	//设置视口Viewport
	glViewport(0, 0, 800, 600);
	glEnable(GL_CULL_FACE);//启动面剔除
	glCullFace(GL_BACK);//剔除背面
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式

	//顶点数组
	float vertices[] = {
	-0.5f,-0.5f,0.0f,   1.0f,0,0, // 0
	0.5f,-0.5f,0.0f,    0,1.0f,0, // 1
	0.0f,0.5f,0.0f,     0,0,1.0f, // 2
	//0.5f,-0.5f,0.0f,
	//0.0f,0.5f,0.0f,
	0.8f,0.8f,0.0f,	  0.2f,0.3f,0.6f// 3
	};//三角形的三个点是逆时针画出来的

	unsigned int indices[] = {
		0,1,2,
		2,1,3
	};
	//创建VAO，VBO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//创建EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//0号栏位，每三个栏位当成一个资料，不必规范化，每隔3个f长度读取下一个，该属性偏移量为0
	glEnableVertexAttribArray(0);
	//挖颜色值
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(7);


	//渲染循环
	while(!glfwWindowShouldClose(window)){
		processInput(window);//检查esc

		// 渲染背景颜色
		glClearColor(0.1f, 0.6f, 0.8f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);

		//画三角形
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		////uniform
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		//glBindVertexArray(VAO);
		//glUseProgram(shaderProgram);
		//glUniform4f(vertexColorLocation, 0, greenValue, 0, 1.0f);
		myShader->use();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);//交换前后缓存
		glfwPollEvents();//监听事件
	
	}
	glfwTerminate();//结束后释放资源

	return 0;
}

//press esc
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
