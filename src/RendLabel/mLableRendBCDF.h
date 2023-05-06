//////////////////////////////////////////////////////////////////////
//此类用于渲染 边界条件中约束自由度的图标（箭头组合）/////////////////
//////////////////////////////////////////////////////////////////////
#pragma once
#include"mLableRendBase.h"
#include"mLableRendEnum.h"
#include"mLableDataStruct.h"
#include"mLableShapeVertex.h"
using namespace MDataLable;
namespace MLableRend
{
	//箭头组合类型图标绘制（自由度约束）
	template<class T>
	class mLableRendBCDF : public mLableRendBase<mDrawElement>
	{
	public:
		explicit mLableRendBCDF(const QHash<QString, T> &data) :mLableRendBase<mDrawElement>(8, GL_LINES), _data(data) {};
		virtual ~mLableRendBCDF() override {};
		//位移 速度 加速度边界条件图标的形状类型
		enum ShapeType
		{
			Cone_0,	    //固定圆锥
			Cylinder_0, //固定圆柱
			Cone_Line,  //非固定圆锥加线（箭头）
			Cylinder,   //非固定圆柱

		};
		/*
		* 单个图标更新
		*/
		virtual void updateSingleRend(LableOperateMode mode, const QString &name) override
		{
			auto tmp = _data.value(name);
			int ID = tmp.lableData.getLableID();
			bool showState = tmp.lableData.getLableShowState();
			bool highlightState = tmp.lableData.getLableHighlightState();
			bool depthState = tmp.lableData.getLableDepthState();
			float size = tmp.lableData.getLableSize();
			QVector3D color_light = tmp.lableData.getLableColor(); //浅色
			QVector3D color_deep = color_light / 2;				//深色
			QVector<QVector3D> pos = tmp.lableData.getParameter1();
			QMap<unsigned int, float> value = tmp.lableData.getParameter2();
			if (mode == Append)
			{
				if (_vertexIndex.contains(ID))
				{
					updateSingleRend(Delete, name);
				}
				//顶点更新起点位置
				int vertexBegin = _rendObject->_vertexNum;
				//索引更新起点位置
				int indexBegin = _rendObject->_indexNum;

				//获取形状的顶点数据
				const QVector<QVector3D> &Cone_Vertices_0 = mLableShapeVertex::getInstance()->_Cone_Vertices_0;
				const QVector<QVector3D> &Cylinder_Vertices_0 = mLableShapeVertex::getInstance()->_Cylinder_Vertices_0;
				const QVector<QVector3D> &Cone_Line_Vertices = mLableShapeVertex::getInstance()->_Cone_Line_Vertices;
				const QVector<QVector3D> &Cylinder_Vertices = mLableShapeVertex::getInstance()->_Cylinder_Vertices;

				//添加顶点数据以及索引
				for (int i = 0; i < pos.size(); i++)
				{
					for (auto it = value.begin(); it != value.end(); it++)
					{
						if (it.key() == 1)
						{
							if (it.value() == 0)
							{
								//判断1=0时 是否4也等于0
								if (value.count(4) != 0 && value.value(4) == 0)
								{

									//圆锥_0	
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转180）（浅色）
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));


									//圆柱_0
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cylinder_0, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转180）（深色）
									appendVertex(Cylinder_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));


								}
								else
								{
									//圆锥_0
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转180）（浅色）
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));

								}

							}
							else if (it.value() < 0)
							{
								//判断1<0时 是否4也<0
								if (value.count(4) != 0 && value.value(4) < 0)
								{
									//圆锥+直线(箭头)	
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转180）（浅色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));


									//圆柱		
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cylinder, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转180）（深色）
									appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));

								}
								else
								{
									//圆锥+直线(箭头)	
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转180）（浅色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));
								}
							}
							else
							{
								//判断1>0时 是否4也>0
								if (value.count(4) != 0 && value.value(4) > 0)
								{
									//圆锥+直线(箭头)	
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （不旋转）（浅色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 0, 0));


									//圆柱		
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cylinder, _rendObject->_vertexNum);
									//添加顶点数据 （不旋转）（深色）
									appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 0, 0));

								}
								else
								{
									//圆锥+直线(箭头)	
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （不旋转）（浅色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 0, 0));

								}

							}

						}
						else if (it.key() == 2)
						{
							if (it.value() == 0)
							{
								//判断2=0时 是否5也等于0
								if (value.count(5) != 0 && value.value(5) == 0)
								{
									//圆锥_0
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转-90°）（浅色）
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));


									//圆柱_0
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cylinder_0, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转180）（深色）
									appendVertex(Cylinder_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));

								}
								else
								{

									//圆锥_0
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转-90°）（浅色）
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));

								}

							}
							else if (it.value() < 0)
							{
								//判断2<0时 是否5也<0
								if (value.count(5) != 0 && value.value(5) < 0)
								{
									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转-90°）（浅色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));


									//圆柱
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cylinder, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转-90°）（深色）
									appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));


								}
								else
								{
									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转-90°）（浅色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));

								}

							}
							else
							{
								//判断2>0时 是否5也>0
								if (value.count(5) != 0 && value.value(5) > 0)
								{
									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转90°）（浅色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 90));


									//圆柱
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cylinder, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转90°）（深色）
									appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 90));

								}
								else
								{
									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转90°）（浅色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 90));
								}
							}

						}
						else if (it.key() == 3)
						{
							if (it.value() == 0)
							{
								//判断3=0时 是否6也等于0
								if (value.count(6) != 0 && value.value(6) == 0)
								{
									//圆锥_0
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Y轴旋转90）（浅色）
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));


									//圆柱_0
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cylinder_0, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Y轴旋转90）（深色）
									appendVertex(Cylinder_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));
								}
								else
								{
									//圆锥_0
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Y轴旋转90）（浅色）
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));

								}

							}
							else if (it.value() < 0)
							{
								//判断3<0时 是否6也<0
								if (value.count(6) != 0 && value.value(6) < 0)
								{

									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Y轴旋转90°）（浅色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));


									//圆柱
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cylinder, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Y轴旋转90°）（深色）
									appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));


								}
								else
								{
									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Y轴旋转90°）（浅色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));

								}

							}
							else
							{
								//判断3>0时 是否6也>0
								if (value.count(6) != 0 && value.value(6) > 0)
								{

									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Y轴旋转-90°）（浅色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, -90));


									//圆柱
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cylinder, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Y轴旋转-90°）（深色）
									appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, -90));



								}
								else
								{
									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Y轴旋转-90°）（浅色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, -90));
								}
							}
						}
						else if (it.key() == 4)
						{
							if (it.value() == 0 && (value.count(1) == 0 || value.value(1) != 0)) //判断前面是否已经画过1,4都存在的情况
							{

								//圆锥_0
								//添加 圆锥_0 EBO索引
								appendEBOIndex(Cone_0, _rendObject->_vertexNum);
								//添加顶点数据 （绕着Z轴旋转180°）（深色）
								appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));


							}
							else if (it.value() < 0 && (value.count(1) == 0 || value.value(1) >= 0))
							{

								//圆锥+直线(箭头)
								//添加 圆锥_0 EBO索引
								appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
								//添加顶点数据 （绕着Z轴旋转180）（深色）
								appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));


							}
							else if (it.value() > 0 && (value.count(1) == 0 || value.value(1) <= 0))
							{
								//圆锥+直线(箭头)
								//添加 圆锥_0 EBO索引
								appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
								//添加顶点数据 （不旋转）（深色）
								appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 0, 0));

							}
						}
						else if (it.key() == 5)
						{
							if (it.value() == 0 && (value.count(2) == 0 || value.value(2) != 0))
							{
								//圆锥_0
								//添加 圆锥_0 EBO索引
								appendEBOIndex(Cone_0, _rendObject->_vertexNum);
								//添加顶点数据 （绕着Z轴旋转 - 90）（深色）
								appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));


							}
							else if (it.value() < 0 && (value.count(2) == 0 || value.value(2) >= 0))
							{
								//圆锥+直线(箭头)
								//添加 圆锥_0 EBO索引
								appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
								//添加顶点数据 （绕着Z轴旋转 - 90）（深色）
								appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));


							}
							else if (it.value() > 0 && (value.count(2) == 0 || value.value(2) <= 0))
							{
								//圆锥+直线(箭头)
								//添加 圆锥_0 EBO索引
								appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
								//添加顶点数据 （绕着Z轴旋转 90）（深色）
								appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 90));

							}
						}
						else if (it.key() == 6)
						{
							if (it.value() == 0 && (value.count(3) == 0 || value.value(3) != 0))
							{
								//圆锥_0
								//添加 圆锥_0 EBO索引
								appendEBOIndex(Cone_0, _rendObject->_vertexNum);
								//添加顶点数据 （绕着Y轴旋转90°）（深色）
								appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));


							}
							else if (it.value() < 0 && (value.count(3) == 0 || value.value(3) >= 0))
							{
								//圆锥+直线(箭头)
								//添加 圆锥_0 EBO索引
								appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
								//添加顶点数据 （绕着Y轴旋转90°）（深色）
								appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));


							}
							else if (it.value() > 0 && (value.count(3) == 0 || value.value(3) <= 0))
							{
								//圆锥+直线(箭头)
								//添加 圆锥_0 EBO索引
								appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
								//添加顶点数据 （绕着Y轴旋转-90°）（深色）
								appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, -90));

							}
						}
					}
				}

				//更新顶点索引数量
				_rendObject->_indexNum = _EBOindex.size();
				//传递箭头顶点坐标的ID和索引 便于删除操作
				_vertexIndex[ID] = { vertexBegin,(int)_vertex_HighlightState.size() };
				_elementIndex[ID] = { indexBegin ,(int)_EBOindex.size()- indexBegin };

				//更新buffer
				initialBuffer_append_index<float, GLshort>(3, 4, _vertex_Pos, _vertex_Rotate, vertexBegin, _EBOindex);
				//清空数据
				deleteVertex();
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
				//更新顶点数量
				_rendObject->_vertexNum -= deleteVertexSize;
				//更新索引数量
				_rendObject->_indexNum -= deleteIndexSize;

				//更新索引_EBOIndex
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
				swap(tmpEBOIndex,_EBOindex);

				//更新渲染
				initialBuffer_delete_index<float, GLshort>(3, 4, deleteVertexBegin, deleteVertexSize,_EBOindex);

				_vertexIndex.remove(ID);
				_elementIndex.remove(ID);
				//更新索引
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
				if (_vertexIndex.find(ID) == _vertexIndex.end() || _elementIndex.find(ID) == _elementIndex.end())
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
						tmpVertex.push_back(color_light.x());
						tmpVertex.push_back(color_light.y());
						tmpVertex.push_back(color_light.z());
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
		}
		/*
		* 所有图标更新
		*/
		virtual void updateAllRend(LableOperateMode mode) override
		{
			if (mode == Append)
			{
				//顶点更新起点位置
				int vertexBegin = _rendObject->_vertexNum;
				//索引更新起点位置
				int indexBegin = _rendObject->_indexNum;
				for (auto it = _data.begin(); it != _data.end(); ++it)
				{
					auto tmp = it.value();
					int ID = tmp.lableData.getLableID();
					bool showState = tmp.lableData.getLableShowState();
					bool highlightState = tmp.lableData.getLableHighlightState();
					bool depthState = tmp.lableData.getLableDepthState();
					float size = tmp.lableData.getLableSize();
					QVector3D color_light = tmp.lableData.getLableColor(); //浅色
					QVector3D color_deep = color_light / 2;				//深色
					QVector<QVector3D> pos = tmp.lableData.getParameter1();
					QMap<unsigned int, float> value = tmp.lableData.getParameter2();

					//获取形状的顶点数据
					const QVector<QVector3D> &Cone_Vertices_0 = mLableShapeVertex::getInstance()->_Cone_Vertices_0;
					const QVector<QVector3D> &Cylinder_Vertices_0 = mLableShapeVertex::getInstance()->_Cylinder_Vertices_0;
					const QVector<QVector3D> &Cone_Line_Vertices = mLableShapeVertex::getInstance()->_Cone_Line_Vertices;
					const QVector<QVector3D> &Cylinder_Vertices = mLableShapeVertex::getInstance()->_Cylinder_Vertices;
					
					int tmpVertexBegin = _rendObject->_vertexNum;
					int tmpElementBegin = _rendObject->_indexNum;

					//添加顶点数据以及索引
					for (int i = 0; i < pos.size(); i++)
					{
						for (auto it = value.begin(); it != value.end(); it++)
						{
							if (it.key() == 1)
							{
								if (it.value() == 0)
								{
									//判断1=0时 是否4也等于0
									if (value.count(4) != 0 && value.value(4) == 0)
									{

										//圆锥_0	
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_0, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转180）（浅色）
										appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));


										//圆柱_0
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cylinder_0, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转180）（深色）
										appendVertex(Cylinder_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));


									}
									else
									{
										//圆锥_0
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_0, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转180）（浅色）
										appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));

									}

								}
								else if (it.value() < 0)
								{
									//判断1<0时 是否4也<0
									if (value.count(4) != 0 && value.value(4) < 0)
									{
										//圆锥+直线(箭头)	
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转180）（浅色）
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));


										//圆柱		
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cylinder, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转180）（深色）
										appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));

									}
									else
									{
										//圆锥+直线(箭头)	
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转180）（浅色）
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 180));
									}
								}
								else
								{
									//判断1>0时 是否4也>0
									if (value.count(4) != 0 && value.value(4) > 0)
									{
										//圆锥+直线(箭头)	
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//添加顶点数据 （不旋转）（浅色）
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 0, 0));


										//圆柱		
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cylinder, _rendObject->_vertexNum);
										//添加顶点数据 （不旋转）（深色）
										appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 0, 0));

									}
									else
									{
										//圆锥+直线(箭头)	
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//添加顶点数据 （不旋转）（浅色）
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 0, 0));

									}

								}

							}
							else if (it.key() == 2)
							{
								if (it.value() == 0)
								{
									//判断2=0时 是否5也等于0
									if (value.count(5) != 0 && value.value(5) == 0)
									{
										//圆锥_0
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_0, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转-90°）（浅色）
										appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));


										//圆柱_0
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cylinder_0, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转180）（深色）
										appendVertex(Cylinder_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));

									}
									else
									{

										//圆锥_0
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_0, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转-90°）（浅色）
										appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));

									}

								}
								else if (it.value() < 0)
								{
									//判断2<0时 是否5也<0
									if (value.count(5) != 0 && value.value(5) < 0)
									{
										//圆锥+直线(箭头)
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转-90°）（浅色）
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));


										//圆柱
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cylinder, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转-90°）（深色）
										appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));


									}
									else
									{
										//圆锥+直线(箭头)
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转-90°）（浅色）
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, -90));

									}

								}
								else
								{
									//判断2>0时 是否5也>0
									if (value.count(5) != 0 && value.value(5) > 0)
									{
										//圆锥+直线(箭头)
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转90°）（浅色）
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 90));


										//圆柱
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cylinder, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转90°）（深色）
										appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 90));

									}
									else
									{
										//圆锥+直线(箭头)
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Z轴旋转90°）（浅色）
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 0, 1, 90));
									}
								}

							}
							else if (it.key() == 3)
							{
								if (it.value() == 0)
								{
									//判断3=0时 是否6也等于0
									if (value.count(6) != 0 && value.value(6) == 0)
									{
										//圆锥_0
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_0, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Y轴旋转90）（浅色）
										appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));


										//圆柱_0
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cylinder_0, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Y轴旋转90）（深色）
										appendVertex(Cylinder_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));
									}
									else
									{
										//圆锥_0
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_0, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Y轴旋转90）（浅色）
										appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));

									}

								}
								else if (it.value() < 0)
								{
									//判断3<0时 是否6也<0
									if (value.count(6) != 0 && value.value(6) < 0)
									{

										//圆锥+直线(箭头)
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Y轴旋转90°）（浅色）
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));


										//圆柱
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cylinder, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Y轴旋转90°）（深色）
										appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));


									}
									else
									{
										//圆锥+直线(箭头)
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Y轴旋转90°）（浅色）
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, 90));

									}

								}
								else
								{
									//判断3>0时 是否6也>0
									if (value.count(6) != 0 && value.value(6) > 0)
									{

										//圆锥+直线(箭头)
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Y轴旋转-90°）（浅色）
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_light, pos[i], QVector4D(0, 1, 0, -90));


										//圆柱
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cylinder, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Y轴旋转-90°）（深色）
										appendVertex(Cylinder_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, -90));



									}
									else
									{
										//圆锥+直线(箭头)
										//添加 圆锥_0 EBO索引
										appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
										//添加顶点数据 （绕着Y轴旋转-90°）（浅色）
										appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, -90));
									}
								}
							}
							else if (it.key() == 4)
							{
								if (it.value() == 0 && (value.count(1) == 0 || value.value(1) != 0)) //判断前面是否已经画过1,4都存在的情况
								{

									//圆锥_0
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转180°）（深色）
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));


								}
								else if (it.value() < 0 && (value.count(1) == 0 || value.value(1) >= 0))
								{

									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转180）（深色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 180));


								}
								else if (it.value() > 0 && (value.count(1) == 0 || value.value(1) <= 0))
								{
									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （不旋转）（深色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 0, 0));

								}
							}
							else if (it.key() == 5)
							{
								if (it.value() == 0 && (value.count(2) == 0 || value.value(2) != 0))
								{
									//圆锥_0
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转 - 90）（深色）
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));


								}
								else if (it.value() < 0 && (value.count(2) == 0 || value.value(2) >= 0))
								{
									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转 - 90）（深色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, -90));


								}
								else if (it.value() > 0 && (value.count(2) == 0 || value.value(2) <= 0))
								{
									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Z轴旋转 90）（深色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 0, 1, 90));

								}
							}
							else if (it.key() == 6)
							{
								if (it.value() == 0 && (value.count(3) == 0 || value.value(3) != 0))
								{
									//圆锥_0
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_0, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Y轴旋转90°）（深色）
									appendVertex(Cone_Vertices_0, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));


								}
								else if (it.value() < 0 && (value.count(3) == 0 || value.value(3) >= 0))
								{
									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Y轴旋转90°）（深色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, 90));


								}
								else if (it.value() > 0 && (value.count(3) == 0 || value.value(3) <= 0))
								{
									//圆锥+直线(箭头)
									//添加 圆锥_0 EBO索引
									appendEBOIndex(Cone_Line, _rendObject->_vertexNum);
									//添加顶点数据 （绕着Y轴旋转-90°）（深色）
									appendVertex(Cone_Line_Vertices, highlightState, showState, depthState, size, color_deep, pos[i], QVector4D(0, 1, 0, -90));

								}
							}
						}
					}

					//更新顶点索引数量
					_rendObject->_indexNum = (int)_EBOindex.size();

					int tmpVertexSize = _rendObject->_vertexNum - tmpVertexBegin;
					int tmpElementSize = _rendObject->_indexNum - tmpElementBegin;
					//传递箭头顶点坐标的ID和索引 便于删除操作
					_vertexIndex[ID] = { tmpVertexBegin,tmpVertexSize };
					_elementIndex[ID] = { tmpElementBegin ,tmpElementSize };

	
				}
				//更新buffer
				initialBuffer_append_index<float, GLshort>(3, 4, _vertex_Pos, _vertex_Rotate, vertexBegin, _EBOindex);
				//清空数据
				deleteVertex();
			}
			else if (mode == Delete)
			{
				int deleteVertexSize = _rendObject->_vertexNum;
				vector<GLuint>().swap(_EBOindex);
				//更新顶点数量
				_rendObject->_vertexNum = 0;
				//更新索引数量
				_rendObject->_indexNum = 0;
				//更新渲染
				initialBuffer_delete_index<float, GLshort>(3, 4, 0, deleteVertexSize, _EBOindex);
				_vertexIndex.clear();
				_elementIndex.clear();
			}
			else if (mode == Show || mode == Hide || mode == Highlight || mode == NoHighlight|| mode == SetColor|| mode == SetSize)
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
				else if (mode == SetColor)
				{
					int updateVertexSize = _rendObject->_vertexNum;
					vector<GLubyte> tmpVertex;
					for (auto it = _data.begin(); it != _data.end(); ++it)
					{
						auto tmp = it.value();
						int ID = tmp.lableData.getLableID();
						QVector3D color = tmp.lableData.getLableColor();
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
		}
		/*
		* 添加顶点数据
		*/
		void appendVertex(const QVector<QVector3D> & shapeVertex, const bool highlightstate, const bool showstate, const bool depthstate, const float size,
			const QVector3D color, const QVector3D pos, const QVector4D rotate)
		{
			for (int i = 0; i < shapeVertex.size(); ++i)
			{
				appendCommonVertex(highlightstate, showstate, depthstate, size, color, shapeVertex[i]);
				//图标的位置（模型坐标下）
				_vertex_Pos.push_back(pos.x());
				_vertex_Pos.push_back(pos.y());
				_vertex_Pos.push_back(pos.z());
				//旋转轴和旋转角度
				_vertex_Rotate.push_back(rotate.x());
				_vertex_Rotate.push_back(rotate.y());
				_vertex_Rotate.push_back(rotate.z());
				_vertex_Rotate.push_back(rotate.w());
			}
			_rendObject->_vertexNum += shapeVertex.size();

		}
		/*
		* 删除顶点数据
		*/
		inline void deleteVertex()
		{
			deleteCommonVerrtex();
			vector<float>().swap(_vertex_Pos);
			vector<GLshort>().swap(_vertex_Rotate);

			//注意 不能删除_EBOindex  
			//vector<GLuint>().swap(_EBOindex);


		}
		/*
		* 添加EBO索引
		*/
		inline void appendEBOIndex(ShapeType type, int begin)
		{
			//固定圆锥
			if (type == Cone_0)
			{
				_EBOindex.push_back(begin);
				_EBOindex.push_back(begin + 1);

				_EBOindex.push_back(begin);
				_EBOindex.push_back(begin + 2);

				_EBOindex.push_back(begin);
				_EBOindex.push_back(begin + 3);

				_EBOindex.push_back(begin);
				_EBOindex.push_back(begin + 4);

				_EBOindex.push_back(begin + 1);
				_EBOindex.push_back(begin + 2);

				_EBOindex.push_back(begin + 2);
				_EBOindex.push_back(begin + 3);

				_EBOindex.push_back(begin + 3);
				_EBOindex.push_back(begin + 4);

				_EBOindex.push_back(begin + 4);
				_EBOindex.push_back(begin + 1);
			}
			//固定圆柱
			else if (type == Cylinder_0)
			{
				_EBOindex.push_back(begin);
				_EBOindex.push_back(begin + 4);

				_EBOindex.push_back(begin + 1);
				_EBOindex.push_back(begin + 5);

				_EBOindex.push_back(begin + 2);
				_EBOindex.push_back(begin + 6);

				_EBOindex.push_back(begin + 3);
				_EBOindex.push_back(begin + 7);

				_EBOindex.push_back(begin + 4);
				_EBOindex.push_back(begin + 5);

				_EBOindex.push_back(begin + 5);
				_EBOindex.push_back(begin + 6);

				_EBOindex.push_back(begin + 6);
				_EBOindex.push_back(begin + 7);

				_EBOindex.push_back(begin + 7);
				_EBOindex.push_back(begin + 4);
			}
			//非固定 圆锥加直线
			else if (type == Cone_Line)
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
			//非固定圆柱
			else if (type == Cylinder)
			{
				_EBOindex.push_back(begin);
				_EBOindex.push_back(begin + 4);

				_EBOindex.push_back(begin + 1);
				_EBOindex.push_back(begin + 5);

				_EBOindex.push_back(begin + 2);
				_EBOindex.push_back(begin + 6);

				_EBOindex.push_back(begin + 3);
				_EBOindex.push_back(begin + 7);

				_EBOindex.push_back(begin + 4);
				_EBOindex.push_back(begin + 5);

				_EBOindex.push_back(begin + 5);
				_EBOindex.push_back(begin + 6);

				_EBOindex.push_back(begin + 6);
				_EBOindex.push_back(begin + 7);

				_EBOindex.push_back(begin + 7);
				_EBOindex.push_back(begin + 4);

			}
		}


	private:
		//渲染数据
		const QHash<QString, T> &_data;
		//顶点数据
		vector<float> _vertex_Pos;  //需要移动的坐标
		vector<GLshort> _vertex_Rotate;	//旋转轴和角度  例如（1,0,0,180）  绕着X轴旋转180度
		//EBO索引数据
		vector<GLuint> _EBOindex; //一直存在 不会清空 便于删除索引
		QHash<int, QPair<int, int>> _elementIndex;	//QHash<ID,索引的起始位置，索引的个数>
	};
}
