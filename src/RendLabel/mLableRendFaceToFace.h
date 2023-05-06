#pragma once
//////////////////////////////////////////////////////////////////////
//����������Ⱦ����������ģ������ϵ�µ�ͼ�� �������ǽ ���ӵ�//////////
//////////////////////////////////////////////////////////////////////
#include <QObject>
#include"mLableRendBase.h"
#include"mLableRendEnum.h"
#include"mLableDataStruct.h"
using namespace MDataLable;
namespace MLableRend
{
	//�������  �����ڲ�ƫ�� ������ģ������ϵ��
	template<class T>
	class mLableRendFaceToFace : public mLableRendBase<mDrawArray>
	{
	public:
		//ģ������ϵ������ƫ�ƻ���
		explicit mLableRendFaceToFace(const QHash<QString, T> &data, const GLenum drawMode) :mLableRendBase<mDrawArray>(6, drawMode), _data(data) 
		{
			_type = 0;
		};
		//ͨ������ϵ�¹̶���״ͼ�����
		explicit mLableRendFaceToFace(const QHash<QString, T> &data, const QVector<QVector3D> &shapeVertex, const GLenum drawMode)
			:mLableRendBase<mDrawArray>(7, drawMode), _shapeVertex(shapeVertex), _data(data) 
		{
			_type = 1;
		};
		virtual ~mLableRendFaceToFace() override {};
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
			//ǰ�漸�ֲ��� �����涼��ͬ  ��ȡһ�ּ���
			QVector3D color1 = tmp.lableData.getLableColor();  //������ɫ
			QVector3D color2 = tmp.lableData2.getLableColor();	//������ɫ
			QVector<QVector3D> para1 = tmp.lableData.getParameter(); //���涥�� ����  ��Ҫ�ƶ�������
			QVector<QVector3D> para2 = tmp.lableData2.getParameter();	//���涥�� ���� ��Ҫ�ƶ�������
			//�̶�����ͼ��
			if (mode == Append)
			{
				if (_vertexIndex.contains(ID))
				{
					updateSingleRend(Delete, name);
				}
				if (_type == 0)
				{
					//����ͼ�궥�������ID������ ����ɾ������
					_vertexIndex[ID] = { _rendObject->_vertexNum,(int)para1.size() + (int)para2.size() };

					//����������λ��
					int vertexBegin = _rendObject->_vertexNum;

					//��Ӷ�������
					for (int i = 0; i < para1.size(); i++)
					{
						//���ͨ�õ�����
						appendCommonVertex(highlightState, showState, depthState, size, color1, para1[i]);
					}
					for (int i = 0; i < para2.size(); i++)
					{
						//���ͨ�õ�����
						appendCommonVertex(highlightState, showState, depthState, size, color2, para2[i]);
					}

					//���¶�������
					_rendObject->_vertexNum += (para1.size() + para2.size());
					//����buffer
					initialBuffer_append(vertexBegin);
				}
				else
				{
					//����ͼ�궥�������ID������ ����ɾ������
					_vertexIndex[ID] = { _rendObject->_vertexNum,((int)para1.size() + (int)para2.size())*(int)_shapeVertex.size() };

					//����������λ��
					int vertexBegin = _rendObject->_vertexNum;

					//������涥������
					for (int i = 0; i < para1.size(); i++)
					{
						//��Ӽ�ͷ��������
						for (int j = 0; j < _shapeVertex.size(); j++)
						{
							//���ͨ�õ�����
							appendCommonVertex(highlightState, showState, depthState, size, color1, _shapeVertex[j]);
							//������е�����
							appendSpecificVertex(para1[i]);
						}
						//���¶�������
						_rendObject->_vertexNum += _shapeVertex.size();
					}
					//��Ӵ��涥������
					for (int i = 0; i < para2.size(); i++)
					{
						//��Ӽ�ͷ��������
						for (int j = 0; j < _shapeVertex.size(); j++)
						{
							//���ͨ�õ�����
							appendCommonVertex(highlightState, showState, depthState, size, color2, _shapeVertex[j]);
							//������е�����
							appendSpecificVertex(para2[i]);
						}
						//���¶�������
						_rendObject->_vertexNum += _shapeVertex.size();
					}
					//����buffer
					initialBuffer_append<float>(3, _vertex_Pos, vertexBegin);
				}
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
				if (_type == 0)
				{
					initialBuffer_delete(deleteVertexBegin, deleteVertexSize);
				}
				else
				{
					initialBuffer_delete<float>(3, deleteVertexBegin, deleteVertexSize);
				}

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
					//����
					for (int i = 0; i < para1.size(); ++i)
					{
						for (int j = 0; j < _shapeVertex.size(); j++)
						{
							tmpVertex.push_back(color1.x());
							tmpVertex.push_back(color1.y());
							tmpVertex.push_back(color1.z());
						}
					
					}
					//����
					for (int i = 0; i < para2.size(); ++i)
					{
						for (int j = 0; j < _shapeVertex.size(); j++)
						{
							tmpVertex.push_back(color2.x());
							tmpVertex.push_back(color2.y());
							tmpVertex.push_back(color2.z());
						}
					}
					initialBuffer_update<GLubyte>(mode, 3, tmpVertex, updateVertexBegin, updateVertexSize);
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
					//ǰ�漸�ֲ��� �����涼��ͬ  ��ȡһ�ּ���
					QVector3D color1 = tmp.lableData.getLableColor();  //������ɫ
					QVector3D color2 = tmp.lableData2.getLableColor();	//������ɫ
					QVector<QVector3D> para1 = tmp.lableData.getParameter(); //���涥��
					QVector<QVector3D> para2 = tmp.lableData2.getParameter();	//���涥��
					if (_type == 0)
					{
						//����ͼ�궥�������ID������ ����ɾ������
						_vertexIndex[ID] = { _rendObject->_vertexNum,(int)para1.size() + (int)para2.size() };

						//����������λ��
						int vertexBegin = _rendObject->_vertexNum;

						//��Ӷ�������
						for (int i = 0; i < para1.size(); i++)
						{
							//���ͨ�õ�����
							appendCommonVertex(highlightState, showState, depthState, size, color1, para1[i]);
						}
						for (int i = 0; i < para2.size(); i++)
						{
							//���ͨ�õ�����
							appendCommonVertex(highlightState, showState, depthState, size, color2, para2[i]);
						}

						//���¶�������
						_rendObject->_vertexNum += para1.size() + para2.size();
					}
					else
					{
						//����ͼ�궥�������ID������ ����ɾ������
						_vertexIndex[ID] = { _rendObject->_vertexNum,((int)para1.size() + (int)para2.size())*(int)_shapeVertex.size() };

						//����������λ��
						int vertexBegin = _rendObject->_vertexNum;

						//������涥������
						for (int i = 0; i < para1.size(); i++)
						{
							//��Ӽ�ͷ��������
							for (int j = 0; j < _shapeVertex.size(); j++)
							{
								//���ͨ�õ�����
								appendCommonVertex(highlightState, showState, depthState, size, color1, _shapeVertex[j]);
								//������е�����
								appendSpecificVertex(para1[i]);
							}
							//���¶�������
							_rendObject->_vertexNum += _shapeVertex.size();
						}
						//��Ӵ��涥������
						for (int i = 0; i < para2.size(); i++)
						{
							//��Ӽ�ͷ��������
							for (int j = 0; j < _shapeVertex.size(); j++)
							{
								//���ͨ�õ�����
								appendCommonVertex(highlightState, showState, depthState, size, color2, _shapeVertex[j]);
								//������е�����
								appendSpecificVertex(para2[i]);
							}
							//���¶�������
							_rendObject->_vertexNum += _shapeVertex.size();
						}
					}
				}
				//����buffer
				if (_type == 0)
				{
					initialBuffer_append(vertexBegin);
				}
				else
				{
					initialBuffer_append<float>(3, _vertex_Pos, vertexBegin);
				}
				//ɾ��ͨ������
				deleteSpecificVertex();
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{

				int deleteVertexSize = _rendObject->_vertexNum;
				//���¶�������
				_rendObject->_vertexNum = 0;
				//������Ⱦ
			
				if (_type == 0)
				{
					initialBuffer_delete(0, deleteVertexSize);
				}
				else
				{
					initialBuffer_delete<float>(3, 0, deleteVertexSize);
				}
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
					QVector3D color1 = tmp.lableData.getLableColor();
					QVector3D color2 = tmp.lableData2.getLableColor();
					QVector<QVector3D> para1 = tmp.lableData.getParameter(); //���涥��
					QVector<QVector3D> para2 = tmp.lableData2.getParameter();	//���涥��
					//����
					for (int i = 0; i < para1.size(); ++i)
					{
						for (int j = 0; j < _shapeVertex.size(); j++)
						{
							tmpVertex.push_back(color1.x());
							tmpVertex.push_back(color1.y());
							tmpVertex.push_back(color1.z());
						}

					}
					//����
					for (int i = 0; i < para2.size(); ++i)
					{
						for (int j = 0; j < _shapeVertex.size(); j++)
						{
							tmpVertex.push_back(color2.x());
							tmpVertex.push_back(color2.y());
							tmpVertex.push_back(color2.z());
						}
					}
				}
				initialBuffer_update<GLubyte>(mode, 3, tmpVertex, 0, updateVertexSize);
			}

		};

		/*
		* ������еĵĶ�������
		*/
		inline void appendSpecificVertex(const QVector3D pos)
		{
			//ͼ���λ�ã�ģ�������£�
			_vertex_Pos.push_back(pos.x());
			_vertex_Pos.push_back(pos.y());
			_vertex_Pos.push_back(pos.z());
		}

		/*
		* ɾ�����еĵĶ�������
		*/
		inline void deleteSpecificVertex()
		{
			vector<float>().swap(_vertex_Pos);

		}
	private:
		//��Ⱦ����
		const QHash<QString, T> &_data;
		//��״���ݣ�ͨ������ϵ�»��ƲŴ��ڣ�
		QVector<QVector3D> _shapeVertex;
		//��Ҫ�ƶ������꣨ͨ������ϵ�»��ƲŴ��ڣ�
		vector<float> _vertex_Pos;  
		//0��������ƫ��  1����̶�ͼ�����
		int _type = 0;
	};


}
