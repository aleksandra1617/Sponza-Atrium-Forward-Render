#include "ShaderHandler.h"


ShaderHandler::ShaderHandler()
{
}

ShaderHandler::~ShaderHandler()
{
	glDeleteProgram(program_);
}


GLuint ShaderHandler::CreateShader(GLenum shaderType, std::string path)
{

	GLuint shader = glCreateShader(shaderType);
	std::string shader_string = tygra::createStringFromFile(path);

	const char * shader_code = shader_string.c_str();
	glShaderSource(shader, 1, (const GLchar **)&shader_code, NULL);
	glCompileShader(shader);

	CheckShaderCompilation(shader);

	return shader;
}

void ShaderHandler::CheckShaderCompilation(GLuint shader)
{
	GLint compile_status = GL_FALSE;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

	if (compile_status != GL_TRUE)
	{
		const int string_length = 1024;
		GLchar log[string_length] = "";
		glGetShaderInfoLog(shader, string_length, NULL, log);
		std::cout << "Error Detected in CreateShader()"; //TODO: check why this is triggered! 
		std::cerr << log << std::endl;
	}
	else
	{
		std::cout << "Shader: " << shader << " compiled sucessfully. " << std::endl;
	}
}

void ShaderHandler::LinkShaders(std::vector <GLuint> shaders)
{
	program_ = glCreateProgram();

	//Attach shaders
	glAttachShader(program_, shaders[0]);
	glDeleteShader(shaders[0]);
	glAttachShader(program_, shaders[1]);
	glDeleteShader(shaders[1]);

	glLinkProgram(program_);

	CheckProgramLink();
}

void ShaderHandler::CheckProgramLink()
{
	GLint link_status;
	glGetProgramiv(program_, GL_LINK_STATUS, &link_status);

	std::cout << "Link status: " << link_status << std::endl;

	if (link_status != GL_TRUE)
	{
		const int string_length = 1024;
		GLchar errorLog[string_length] = "";
		glGetProgramInfoLog(program_, string_length, NULL, errorLog);
		std::cout << "Error Detected in LinkProgram()" << std::endl; //TODO: check why this is triggered!
		std::cerr << errorLog << std::endl;
	}
	else
	{
		std::cout << "Successful Link Established!" << std::endl;
	}
}
