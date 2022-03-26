#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

#pragma region 函数声明
void frame_buffersize_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int LoadTexure(const char* imgPath, unsigned int glTexture, GLint internalformat, GLint format);
#pragma endregion

#pragma region 初始化值
const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Hello OpenGL";
float vertices[] = {
	//顶点位置			  贴图位置
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
	
#pragma region 初始化GLFW和GLEW 创建窗口
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//创建一个窗口
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//初始化GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}

	//渲染视口
	glViewport(0, 0, WIDTH, HEIGHT);
	//当窗口改变时改变渲染视口
	glfwSetFramebufferSizeCallback(window, frame_buffersize_callback);
	//贴图y轴反转
	stbi_set_flip_vertically_on_load(true);
#pragma endregion
	
	//创建Shader
	Shader* shader = new Shader("Shader/VertexShader.vert", "Shader/FragmentShader.frag");

#pragma region VBO,VAO,贴图
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
		//输入
		processInput(window);

#pragma region 渲染指令
		//清屏
		glClearColor(.2f, .3f, .5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->Use();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
#pragma endregion

		//交换缓冲
		glfwSwapBuffers(window);
		//事件处理
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

#pragma region 函数实现
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