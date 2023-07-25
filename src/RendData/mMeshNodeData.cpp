#include "mMeshNodeData.h"
#include <fstream>
#include "mGlobalVarible.h"

#include <qguiapplication.h>

using namespace MViewBasic;
namespace MDataMesh
{
	mMeshNodeData::mMeshNodeData(int ID, QVector3D vertex)
	{
		_nodeID = ID;
		_nodeVertex = vertex;
	}

	mMeshNodeData::~mMeshNodeData()
	{
	}

	//void mMeshNodeData::addMesh(int meshID)
	//{
	//	//_meshIds.insert(meshID);
	//}

	bool mMeshNodeData::deleteMesh(int meshID)
	{
		//_meshIds.erase(meshID);
		//if (_meshIds.size()==0)
		//{
		//	return true;
		//}
		return false;
	}
	int mMeshNodeData::getNodeID()
	{
		return _nodeID;
	}

	QVector3D mMeshNodeData::getNodeVertex()
	{
		return _nodeVertex;
	}

	void mMeshNodeData::setNodeVertex(int position, float value)
	{
		_nodeVertex[position] = value;
	}

	//void mMeshNodeData::setNodeInMeshIDs(const std::set<int>& ids)
	//{
	//	//_meshIds = ids;
	//}

	std::set<int> mMeshNodeData::getNodeInMeshIDs()
	{
		std::set <int> ids;
		if (pointcell.size == 0)
		{
			return ids;
		}
		
		std::ifstream fin(_postPointCellFileName, std::ios::binary);
		fin.seekg(pointcell.addr, std::ios::beg);
		void* data = malloc(pointcell.size);
		fin.read((char*)data, pointcell.size);
		fin.close();
		for (int i = 0; i < pointcell.size / sizeof(int); i++)
		{
			int nNume = 0;
			memcpy(&nNume, (char*)data + i * sizeof(int), sizeof(int));
			ids.insert(nNume);
		}
		free(data);
		return ids;
	}

	std::set<int> mMeshNodeData::getNodeInMeshLineIDs()
	{
		std::set <int> ids;
		if (pointline.size == 0)
		{
			return ids;
		}
		std::ifstream fin(_postPointCellFileName, std::ios::binary);
		fin.seekg(pointline.addr, std::ios::beg);
		for (int i = 0; i < pointline.size / sizeof(int); i++)
		{
			int nNum;
			fin.read((char*)&nNum, sizeof(int));
			ids.insert(nNum);
		}
		fin.close();
		return ids;
	}


	bool mMeshNodeData::getNodeIsFreeNode()
	{
		std::set<int> ids = getNodeInMeshIDs();
		return ids.empty();
	}
	bool operator<(const std::shared_ptr<mMeshNodeData>& left, const std::shared_ptr<mMeshNodeData>& right)
	{
		//return false;
		if (left->getNodeID() == right->getNodeID())     //根据id去重
		{
			return false;
		}
		else
		{
			if (left->getNodeID() != right->getNodeID())
			{
				return left->getNodeID() < right->getNodeID();      //降序
			}
			else
			{
				return left->getNodeID() < right->getNodeID();
			}

		}
	};
}