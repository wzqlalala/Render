#pragma once

#include <vector>
#include "rendbase_global.h"
#include "asset.h"
#include <qdebug.h>
#include <atomic>
#include <iostream>
namespace mxr
{
	class RENDBASE_EXPORT IBuffer :public QOpenGLFunctions_4_5_Core
	{
	protected:
		GLuint id;
		GLsizeiptr _size;     // 当前使用大小 (used size)
		GLsizeiptr _capacity; // 总分配大小 (allocated size)
		mutable void* data_ptr;
		GLbitfield _access;
		IBuffer();
		IBuffer(GLsizeiptr size, const void* data, GLbitfield access);
		virtual ~IBuffer();

		IBuffer(const IBuffer&) = delete;
		IBuffer& operator=(const IBuffer&) = delete;
		IBuffer(IBuffer&& other) noexcept;
		IBuffer& operator=(IBuffer&& other) noexcept;

	public:
		GLsizeiptr Capacity() const { return this->_capacity; }
		void Resize(GLsizeiptr new_capacity);  // 新方法：扩展容量
		GLuint ID() const { return this->id; }
		GLsizeiptr Size() const { return this->_size; }
		void* const Data() const { return this->data_ptr; }

		void Copy(GLuint fr, GLuint to, GLintptr fr_offset, GLintptr to_offset, GLsizei size);

		void GetData(void* data);
		void GetData(GLintptr offset, GLsizeiptr size, void* data);
		void SetData(const void* data);
		void SetData(GLintptr offset, GLsizeiptr size, const void* data);
		void DeleteData(GLintptr offset, GLsizeiptr size);

		void Acquire(GLbitfield access);
		void Release();
		void Clear();
		void Clear(GLintptr offset, GLsizeiptr size);
		void Flush();
		void Flush(GLintptr offset, GLsizeiptr size);
		void Invalidate();
		void Invalidate(GLintptr offset, GLsizeiptr size);
	};

	///////////////////////////////////////////////////////////////////////////////////////////////
	static std::atomic<int> g_vbo_count{ 0 };
	class RENDBASE_EXPORT VBO : public IBuffer
	{
	public:
		VBO(GLsizeiptr size, const void* data, GLbitfield access = 0) : IBuffer(size, data, access) { 
			++g_vbo_count; 
			std::cout << "[VBO] ctor id=" << this << " total=" << g_vbo_count.load() << "\n";
		}
		virtual ~VBO() { 
			glDeleteBuffers(1, &id);
			--g_vbo_count;
			std::cout << "[VBO] dtor id=" << this << " total=" << g_vbo_count.load() << "\n";
		}
	};

	class RENDBASE_EXPORT IBO : public IBuffer
	{
	public:
		IBO(GLsizeiptr size, const void* data, GLbitfield access = 0) : IBuffer(size, data, access) 
		{
					
		}
		virtual ~IBO() { glDeleteBuffers(1, &id); }
	};

	class RENDBASE_EXPORT PBO : public IBuffer
	{
	public:
		PBO(GLsizeiptr size, const void* data, GLbitfield access = 0) : IBuffer(size, data, access) {}
	};


	class RENDBASE_EXPORT DrawBuffer :public IBuffer
	{
	public:
		DrawBuffer(GLsizeiptr size, const void* data, GLbitfield access = 0) : IBuffer(size, data, access) {}

	};



	///////////////////////////////////////////////////////////////////////////////////////////////

	class RENDBASE_EXPORT IIndexedBuffer : public IBuffer
	{
	protected:
		GLuint index;
		GLenum target;
		IIndexedBuffer() : IBuffer(), index(0), target(0) {}
		IIndexedBuffer(GLuint index, GLsizeiptr size, GLbitfield access) : IBuffer(size, NULL, access), index(index) {}

	public:
		void Reset(GLuint index);
	};

	///////////////////////////////////////////////////////////////////////////////////////////////

	class RENDBASE_EXPORT ATC: public IIndexedBuffer
	{
	public:
		ATC() = default;
		ATC(GLuint index, GLsizeiptr size, GLbitfield access = GL_DYNAMIC_STORAGE_BIT);
	};

	class RENDBASE_EXPORT SSBO: public IIndexedBuffer
	{
	public:
		SSBO() = default;
		SSBO(GLuint index, GLsizeiptr size, GLbitfield access = GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT);
	};

	class RENDBASE_EXPORT UBO: public IIndexedBuffer
	{
	private:
		using u_vec = std::vector<GLuint>;
		u_vec offset_vec;  // each uniform's aligned byte offset
		u_vec stride_vec;  // each uniform's byte stride (with padding)
		u_vec length_vec;  // each uniform's byte length (w/o. padding)

	public:
		UBO() = default;
		UBO(GLuint index, const u_vec& offset, const u_vec& length, const u_vec& stride);
		UBO(GLuint shader, GLuint block_id, GLbitfield access = GL_DYNAMIC_STORAGE_BIT);
		void SetUniform(GLuint uid, const void* data);
		void SetUniform(GLuint fr, GLuint to, const void* data);
	};
}

