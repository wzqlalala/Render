#pragma once
#include <qopenglfunctions_4_5_core.h>
#include <qvector.h>
#include <qvector4d.h>
#include <qvector3d.h>
#include <qvector2d.h>
#include <qmatrix4x4.h>
#include "buffer.h"
#include "asset.h"

namespace mxr
{
	struct RENDBASE_EXPORT BufferAttribute
	{
		int _start;
		int _end;
	};


	class RENDBASE_EXPORT Array
	{
	public:
		Array() {}
		virtual ~Array() {}
		virtual const  GLvoid* data() const = 0;
		virtual GLenum getDataType() = 0;
		virtual int    getDataSize() = 0;
		//�����ܵ����ݸ���
		virtual int    size() = 0;
		virtual bool   CompareForm(Array *other) = 0;
		virtual void   SetBuffer(asset_ref<IBuffer> buffer) = 0;
		virtual asset_ref<IBuffer>   getBuffer() = 0;
		virtual void   getData(void* data) = 0;
		virtual void   updata(const void* data) = 0;
		virtual void   updata(int begin, int _size, const void* data) = 0;
		virtual void   setBufferAttribute(BufferAttribute attribute) = 0;
		virtual BufferAttribute getBufferAttribute() = 0;
		virtual void clear() = 0;
		
	};

	template<class T, int DataSize, GLenum DataType>
	class TemplateArray :public Array
	{
	public:
		TemplateArray() :Array()
		{
			_dataSize = DataSize;
			_dataType = DataType;
			_buffer = nullptr;
			_Size = 0;
		};

		TemplateArray(const QVector<T> &data) :Array()
		{
			_data = data;
			_dataSize = DataSize;
			_dataType = DataType;
			_buffer = nullptr;
			_Size = data.size();
		}


		virtual ~TemplateArray()
		{
			//_bufferҪ��������
			


			_data.clear();
		}
		
		virtual int size() { return DataSize * _Size; }
		virtual const GLvoid* data() const { return _data.data(); }
		virtual GLenum getDataType() { return DataType; }
		virtual int  getDataSize() { return DataSize; }
		virtual void SetBuffer(asset_ref<IBuffer> buffer) { _buffer = buffer; }
		virtual asset_ref<IBuffer>   getBuffer() { return _buffer; };
		virtual void setBufferAttribute(BufferAttribute attribute) { _attribute = attribute; }
		virtual BufferAttribute getBufferAttribute() { return _attribute; }
	
		void push_back(const T &data) 
		{ 
			_data.push_back(data); 
			_Size++; 
		}

		void append(const T &data)
		{
			_data.append(data);
			_Size++;
		}

		void append(const QVector<T> &data) 
		{ 
			_data.append(data); 
			_Size += data.size(); 
		}

		void remove(int i) 
		{ 
			_data.remove(i);
			_Size -- ��
		}
		virtual void clear() { QVector<T>().swap(_data); }

		virtual void getData(void* data)
		{
			if (_buffer)
			{
				//T *data;
				//QVector<T> data(size());
				_buffer->GetData(_attribute._start, size() * 4, data);
			}
		}

		virtual void updata(const void* data)
		{
			if (_buffer)
			{
				_buffer->SetData(_attribute._start, size() * 4, data);
			}
		}

		virtual void updata(int begin, int _size, const void* data)
		{
			if (_buffer)
			{
				_buffer->SetData(_attribute._start + begin, _size, data);
			}
		}

		void updata(T data)
		{
			if (_buffer)
			{
				QVector<T> datas(_buffer->Size(), data);
				_buffer->SetData(datas.data());
			}
		}

		virtual bool CompareForm(Array *other)
		{
			if (other->getDataType() == getDataType() && other->getDataSize() == getDataSize())
			{
				return true;
			}
			return false;
		}

	protected:
		QVector<T> _data;
		asset_ref<IBuffer> _buffer;	

		//���ݺ��еķ�������,����QVector3D,_dataSize = 3;
		GLint   _dataSize;
		//��������
		GLenum  _dataType;
		//���е����ݵĸ������ȼ���_data.size();
		GLuint  _Size;
		BufferAttribute _attribute;
	};




	 typedef  TemplateArray<GLbyte, 1, GL_BYTE>      ByteArray;
	 typedef  TemplateArray<GLshort, 1, GL_SHORT>    ShortArray;
	 typedef  TemplateArray<GLint, 1, GL_INT>        IntArray;

	 typedef  TemplateArray<GLubyte, 1, GL_UNSIGNED_BYTE>      UByteArray;
	 typedef  TemplateArray<GLushort, 1, GL_UNSIGNED_SHORT>   UShortArray;
	 typedef  TemplateArray<GLuint, 1, GL_UNSIGNED_INT>       UIntArray;

	 typedef  TemplateArray<GLfloat, 1, GL_FLOAT>     FloatArray;
	 typedef  TemplateArray<GLdouble, 1, GL_DOUBLE>   DoubleArray;

	 typedef  TemplateArray<QVector2D, 2, GL_FLOAT>  Vec2Array;
	 typedef  TemplateArray<QVector3D, 3, GL_FLOAT>  Vec3Array;
	 typedef  TemplateArray<QVector4D, 4, GL_FLOAT>  Vec4bArray;



}







