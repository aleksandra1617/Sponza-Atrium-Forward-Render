#pragma once
#include <tgl\tgl.h>
#include <tygra\FileHelper.hpp>
#include <vector>
#include <iostream>

class ShaderHandler
{
public:
	ShaderHandler();
	~ShaderHandler();

	GLuint program_{ 0 };

	/*
	*   Shaders are loaded from a text file into a string which is
	*	passed to OpenGL for compiling.  Compile errors can be
	*	queried via the info log.
	*/
	GLuint CreateShader(GLenum shaderType, std::string path);

	/*
	*	A program object is made to host the shaders and their
	*   settings. This program object attempts to bind the shaders
	*   together when it is linked. If the shaders aren't compatible
	*   the link process will fail.
	*/
	void LinkShaders(std::vector <GLuint> shaders);

private:

	void CheckShaderCompilation(GLuint shader);

	/*
	*	Test if the shader program linked successfully. If not then
	*   get the error log and display in the console window.
	*/
	void CheckProgramLink();
};

