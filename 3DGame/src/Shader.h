#pragma once

#include <iostream>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader {
private:
	unsigned int m_rendererID;
	std::unordered_map<std::string, int> m_uniformLocations;
public:
	Shader(const std::string& basename);
	~Shader();

	void Bind();
	void Unbind();

	void SetUniform4f(const std::string& name, glm::vec4 vec4);
	void SetUniformMatrix4fv(const std::string& name, glm::mat4 mat4);
private:
	int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string& source);
};
