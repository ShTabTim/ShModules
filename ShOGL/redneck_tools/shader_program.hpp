#ifndef SH_OGL_REDNECK_TOOLS_SHADER_PROGRAM_HPP
#define SH_OGL_REDNECK_TOOLS_SHADER_PROGRAM_HPP

#include "../basic_ogl_includes.hpp"
#include "../highgl/highgl.hpp"

#include <string>
#include <unordered_map>
#include <vector>

GLuint compile_shader(GLenum type, const GLchar* source, const std::wstring& name);

typedef struct shader_program {
	std::unordered_map<std::string, GLint> uniform_locations;
	GLuint id = 0;
	template<typename... Args>
		requires (std::same_as<Args, GLuint> && ...)
	void init(bool delete_shaders, Args... args) {
		if (id && glIsProgram(id)) {
			GLint numShaders = 0;
			glGetProgramiv(id, GL_ATTACHED_SHADERS, &numShaders);
			if (numShaders > 0) {
				std::vector<GLuint> shaders(numShaders);
				glGetAttachedShaders(id, numShaders, NULL, shaders.data());
				for (GLuint s : shaders) glDetachShader(id, s);
			}
		}
		else id = glCreateProgram();
		(glAttachShader(id, args), ...);

		glLinkProgram(id);
		{
			GLint success;
			glGetProgramiv(id, GL_LINK_STATUS, &success);
			if (!success) {
				GLint log_length;
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_length);
				if (log_length > 0) {
					std::string info_log;
					info_log.resize(log_length - 1);
					glGetProgramInfoLog(id, log_length, nullptr, info_log.data());
					error_named(std::wstring(info_log.begin(), info_log.end()).c_str(), L"Ошибка линка шейдерной программы.");
					info_log.clear();
				}
				else error(L"Ошибка линка шейдерной прогрыммы, но без лога.");
				glDeleteProgram(id);
			}
		}
		if (delete_shaders) ((glIsShader(args) ? glDeleteShader(args) : void()), ...);

		glUseProgram(id);
		uniform_locations.clear();
		{
			GLint uniforms_count = 0;
			glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniforms_count);
			if (uniforms_count <= 0) return;
			GLint max_uniform_lenght = 0;
			glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniform_lenght);
			std::vector<GLchar> name(max_uniform_lenght + 1);
			for (uint32_t t(uniforms_count); t--;) {
				GLsizei length;
				GLint size;
				GLenum type;
				glGetActiveUniform(id, t, max_uniform_lenght, &length, &size, &type, name.data());
				uniform_locations[name.data()] = glGetUniformLocation(id, name.data());
			}
			name.clear();
		}
	}
	void finit();
} shader_program;

#endif//SH_OGL_REDNECK_TOOLS_SHADER_PROGRAM_HPP