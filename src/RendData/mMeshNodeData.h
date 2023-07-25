#pragma once
#include "renddata_global.h"

#include <QVector3D>
#include <QVector>

#include <set>

namespace MDataMesh
{

	struct PointCell
	{
		int addr = 0;
		int size = 0;
	};

	struct PointLine
	{
		int addr = 0;
		int size = 0;
	};

	class RENDDATA_EXPORT mMeshNodeData
	{

	public:
		PointCell pointcell;

		PointLine pointline;

		mMeshNodeData(int ID, QVector3D vertex);
		~mMeshNodeData();

		//增加网格
		//void addMesh(int meshID);

		//减少网格
		bool deleteMesh(int meshID);

		//获取节点ID
		int getNodeID();

		//获取节点坐标
		QVector3D getNodeVertex();

		//设置节点坐标
		void setNodeVertex(int position, float value);

		//存储节点所在的网格ID
		//void setNodeInMeshIDs(const std::set<int>& ids);

		//获取节点所在的网格ID
		std::set<int> getNodeInMeshIDs();

		//获取节点所在的单元线ID
		std::set<int> getNodeInMeshLineIDs();

		//判断该节点是不是自由节点
		bool getNodeIsFreeNode();

		//set容器中存放类对象智能指针时,自定义比较函数方式1:friend方式重载<
		friend	bool operator <(const std::shared_ptr<mMeshNodeData>& left, const std::shared_ptr<mMeshNodeData>&  right);

	private:
		//节点ID
		int _nodeID;
		//节点坐标
		QVector3D _nodeVertex;


	};
}
