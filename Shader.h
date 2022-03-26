#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <fstream>
#include<sstream>
/// <summary>
/// Shader类，用于读取vertexShader顶点着色器，FragmentShader片段着色器
/// </summary>
class Shader
{
public:
	Shader(const char* vertexShaderPath,const char * fragmentShaderPath);
	void Use();
	void SetBool(const char* name, bool value)const;
	void SetInt(const char* name, int value)const;
	void SetFloat(const char* name, float value)const;
public:
	const char* vShaderCode;
	const char* fShaderCode;
	unsigned int ID;
private:
	void Check(GLuint shader,const char* type);
};

