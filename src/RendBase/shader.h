#pragma once

#include <string>
#include <vector>
#include "asset.h"

namespace mxr
{
	class RENDBASE_EXPORT Shader : public IAsset
	{
	protected:
		QString source_path;
		std::string source_code;
		std::vector<GLuint> shaders;

		void ReadShader(const QString& path, const std::string& macro, std::string& output);
		void LoadShader(GLenum type);
		void LinkShaders();

	public:  // rule of five
		Shader() : IAsset() {}
		Shader(const QString& source_path);
		Shader(const QString& binary_path, GLenum format);
		virtual ~Shader();

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader(Shader&& other) = default;
		Shader& operator=(Shader&& other) = default;

	public:
		void Bind()  override;
		void Unbind() override;

		void Save();
		void Inspect();

		template<typename T>
		void SetUniform(GLuint location, const T& val);

		template<typename T>
		void SetUniform(const char* name, const T& val);

		template<typename T>
		void SetUniformArray(GLuint location, GLuint size, const std::vector<T>& vec);
	};

	class CShader : public Shader
	{
	private:
		GLint local_size_x, local_size_y, local_size_z;

	public:  // rule of zero
		CShader(const QString& source_path);
		CShader(const QString& binary_path, GLenum format);

	public:
		void Dispatch(GLuint nx, GLuint ny, GLuint nz = 1);
		void SyncWait(GLbitfield barriers = GL_SHADER_STORAGE_BARRIER_BIT);
	};
}

