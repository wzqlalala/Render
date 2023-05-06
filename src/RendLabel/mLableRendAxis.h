//////////////////////////////////////////////////////////////////////
//����������Ⱦ ��ͷ���͵�ͼ��////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#pragma once
#include"mLableRendBase.h"
#include"mLableRendEnum.h"
#include"mLableDataStruct.h"
#include"mLableShapeVertex.h"
using namespace MDataLable;
namespace MLableRend
{
	//��ͷ����ͼ�����
	template<class T>
	class mLableRendAxis : public mLableRendBase<mDrawElement>
	{
	public:

		explicit mLableRendAxis(const QHash<QString, T> &data) :mLableRendBase<mDrawElement>(8, GL_LINES), _data(data) {};
		virtual ~mLableRendAxis()  override {};

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
			QVector3D color_x = tmp.lableData.getLableColor();
			QVector3D color_y = tmp.lableData2.getLableColor();
			QVector3D color_z = tmp.lableData3.getLableColor();

			QVector<POS_DIR> pos_dir_x = tmp.lableData.getParameter();
			QVector<POS_DIR> pos_dir_y = tmp.lableData2.getParameter();
			QVector<POS_DIR> pos_dir_z = tmp.lableData3.getParameter();
			//�غ���ͼ��
			if (mode == Append)
			{
				if (_vertexIndex.contains(ID))
				{
					updateSingleRend(Delete, name);
				}
				//����������λ��
				int vertexBegin = _rendObject->_vertexNum;
				//�����������λ��
				int indexBegin = _rendObject->_indexNum;

				QVector<QVector3D> shapeVertex = mLableShapeVertex::getInstance()->_Arrow_Vertices;

				//��Ӷ�������
				for (int i = 0; i < pos_dir_x.size(); i++)
				{
					//��Ӽ�ͷ��������
					appendEBOIndex(_rendObject->_vertexNum);

					//��Ӽ�ͷ��������
					for (int j = 0; j < shapeVertex.size(); j++)
					{
						//���ͨ�õ�����
						appendCommonVertex(highlightState, showState, depthState, size, color_x, shapeVertex[j]);
						//������е�����
						appendSpecificVertex(pos_dir_x[i].first, pos_dir_x[i].second);
					}
					//���¶�������
					_rendObject->_vertexNum += shapeVertex.size();
				}
				for (int i = 0; i < pos_dir_y.size(); i++)
				{
					//��Ӽ�ͷ��������
					appendEBOIndex(_rendObject->_vertexNum);

					//��Ӽ�ͷ��������
					for (int j = 0; j < shapeVertex.size(); j++)
					{
						//���ͨ�õ�����
						appendCommonVertex(highlightState, showState, depthState, size, color_y, shapeVertex[j]);
						//������е�����
						appendSpecificVertex(pos_dir_y[i].first, pos_dir_y[i].second);
					}
					//���¶�������
					_rendObject->_vertexNum += shapeVertex.size();
				}
				for (int i = 0; i < pos_dir_z.size(); i++)
				{
					//��Ӽ�ͷ��������
					appendEBOIndex(_rendObject->_vertexNum);

					//��Ӽ�ͷ��������
					for (int j = 0; j < shapeVertex.size(); j++)
					{
						//���ͨ�õ�����
						appendCommonVertex(highlightState, showState, depthState, size, color_z, shapeVertex[j]);
						//������е�����
						appendSpecificVertex(pos_dir_z[i].first, pos_dir_z[i].second);
					}
					//���¶�������
					_rendObject->_vertexNum += shapeVertex.size();
				}

				//���¶�����������
				_rendObject->_indexNum = _EBOindex.size();

				//���ݼ�ͷ���������ID������ ����ɾ������
				_vertexIndex[ID] = { vertexBegin,(int)_vertex_HighlightState.size() };
				_elementIndex[ID] = { indexBegin ,(int)_EBOindex.size() - indexBegin };
				//����buffer
				initialBuffer_append_index<float, float>(3, 3, _vertex_Pos, _vertex_Dir, vertexBegin, _EBOindex);

				//�������
				deleteSpecificVertex();
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{
				if (_vertexIndex.find(ID) == _vertexIndex.end() || _elementIndex.find(ID) == _elementIndex.end())
				{
					return;
				}
				int deleteVertexBegin = _vertexIndex[ID].first;
				int deleteVertexSize = _vertexIndex[ID].second;

				int deleteIndexBegin = _elementIndex[ID].first;
				int deleteIndexSize = _elementIndex[ID].second;
				//���¶�������
				_rendObject->_vertexNum -= deleteVertexSize;
				//������������
				_rendObject->_indexNum -= deleteIndexSize;
				//��������_EBOIndex
				vector<GLuint> tmpEBOIndex;
				for (int i = 0; i < _EBOindex.size(); ++i)
				{
					if (i < deleteIndexBegin)
					{
						tmpEBOIndex.push_back(_EBOindex[i]);
					}
					else if (i >= deleteIndexBegin + deleteIndexSize)
					{
						tmpEBOIndex.push_back(_EBOindex[i] - deleteVertexSize);
					}
				}
				swap(tmpEBOIndex, _EBOindex);
				//������Ⱦ
				initialBuffer_delete_index<float, float>(3, 3, deleteVertexBegin, deleteVertexSize, _EBOindex);


				_vertexIndex.remove(ID);
				_elementIndex.remove(ID);

				//��������
				for (auto it = _vertexIndex.begin(); it != _vertexIndex.end(); ++it)
				{
					if (it.value().first > deleteVertexBegin)
					{
						_vertexIndex[it.key()] = { it.value().first - deleteVertexSize, it.value().second };
					}
				}
				for (auto it = _elementIndex.begin(); it != _elementIndex.end(); ++it)
				{
					if (it.value().first > deleteIndexBegin)
					{
						_elementIndex[it.key()] = { it.value().first - deleteIndexSize, it.value().second };
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
					for (int i = 0; i < updateVertexSize/3; ++i)
					{
						tmpVertex.push_back(color_x.x());
						tmpVertex.push_back(color_x.y());
						tmpVertex.push_back(color_x.z());
					}
					for (int i = 0; i < updateVertexSize / 3; ++i)
					{
						tmpVertex.push_back(color_y.x());
						tmpVertex.push_back(color_y.y());
						tmpVertex.push_back(color_y.z());
					}
					for (int i = 0; i < updateVertexSize / 3; ++i)
					{
						tmpVertex.push_back(color_z.x());
						tmpVertex.push_back(color_z.y());
						tmpVertex.push_back(color_z.z());
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
				//�����������λ��
				int indexBegin = _rendObject->_indexNum;
				for (auto it = _data.begin(); it != _data.end(); ++it)
				{
					auto tmp = it.value();
					long long ID = tmp.lableData.getLableID();
					bool showState = tmp.lableData.getLableShowState();
					bool highlightState = tmp.lableData.getLableHighlightState();
					bool depthState = tmp.lableData.getLableDepthState();
					float size = tmp.lableData.getLableSize();
					QVector3D color_x = tmp.lableData.getLableColor();
					QVector3D color_y = tmp.lableData2.getLableColor();
					QVector3D color_z = tmp.lableData3.getLableColor();

					QVector<POS_DIR> pos_dir_x = tmp.lableData.getParameter();
					QVector<POS_DIR> pos_dir_y = tmp.lableData2.getParameter();
					QVector<POS_DIR> pos_dir_z = tmp.lableData3.getParameter();

					QVector<QVector3D> shapeVertex = mLableShapeVertex::getInstance()->_Arrow_Vertices;

					int tmpVertexBegin = _rendObject->_vertexNum;
					int tmpElementBegin = _rendObject->_indexNum;

					//��Ӷ�������
					for (int i = 0; i < pos_dir_x.size(); i++)
					{
						//��Ӽ�ͷ��������
						appendEBOIndex(_rendObject->_vertexNum);

						//��Ӽ�ͷ��������
						for (int j = 0; j < shapeVertex.size(); j++)
						{
							//���ͨ�õ�����
							appendCommonVertex(highlightState, showState, depthState, size, color_x, shapeVertex[j]);
							//������е�����
							appendSpecificVertex(pos_dir_x[i].first, pos_dir_x[i].second);
						}
						//���¶�������
						_rendObject->_vertexNum += shapeVertex.size();
					}
					for (int i = 0; i < pos_dir_y.size(); i++)
					{
						//��Ӽ�ͷ��������
						appendEBOIndex(_rendObject->_vertexNum);

						//��Ӽ�ͷ��������
						for (int j = 0; j < shapeVertex.size(); j++)
						{
							//���ͨ�õ�����
							appendCommonVertex(highlightState, showState, depthState, size, color_y, shapeVertex[j]);
							//������е�����
							appendSpecificVertex(pos_dir_y[i].first, pos_dir_y[i].second);
						}
						//���¶�������
						_rendObject->_vertexNum += shapeVertex.size();
					} 
					for (int i = 0; i < pos_dir_z.size(); i++)
					{
						//��Ӽ�ͷ��������
						appendEBOIndex(_rendObject->_vertexNum);

						//��Ӽ�ͷ��������
						for (int j = 0; j < shapeVertex.size(); j++)
						{
							//���ͨ�õ�����
							appendCommonVertex(highlightState, showState, depthState, size, color_z, shapeVertex[j]);
							//������е�����
							appendSpecificVertex(pos_dir_z[i].first, pos_dir_z[i].second);
						}
						//���¶�������
						_rendObject->_vertexNum += shapeVertex.size();
					}
					//���¶�����������
					_rendObject->_indexNum = (int)_EBOindex.size();

					int tmpVertexSize = _rendObject->_vertexNum - tmpVertexBegin;
					int tmpElementSize = _rendObject->_indexNum - tmpElementBegin;
					//���ݼ�ͷ���������ID������ ����ɾ������
					_vertexIndex[ID] = { tmpVertexBegin,tmpVertexSize };
					_elementIndex[ID] = { tmpElementBegin ,tmpElementSize };
				}

				//����buffer
				initialBuffer_append_index<float, float>(3, 3, _vertex_Pos, _vertex_Dir, vertexBegin, _EBOindex);

				//�������
				deleteSpecificVertex();
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{
				int deleteVertexSize = _rendObject->_vertexNum;
				vector<GLuint>().swap(_EBOindex);

				//���¶�������
				_rendObject->_vertexNum = 0;
				//������������
				_rendObject->_indexNum = 0;
				//������Ⱦ
				initialBuffer_delete_index<float, float>(3, 3, 0, deleteVertexSize, _EBOindex);
				_vertexIndex.clear();
				_elementIndex.clear();
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
					int ID = tmp.lableData.getLableID();
					QVector3D color_x = tmp.lableData.getLableColor();
					QVector3D color_y = tmp.lableData2.getLableColor();
					QVector3D color_z = tmp.lableData3.getLableColor();
					int vertexSize = _vertexIndex[ID].second;
					for (int i = 0; i < vertexSize/3; ++i)
					{
						tmpVertex.push_back(color_x.x());
						tmpVertex.push_back(color_x.y());
						tmpVertex.push_back(color_x.z());
					}
					for (int i = 0; i < vertexSize / 3; ++i)
					{
						tmpVertex.push_back(color_y.x());
						tmpVertex.push_back(color_y.y());
						tmpVertex.push_back(color_y.z());
					}
					for (int i = 0; i < vertexSize / 3; ++i)
					{
						tmpVertex.push_back(color_z.x());
						tmpVertex.push_back(color_z.y());
						tmpVertex.push_back(color_z.z());
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
					int ID = tmp.lableData.getLableID();
					float size = tmp.lableData.getLableSize();
					int vertexSize = _vertexIndex[ID].second;
					for (int i = 0; i < vertexSize; ++i)
					{
						tmpVertex.push_back(size);
					}
				}
				initialBuffer_update<float>(mode, 1, tmpVertex, 0, updateVertexSize);
			}
		}

		/*
		* ������еĵĶ�������
		*/
		inline void appendSpecificVertex(const QVector3D pos, const QVector3D dir)
		{
			//ͼ���λ�ã�ģ�������£�
			_vertex_Pos.push_back(pos.x());
			_vertex_Pos.push_back(pos.y());
			_vertex_Pos.push_back(pos.z());
			//�غ�ֵ����ͷ����
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
		/*
		* ��Ӽ�ͷEBO����
		*/
		inline void appendEBOIndex(int begin)
		{
			_EBOindex.push_back(begin);//0
			_EBOindex.push_back(begin + 1);//1		

			_EBOindex.push_back(begin + 1);//1
			_EBOindex.push_back(begin + 2);//2

			_EBOindex.push_back(begin + 1);//1
			_EBOindex.push_back(begin + 3);//3

			_EBOindex.push_back(begin + 1);//1
			_EBOindex.push_back(begin + 4);//4

			_EBOindex.push_back(begin + 1);//1
			_EBOindex.push_back(begin + 5);//5

			_EBOindex.push_back(begin + 2);//2
			_EBOindex.push_back(begin + 3);//3

			_EBOindex.push_back(begin + 3);//3
			_EBOindex.push_back(begin + 4);//4

			_EBOindex.push_back(begin + 4);//4
			_EBOindex.push_back(begin + 5);//5

			_EBOindex.push_back(begin + 5);//5
			_EBOindex.push_back(begin + 2);//2		
		}
	private:
		//��Ⱦ����
		const QHash<QString, T> &_data;
		//��������
		vector<float> _vertex_Pos;  //��Ҫ�ƶ�������
		vector<float> _vertex_Dir;	//��ͷ����
		//EBO��������
		vector<GLuint> _EBOindex;//һֱ���� ������� ����ɾ������
		QHash<int, QPair<int, int>> _elementIndex;	//QHash<ID,��������ʼλ�ã������ĸ���>

	};
}
