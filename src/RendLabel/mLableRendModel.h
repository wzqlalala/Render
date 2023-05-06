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
	//ͨ������ͼ�����
	template<class T>
	class mLableRendModel : public mLableRendBase<mDrawArray>
	{
	public:

		explicit mLableRendModel(const QHash<QString, T> &data, const GLenum drawMode)
			:mLableRendBase<mDrawArray>(6, drawMode), _data(data) {};

		virtual ~mLableRendModel() override {};

		friend class mLableRendIntegrate_pre;
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
			QVector<QVector3D> vertex = tmp.lableData.getParameter();
			//�̶�����ͼ��
			if (mode == Append)
			{
				if (_vertexIndex.contains(ID))
				{
					updateSingleRend(Delete, name);
				}
				//����ͼ�궥�������ID������ ����ɾ������
				_vertexIndex[ID] = { _rendObject->_vertexNum,(int)vertex.size() };

				//����������λ��
				int vertexBegin = _rendObject->_vertexNum;

				//��Ӷ�������
				for (int i = 0; i < vertex.size(); i++)
				{				
					//���ͨ�õ�����
					appendCommonVertex(highlightState, showState, depthState, size, color, vertex[i]);				
				}
				//���¶�������
				_rendObject->_vertexNum += vertex.size();
				//����buffer
				initialBuffer_append(vertexBegin);
				//�������
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
				initialBuffer_delete(deleteVertexBegin, deleteVertexSize);

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
					QVector<QVector3D> vertex = tmp.lableData.getParameter();
					//����ͼ�궥�������ID������ ����ɾ������
					_vertexIndex[ID] = { _rendObject->_vertexNum,(int)vertex.size() };

					//����������λ��
					int vertexBegin = _rendObject->_vertexNum;

					//��Ӷ�������
					for (int i = 0; i < vertex.size(); i++)
					{
						//���ͨ�õ�����
						appendCommonVertex(highlightState, showState, depthState, size, color, vertex[i]);
					}
					//���¶�������
					_rendObject->_vertexNum += vertex.size();
				}
				//����buffer
				initialBuffer_append(vertexBegin);
				//ɾ��ͨ������
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{
				int deleteVertexSize = _rendObject->_vertexNum;
				//���¶�������
				_rendObject->_vertexNum = 0;
				//������Ⱦ
				initialBuffer_delete(0, deleteVertexSize);
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
					QVector<QVector3D> vertex = tmp.lableData.getParameter();
					for (int i = 0; i < vertex.size(); ++i)
					{
						tmpVertex.push_back(color.x());
						tmpVertex.push_back(color.y());
						tmpVertex.push_back(color.z());
					}
				}
				initialBuffer_update<GLubyte>(mode, 3, tmpVertex, 0, updateVertexSize);
			}
			//����ͼ��û�����ô�С�Ĺ���
			/*else if (mode == SetSize)
			{

			}*/
		};
	private:
		//��Ⱦ����
		const QHash<QString, T> &_data;
	};


}
