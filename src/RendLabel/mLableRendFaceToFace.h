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
	//面面绘制  网格内部偏移 绘制在模型坐标系下
	template<class T>
	class mLableRendFaceToFace : public mLableRendBase<mDrawArray>
	{
	public:
		//模型坐标系下网格偏移绘制
		explicit mLableRendFaceToFace(const QHash<QString, T> &data, const GLenum drawMode) :mLableRendBase<mDrawArray>(6, drawMode), _data(data) 
		{
			_type = 0;
		};
		//通用坐标系下固定形状图标绘制
		explicit mLableRendFaceToFace(const QHash<QString, T> &data, const QVector<QVector3D> &shapeVertex, const GLenum drawMode)
			:mLableRendBase<mDrawArray>(7, drawMode), _shapeVertex(shapeVertex), _data(data) 
		{
			_type = 1;
		};
		virtual ~mLableRendFaceToFace() override {};
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
			//前面几种参数 主从面都相同  获取一种即可
			QVector3D color1 = tmp.lableData.getLableColor();  //主面颜色
			QVector3D color2 = tmp.lableData2.getLableColor();	//从面颜色
			QVector<QVector3D> para1 = tmp.lableData.getParameter(); //主面顶点 或者  需要移动的坐标
			QVector<QVector3D> para2 = tmp.lableData2.getParameter();	//从面顶点 或者 需要移动的坐标
			//固定类型图标
			if (mode == Append)
			{
				if (_vertexIndex.contains(ID))
				{
					updateSingleRend(Delete, name);
				}
				if (_type == 0)
				{
					//传递图标顶点坐标的ID和索引 便于删除操作
					_vertexIndex[ID] = { _rendObject->_vertexNum,(int)para1.size() + (int)para2.size() };

					//顶点更新起点位置
					int vertexBegin = _rendObject->_vertexNum;

					//添加顶点数据
					for (int i = 0; i < para1.size(); i++)
					{
						//添加通用的数据
						appendCommonVertex(highlightState, showState, depthState, size, color1, para1[i]);
					}
					for (int i = 0; i < para2.size(); i++)
					{
						//添加通用的数据
						appendCommonVertex(highlightState, showState, depthState, size, color2, para2[i]);
					}

					//更新顶点数量
					_rendObject->_vertexNum += (para1.size() + para2.size());
					//更新buffer
					initialBuffer_append(vertexBegin);
				}
				else
				{
					//传递图标顶点坐标的ID和索引 便于删除操作
					_vertexIndex[ID] = { _rendObject->_vertexNum,((int)para1.size() + (int)para2.size())*(int)_shapeVertex.size() };

					//顶点更新起点位置
					int vertexBegin = _rendObject->_vertexNum;

					//添加主面顶点数据
					for (int i = 0; i < para1.size(); i++)
					{
						//添加箭头顶点数据
						for (int j = 0; j < _shapeVertex.size(); j++)
						{
							//添加通用的数据
							appendCommonVertex(highlightState, showState, depthState, size, color1, _shapeVertex[j]);
							//添加特有的数据
							appendSpecificVertex(para1[i]);
						}
						//更新顶点数量
						_rendObject->_vertexNum += _shapeVertex.size();
					}
					//添加从面顶点数据
					for (int i = 0; i < para2.size(); i++)
					{
						//添加箭头顶点数据
						for (int j = 0; j < _shapeVertex.size(); j++)
						{
							//添加通用的数据
							appendCommonVertex(highlightState, showState, depthState, size, color2, _shapeVertex[j]);
							//添加特有的数据
							appendSpecificVertex(para2[i]);
						}
						//更新顶点数量
						_rendObject->_vertexNum += _shapeVertex.size();
					}
					//更新buffer
					initialBuffer_append<float>(3, _vertex_Pos, vertexBegin);
				}
				//清空数据
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
				//更新顶点数量
				_rendObject->_vertexNum -= deleteVertexSize;

				//更新渲染
				if (_type == 0)
				{
					initialBuffer_delete(deleteVertexBegin, deleteVertexSize);
				}
				else
				{
					initialBuffer_delete<float>(3, deleteVertexBegin, deleteVertexSize);
				}

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
					//主面
					for (int i = 0; i < para1.size(); ++i)
					{
						for (int j = 0; j < _shapeVertex.size(); j++)
						{
							tmpVertex.push_back(color1.x());
							tmpVertex.push_back(color1.y());
							tmpVertex.push_back(color1.z());
						}
					
					}
					//从面
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
					//前面几种参数 主从面都相同  获取一种即可
					QVector3D color1 = tmp.lableData.getLableColor();  //主面颜色
					QVector3D color2 = tmp.lableData2.getLableColor();	//从面颜色
					QVector<QVector3D> para1 = tmp.lableData.getParameter(); //主面顶点
					QVector<QVector3D> para2 = tmp.lableData2.getParameter();	//从面顶点
					if (_type == 0)
					{
						//传递图标顶点坐标的ID和索引 便于删除操作
						_vertexIndex[ID] = { _rendObject->_vertexNum,(int)para1.size() + (int)para2.size() };

						//顶点更新起点位置
						int vertexBegin = _rendObject->_vertexNum;

						//添加顶点数据
						for (int i = 0; i < para1.size(); i++)
						{
							//添加通用的数据
							appendCommonVertex(highlightState, showState, depthState, size, color1, para1[i]);
						}
						for (int i = 0; i < para2.size(); i++)
						{
							//添加通用的数据
							appendCommonVertex(highlightState, showState, depthState, size, color2, para2[i]);
						}

						//更新顶点数量
						_rendObject->_vertexNum += para1.size() + para2.size();
					}
					else
					{
						//传递图标顶点坐标的ID和索引 便于删除操作
						_vertexIndex[ID] = { _rendObject->_vertexNum,((int)para1.size() + (int)para2.size())*(int)_shapeVertex.size() };

						//顶点更新起点位置
						int vertexBegin = _rendObject->_vertexNum;

						//添加主面顶点数据
						for (int i = 0; i < para1.size(); i++)
						{
							//添加箭头顶点数据
							for (int j = 0; j < _shapeVertex.size(); j++)
							{
								//添加通用的数据
								appendCommonVertex(highlightState, showState, depthState, size, color1, _shapeVertex[j]);
								//添加特有的数据
								appendSpecificVertex(para1[i]);
							}
							//更新顶点数量
							_rendObject->_vertexNum += _shapeVertex.size();
						}
						//添加从面顶点数据
						for (int i = 0; i < para2.size(); i++)
						{
							//添加箭头顶点数据
							for (int j = 0; j < _shapeVertex.size(); j++)
							{
								//添加通用的数据
								appendCommonVertex(highlightState, showState, depthState, size, color2, _shapeVertex[j]);
								//添加特有的数据
								appendSpecificVertex(para2[i]);
							}
							//更新顶点数量
							_rendObject->_vertexNum += _shapeVertex.size();
						}
					}
				}
				//更新buffer
				if (_type == 0)
				{
					initialBuffer_append(vertexBegin);
				}
				else
				{
					initialBuffer_append<float>(3, _vertex_Pos, vertexBegin);
				}
				//删除通用数据
				deleteSpecificVertex();
				deleteCommonVerrtex();
			}
			else if (mode == Delete)
			{

				int deleteVertexSize = _rendObject->_vertexNum;
				//更新顶点数量
				_rendObject->_vertexNum = 0;
				//更新渲染
			
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
					QVector<QVector3D> para1 = tmp.lableData.getParameter(); //主面顶点
					QVector<QVector3D> para2 = tmp.lableData2.getParameter();	//从面顶点
					//主面
					for (int i = 0; i < para1.size(); ++i)
					{
						for (int j = 0; j < _shapeVertex.size(); j++)
						{
							tmpVertex.push_back(color1.x());
							tmpVertex.push_back(color1.y());
							tmpVertex.push_back(color1.z());
						}

					}
					//从面
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
		* 添加特有的的顶点数据
		*/
		inline void appendSpecificVertex(const QVector3D pos)
		{
			//图标的位置（模型坐标下）
			_vertex_Pos.push_back(pos.x());
			_vertex_Pos.push_back(pos.y());
			_vertex_Pos.push_back(pos.z());
		}

		/*
		* 删除特有的的顶点数据
		*/
		inline void deleteSpecificVertex()
		{
			vector<float>().swap(_vertex_Pos);

		}
	private:
		//渲染数据
		const QHash<QString, T> &_data;
		//形状数据（通用坐标系下绘制才存在）
		QVector<QVector3D> _shapeVertex;
		//需要移动的坐标（通用坐标系下绘制才存在）
		vector<float> _vertex_Pos;  
		//0代表网格偏移  1代表固定图标绘制
		int _type = 0;
	};


}
