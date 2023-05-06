#pragma once

//////////////////////////////////////////////////////////////////////
//����������Ⱦ ��ͬ��״���� �������ڶ�Ӧ ģ������ϵ�� ����λ�õ�ͼ��//
//////////////////////////////////////////////////////////////////////

#include <QObject>
#include"mLableRendBase.h"
#include"mLableRendEnum.h"
#include"mLableDataStruct.h"
#include"mLableDataEnum.h"
using namespace MDataLable;
namespace MLableRend
{
	//ͨ������ͼ�����
	template<class T>
	class mLableRendFixShapeRotate : public mLableRendBase<mDrawArray>
	{
	public:
		//���Գ�ʼ��ָ��shapeVertex  
		explicit mLableRendFixShapeRotate(const QHash<QString, T> &data, const QVector<QVector3D> &shapeVertex, const GLenum drawMode = GL_LINES)
			:mLableRendBase<mDrawArray>(8, drawMode), _shapeVertex(shapeVertex), _data(data) 
		{
		};
		//���Ը���ǰָ��shapeVertex
		explicit mLableRendFixShapeRotate(const QHash<QString, T> &data, const GLenum drawMode = GL_LINES)
			:mLableRendBase<mDrawArray>(8, drawMode), _data(data) 
		{
		};

		virtual ~mLableRendFixShapeRotate() override {};

		/*
		* ����ͼ�����
		*/
		virtual void updateSingleRend(LableOperateMode mode, const QString &name) override
		{
			auto tmp = _data.value(name);
			long long ID = tmp.lableData.getLableID();
			bool showState = tmp.lableData.getLableShowState();
			bool highlightState = tmp.lableData.getLableHighlightState();
			bool depthState = tmp.lableData.getLableDepthState();
			float size = tmp.lableData.getLableSize();
			QVector3D color = tmp.lableData.getLableColor();
			QVector<POS_DIR> pos_dir = tmp.lableData.getParameter1();
			FixShapeRotateType type = tmp.lableData.getParameter2();
			switch (type)
			{
			case MDataLable::FixShapeRotateType_Revolute:
				_shapeVertex = mLableShapeVertex::getInstance()->_Revolute_Vertex;
				break;
			case MDataLable::FixShapeRotateType_Planar:
				_shapeVertex = mLableShapeVertex::getInstance()->_PlanarRC_Vertex;
				break;
			case MDataLable::FixShapeRotateType_Cylindrical:
				_shapeVertex = mLableShapeVertex::getInstance()->_Cylindrical_Vertex;
				break;
			case MDataLable::FixShapeRotateType_Prismatic:
				_shapeVertex = mLableShapeVertex::getInstance()->_Prismatic_Vertex;
				break;
			default:
				break;
			}
			//�̶�����ͼ��
			if (mode == Append)
			{
				if (_vertexIndex.contains(ID))
				{
					updateSingleRend(Delete, name);
				}

				//����ͼ�궥�������ID������ ����ɾ������
				_vertexIndex[ID] = { _rendObject->_vertexNum,pos_dir.size()*(int)_shapeVertex.size() };

				//����������λ��
				int vertexBegin = _rendObject->_vertexNum;

				//��Ӷ�������
				for (int i = 0; i < pos_dir.size(); i++)
				{
					//��Ӽ�ͷ��������
					for (int j = 0; j < _shapeVertex.size(); j++)
					{
						//���ͨ�õ�����
						appendCommonVertex(highlightState, showState, depthState, size, color, _shapeVertex[j]);
						//������е�����
						appendSpecificVertex(pos_dir[i].first, pos_dir[i].second);
					}
					//���¶�������
					_rendObject->_vertexNum += _shapeVertex.size();
				}
				//����buffer
				initialBuffer_append<float, float>(3, 3, _vertex_Pos, _vertex_Dir, vertexBegin);

				//�������
				deleteSpecificVertex();
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{
				if (_vertexIndex.find(ID) == _vertexIndex.end())
				{
					return;
				}
				int deleteVertexBegin = _vertexIndex[ID].first;
				int deleteVertexSize = _vertexIndex[ID].second;
				//���¶�������
				_rendObject->_vertexNum -= deleteVertexSize;

				//������Ⱦ
				initialBuffer_delete<float, float>(3, 3, deleteVertexBegin, deleteVertexSize);

				_vertexIndex.remove(ID);

				//��������
				for (auto it = _vertexIndex.begin(); it != _vertexIndex.end(); ++it)
				{
					if (it.value().first > deleteVertexBegin)
					{
						_vertexIndex[it.key()] = { it.value().first - deleteVertexSize, it.value().second };
					}
				}


			}
			else if (mode == Show || mode == Hide || mode == Highlight || mode == NoHighlight || mode == SetColor || mode == SetSize)
			{
				if (_vertexIndex.find(ID) == _vertexIndex.end())
				{
					return;
				}
				int updateVertexBegin = _vertexIndex[ID].first;
				int updateVertexSize = _vertexIndex[ID].second;
				if (mode == Show || mode == Highlight)
				{
					vector<GLubyte> tmpVertex(updateVertexSize, 1);
					initialBuffer_update<GLubyte>(mode, 1, tmpVertex, updateVertexBegin, updateVertexSize);

				}
				else if (mode == Hide || mode == NoHighlight)
				{
					vector<GLubyte> tmpVertex(updateVertexSize, 0);
					initialBuffer_update<GLubyte>(mode, 1, tmpVertex, updateVertexBegin, updateVertexSize);
				}
				else if (mode == SetColor)
				{
					vector<GLubyte> tmpVertex;
					for (int i = 0; i < updateVertexSize; ++i)
					{
						tmpVertex.push_back(color.x());
						tmpVertex.push_back(color.y());
						tmpVertex.push_back(color.z());
					}
					initialBuffer_update<GLubyte>(mode, 3, tmpVertex, updateVertexBegin, updateVertexSize);
				}
				else if (mode == SetSize)
				{
					vector<float> tmpVertex;
					for (int i = 0; i < updateVertexSize; ++i)
					{
						tmpVertex.push_back(size);

					}
					initialBuffer_update<float>(mode, 1, tmpVertex, updateVertexBegin, updateVertexSize);
				}

			}

		};

		/*
		* ����ͼ�����
		*/
		virtual void updateAllRend(LableOperateMode mode) override
		{
			if (mode == Append)
			{
				//����������λ��
				int vertexBegin = _rendObject->_vertexNum;
				for (auto it = _data.begin(); it != _data.end(); ++it)
				{
					auto tmp = it.value();
					long long ID = tmp.lableData.getLableID();
					bool showState = tmp.lableData.getLableShowState();
					bool highlightState = tmp.lableData.getLableHighlightState();
					bool depthState = tmp.lableData.getLableDepthState();
					float size = tmp.lableData.getLableSize();
					QVector3D color = tmp.lableData.getLableColor();
					QVector<QPair<QVector3D, QVector3D>> pos_dir = tmp.lableData.getParameter1();
					FixShapeRotateType type = tmp.lableData.getParameter2();
					switch (type)
					{
					case MDataLable::FixShapeRotateType_Revolute:
						_shapeVertex = mLableShapeVertex::getInstance()->_Revolute_Vertex;
						break;
					case MDataLable::FixShapeRotateType_Planar:
						_shapeVertex = mLableShapeVertex::getInstance()->_PlanarRC_Vertex;
						break;
					case MDataLable::FixShapeRotateType_Cylindrical:
						_shapeVertex = mLableShapeVertex::getInstance()->_Cylindrical_Vertex;
						break;
					case MDataLable::FixShapeRotateType_Prismatic:
						_shapeVertex = mLableShapeVertex::getInstance()->_Prismatic_Vertex;
					default:
						break;
					}
					//����ͼ�궥�������ID������ ����ɾ������
					_vertexIndex[ID] = { _rendObject->_vertexNum,pos_dir.size()*(int)_shapeVertex.size() };

					//����������λ��
					int vertexBegin = _rendObject->_vertexNum;

					//��Ӷ�������
					for (int i = 0; i < pos_dir.size(); i++)
					{
						//��Ӽ�ͷ��������
						for (int j = 0; j < _shapeVertex.size(); j++)
						{
							//���ͨ�õ�����
							appendCommonVertex(highlightState, showState, depthState, size, color, _shapeVertex[j]);
							//������е�����
							appendSpecificVertex(pos_dir[i].first, pos_dir[i].second);
						}
						//���¶�������
						_rendObject->_vertexNum += _shapeVertex.size();
					}
				}
				//����buffer
				initialBuffer_append<float, float>(3, 3, _vertex_Pos, _vertex_Dir,vertexBegin);
				//�������
				deleteSpecificVertex();
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{

				int deleteVertexSize = _rendObject->_vertexNum;
				//���¶�������
				_rendObject->_vertexNum = 0;
				//������Ⱦ
				initialBuffer_delete<float, float>(3, 3, 0, deleteVertexSize);
				_vertexIndex.clear();
			}
			else if (mode == Show || mode == Hide || mode == Highlight || mode == NoHighlight)
			{

				int updateVertexSize = _rendObject->_vertexNum;
				if (mode == Show || mode == Highlight)
				{
					vector<GLubyte> tmpVertex(updateVertexSize, 1);
					initialBuffer_update<GLubyte>(mode, 1, tmpVertex, 0, updateVertexSize);
				}
				else if (mode == Hide || mode == NoHighlight)
				{
					vector<GLubyte> tmpVertex(updateVertexSize, 0);
					initialBuffer_update<GLubyte>(mode, 1, tmpVertex, 0, updateVertexSize);
				}

			}
			else if (mode == SetColor)
			{
				int updateVertexSize = _rendObject->_vertexNum;
				vector<GLubyte> tmpVertex;
				for (auto it = _data.begin(); it != _data.end(); ++it)
				{
					auto tmp = it.value();
					QVector3D color = tmp.lableData.getLableColor();
					int ID = tmp.lableData.getLableID();
					int vertexSize = _vertexIndex[ID].second;
					for (int i = 0; i < vertexSize; ++i)
					{
						tmpVertex.push_back(color.x());
						tmpVertex.push_back(color.y());
						tmpVertex.push_back(color.z());
					}
				}
				initialBuffer_update<GLubyte>(mode, 3, tmpVertex, 0, updateVertexSize);
			}
			else if (mode == SetSize)
			{
				int updateVertexSize = _rendObject->_vertexNum;
				vector<float> tmpVertex;
				for (auto it = _data.begin(); it != _data.end(); ++it)
				{
					auto tmp = it.value();
					float size = tmp.lableData.getLableSize();
					int ID = tmp.lableData.getLableID();
					int vertexSize = _vertexIndex[ID].second;
					for (int i = 0; i < vertexSize; ++i)
					{
						tmpVertex.push_back(size);
					}
				}
				initialBuffer_update<float>(mode, 1, tmpVertex, 0, updateVertexSize);
			}
		};

		/*
		* ������еĵĶ�������
		*/
		inline void appendSpecificVertex(const QVector3D pos, const QVector3D dir)
		{
			//ͼ���λ�ã�ģ�������£�
			_vertex_Pos.push_back(pos.x());
			_vertex_Pos.push_back(pos.y());
			_vertex_Pos.push_back(pos.z());
			//��ת�ķ���ģ�������£�
			_vertex_Dir.push_back(dir.x());
			_vertex_Dir.push_back(dir.y());
			_vertex_Dir.push_back(dir.z());
		}
		
		/*
		* ɾ�����еĵĶ�������
		*/
		inline void deleteSpecificVertex()
		{
			vector<float>().swap(_vertex_Pos);
			vector<float>().swap(_vertex_Dir);
		}

	private:
		//��Ⱦ����
		const QHash<QString, T> &_data;
		//��״����
		QVector<QVector3D> _shapeVertex;
		//��������
		vector<float> _vertex_Pos;  //��Ҫ�ƶ�������
		//����
		vector<float> _vertex_Dir;  //��Ҫת���ķ���



	};
}
