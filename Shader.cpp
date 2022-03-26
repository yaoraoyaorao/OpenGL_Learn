#include "Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertexFile.open(vertexShaderPath);
		fragmentFile.open(fragmentShaderPath);
		std::stringstream vShaderStream, fShaderStream;
		//读取文件的缓冲内容到数据流
		vShaderStream << vertexFile.rdbuf();
		fShaderStream << fragmentFile.rdbuf();
		//关闭文件处理器
		vertexFile.close();
		fragmentFile.close();

		//转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		this->fShaderCode = fragmentCode.c_str();
		this->vShaderCode = vertexCode.c_str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Error:FILE " <<e.what()<< std::endl;
	}

	unsigned int vertexShader, fragmentShader;
	
	//创建顶点着色器
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
	glCompileShader(vertexShader);
	Check(vertexShader, "VERTEX");

	//创建片段着色器
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
	glCompileShader(fragmentShader);
	Check(fragmentShader, "FRAGMENT");

	//创建着色器程序
	this->ID = glCreateProgram();
	glAttachShader(this->ID,vertexShader);
	glAttachShader(this->ID, fragmentShader);
	glLinkProgram(this->ID);
	Check(this->ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use()
{
	glUseProgram(this->ID);
}

void Shader::SetBool(const char* name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->ID, name), (int)value);
}

void Shader::SetInt(const char* name, int value) const
{
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::SetFloat(const char* name, float value) const
{
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::Check(GLuint shader,const char* type)
{
	int success;
	char infoLog[512];
	if (type == "PROGRAM")
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 512, nullptr, infoLog);
			std::cout << "Error:Link" << infoLog << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		}
	}
}
