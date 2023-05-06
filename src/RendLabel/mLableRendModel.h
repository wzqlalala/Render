#pragma once
//////////////////////////////////////////////////////////////////////
//此类用于渲染顶点数据在模型坐标系下的图标 例如刚性墙 连接等//////////
//////////////////////////////////////////////////////////////////////
#include <QObject>
#include"mLableRendBase.h"
#include"mLableRendEnum.h"
#include"mLableDataStruct.h"
using namespace MDataLable;
namespace MLableRend
{
	//通用类型图标绘制
	template<class T>
	class mLableRendModel : public mLableRendBase<mDrawArray>
	{
	public:

		explicit mLableRendModel(const QHash<QString, T> &data, const GLenum drawMode)
			:mLableRendBase<mDrawArray>(6, drawMode), _data(data) {};

		virtual ~mLableRendModel() override {};

		friend class mLableRendIntegrate_pre;
		/*
		* 单个图标更新
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
			//固定类型图标
			if (mode == Append)
			{
				if (_vertexIndex.contains(ID))
				{
					updateSingleRend(Delete, name);
				}
				//传递图标顶点坐标的ID和索引 便于删除操作
				_vertexIndex[ID] = { _rendObject->_vertexNum,(int)vertex.size() };

				//顶点更新起点位置
				int vertexBegin = _rendObject->_vertexNum;

				//添加顶点数据
				for (int i = 0; i < vertex.size(); i++)
				{				
					//添加通用的数据
					appendCommonVertex(highlightState, showState, depthState, size, color, vertex[i]);				
				}
				//更新顶点数量
				_rendObject->_vertexNum += vertex.size();
				//更新buffer
				initialBuffer_append(vertexBegin);
				//清空数据
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
				//更新顶点数量
				_rendObject->_vertexNum -= deleteVertexSize;

				//更新渲染
				initialBuffer_delete(deleteVertexBegin, deleteVertexSize);

				_vertexIndex.remove(ID);

				//更新索引
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
		* 所有图标更新
		*/
		virtual void updateAllRend(LableOperateMode mode) override
		{
			if (mode == Append)
			{
				//顶点更新起点位置
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
					//传递图标顶点坐标的ID和索引 便于删除操作
					_vertexIndex[ID] = { _rendObject->_vertexNum,(int)vertex.size() };

					//顶点更新起点位置
					int vertexBegin = _rendObject->_vertexNum;

					//添加顶点数据
					for (int i = 0; i < vertex.size(); i++)
					{
						//添加通用的数据
						appendCommonVertex(highlightState, showState, depthState, size, color, vertex[i]);
					}
					//更新顶点数量
					_rendObject->_vertexNum += vertex.size();
				}
				//更新buffer
				initialBuffer_append(vertexBegin);
				//删除通用数据
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{
				int deleteVertexSize = _rendObject->_vertexNum;
				//更新顶点数量
				_rendObject->_vertexNum = 0;
				//更新渲染
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
			//这类图标没有设置大小的功能
			/*else if (mode == SetSize)
			{

			}*/
		};
	private:
		//渲染数据
		const QHash<QString, T> &_data;
	};


}
