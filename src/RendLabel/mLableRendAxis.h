//////////////////////////////////////////////////////////////////////
//此类用于渲染 箭头类型的图标////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#pragma once
#include"mLableRendBase.h"
#include"mLableRendEnum.h"
#include"mLableDataStruct.h"
#include"mLableShapeVertex.h"
using namespace MDataLable;
namespace MLableRend
{
	//箭头类型图标绘制
	template<class T>
	class mLableRendAxis : public mLableRendBase<mDrawElement>
	{
	public:

		explicit mLableRendAxis(const QHash<QString, T> &data) :mLableRendBase<mDrawElement>(8, GL_LINES), _data(data) {};
		virtual ~mLableRendAxis()  override {};

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
			QVector3D color_x = tmp.lableData.getLableColor();
			QVector3D color_y = tmp.lableData2.getLableColor();
			QVector3D color_z = tmp.lableData3.getLableColor();

			QVector<POS_DIR> pos_dir_x = tmp.lableData.getParameter();
			QVector<POS_DIR> pos_dir_y = tmp.lableData2.getParameter();
			QVector<POS_DIR> pos_dir_z = tmp.lableData3.getParameter();
			//载荷类图标
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

				QVector<QVector3D> shapeVertex = mLableShapeVertex::getInstance()->_Arrow_Vertices;

				//添加顶点数据
				for (int i = 0; i < pos_dir_x.size(); i++)
				{
					//添加箭头绘制索引
					appendEBOIndex(_rendObject->_vertexNum);

					//添加箭头顶点数据
					for (int j = 0; j < shapeVertex.size(); j++)
					{
						//添加通用的数据
						appendCommonVertex(highlightState, showState, depthState, size, color_x, shapeVertex[j]);
						//添加特有的数据
						appendSpecificVertex(pos_dir_x[i].first, pos_dir_x[i].second);
					}
					//更新顶点数量
					_rendObject->_vertexNum += shapeVertex.size();
				}
				for (int i = 0; i < pos_dir_y.size(); i++)
				{
					//添加箭头绘制索引
					appendEBOIndex(_rendObject->_vertexNum);

					//添加箭头顶点数据
					for (int j = 0; j < shapeVertex.size(); j++)
					{
						//添加通用的数据
						appendCommonVertex(highlightState, showState, depthState, size, color_y, shapeVertex[j]);
						//添加特有的数据
						appendSpecificVertex(pos_dir_y[i].first, pos_dir_y[i].second);
					}
					//更新顶点数量
					_rendObject->_vertexNum += shapeVertex.size();
				}
				for (int i = 0; i < pos_dir_z.size(); i++)
				{
					//添加箭头绘制索引
					appendEBOIndex(_rendObject->_vertexNum);

					//添加箭头顶点数据
					for (int j = 0; j < shapeVertex.size(); j++)
					{
						//添加通用的数据
						appendCommonVertex(highlightState, showState, depthState, size, color_z, shapeVertex[j]);
						//添加特有的数据
						appendSpecificVertex(pos_dir_z[i].first, pos_dir_z[i].second);
					}
					//更新顶点数量
					_rendObject->_vertexNum += shapeVertex.size();
				}

				//更新顶点索引数量
				_rendObject->_indexNum = _EBOindex.size();

				//传递箭头顶点坐标的ID和索引 便于删除操作
				_vertexIndex[ID] = { vertexBegin,(int)_vertex_HighlightState.size() };
				_elementIndex[ID] = { indexBegin ,(int)_EBOindex.size() - indexBegin };
				//更新buffer
				initialBuffer_append_index<float, float>(3, 3, _vertex_Pos, _vertex_Dir, vertexBegin, _EBOindex);

				//清空数据
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
				swap(tmpEBOIndex, _EBOindex);
				//更新渲染
				initialBuffer_delete_index<float, float>(3, 3, deleteVertexBegin, deleteVertexSize, _EBOindex);


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

					//添加顶点数据
					for (int i = 0; i < pos_dir_x.size(); i++)
					{
						//添加箭头绘制索引
						appendEBOIndex(_rendObject->_vertexNum);

						//添加箭头顶点数据
						for (int j = 0; j < shapeVertex.size(); j++)
						{
							//添加通用的数据
							appendCommonVertex(highlightState, showState, depthState, size, color_x, shapeVertex[j]);
							//添加特有的数据
							appendSpecificVertex(pos_dir_x[i].first, pos_dir_x[i].second);
						}
						//更新顶点数量
						_rendObject->_vertexNum += shapeVertex.size();
					}
					for (int i = 0; i < pos_dir_y.size(); i++)
					{
						//添加箭头绘制索引
						appendEBOIndex(_rendObject->_vertexNum);

						//添加箭头顶点数据
						for (int j = 0; j < shapeVertex.size(); j++)
						{
							//添加通用的数据
							appendCommonVertex(highlightState, showState, depthState, size, color_y, shapeVertex[j]);
							//添加特有的数据
							appendSpecificVertex(pos_dir_y[i].first, pos_dir_y[i].second);
						}
						//更新顶点数量
						_rendObject->_vertexNum += shapeVertex.size();
					} 
					for (int i = 0; i < pos_dir_z.size(); i++)
					{
						//添加箭头绘制索引
						appendEBOIndex(_rendObject->_vertexNum);

						//添加箭头顶点数据
						for (int j = 0; j < shapeVertex.size(); j++)
						{
							//添加通用的数据
							appendCommonVertex(highlightState, showState, depthState, size, color_z, shapeVertex[j]);
							//添加特有的数据
							appendSpecificVertex(pos_dir_z[i].first, pos_dir_z[i].second);
						}
						//更新顶点数量
						_rendObject->_vertexNum += shapeVertex.size();
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
				initialBuffer_append_index<float, float>(3, 3, _vertex_Pos, _vertex_Dir, vertexBegin, _EBOindex);

				//清空数据
				deleteSpecificVertex();
				deleteCommonVerrtex();
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
		* 添加特有的的顶点数据
		*/
		inline void appendSpecificVertex(const QVector3D pos, const QVector3D dir)
		{
			//图标的位置（模型坐标下）
			_vertex_Pos.push_back(pos.x());
			_vertex_Pos.push_back(pos.y());
			_vertex_Pos.push_back(pos.z());
			//载荷值（箭头方向）
			_vertex_Dir.push_back(dir.x());
			_vertex_Dir.push_back(dir.y());
			_vertex_Dir.push_back(dir.z());
		}

		/*
		* 删除特有的的顶点数据
		*/
		inline void deleteSpecificVertex()
		{
			vector<float>().swap(_vertex_Pos);
			vector<float>().swap(_vertex_Dir);
		}
		/*
		* 添加箭头EBO索引
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
		//渲染数据
		const QHash<QString, T> &_data;
		//顶点数据
		vector<float> _vertex_Pos;  //需要移动的坐标
		vector<float> _vertex_Dir;	//箭头朝向
		//EBO索引数据
		vector<GLuint> _EBOindex;//一直存在 不会清空 便于删除索引
		QHash<int, QPair<int, int>> _elementIndex;	//QHash<ID,索引的起始位置，索引的个数>

	};
}
