#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"//<>��ϵͳ�⣬�������û��Զ���
void processInput(GLFWwindow* window);


int main() {
	glfwInit();
	//������Ҫ�ʹ�Ҫ�汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//���ú���ģʽ
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGl Game", NULL, NULL);
	if (window == NULL) {
		std::cout << "��������ʧ��" << std::endl;
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
	//GLEW��ʼ����ʼ����shader����
	Shader* myShader = new Shader("vertexSource.txt", "fragmentSource.txt");

	//�����ӿ�Viewport
	glViewport(0, 0, 800, 600);
	glEnable(GL_CULL_FACE);//�������޳�
	glCullFace(GL_BACK);//�޳�����
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ

	//��������
	float vertices[] = {
	-0.5f,-0.5f,0.0f,   1.0f,0,0, // 0
	0.5f,-0.5f,0.0f,    0,1.0f,0, // 1
	0.0f,0.5f,0.0f,     0,0,1.0f, // 2
	//0.5f,-0.5f,0.0f,
	//0.0f,0.5f,0.0f,
	0.8f,0.8f,0.0f,	  0.2f,0.3f,0.6f// 3
	};//�����ε�����������ʱ�뻭������

	unsigned int indices[] = {
		0,1,2,
		2,1,3
	};
	//����VAO��VBO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//����EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//0����λ��ÿ������λ����һ�����ϣ����ع淶����ÿ��3��f���ȶ�ȡ��һ����������ƫ����Ϊ0
	glEnableVertexAttribArray(0);
	//����ɫֵ
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(7);


	//��Ⱦѭ��
	while(!glfwWindowShouldClose(window)){
		processInput(window);//���esc

		// ��Ⱦ������ɫ
		glClearColor(0.1f, 0.6f, 0.8f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);

		//��������
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

		glfwSwapBuffers(window);//����ǰ�󻺴�
		glfwPollEvents();//�����¼�
	
	}
	glfwTerminate();//�������ͷ���Դ

	return 0;
}

//press esc
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
