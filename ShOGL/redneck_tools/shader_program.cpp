#include "shader_program.hpp"

GLuint compile_shader(GLenum type, const GLchar* source, const std::wstring& name) {
	GLint success;
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLint log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 0) {
			std::string info_log;
			info_log.resize(log_length - 1);
			glGetShaderInfoLog(shader, log_length, nullptr, info_log.data());
			error_named(std::wstring(info_log.begin(), info_log.end()).c_str(), (L"ќшибка компил€ции шейдера " + name).c_str());
			info_log.clear();
		}
		else error((L"ќшибка компил€ции шейдера " + name + L", но без лога").c_str());
		glDeleteShader(shader);
	}
	return shader;
}

void shader_program::finit() {
	if (glIsProgram(id)) glDeleteProgram(id);
	uniform_locations.clear();
}