#include "mMeshFaceData.h"

#include "mMeshData.h"
#include "mMeshModelData.h"
#include "mMeshNodeData.h"
namespace MDataMesh
{
	mMeshFaceData::mMeshFaceData(QVector<int> index, std::shared_ptr<mMeshData> meshID)
	{
		_index.reserve(index.size());
		for (auto i : index)
		{
			_index.append(meshID->getMeshModelData()->getNodeDataByID(i));
		}

		_meshID1 = meshID;
		
		_meshID2 = nullptr;
		_isShow = true;
	}

	mMeshFaceData::~mMeshFaceData()
	{
	}

	void mMeshFaceData::appendMeshID(std::shared_ptr<mMeshData> meshID, int startIndex)
	{
		_meshID2 = meshID;

		for (int i = 0;i< _index.size();++i)
		{
			if (_index[i]->getNodeID() == startIndex)
			{
				_startIndex = i;
				break;
			}
		}
	}

	QVector<shared_ptr<mMeshNodeData>> mMeshFaceData::getNodeData()
	{
		//QVector<int> sindex;
		//for (int i = 0; i < 4 ; i++)
		//{
		//	if (_index[i] != -1)
		//	{
		//		sindex.append(_index[i]);
		//	}
		//}
		return _index;	
	}

	QVector<shared_ptr<mMeshNodeData>> mMeshFaceData::getNodeData(int meshID)
	{
		if (meshID == _meshID1->getMeshID())
		{
			return _index;
		}
		else
		{
			if (_index.size() == 4)//四边形
			{
				switch (_startIndex)
				{
				case 0:return QVector<shared_ptr<mMeshNodeData>>{_index[0], _index[3], _index[2], _index[1]};
					break;				
				case 1:return QVector<shared_ptr<mMeshNodeData>>{_index[1], _index[0], _index[3], _index[2]};
					break;
				case 2:return QVector<shared_ptr<mMeshNodeData>>{_index[2], _index[1], _index[0], _index[3]};
					   break;
				case 3:return QVector<shared_ptr<mMeshNodeData>>{_index[3], _index[2], _index[1], _index[0]};
					   break;
				default:
					break;
				}
			}
			else//三角形
			{
				switch (_startIndex)
				{
				case 0:return QVector<shared_ptr<mMeshNodeData>>{_index[0], _index[2], _index[1]};
					   break;
				case 1:return QVector<shared_ptr<mMeshNodeData>>{_index[1], _index[0], _index[2]};
					   break;
				case 2:return QVector<shared_ptr<mMeshNodeData>>{_index[2], _index[1], _index[0]};
					   break;
				case 3:return QVector<shared_ptr<mMeshNodeData>>{_index[2], _index[1], _index[0]};
					   break;
				default:
					break;
				}
			}
		}
		return _index;
	}

	QVector<QVector3D> mMeshFaceData::getNodeVertex()
	{
		QVector<QVector3D> vertexs;
		vertexs.reserve(_index.size());
		for (auto index : _index)
		{
			vertexs.append(index->getNodeVertex());
		}
		return vertexs;
	}

	QVector<QVector3D> mMeshFaceData::getNodeVertex(int meshID)
	{
		QVector<shared_ptr<mMeshNodeData>> nodeDatas = this->getNodeData();
		QVector<QVector3D> vertexs;
		vertexs.reserve(_index.size());
		for (auto index : nodeDatas)
		{
			vertexs.append(index->getNodeVertex());
		}
		return vertexs;
	}

	QPair<int, int> mMeshFaceData::getFirstMesh()
	{
		return QPair<int, int>();
	}
	QString mMeshFaceData::getPartName()
	{
		return _meshID1->getPartName();
	}
	std::shared_ptr<mMeshData> mMeshFaceData::getMeshID()
	{
		return std::shared_ptr<mMeshData>();
	}
	std::shared_ptr<mMeshData> mMeshFaceData::getMeshID1()
	{
		return _meshID1;
	}

	short int mMeshFaceData::getMeshOrder1()
	{
		//return _order1;
		return short int();
	}

	std::shared_ptr<mMeshData> mMeshFaceData::getMeshID2()
	{
		return _meshID2;
	}

	short int mMeshFaceData::getMeshOrder2()
	{
		//return _order2;
		return short int();
	}

	bool mMeshFaceData::deleteMesh(int meshID)
	{
		//if (_meshID2 == meshID)
		//{
		//	_meshID2 = 0;
		//	return false;
		//}
		//else if(_meshID1 == meshID&& _meshID2 == 0)
		//{
		//	return true;
		//}
		//else if (_meshID1 == meshID && _meshID2 !=0)
		//{
		//	_meshID1 = _meshID2;
		//	_meshID2 = 0;
		//	return false;
		//}
		return false;
	}

	bool mMeshFaceData::getVisual()
	{
		return _isShow;
	}

	void mMeshFaceData::setVisual(bool isShow)
	{
		_isShow = isShow;
	}
}