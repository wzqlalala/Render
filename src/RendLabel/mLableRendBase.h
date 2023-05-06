#pragma once

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include<QHash>
#include<QPair>
#include"mLableRendEnum.h"
#include "qopenglfunctions.h"
#include <qopenglcontext.h>

using namespace std;
namespace MLableRend
{
	//顶点绘制方式
	class mDrawArray:public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
	{
	public:
		explicit mDrawArray(int VBOnum, GLenum drawModel):_VBONum(VBOnum),_drawModel(drawModel)
		{	
			this->initializeOpenGLFunctions();
			_VBO = new GLuint[VBOnum]; 
			glGenVertexArrays(1, &_VAO);
			glGenBuffers(VBOnum, _VBO);
		}
		~mDrawArray()
		{	
			if (isInitialized())
			{
				glDeleteVertexArrays(1, &_VAO);
				glDeleteBuffers(_VBONum, _VBO);
				if (_VBO != nullptr)
				{
					delete _VBO;
					_VBO = nullptr;
				}
			}
		}
	public:
		/*
		* 绘制
		*/
		void draw()
		{
			glBindVertexArray(_VAO);
			glDrawArrays(_drawModel, 0, _vertexNum);
			
		}
	public:
		//渲染缓冲
		GLuint _VAO, *_VBO = nullptr;
		//顶点数量
		int _vertexNum = 0;
		GLenum _drawModel = GL_LINES;
		//VBO数量
		int _VBONum = 0;
	};
	//索引绘制方式
	class mDrawElement :public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
	{
	public:
		explicit mDrawElement(int VBOnum, GLenum drawModel) :_VBONum(VBOnum),_drawModel(drawModel)
		{
			this->initializeOpenGLFunctions();
			_VBO = new GLuint[VBOnum];
			glGenVertexArrays(1, &_VAO);
			glGenBuffers(VBOnum, _VBO);
			glGenBuffers(1, &_EBO);
		}
		~mDrawElement()
		{
			if (isInitialized())
			{
				glDeleteVertexArrays(1, &_VAO);
				glDeleteBuffers(1, &_EBO);
				glDeleteBuffers(_VBONum, _VBO);
				if (_VBO != nullptr)
				{
					delete _VBO;
					_VBO = nullptr;
				}
			}
		
		}

	public:
		/*
		* 绘制
		*/
		void draw()
		{
			glBindVertexArray(_VAO);
			glDrawElements(_drawModel, _indexNum, GL_UNSIGNED_INT, 0);
		
		}

		
	public:
		GLuint _VAO, *_VBO = nullptr, _EBO;
		//顶点数量
		int _vertexNum = 0;
		//索引数量
		int _indexNum = 0;
		GLenum _drawModel = GL_LINES;
		//VBO数量
		int _VBONum = 0;

	};
	//纹理绘制方式
	class mDrawTexture:public mDrawArray
	{
	public:
		explicit mDrawTexture(int VBOnum, GLenum drawModel) :mDrawArray(VBOnum, drawModel) 
		{
			this->initializeOpenGLFunctions();
		};
		~mDrawTexture() {};
		/*
		* 绘制
		*/
		void draw()
		{
			glBindTexture(GL_TEXTURE_2D,_texture);
			glBindVertexArray(_VAO);
			glDrawArrays(_drawModel, 0, _vertexNum);
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	public:
		GLuint _texture; //纹理
		GLenum _drawModel = GL_QUADS;

	};


	template <class T>
	class mLableRendBase : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
	{
	public:
		mLableRendBase(int VBOnum, GLenum drawModel)
		{
			this->initializeOpenGLFunctions();
			_context = QOpenGLContext::currentContext();
			_rendObject = make_shared<T>(VBOnum, drawModel);
		
		}
		virtual ~mLableRendBase()
		{
		};

		/*
		* 单个图标更新
		*/
		virtual void updateSingleRend(LableOperateMode mode, const QString &name) {};
		virtual void updateSingleRend_OneFrame(const int frameID, LableOperateMode mode, const QString &name) {};
		/*
		* 所有图标更新
		*/
		virtual void updateAllRend(LableOperateMode mode) {};  
		/*
		* 添加某一帧所有图标 
		*/
		virtual void updateAllRend_OneFrame(const int frameID, LableOperateMode mode) {};

	protected:
		/*
		* 创建单个VBO buffer
		*/
		inline void createVBObuffer(GLuint &VBO, const int size = 0, const GLenum usage = GL_STATIC_DRAW)
		{
			glGenBuffers(1, &VBO);
			if (size > 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, VBO);//point		
				glBufferData(GL_ARRAY_BUFFER, size, NULL, usage);
			}
		}
		/*
		* 创建单个EBO buffer
		*/
		inline void createEBObuffer(GLuint &EBO, const int size = 0, const GLenum usage = GL_STATIC_DRAW)
		{
			glGenBuffers(1, &EBO);
			if (size > 0)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, usage);
			}
		}
		/*
		* 复制buffer
		*/
		inline void copyBuffer(const GLuint read, const GLuint write, const int read_offset, const int write_offset, const int size)
		{
			glBindBuffer(GL_COPY_READ_BUFFER, read);
			glBindBuffer(GL_COPY_WRITE_BUFFER, write);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, read_offset, write_offset, size);
		}
		/*
		* 根据数据类型 获取 GLenum类型
		*/
		template<class T1>
		GLenum getGLenumType()
		{
			if (is_same<T1, float>::value) return GL_FLOAT;
			else if (is_same<T1, GLubyte>::value) return GL_UNSIGNED_BYTE;
			else if (is_same<T1, GLushort>::value) return GL_UNSIGNED_SHORT;
			else if (is_same<T1, GLshort>::value) return GL_SHORT;
			else if (is_same<T1, int>::value) return GL_INT;
		}

		/*
		* 添加固定的顶点数据
		*/
		inline void appendCommonVertex(const bool highlightstate, const bool showstate, const bool depthstate, const float size,const QVector3D color, const QVector3D vertex)
		{
			//控制高亮的参数
			_vertex_HighlightState.push_back(highlightstate);
			//控制显隐的参数
			_vertex_ShowState.push_back(showstate);
			//控制深度测试参数
			_vertex_DepthState.push_back(depthstate);
			//控制大小的参数
			_vertex_Size.push_back(size);
			//颜色	
			_vertex_Color.push_back(color.x());
			_vertex_Color.push_back(color.y());
			_vertex_Color.push_back(color.z());
			//顶点坐标
			_vertex_Vertex.push_back(vertex.x());
			_vertex_Vertex.push_back(vertex.y());
			_vertex_Vertex.push_back(vertex.z());

		}
		/*
		* 清空固定的顶点数据
		*/
		inline void deleteCommonVerrtex()
		{
			vector<GLubyte>().swap(_vertex_HighlightState);
			vector<GLubyte>().swap(_vertex_ShowState);
			vector<GLubyte>().swap(_vertex_DepthState);
			vector<float>().swap(_vertex_Size);
			vector<GLubyte>().swap(_vertex_Color);
			vector<float>().swap(_vertex_Vertex);
		}

		/*
		* 创建固定数据的临时VBOtmpbuffer
		*/
		inline void createCommonTmpBuffer(GLuint *VBO, const int size = 0,const GLenum usage = GL_STATIC_DRAW)
		{
			createVBObuffer(VBO[0], sizeof(GLubyte)*size);
			createVBObuffer(VBO[1], sizeof(GLubyte)*size);
			createVBObuffer(VBO[2], sizeof(GLubyte)*size);
			createVBObuffer(VBO[3], sizeof(float)*size);
			createVBObuffer(VBO[4], 3 * sizeof(GLubyte)*size);
			createVBObuffer(VBO[5], 3 * sizeof(float)*size);
		}
		/*
		* 复制固定数据至临时VBOtmpbuffer
		*/
		inline void copyCommonTmpBuffer(const GLuint *read, const GLuint *write, const int read_offset_size, const int write_offset_size, const int size)
		{
			copyBuffer(read[0], write[0], sizeof(GLubyte)*read_offset_size, sizeof(GLubyte)*write_offset_size, sizeof(GLubyte)*size);
			copyBuffer(read[1], write[1], sizeof(GLubyte)* read_offset_size, sizeof(GLubyte)*write_offset_size, sizeof(GLubyte)*size);
			copyBuffer(read[2], write[2], sizeof(GLubyte)*read_offset_size, sizeof(GLubyte)*write_offset_size, sizeof(GLubyte)*size);
			copyBuffer(read[3], write[3], sizeof(float)*read_offset_size, sizeof(float)*write_offset_size, sizeof(float)*size);
			copyBuffer(read[4], write[4], 3 * sizeof(GLubyte)*read_offset_size, 3 * sizeof(GLubyte)*write_offset_size, 3 * sizeof(GLubyte)*size);
			copyBuffer(read[5], write[5], 3 * sizeof(float)*read_offset_size, 3 * sizeof(float)*write_offset_size, 3 * sizeof(float)*size);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		/*
		* 添加数据更新buffer
		* propT:数据比例 vertex：添加的额外顶点  EBOindex：添加的额外索引 VBOBegin：新增顶点数据的起始位置 EBOBegin：新增EBO索引的起始位置
		*/
		//6
		void initialBuffer_append(const int vertexBegin)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//当前顶点数量
			int vertexSize = _rendObject->_vertexNum;
			//创建临时buffer
			GLuint tmpVBO[6];
			createCommonTmpBuffer(tmpVBO, vertexSize);
			//复制临时的buffer(复制以前的数据)
			if (vertexBegin != 0)
			{
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, vertexBegin);
				glDeleteBuffers(6, _rendObject->_VBO);
			}

			//高亮状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_HighlightState.size(), _vertex_HighlightState.data());
			//显示隐藏状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_ShowState.size(), _vertex_ShowState.data());
			//深度测试状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_DepthState.size(), _vertex_DepthState.data());
			//大小
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(float), sizeof(float) * _vertex_Size.size(), _vertex_Size.data());
			//颜色
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_Color.size(), _vertex_Color.data());
			//顶点位置
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(float), sizeof(float) * _vertex_Vertex.size(), _vertex_Vertex.data());
			
			//更新VBO数据并绑定VAO
			glBindVertexArray(_rendObject->_VAO);
			
			//分配顶点数据
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindVertexArray(0);
			//交换bufferID
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
		}
		//6+1 
		template <class T1>
		void initialBuffer_append(const int propT1, const vector<T1> &vertex1,const int vertexBegin)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//当前顶点数量
			int vertexSize = _rendObject->_vertexNum;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			//复制临时的buffer(复制以前的数据)
			if (vertexBegin != 0)
			{
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, vertexBegin);
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * vertexBegin * sizeof(T1));
				glDeleteBuffers(7, _rendObject->_VBO);
			}
			//更新VBO数据并绑定VAO
			glBindVertexArray(_rendObject->_VAO);
			//高亮状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_HighlightState.size(), _vertex_HighlightState.data());
			//显示隐藏状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_ShowState.size(), _vertex_ShowState.data());
			//深度测试状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_DepthState.size(), _vertex_DepthState.data());
			//大小
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(float), sizeof(float) * _vertex_Size.size(), _vertex_Size.data());
			//颜色
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_Color.size(), _vertex_Color.data());
			//顶点位置
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(float), sizeof(float) * _vertex_Vertex.size(), _vertex_Vertex.data());
			//更新特定的VBObuffer
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glBufferSubData(GL_ARRAY_BUFFER, propT1 * vertexBegin * sizeof(T1), sizeof(T1) * vertex1.size(), vertex1.data());

			//分配顶点数据
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);


			//交换bufferID
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;

		}

		//6+2 
		template <class T1,class T2>
		void initialBuffer_append(const int propT1, const int propT2, const vector<T1> &vertex1, const vector<T2> &vertex2,
			const int vertexBegin)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//当前顶点数量
			int vertexSize = _rendObject->_vertexNum;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1, tmpVBO_T2;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			createVBObuffer(tmpVBO_T2, propT2 * sizeof(T2)*vertexSize);
			//复制临时的buffer(复制以前的数据)
			if (vertexBegin != 0)
			{
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, vertexBegin);
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * vertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, 0, 0, propT2 * vertexBegin * sizeof(T2));
				glDeleteBuffers(8, _rendObject->_VBO);			
			}
			//更新VBO数据并绑定VAO
			glBindVertexArray(_rendObject->_VAO);
			//高亮状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_HighlightState.size(), _vertex_HighlightState.data());
			//显示隐藏状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_ShowState.size(), _vertex_ShowState.data());
			//深度测试状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_DepthState.size(), _vertex_DepthState.data());
			//大小
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(float), sizeof(float) * _vertex_Size.size(), _vertex_Size.data());
			//颜色
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_Color.size(), _vertex_Color.data());
			//顶点位置
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(float), sizeof(float) * _vertex_Vertex.size(), _vertex_Vertex.data());
			//更新特定的VBObuffer
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glBufferSubData(GL_ARRAY_BUFFER, propT1 * vertexBegin * sizeof(T1), sizeof(T1) * vertex1.size(), vertex1.data());

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T2);
			glBufferSubData(GL_ARRAY_BUFFER, propT2 * vertexBegin * sizeof(T2), sizeof(T2) * vertex2.size(), vertex2.data());

			//分配顶点数据
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T2);
			glVertexAttribPointer(7, propT2, getGLenumType<T2>(), GL_FALSE, propT2 * sizeof(T2), (void*)0);
			glEnableVertexAttribArray(7);
				
			//交换bufferID
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;
			_rendObject->_VBO[7] = tmpVBO_T2;
		}
		//6+2  +EBO
		template <class T1, class T2>
		void initialBuffer_append_index(const int propT1, const int propT2, const vector<T1> &vertex1, const vector<T2> &vertex2,
			const int vertexBegin, const vector<GLuint> &EBOindex)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//当前顶点数量
			int vertexSize = _rendObject->_vertexNum;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1, tmpVBO_T2, tmpEBO;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			createVBObuffer(tmpVBO_T2, propT2 * sizeof(T2)*vertexSize);
			createEBObuffer(tmpEBO, sizeof(GLuint)*_rendObject->_indexNum);
			//复制临时的buffer(复制以前的数据)
			if (vertexBegin != 0)
			{
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, vertexBegin);
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * vertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, 0, 0, propT2 * vertexBegin * sizeof(T2));
				glDeleteBuffers(8, _rendObject->_VBO);
				glDeleteBuffers(1, &_rendObject->_EBO);
			}
			//更新VBO数据并绑定VAO
			glBindVertexArray(_rendObject->_VAO);
			//高亮状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_HighlightState.size(), _vertex_HighlightState.data());
			//显示隐藏状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_ShowState.size(), _vertex_ShowState.data());
			//深度测试状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_DepthState.size(), _vertex_DepthState.data());
			//大小
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(float), sizeof(float) * _vertex_Size.size(), _vertex_Size.data());
			//颜色
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_Color.size(), _vertex_Color.data());
			//顶点位置
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(float), sizeof(float) * _vertex_Vertex.size(), _vertex_Vertex.data());
			//更新特定的VBObuffer
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glBufferSubData(GL_ARRAY_BUFFER, propT1 * vertexBegin * sizeof(T1), sizeof(T1) * vertex1.size(), vertex1.data());

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T2);
			glBufferSubData(GL_ARRAY_BUFFER, propT2 * vertexBegin * sizeof(T2), sizeof(T2) * vertex2.size(), vertex2.data());

			//更新EBO		
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpEBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * EBOindex.size(), EBOindex.data());
			
			//分配顶点数据
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T2);
			glVertexAttribPointer(7, propT2, getGLenumType<T2>(), GL_FALSE, propT2 * sizeof(T2), (void*)0);
			glEnableVertexAttribArray(7);

			//交换bufferID
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;
			_rendObject->_VBO[7] = tmpVBO_T2;
			_rendObject->_EBO = tmpEBO;
		}
		//6+3
		template <class T1, class T2, class T3>
		void initialBuffer_append(const int propT1, const int propT2, const int propT3,const vector<T1> &vertex1, const vector<T2> &vertex2,
			const vector<T3> &vertex3, const int vertexBegin)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//当前顶点数量
			int vertexSize = _rendObject->_vertexNum;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1, tmpVBO_T2, tmpVBO_T3;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			createVBObuffer(tmpVBO_T2, propT2 * sizeof(T2)*vertexSize);
			createVBObuffer(tmpVBO_T3, propT3 * sizeof(T3)*vertexSize);
			//复制临时的buffer(复制以前的数据)
			if (vertexBegin != 0)
			{
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, vertexBegin);
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * vertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, 0, 0, propT2 * vertexBegin * sizeof(T2));
				copyBuffer(_rendObject->_VBO[8], tmpVBO_T3, 0, 0, propT3 * vertexBegin * sizeof(T3));
				glDeleteBuffers(9, _rendObject->_VBO);
			}
			//更新VBO数据并绑定VAO
			glBindVertexArray(_rendObject->_VAO);
			//高亮状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_HighlightState.size(), _vertex_HighlightState.data());
			//显示隐藏状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_ShowState.size(), _vertex_ShowState.data());
			//深度测试状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_DepthState.size(), _vertex_DepthState.data());
			//大小
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(float), sizeof(float) * _vertex_Size.size(), _vertex_Size.data());
			//颜色
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_Color.size(), _vertex_Color.data());
			//顶点位置
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(float), sizeof(float) * _vertex_Vertex.size(), _vertex_Vertex.data());
			//更新特定的VBObuffer
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glBufferSubData(GL_ARRAY_BUFFER, propT1 * vertexBegin * sizeof(T1), sizeof(T1) * vertex1.size(), vertex1.data());

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T2);
			glBufferSubData(GL_ARRAY_BUFFER, propT2 * vertexBegin * sizeof(T2), sizeof(T2) * vertex2.size(), vertex2.data());

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T3);
			glBufferSubData(GL_ARRAY_BUFFER, propT3 * vertexBegin * sizeof(T3), sizeof(T3) * vertex3.size(), vertex3.data());
		
			//分配顶点数据
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T2);
			glVertexAttribPointer(7, propT2, getGLenumType<T2>(), GL_FALSE, propT2 * sizeof(T2), (void*)0);
			glEnableVertexAttribArray(7);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T3);
			glVertexAttribPointer(8, propT3, getGLenumType<T3>(), GL_FALSE, propT3 * sizeof(T3), (void*)0);
			glEnableVertexAttribArray(8);

			//交换bufferID
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;
			_rendObject->_VBO[7] = tmpVBO_T2;
			_rendObject->_VBO[8] = tmpVBO_T3;
		}
		//6+3 +EBO
		template <class T1, class T2, class T3>
		void initialBuffer_append_index(const int propT1, const int propT2, const int propT3, const vector<T1> &vertex1, const vector<T2> &vertex2,
			const vector<T3> &vertex3, const int vertexBegin, const vector<GLuint> &EBOindex)

		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//当前顶点数量
			int vertexSize = _rendObject->_vertexNum;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1, tmpVBO_T2, tmpVBO_T3, tmpEBO;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			createVBObuffer(tmpVBO_T2, propT2 * sizeof(T2)*vertexSize);
			createVBObuffer(tmpVBO_T3, propT3 * sizeof(T3)*vertexSize);
			createEBObuffer(tmpEBO, sizeof(GLuint)*_rendObject->_indexNum);
			//复制临时的buffer(复制以前的数据)
			if (vertexBegin != 0)
			{
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, vertexBegin);
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * vertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, 0, 0, propT2 * vertexBegin * sizeof(T2));
				copyBuffer(_rendObject->_VBO[8], tmpVBO_T3, 0, 0, propT3 * vertexBegin * sizeof(T3));
				glDeleteBuffers(9, _rendObject->_VBO);			
				glDeleteBuffers(1, &_rendObject->_EBO);
			
			}
			//更新VBO数据并绑定VAO
			glBindVertexArray(_rendObject->_VAO);
			//高亮状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_HighlightState.size(), _vertex_HighlightState.data());
			//显示隐藏状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_ShowState.size(), _vertex_ShowState.data());
			//深度测试状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_DepthState.size(), _vertex_DepthState.data());
			//大小
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(float), sizeof(float) * _vertex_Size.size(), _vertex_Size.data());
			//颜色
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_Color.size(), _vertex_Color.data());
			//顶点位置
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(float), sizeof(float) * _vertex_Vertex.size(), _vertex_Vertex.data());
			//更新特定的VBObuffer
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glBufferSubData(GL_ARRAY_BUFFER, propT1 * vertexBegin * sizeof(T1), sizeof(T1) * vertex1.size(), vertex1.data());

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T2);
			glBufferSubData(GL_ARRAY_BUFFER, propT2 * vertexBegin * sizeof(T2), sizeof(T2) * vertex2.size(), vertex2.data());

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T3);
			glBufferSubData(GL_ARRAY_BUFFER, propT3 * vertexBegin * sizeof(T3), sizeof(T3) * vertex3.size(), vertex3.data());

			//更新EBO		
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpEBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * EBOindex.size(), EBOindex.data());
			//分配顶点数据
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T2);
			glVertexAttribPointer(7, propT2, getGLenumType<T2>(), GL_FALSE, propT2 * sizeof(T2), (void*)0);
			glEnableVertexAttribArray(7);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T3);
			glVertexAttribPointer(8, propT3, getGLenumType<T3>(), GL_FALSE, propT3 * sizeof(T3), (void*)0);
			glEnableVertexAttribArray(8);

			//交换bufferID
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;
			_rendObject->_VBO[7] = tmpVBO_T2;
			_rendObject->_VBO[8] = tmpVBO_T3;
			_rendObject->_EBO = tmpEBO;
		}
		//6+4
		template <class T1, class T2, class T3,class T4>
		void initialBuffer_append(const int propT1, const int propT2, const int propT3, const int propT4, const vector<T1> &vertex1, const vector<T2> &vertex2,
			const vector<T3> &vertex3, const vector<T4> &vertex4,const int vertexBegin)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//当前顶点数量
			int vertexSize = _rendObject->_vertexNum;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1, tmpVBO_T2, tmpVBO_T3, tmpVBO_T4;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			createVBObuffer(tmpVBO_T2, propT2 * sizeof(T2)*vertexSize);
			createVBObuffer(tmpVBO_T3, propT3 * sizeof(T3)*vertexSize);
			createVBObuffer(tmpVBO_T4, propT4 * sizeof(T4)*vertexSize);
			//复制临时的buffer(复制以前的数据)
			if (vertexBegin != 0)
			{
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, vertexBegin);
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * vertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, 0, 0, propT2 * vertexBegin * sizeof(T2));
				copyBuffer(_rendObject->_VBO[8], tmpVBO_T3, 0, 0, propT3 * vertexBegin * sizeof(T3));
				copyBuffer(_rendObject->_VBO[9], tmpVBO_T4, 0, 0, propT4 * vertexBegin * sizeof(T4));
				glDeleteBuffers(10, _rendObject->_VBO);

			}
			//更新VBO数据并绑定VAO
			glBindVertexArray(_rendObject->_VAO);
			//高亮状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_HighlightState.size(), _vertex_HighlightState.data());
			//显示隐藏状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_ShowState.size(), _vertex_ShowState.data());
			//深度测试状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_DepthState.size(), _vertex_DepthState.data());
			//大小
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(float), sizeof(float) * _vertex_Size.size(), _vertex_Size.data());
			//颜色
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_Color.size(), _vertex_Color.data());
			//顶点位置
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(float), sizeof(float) * _vertex_Vertex.size(), _vertex_Vertex.data());
			//更新特定的VBObuffer
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glBufferSubData(GL_ARRAY_BUFFER, propT1 * vertexBegin * sizeof(T1), sizeof(T1) * vertex1.size(), vertex1.data());

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T2);
			glBufferSubData(GL_ARRAY_BUFFER, propT2 * vertexBegin * sizeof(T2), sizeof(T2) * vertex2.size(), vertex2.data());

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T3);
			glBufferSubData(GL_ARRAY_BUFFER, propT3 * vertexBegin * sizeof(T3), sizeof(T3) * vertex3.size(), vertex3.data());

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T4);
			glBufferSubData(GL_ARRAY_BUFFER, propT4 * vertexBegin * sizeof(T4), sizeof(T4) * vertex4.size(), vertex4.data());
			
			//分配顶点数据
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T2);
			glVertexAttribPointer(7, propT2, getGLenumType<T2>(), GL_FALSE, propT2 * sizeof(T2), (void*)0);
			glEnableVertexAttribArray(7);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T3);
			glVertexAttribPointer(8, propT3, getGLenumType<T3>(), GL_FALSE, propT3 * sizeof(T3), (void*)0);
			glEnableVertexAttribArray(8);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T4);
			glVertexAttribPointer(9, propT4, getGLenumType<T4>(), GL_FALSE, propT4 * sizeof(T4), (void*)0);
			glEnableVertexAttribArray(9);

			//交换bufferID
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;
			_rendObject->_VBO[7] = tmpVBO_T2;
			_rendObject->_VBO[8] = tmpVBO_T3;
			_rendObject->_VBO[9] = tmpVBO_T4;
		}
		//6+4 +EBO
		template <class T1, class T2, class T3, class T4>
		void initialBuffer_append_index(const int propT1, const int propT2, const int propT3, const int propT4, const vector<T1> &vertex1, const vector<T2> &vertex2,
			const vector<T3> &vertex3, const vector<T4> &vertex4, const int vertexBegin, const vector<GLuint> &EBOindex)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//当前顶点数量
			int vertexSize = _rendObject->_vertexNum;
			//创建临时buffer
                        GLuint tmpVBO[6], tmpVBO_T1, tmpVBO_T2, tmpVBO_T3, tmpVBO_T4,tmpEBO;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			createVBObuffer(tmpVBO_T2, propT2 * sizeof(T2)*vertexSize);
			createVBObuffer(tmpVBO_T3, propT3 * sizeof(T3)*vertexSize);
			createVBObuffer(tmpVBO_T4, propT4 * sizeof(T4)*vertexSize);
			createEBObuffer(tmpEBO, sizeof(GLuint)*_rendObject->_indexNum);
			//复制临时的buffer(复制以前的数据)
			if (vertexBegin != 0)
			{
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, vertexBegin);
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * vertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, 0, 0, propT2 * vertexBegin * sizeof(T2));
				copyBuffer(_rendObject->_VBO[8], tmpVBO_T3, 0, 0, propT3 * vertexBegin * sizeof(T3));
				copyBuffer(_rendObject->_VBO[9], tmpVBO_T4, 0, 0, propT4 * vertexBegin * sizeof(T4));
				glDeleteBuffers(10, _rendObject->_VBO);
				glDeleteBuffers(1, &_rendObject->_EBO);
				

			}
			//更新VBO数据并绑定VAO
			glBindVertexArray(_rendObject->_VAO);
			//高亮状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_HighlightState.size(), _vertex_HighlightState.data());
			//显示隐藏状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_ShowState.size(), _vertex_ShowState.data());
			//深度测试状态
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_DepthState.size(), _vertex_DepthState.data());
			//大小
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glBufferSubData(GL_ARRAY_BUFFER, vertexBegin * sizeof(float), sizeof(float) * _vertex_Size.size(), _vertex_Size.data());
			//颜色
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(GLubyte), sizeof(GLubyte) * _vertex_Color.size(), _vertex_Color.data());
			//顶点位置
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glBufferSubData(GL_ARRAY_BUFFER, 3 * vertexBegin * sizeof(float), sizeof(float) * _vertex_Vertex.size(), _vertex_Vertex.data());
			//更新特定的VBObuffer
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glBufferSubData(GL_ARRAY_BUFFER, propT1 * vertexBegin * sizeof(T1), sizeof(T1) * vertex1.size(), vertex1.data());

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T2);
			glBufferSubData(GL_ARRAY_BUFFER, propT2 * vertexBegin * sizeof(T2), sizeof(T2) * vertex2.size(), vertex2.data());

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T3);
			glBufferSubData(GL_ARRAY_BUFFER, propT3 * vertexBegin * sizeof(T3), sizeof(T3) * vertex3.size(), vertex3.data());

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T4);
			glBufferSubData(GL_ARRAY_BUFFER, propT4 * vertexBegin * sizeof(T4), sizeof(T4) * vertex4.size(), vertex4.data());
			//更新EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpEBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * EBOindex.size(), EBOindex.data());
			
			//分配顶点数据
			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T1);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T2);
			glVertexAttribPointer(7, propT2, getGLenumType<T2>(), GL_FALSE, propT2 * sizeof(T2), (void*)0);
			glEnableVertexAttribArray(7);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T3);
			glVertexAttribPointer(8, propT3, getGLenumType<T3>(), GL_FALSE, propT3 * sizeof(T3), (void*)0);
			glEnableVertexAttribArray(8);

			glBindBuffer(GL_ARRAY_BUFFER, tmpVBO_T4);
			glVertexAttribPointer(9, propT4, getGLenumType<T4>(), GL_FALSE, propT4 * sizeof(T4), (void*)0);
			glEnableVertexAttribArray(9);

			//交换bufferID
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;
			_rendObject->_VBO[7] = tmpVBO_T2;
			_rendObject->_VBO[8] = tmpVBO_T3;
			_rendObject->_VBO[9] = tmpVBO_T4;
			_rendObject->_EBO = tmpEBO;
		}
		/*
		* 删除数据更新buffer
		* propT:数据比例 vertex：添加的额外顶点  deletevertexBegin：删除顶点数据的起始位置（比例为1情况下）
		* deleteVBOsize：删除顶点数据的个数（比例为1情况下）  deleteEBOsize ：删除索引数据的个数
		*/
		//6
		void initialBuffer_delete(const int deleteVertexBegin, const int deleteVertexSize)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//剩下的大小
			int vertexSize = _rendObject->_vertexNum;
			//删除终点位置
			int deleteVertexEnd = deleteVertexBegin + deleteVertexSize;
			//创建临时buffer
			GLuint tmpVBO[6];
			createCommonTmpBuffer(tmpVBO, vertexSize);

			//复制数据
			if (vertexSize > 0)
			{
				//复制通用数据
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, deleteVertexBegin);//复制前半段
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, deleteVertexEnd, deleteVertexBegin, vertexSize - deleteVertexBegin);//复制后半段
				//复制额外数据
				glDeleteBuffers(6, _rendObject->_VBO);

			}
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}

			//分配顶点数据
			glBindVertexArray(_rendObject->_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindVertexArray(0);

		}
		//6+1
		template <class T1>
		void initialBuffer_delete(const int propT1, const int deleteVertexBegin, const int deleteVertexSize)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//剩下的大小
			int vertexSize = _rendObject->_vertexNum;
			//删除终点位置
			int deleteVertexEnd = deleteVertexBegin + deleteVertexSize;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);


			//复制数据
			if (vertexSize > 0)
			{
				//复制通用数据
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, deleteVertexBegin);//复制前半段
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, deleteVertexEnd, deleteVertexBegin, vertexSize - deleteVertexBegin);//复制后半段
				//复制额外数据
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * deleteVertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, deleteVertexEnd * propT1 * sizeof(T1), deleteVertexBegin * propT1 * sizeof(T1), propT1 * (vertexSize - deleteVertexBegin) * sizeof(T1));
				glDeleteBuffers(7, _rendObject->_VBO);

			}
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;

			//分配顶点数据
			glBindVertexArray(_rendObject->_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[6]);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindVertexArray(0);
			
		}
		//6+2
		template <class T1, class T2>
		void initialBuffer_delete(const int propT1, const int propT2, const int deleteVertexBegin, const int deleteVertexSize)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//剩下的大小
			int vertexSize = _rendObject->_vertexNum;
			//删除终点位置
			int deleteVertexEnd = deleteVertexBegin + deleteVertexSize;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1, tmpVBO_T2;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			createVBObuffer(tmpVBO_T2, propT2 * sizeof(T2)*vertexSize);
		
			
			//复制数据
			if (vertexSize > 0)
			{
				//复制通用数据
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, deleteVertexBegin);//复制前半段
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, deleteVertexEnd, deleteVertexBegin, vertexSize - deleteVertexBegin);//复制后半段
				//复制额外数据
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * deleteVertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, deleteVertexEnd * propT1 * sizeof(T1), deleteVertexBegin * propT1 * sizeof(T1), propT1 * (vertexSize - deleteVertexBegin) * sizeof(T1));

				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, 0, 0, propT2 * deleteVertexBegin * sizeof(T2));
				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, deleteVertexEnd * propT2 * sizeof(T2), deleteVertexBegin * propT2 * sizeof(T2), propT2  * (vertexSize - deleteVertexBegin) * sizeof(T2));
				glDeleteBuffers(8, _rendObject->_VBO);
					
			}
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;
			_rendObject->_VBO[7] = tmpVBO_T2;

			//分配顶点数据
			glBindVertexArray(_rendObject->_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[6]);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[7]);
			glVertexAttribPointer(7, propT2, getGLenumType<T2>(), GL_FALSE, propT2 * sizeof(T2), (void*)0);
			glEnableVertexAttribArray(7);

			glBindVertexArray(0);
		}	
		//6+2 +EBO
		template <class T1, class T2>
		void initialBuffer_delete_index(const int propT1, const int propT2, const int deleteVertexBegin, const int deleteVertexSize, const vector<GLuint> &EBOindex)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//剩下的大小
			int vertexSize = _rendObject->_vertexNum;
			int indexSize = _rendObject->_indexNum;
			//删除终点位置
			int deleteVertexEnd = deleteVertexBegin + deleteVertexSize;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1, tmpVBO_T2, tmpEBO;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			createVBObuffer(tmpVBO_T2, propT2 * sizeof(T2)*vertexSize);		
			createEBObuffer(tmpEBO, sizeof(GLuint)*indexSize);
			
			//复制数据
			if (vertexSize > 0)
			{
				//复制通用数据
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, deleteVertexBegin);//复制前半段
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, deleteVertexEnd, deleteVertexBegin, vertexSize - deleteVertexBegin);//复制后半段
				//复制额外数据
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * deleteVertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, deleteVertexEnd * propT1 * sizeof(T1), deleteVertexBegin * propT1 * sizeof(T1), propT1 * (vertexSize - deleteVertexBegin) * sizeof(T1));

				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, 0, 0, propT2 * deleteVertexBegin * sizeof(T2));
				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, deleteVertexEnd * propT2 * sizeof(T2), deleteVertexBegin * propT2 * sizeof(T2), propT2  * (vertexSize - deleteVertexBegin) * sizeof(T2));
				glDeleteBuffers(8, _rendObject->_VBO);

				//删除以前的EBO
				glDeleteBuffers(1, &_rendObject->_EBO);

			}
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;
			_rendObject->_VBO[7] = tmpVBO_T2;
			_rendObject->_EBO = tmpEBO;

			//分配顶点数据
			glBindVertexArray(_rendObject->_VAO);

			//更新EBO		
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendObject->_EBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * EBOindex.size(), EBOindex.data());

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[6]);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[7]);
			glVertexAttribPointer(7, propT2, getGLenumType<T2>(), GL_FALSE, propT2 * sizeof(T2), (void*)0);
			glEnableVertexAttribArray(7);

			glBindVertexArray(0);
			
		}
		//6+3
		template <class T1, class T2, class T3>
		void initialBuffer_delete(const int propT1, const int propT2, const int propT3, const int deleteVertexBegin, const int deleteVertexSize)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//剩下的大小
			int vertexSize = _rendObject->_vertexNum;
			//删除终点位置
			int deleteVertexEnd = deleteVertexBegin + deleteVertexSize;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1, tmpVBO_T2, tmpVBO_T3;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			createVBObuffer(tmpVBO_T2, propT2 * sizeof(T2)*vertexSize);
			createVBObuffer(tmpVBO_T3, propT3 * sizeof(T3)*vertexSize);

			//复制数据
			if (vertexSize > 0)
			{
				//复制通用数据
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, deleteVertexBegin);//复制前半段
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, deleteVertexEnd, deleteVertexBegin, vertexSize - deleteVertexBegin);//复制后半段
				//复制额外数据
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * deleteVertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, deleteVertexEnd * propT1 * sizeof(T1), deleteVertexBegin * propT1 * sizeof(T1), propT1 * (vertexSize - deleteVertexBegin) * sizeof(T1));

				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, 0, 0, propT2 * deleteVertexBegin * sizeof(T2));
				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, deleteVertexEnd * propT2 * sizeof(T2), deleteVertexBegin * propT2 * sizeof(T2), propT2  * (vertexSize - deleteVertexBegin) * sizeof(T2));

				copyBuffer(_rendObject->_VBO[8], tmpVBO_T3, 0, 0, propT3 * deleteVertexBegin * sizeof(T3));
				copyBuffer(_rendObject->_VBO[8], tmpVBO_T3, deleteVertexEnd * propT3 * sizeof(T3), deleteVertexBegin * propT3 * sizeof(T3), propT3  * (vertexSize - deleteVertexBegin) * sizeof(T3));

				glDeleteBuffers(9, _rendObject->_VBO);


			}
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;
			_rendObject->_VBO[7] = tmpVBO_T2;
			_rendObject->_VBO[8] = tmpVBO_T3;

			//分配顶点数据
			glBindVertexArray(_rendObject->_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[6]);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[7]);
			glVertexAttribPointer(7, propT2, getGLenumType<T2>(), GL_FALSE, propT2 * sizeof(T2), (void*)0);
			glEnableVertexAttribArray(7);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[8]);
			glVertexAttribPointer(8, propT3, getGLenumType<T3>(), GL_FALSE, propT3 * sizeof(T3), (void*)0);
			glEnableVertexAttribArray(8);

			glBindVertexArray(0);
		}
		//6+3 +EBO
		template <class T1, class T2, class T3>
		void initialBuffer_delete_index(const int propT1, const int propT2, const int propT3, const int deleteVertexBegin, const int deleteVertexSize, const vector<GLuint> &EBOindex)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//剩下的大小
			int vertexSize = _rendObject->_vertexNum;
			int indexSize = _rendObject->_indexNum;
			//删除终点位置
			int deleteVertexEnd = deleteVertexBegin + deleteVertexSize;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1, tmpVBO_T2, tmpVBO_T3, tmpEBO;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			createVBObuffer(tmpVBO_T2, propT2 * sizeof(T2)*vertexSize);
			createVBObuffer(tmpVBO_T3, propT3 * sizeof(T3)*vertexSize);	
			createEBObuffer(tmpEBO, sizeof(GLuint)*indexSize);
			
			//复制数据
			if (vertexSize > 0 && indexSize > 0)
			{
				//复制通用数据
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, deleteVertexBegin);//复制前半段
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, deleteVertexEnd, deleteVertexBegin, vertexSize - deleteVertexBegin);//复制后半段
				//复制额外数据
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * deleteVertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, deleteVertexEnd * propT1 * sizeof(T1), deleteVertexBegin * propT1 * sizeof(T1), propT1 * (vertexSize - deleteVertexBegin) * sizeof(T1));

				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, 0, 0, propT2 * deleteVertexBegin * sizeof(T2));
				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, deleteVertexEnd * propT2 * sizeof(T2), deleteVertexBegin * propT2 * sizeof(T2), propT2  * (vertexSize - deleteVertexBegin) * sizeof(T2));

				copyBuffer(_rendObject->_VBO[8], tmpVBO_T3, 0, 0, propT3 * deleteVertexBegin * sizeof(T3));
				copyBuffer(_rendObject->_VBO[8], tmpVBO_T3, deleteVertexEnd * propT3 * sizeof(T3), deleteVertexBegin * propT3 * sizeof(T3), propT3  * (vertexSize - deleteVertexBegin) * sizeof(T3));

				glDeleteBuffers(9, _rendObject->_VBO);
				glDeleteBuffers(1, &_rendObject->_EBO);
				

			}
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;
			_rendObject->_VBO[7] = tmpVBO_T2;
			_rendObject->_VBO[8] = tmpVBO_T3;		
			_rendObject->_EBO = tmpEBO;

			//分配顶点数据
			glBindVertexArray(_rendObject->_VAO);

			//更新EBO		
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendObject->_EBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * EBOindex.size(), EBOindex.data());

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[6]);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[7]);
			glVertexAttribPointer(7, propT2, getGLenumType<T2>(), GL_FALSE, propT2 * sizeof(T2), (void*)0);
			glEnableVertexAttribArray(7);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[8]);
			glVertexAttribPointer(8, propT3, getGLenumType<T3>(), GL_FALSE, propT3 * sizeof(T3), (void*)0);
			glEnableVertexAttribArray(8);

			glBindVertexArray(0);
			
		}
		//6+4
		template <class T1, class T2, class T3, class T4>
		void initialBuffer_delete(const int propT1, const int propT2, const int propT3, const int propT4,const int deleteVertexBegin, const int deleteVertexSize)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//剩下的大小
			int vertexSize = _rendObject->_vertexNum;
			//删除终点位置
			int deleteVertexEnd = deleteVertexBegin + deleteVertexSize;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1, tmpVBO_T2, tmpVBO_T3, tmpVBO_T4;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			createVBObuffer(tmpVBO_T2, propT2 * sizeof(T2)*vertexSize);
			createVBObuffer(tmpVBO_T3, propT3 * sizeof(T3)*vertexSize);
			createVBObuffer(tmpVBO_T4, propT4 * sizeof(T4)*vertexSize);

			//复制数据
			if (vertexSize > 0)
			{
				//复制通用数据
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, deleteVertexBegin);//复制前半段
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, deleteVertexEnd, deleteVertexBegin, vertexSize - deleteVertexBegin);//复制后半段
				//复制额外数据
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * deleteVertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, deleteVertexEnd * propT1 * sizeof(T1), deleteVertexBegin * propT1 * sizeof(T1), propT1 * (vertexSize - deleteVertexBegin) * sizeof(T1));

				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, 0, 0, propT2 * deleteVertexBegin * sizeof(T2));
				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, deleteVertexEnd * propT2 * sizeof(T2), deleteVertexBegin * propT2 * sizeof(T2), propT2  * (vertexSize - deleteVertexBegin) * sizeof(T2));

				copyBuffer(_rendObject->_VBO[8], tmpVBO_T3, 0, 0, propT3 * deleteVertexBegin * sizeof(T3));
				copyBuffer(_rendObject->_VBO[8], tmpVBO_T3, deleteVertexEnd * propT3 * sizeof(T3), deleteVertexBegin * propT3 * sizeof(T3), propT3  * (vertexSize - deleteVertexBegin) * sizeof(T3));

				copyBuffer(_rendObject->_VBO[9], tmpVBO_T4, 0, 0, propT4 * deleteVertexBegin * sizeof(T4));
				copyBuffer(_rendObject->_VBO[9], tmpVBO_T4, deleteVertexEnd * propT4 * sizeof(T4), deleteVertexBegin * propT4 * sizeof(T4), propT4  * (vertexSize - deleteVertexBegin) * sizeof(T4));

				glDeleteBuffers(10, _rendObject->_VBO);
				

			}
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;
			_rendObject->_VBO[7] = tmpVBO_T2;
			_rendObject->_VBO[8] = tmpVBO_T3;
			_rendObject->_VBO[9] = tmpVBO_T4;

			//分配顶点数据
			glBindVertexArray(_rendObject->_VAO);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[6]);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[7]);
			glVertexAttribPointer(7, propT2, getGLenumType<T2>(), GL_FALSE, propT2 * sizeof(T2), (void*)0);
			glEnableVertexAttribArray(7);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[8]);
			glVertexAttribPointer(8, propT3, getGLenumType<T3>(), GL_FALSE, propT3 * sizeof(T3), (void*)0);
			glEnableVertexAttribArray(8);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[9]);
			glVertexAttribPointer(9, propT4, getGLenumType<T4>(), GL_FALSE, propT4 * sizeof(T4), (void*)0);
			glEnableVertexAttribArray(9);

			glBindVertexArray(0);
		}
		//6+4 +EBO
		template <class T1, class T2, class T3, class T4>
		void initialBuffer_delete_index(const int propT1, const int propT2, const int propT3, const int propT4, const int deleteVertexBegin, const int deleteVertexSize, const vector<GLuint> &EBOindex)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			//剩下的大小
			int vertexSize = _rendObject->_vertexNum;
			int indexSize = _rendObject->_indexNum;
			//删除终点位置
			int deleteVertexEnd = deleteVertexBegin + deleteVertexSize;
			//创建临时buffer
			GLuint tmpVBO[6], tmpVBO_T1, tmpVBO_T2, tmpVBO_T3, tmpVBO_T4, tmpEBO;
			createCommonTmpBuffer(tmpVBO, vertexSize);
			createVBObuffer(tmpVBO_T1, propT1 * sizeof(T1)*vertexSize);
			createVBObuffer(tmpVBO_T2, propT2 * sizeof(T2)*vertexSize);
			createVBObuffer(tmpVBO_T3, propT3 * sizeof(T3)*vertexSize);
			createVBObuffer(tmpVBO_T4, propT4 * sizeof(T4)*vertexSize);	
			createEBObuffer(tmpEBO, sizeof(GLuint)*indexSize);
			
			//复制数据
			if (vertexSize > 0 && indexSize > 0)
			{
				//复制通用数据
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, 0, 0, deleteVertexBegin);//复制前半段
				copyCommonTmpBuffer(_rendObject->_VBO, tmpVBO, deleteVertexEnd, deleteVertexBegin, vertexSize - deleteVertexBegin);//复制后半段
				//复制额外数据
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, 0, 0, propT1 * deleteVertexBegin * sizeof(T1));
				copyBuffer(_rendObject->_VBO[6], tmpVBO_T1, deleteVertexEnd * propT1 * sizeof(T1), deleteVertexBegin * propT1 * sizeof(T1), propT1 * (vertexSize - deleteVertexBegin) * sizeof(T1));

				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, 0, 0, propT2 * deleteVertexBegin * sizeof(T2));
				copyBuffer(_rendObject->_VBO[7], tmpVBO_T2, deleteVertexEnd * propT2 * sizeof(T2), deleteVertexBegin * propT2 * sizeof(T2), propT2  * (vertexSize - deleteVertexBegin) * sizeof(T2));

				copyBuffer(_rendObject->_VBO[8], tmpVBO_T3, 0, 0, propT3 * deleteVertexBegin * sizeof(T3));
				copyBuffer(_rendObject->_VBO[8], tmpVBO_T3, deleteVertexEnd * propT3 * sizeof(T3), deleteVertexBegin * propT3 * sizeof(T3), propT3  * (vertexSize - deleteVertexBegin) * sizeof(T3));

				copyBuffer(_rendObject->_VBO[9], tmpVBO_T4, 0, 0, propT4 * deleteVertexBegin * sizeof(T4));
				copyBuffer(_rendObject->_VBO[9], tmpVBO_T4, deleteVertexEnd * propT4 * sizeof(T4), deleteVertexBegin * propT4 * sizeof(T4), propT4  * (vertexSize - deleteVertexBegin) * sizeof(T4));

				glDeleteBuffers(10, _rendObject->_VBO);
				glDeleteBuffers(1, &_rendObject->_EBO);

			}
			for (int i = 0; i < 6; ++i)
			{
				_rendObject->_VBO[i] = tmpVBO[i];
			}
			_rendObject->_VBO[6] = tmpVBO_T1;
			_rendObject->_VBO[7] = tmpVBO_T2;
			_rendObject->_VBO[8] = tmpVBO_T3;
			_rendObject->_VBO[9] = tmpVBO_T4;	
			_rendObject->_EBO = tmpEBO;

			//分配顶点数据
			glBindVertexArray(_rendObject->_VAO);

			//更新EBO		
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendObject->_EBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * EBOindex.size(), EBOindex.data());

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[0]);
			glVertexAttribPointer(0, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[1]);
			glVertexAttribPointer(1, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[2]);
			glVertexAttribPointer(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[3]);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[4]);
			glVertexAttribPointer(4, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(GLubyte), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[5]);
			glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(5);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[6]);
			glVertexAttribPointer(6, propT1, getGLenumType<T1>(), GL_FALSE, propT1 * sizeof(T1), (void*)0);
			glEnableVertexAttribArray(6);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[7]);
			glVertexAttribPointer(7, propT2, getGLenumType<T2>(), GL_FALSE, propT2 * sizeof(T2), (void*)0);
			glEnableVertexAttribArray(7);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[8]);
			glVertexAttribPointer(8, propT3, getGLenumType<T3>(), GL_FALSE, propT3 * sizeof(T3), (void*)0);
			glEnableVertexAttribArray(8);

			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[9]);
			glVertexAttribPointer(9, propT4, getGLenumType<T4>(), GL_FALSE, propT4 * sizeof(T4), (void*)0);
			glEnableVertexAttribArray(9);

			glBindVertexArray(0);
		}
		/*
		*修改状态(显示，隐藏，颜色，大小)
		*/
		template<class T1>
		void initialBuffer_update(const LableOperateMode mode, const int prop, const vector<T1> &vertex, const int vertexBegin, const int vertexSize)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			if (mode == Show || mode == Hide)
			{
				glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[1]);
			}
			else if (mode == Highlight || mode == NoHighlight)
			{
				glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[0]);
			}
			else if (mode == SetColor)
			{
				glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[4]);
			}
			else if (mode == SetSize)
			{
				glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[3]);
			}
			glBufferSubData(GL_ARRAY_BUFFER, prop * vertexBegin * sizeof(T1), sizeof(T1) * vertex.size(), vertex.data());
		}
		/*
		*修改大小（用于更新图标大小时 同时更新文字的位置 用于箭头末端文字 或者固定图标文字）
		*/
		template<class T1>
		void initialBuffer_update(const int prop, const vector<T1> &vertex, const int vertexBegin, const int vertexSize)
		{
			if (!this->isInitialized()) return;
			//**保证上下文一致**
			_context->makeCurrent(_context->surface());
			glBindBuffer(GL_ARRAY_BUFFER, _rendObject->_VBO[8]);			
			glBufferSubData(GL_ARRAY_BUFFER, prop * vertexBegin * sizeof(T1), sizeof(T1) * vertex.size(), vertex.data());
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	public:
		//绘制对象
		shared_ptr<T> _rendObject{};
	protected:
		//顶点索引 QHash<ID, QPair<起始位置，个数>>
		QHash<int, QPair<int, int>> _vertexIndex;
		//基本顶点数据(每种类型都存在)
		vector<GLubyte> _vertex_HighlightState; //高亮状态
		vector<GLubyte> _vertex_ShowState;		//显示隐藏状态
		vector<GLubyte> _vertex_DepthState;//深度测试状态
		vector<float> _vertex_Size;//大小
		vector<GLubyte> _vertex_Color;	//颜色
		vector<float> _vertex_Vertex;	//顶点位置
		//OpenGL上下文
		QOpenGLContext *_context{};
	};

}
