#include "mMeshLineData.h"
namespace MDataMesh
{
	mMeshLineData::mMeshLineData(int ID, std::set<int> index, int meshID,int order, QString partName)
	{
		_ID = ID;
		_index1 = *index.begin();
		_index2 = *(++index.begin());
		_meshID1 = meshID;
		_order = order;
		_partName = partName;
	}

	mMeshLineData::~mMeshLineData()
	{
	}
	int mMeshLineData::getMeshLineNodeIndex1()
	{
		return _index1;
	}

	int mMeshLineData::getMeshLineNodeIndex2()
	{
		return _index2;
	}
	int mMeshLineData::getMeshLineID()
	{
		return _ID;
	}
	QString mMeshLineData::getPartName()
	{
		return _partName;
	}
	int mMeshLineData::getMeshID()
	{
		return _meshID1;
	}

	short mMeshLineData::getMeshOrder()
	{
		return _order;
	}
}
