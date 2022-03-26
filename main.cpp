#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

#pragma region ��������
void frame_buffersize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int LoadTexure(const char* imgPath, unsigned int glTexture, GLint internalformat, GLint format);
#pragma endregion

#pragma region ��ʼ��ֵ
const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Hello OpenGL";
float vertices[] = {
	//����λ��			  ��ͼλ��
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
#pragma endregion



int main()
{
	
#pragma region ��ʼ��GLFW��GLEW ��������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//����һ������
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//��ʼ��GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}

	//��Ⱦ�ӿ�
	glViewport(0, 0, WIDTH, HEIGHT);
	//�����ڸı�ʱ�ı���Ⱦ�ӿ�
	glfwSetFramebufferSizeCallback(window, frame_buffersize_callback);
	//��ͼy�ᷴת
	stbi_set_flip_vertically_on_load(true);
#pragma endregion
	
	//����Shader
	Shader* shader = new Shader("Shader/VertexShader.vert", "Shader/FragmentShader.frag");

#pragma region VBO,VAO,��ͼ
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int texture1,texture2;
	texture1 = LoadTexure("images/container.jpg", 0,GL_RGB,GL_RGB);
	texture2 = LoadTexure("images/awesomeface.png", 1, GL_RGB, GL_RGBA);
	shader->Use();
	shader->SetInt("ourTexture1", 0);
	shader->SetInt("ourTexture2", 1);
#pragma endregion

	


	while (!glfwWindowShouldClose(window))
	{
		//����
		processInput(window);

#pragma region ��Ⱦָ��
		//����
		glClearColor(.2f, .3f, .5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->Use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
#pragma endregion

		//��������
		glfwSwapBuffers(window);
		//�¼�����
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

#pragma region ����ʵ��
void frame_buffersize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
unsigned int LoadTexure(const char* imgPath,unsigned int glTexture,GLint internalformat = GL_RGB,GLint format = GL_RGB)
{
	unsigned int texure;
	glGenTextures(1, &texure);
	glActiveTexture(GL_TEXTURE0 + glTexture);
	glBindTexture(GL_TEXTURE_2D, texure);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(imgPath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
	return texure;
}
#pragma endregion