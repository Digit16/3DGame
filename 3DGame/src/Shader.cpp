#include "Shader.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "Utils.h"


Shader::Shader(const std::string& basename) {
	std::string vertex_source = Utils::ReadFile(basename + ".vert");
	std::string fragment_source = Utils::ReadFile(basename + ".frag");

	m_rendererID = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertex_source);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragment_source);

	glAttachShader(m_rendererID, vs);
	glAttachShader(m_rendererID, fs);
	glLinkProgram(m_rendererID);
	glValidateProgram(m_rendererID);

	glDeleteShader(vs);
	glDeleteShader(fs);

}

Shader::~Shader() {
	glDeleteProgram(m_rendererID);
}

void Shader::Bind() {
	glUseProgram(m_rendererID);
}
void Shader::Unbind() {
	glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, glm::vec4 vec4) {
	glUniform4f(GetUniformLocation(name), vec4[0], vec4[1], vec4[2], vec4[3]);
}

void Shader::SetUniformMatrix4fv(const std::string& name, glm::mat4 mat4) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(mat4));
}

int Shader::GetUniformLocation(const std::string& name) {
	
	if (m_uniformLocations.find(name) != m_uniformLocations.end()) {
		return m_uniformLocations[name];
	}
	std::cout << "getting uniform location: " << name << std::endl;
	int location = glGetUniformLocation(m_rendererID, name.c_str());
	if (location == -1) {
		std::cout << "Uniform not found!" << std::endl;
	}
	return m_uniformLocations[name] = location;
	return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Shader error!" << std::endl;
		std::cout << message << std::endl;
		delete[] message;

		glDeleteShader(id);
		return 0;
	}

	return id;
}
