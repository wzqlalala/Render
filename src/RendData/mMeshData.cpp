#include "mMeshData.h"
#include "mMeshFaceData.h"
#include "mMeshPartData.h"
#include "mMeshModelData.h"
#include "mMeshNodeData.h"

using namespace MViewBasic;
namespace MDataMesh
{
	mMeshData::mMeshData(int ID, shared_ptr<mMeshPartData> partData)
	{
		_meshID = ID;
		_isShow = true;
		_partData = partData;
		//_modelData = modelData;
		this->getMeshModelData()->appendMeshData(ID, shared_from_this());
	}

	mMeshData::~mMeshData()
	{
	}
	QVector<int> mMeshData::getNodeIndex()
	{
		QVector<shared_ptr<mMeshNodeData>> nodeDatas = this->getNodeData();
		QVector<int> ids;
		ids.reserve(nodeDatas.size());
		for (auto nodeData : nodeDatas)
		{
			ids.append(nodeData->getNodeID());
		}
		return ids;
		//if (_index.size() != 0)
		//{
		//	return _index;
		//}
		//else
		//{
		//	std::set<int> set_index;
		//	QVector<int> indexs;
		//	QVector<int> index;
		//	switch (_meshType)
		//	{
		//	case MeshTet:
		//		indexs.append(faces[0]->getNodeIndex(_meshID)[0]);
		//		indexs.append(faces[2]->getNodeIndex(_meshID));
		//		break;
		//	case MeshWedge:
		//		index = faces[0]->getNodeIndex(_meshID);
		//		indexs.append(index[0]);
		//		indexs.append(index[2]);
		//		indexs.append(index[1]);
		//		indexs.append(faces[1]->getNodeIndex(_meshID));
		//		break;
		//	case MeshHex:
		//		index = faces[0]->getNodeIndex(_meshID);
		//		indexs.append(index[0]);
		//		indexs.append(index[3]);
		//		indexs.append(index[2]);
		//		indexs.append(index[1]);	
		//		indexs.append(faces[1]->getNodeIndex(_meshID));
		//		break;
		//	case MeshPyramid:
		//		indexs.append(faces[0]->getNodeIndex(_meshID));
		//		indexs.append(faces[1]->getNodeIndex(_meshID)[2]);
		//		break;
		//	default:
		//		break;
		//	}
		//	return indexs;
		//}
	}

	QVector<QVector3D> mMeshData::getNodeVertex()
	{
		QVector<shared_ptr<mMeshNodeData>> nodeDatas = this->getNodeData();
		QVector<QVector3D> vertexs;
		vertexs.reserve(nodeDatas.size());
		for (auto nodeData : nodeDatas)
		{
			vertexs.append(nodeData->getNodeVertex());
		}
		return vertexs;
	}

	QVector3D mMeshData::getMeshCenter()
	{
		QVector<QVector3D> vertexs = this->getNodeVertex();
		QVector3D center;
		for (auto vertex : vertexs)
		{
			center += vertex;
		}

		return center / vertexs.size();
	}

	Space::AABB mMeshData::getMeshAABB()
	{
		QVector<QVector3D> vertexs = this->getNodeVertex();
		Space::AABB aabb;
		for (auto vertex : vertexs)
		{
			aabb.push(vertex);
		}
		return aabb;
	}

	void mMeshData::appendMeshLine(int meshLineID)
	{
		//_meshLineIDs.append(meshLineID);
	}

	void mMeshData::setMeshLine(const QVector<int>& ids)
	{
		//_meshLineIDs = ids;
	}

	QVector<int> mMeshData::getMeshLines()
	{
		QVector<int> ids;
		return ids;
		//return _meshLineIDs;
	}

	void mMeshData::setMeshVisual(bool isshow)
	{
		_isShow = isshow;
	}

	QString mMeshData::getPartName()
	{
		return _partData->getPartName();
	}

	shared_ptr<mMeshModelData> mMeshData::getMeshModelData()
	{
		return _partData->getMeshModelData();
	}

	mMesh0DimData::mMesh0DimData(int ID, shared_ptr<mMeshPartData> partData) : mMeshData(ID, partData)
	{
	
	}

	void mMesh0DimData::setNodeIndex(QVector<int> index, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>& _meshFace)
	{
		_index = this->getMeshModelData()->getNodeDataByID(index.at(0));
		_partData->appendMesh0(shared_from_this());
	}

	QVector<shared_ptr<mMeshNodeData>> mMesh0DimData::getNodeData()
	{
		return QVector<shared_ptr<mMeshNodeData>>{_index};
	}

	mMesh1DimData::mMesh1DimData(int ID, shared_ptr<mMeshPartData> partData) : mMeshData(ID, partData)
	{


	}

	void mMesh1DimData::setNodeIndex(QVector<int> index, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>& _meshFace)
	{
		_index.reserve(index.size());
		for (auto i : index)
		{
			_index.append(this->getMeshModelData()->getNodeDataByID(i));
		}
		_partData->appendMesh1(shared_from_this());
	}

	QVector<shared_ptr<mMeshNodeData>> mMesh1DimData::getNodeData()
	{
		return _index;
	}

	mMesh2DimData::mMesh2DimData(int ID, shared_ptr<mMeshPartData> partData) : mMeshData(ID, partData)
	{

	}

	void mMesh2DimData::setNodeIndex(QVector<int> index, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>& _meshFace)
	{
		_index.reserve(index.size());
		for (auto i : index)
		{
			_index.append(this->getMeshModelData()->getNodeDataByID(i));
		}
		_partData->appendMesh2(shared_from_this());
	}

	QVector<shared_ptr<mMeshNodeData>> mMesh2DimData::getNodeData()
	{
		return _index;
	}

	mMeshTriData::mMeshTriData(int ID, shared_ptr<mMeshPartData> partData) : mMesh2DimData(ID, partData)
	{

	}

	mMeshQuadData::mMeshQuadData(int ID, shared_ptr<mMeshPartData> partData) : mMesh2DimData(ID, partData)
	{

	}

	mMesh3DimData::mMesh3DimData(int ID, shared_ptr<mMeshPartData> partData):mMeshData(ID, partData)
	{

	}

	QVector<shared_ptr<mMeshFaceData>> mMesh3DimData::getMeshFace()
	{
		return _faces;
	}

	void mMesh3DimData::setNodeIndex(QVector<int> index, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>& _meshFace)
	{
		//QVector<shared_ptr<mMeshNodeData>> nodeDatas;
		//nodeDatas.reserve(index.size());
		//for (auto i : index)
		//{
		//	nodeDatas.append(this->getMeshModelData()->getNodeDataByID(i));
		//}
		switch (this->getMeshType())
		{
		case MeshTet:
		{
			createMeshFace(QVector<int>{index.at(0), index.at(2), index.at(1)}, 1, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(3)}, 2, _meshFace);
			createMeshFace(QVector<int>{index.at(1), index.at(2), index.at(3)}, 3, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(3), index.at(2)}, 4, _meshFace);
		}
		break;
		case MeshWedge:
		{
			createMeshFace(QVector<int>{index.at(0), index.at(2), index.at(1)}, 1, _meshFace);
			createMeshFace(QVector<int>{index.at(3), index.at(4), index.at(5)}, 2, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(4), index.at(3)}, 3, _meshFace);
			createMeshFace(QVector<int>{index.at(1), index.at(2), index.at(5), index.at(4)}, 4, _meshFace);
			createMeshFace(QVector<int>{index.at(2), index.at(0), index.at(3), index.at(5)}, 5, _meshFace);
		}
		break;
		case MeshPyramid:
		{
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(2), index.at(3)}, 1, _meshFace);
			createMeshFace(QVector<int>{index.at(1), index.at(2), index.at(4)}, 2, _meshFace);
			createMeshFace(QVector<int>{index.at(4), index.at(3), index.at(2)}, 3, _meshFace);
			createMeshFace(QVector<int>{index.at(4), index.at(0), index.at(3)}, 4, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(4)}, 5, _meshFace);
		}
		break;
		case MeshHex:
		{
			createMeshFace(QVector<int>{index.at(0), index.at(3), index.at(2), index.at(1)}, 1, _meshFace);
			createMeshFace(QVector<int>{index.at(4), index.at(5), index.at(6), index.at(7)}, 2, _meshFace);
			createMeshFace(QVector<int>{index.at(0), index.at(1), index.at(5), index.at(4)}, 3, _meshFace);
			createMeshFace(QVector<int>{index.at(1), index.at(2), index.at(6), index.at(5)}, 4, _meshFace);
			createMeshFace(QVector<int>{index.at(2), index.at(3), index.at(7), index.at(6)}, 5, _meshFace);
			createMeshFace(QVector<int>{index.at(3), index.at(0), index.at(4), index.at(7)}, 6, _meshFace);
		}
		break;
		}
		_partData->appendMesh3(shared_from_this());
	}

	void mMesh3DimData::createMeshFace(QVector<int> set, int order, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>& _meshFace)
	{
		auto itermin = min_element(set.begin(), set.end());
		int in = distance(set.begin(), itermin);
		QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>> &face = _meshFace[*itermin];
		bool isNeibu{ false };
		QPair<int, int> pair;
		int n1 = set.size();
		if (n1 == 3)
		{
			switch (in)
			{
			case 0:pair.first = set.at(2); pair.second = set.at(1); break;
			case 1:pair.first = set.at(0); pair.second = set.at(2); break;
			case 2:pair.first = set.at(1); pair.second = set.at(0); break;
			default:
				break;
			}
		}
		else
		{
			switch (in)
			{
			case 0:pair.first = set.at(3); pair.second = set.at(1); break;
			case 1:pair.first = set.at(0); pair.second = set.at(2); break;
			case 2:pair.first = set.at(1); pair.second = set.at(3); break;
			case 3:pair.first = set.at(2); pair.second = set.at(0); break;
			default:
				break;
			}
		}
		for (int i = 0, n = face.size(); i < n; ++i)
		{
			QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>> f = face.at(i);
			int n2 = f.second->getNodeData().size();
			if (f.first.first == pair.second && f.first.second == pair.first)//是内部面
			{
				if (n1 == n2)
				{
					f.second->appendMeshID(shared_from_this(), set[0]);
					//this->appendMeshFace(f.second);
					this->setMeshFace(order, f.second);
					face.removeAt(i);
					f.second->setVisual(false);
					isNeibu = true;
					break;
				}
			}
		}
		if (!isNeibu)//外部面，重新创建
		{
			std::shared_ptr<mMeshFaceData> meshFaceData1 = make_shared<mMeshFaceData>(set, shared_from_this());
			//_modelData->appendMeshFaceData(0, meshFaceData1);
			//this->appendMeshFace(meshFaceData1);
			this->setMeshFace(order, meshFaceData1);
			_meshFace[*itermin].append({ pair,meshFaceData1 });
		}
	}

	void mMesh3DimData::setMeshFace(int order, shared_ptr<mMeshFaceData> meshFace)
	{
		_faces[order] = meshFace;
	}


	mMeshTetData::mMeshTetData(int ID, shared_ptr<mMeshPartData> partData) :mMesh3DimData(ID, partData)
	{
		//创建面
		_faces.resize(4);
	}

	QVector<shared_ptr<mMeshNodeData>> mMeshTetData::getNodeData()
	{
		QVector<shared_ptr<mMeshNodeData>> nodes;
		nodes.append(_faces[0]->getNodeData(_meshID)[0]);
		nodes.append(_faces[2]->getNodeData(_meshID));
		return nodes;
	}

	mMeshPyramidData::mMeshPyramidData(int ID, shared_ptr<mMeshPartData> partData):mMesh3DimData(ID, partData)
	{

	}
	QVector<shared_ptr<mMeshNodeData>> mMeshPyramidData::getNodeData()
	{
		QVector<shared_ptr<mMeshNodeData>> nodes;
		nodes.append(_faces[0]->getNodeData(_meshID));
		nodes.append(_faces[1]->getNodeData(_meshID)[2]);
		return nodes;
	}

	mMeshWedgeData::mMeshWedgeData(int ID, shared_ptr<mMeshPartData> partData) :mMesh3DimData(ID, partData)
	{

	}
	QVector<shared_ptr<mMeshNodeData>> mMeshWedgeData::getNodeData()
	{
		QVector<shared_ptr<mMeshNodeData>> nodes;
		QVector<shared_ptr<mMeshNodeData>> index;
		index = _faces[0]->getNodeData(_meshID);
		nodes.append(index[0]);
		nodes.append(index[2]);
		nodes.append(index[1]);
		nodes.append(_faces[1]->getNodeData(_meshID));
		return nodes;
	}

	mMeshHexData::mMeshHexData(int ID, shared_ptr<mMeshPartData> partData) :mMesh3DimData(ID, partData)
	{

	}
	QVector<shared_ptr<mMeshNodeData>> mMeshHexData::getNodeData()
	{
		QVector<shared_ptr<mMeshNodeData>> nodes;
		QVector<shared_ptr<mMeshNodeData>> index;
		index = _faces[0]->getNodeData(_meshID);
		nodes.append(index[0]);
		nodes.append(index[3]);
		nodes.append(index[2]);
		nodes.append(index[1]);	
		nodes.append(_faces[1]->getNodeData(_meshID));
		return nodes;
	}
}
