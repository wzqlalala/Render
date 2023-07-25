#include "mMeshPartData.h"
#include "mMeshFaceData.h"
#include "mMeshData.h"
namespace MDataMesh
{
	//int mMeshPartData::ID = 0;

	QList<QVector3D> mMeshPartData::Color = QList<QVector3D>{ QVector3D(1.00f, 0.00f, 0.00f),
										   QVector3D(0.00f, 0.00f, 1.00f),
										  QVector3D(0.65f, 0.59f, 0.0f),
										  QVector3D(0.84f, 0.19f, 0.46f),
										  QVector3D(0.57f, 0.43f, 0.85f),
										  QVector3D(0.91f, 0.67f, 0.65f),
										  QVector3D(0.50f, 0.24f, 0.12f),
										  QVector3D(0.05f, 0.94f, 0.04f) };

	mMeshPartData::mMeshPartData()
	{
		//ID++;
		_partName = "";
		_isShow = true;
		_partID = 1;
		int num = (_partID - 1) % Color.size();
		_partColor = Color.at(num);
		//spaceTree = nullptr;
	}

	mMeshPartData::mMeshPartData(QString name, int ID)
	{
		//ID++;
		_partName = name;
		_isShow = true;
		_partID = ID;
		int num = (_partID - 1) % Color.size();
		_partColor = Color.at(num);
		//spaceTree = nullptr;
	}

	mMeshPartData::~mMeshPartData()
	{
		//Space::destoryTree(spaceTree);
	}

	void mMeshPartData::setPartColor(QVector3D color)
	{
		_partColor = color / 255.0;
	}

	void mMeshPartData::setPartVisual(bool isshow)
	{
		_isShow = isshow;
	}

	void mMeshPartData::setMeshID0(const std::set<int>& ids)
	{
		//_meshIDs0 = ids;
	}

	void mMeshPartData::setMeshID1(const std::set<int>& ids)
	{
		//_meshIDs1 = ids;
	}

	void mMeshPartData::setMeshID2(const std::set<int>& ids)
	{
		//_meshIDs2 = ids;
	}

	void mMeshPartData::setMeshID3(const std::set<int>& ids)
	{
		//_meshIDs3= ids;
	}

	void mMeshPartData::setMeshLineID(const std::set<int>& ids)
	{
		_meshLineID = ids;
	}

	void mMeshPartData::setMeshFaceID(const std::set<int>& ids)
	{
		//_meshFaceID = ids;
	}

	
	void mMeshPartData::appendMesh0(shared_ptr<mMeshData> meshID)
	{
		_meshData0.push_back(meshID);
		_aabb.push(meshID->getMeshAABB());
	}

	void mMeshPartData::appendMesh1(shared_ptr<mMeshData> meshID)
	{
		_meshData1.push_back(meshID);
		_aabb.push(meshID->getMeshAABB());

	}

	void mMeshPartData::appendMesh2(shared_ptr<mMeshData>meshID)
	{
		_meshData2.push_back(meshID);
		_aabb.push(meshID->getMeshAABB());
	}

	void mMeshPartData::appendMesh3(shared_ptr<mMeshData> meshID)
	{
		_meshData3.push_back(meshID);
		_aabb.push(meshID->getMeshAABB());
	}

	void mMeshPartData::appendMeshLineID(int geoLineID)
	{
		_meshLineID.insert(geoLineID);
	}

	void mMeshPartData::appendMeshFaceID(int geoFaceID)
	{
		//_meshFaceID.insert(geoFaceID);
	}

	void mMeshPartData::deleteMeshID0(int meshID)
	{
		//_meshIDs0.erase(meshID);
	}

	void mMeshPartData::deleteMeshID1(int meshID)
	{
		//_meshIDs1.erase(meshID);
	}

	void mMeshPartData::deleteMeshID2(int meshID)
	{
		//_meshIDs2.erase(meshID);
	}

	void mMeshPartData::deleteMeshID3(int meshID)
	{
		//_meshIDs3.erase(meshID);
	}

	void mMeshPartData::deleteMeshLineID(int geoLineID)
	{
		_meshLineID.erase(geoLineID);
	}

	void mMeshPartData::deleteMeshFaceID(int geoFaceID)
	{
		//_meshFaceID.erase(geoFaceID);
	}

	std::set<int> mMeshPartData::getMeshIDs0()
	{
		std::set<int> ids;
		for (int i = 0; i < _meshData0.size(); i++)
			ids.insert(_meshData0[i]->getMeshID());
		return ids;
	}

	std::set<int> mMeshPartData::getMeshIDs1()
	{
		std::set<int> ids;
		for (int i = 0; i < _meshData1.size(); i++)
			ids.insert(_meshData1[i]->getMeshID());
		return ids;
	}

	std::set<int> mMeshPartData::getMeshIDs2()
	{
		std::set<int> ids;
		for (int i = 0; i < _meshData2.size(); i++)
			ids.insert(_meshData2[i]->getMeshID());
		return ids;
	}

	std::set<int> mMeshPartData::getMeshIDs3()
	{
		std::set<int> ids;
		for (int i = 0; i < _meshData3.size(); i++)
			ids.insert(_meshData3[i]->getMeshID());
		return ids;
	}

	QVector<shared_ptr<mMeshData>> mMeshPartData::getMeshDatas0()
	{
		return _meshData0;
	}

	QVector<shared_ptr<mMeshData>> mMeshPartData::getMeshDatas1()
	{
		return _meshData1;
	}

	QVector<shared_ptr<mMeshData>> mMeshPartData::getMeshDatas2()
	{
		return _meshData2;
	}

	QVector<shared_ptr<mMeshData>> mMeshPartData::getMeshDatas3()
	{
		return _meshData3;
	}

	QVector<shared_ptr<mMeshData>> mMeshPartData::getMeshDatas()
	{
		return _meshData0 + _meshData1 + _meshData2 + _meshData3;
	}

	int mMeshPartData::getMeshCount()
	{
		return _meshData0.size() + _meshData1.size() + _meshData2.size() + _meshData3.size();
	}

	std::set<int> mMeshPartData::getMeshLineIDs()
	{
		return _meshLineID;
	}

	std::set<int> mMeshPartData::getMeshFaceIDs()
	{	
		std::set<int> ids;
		
		//for (int i = 0; i < _meshData3.size(); i++)
		//{
		//	QVector<int> _ids = _meshData3[i]->getMeshFaces();
		//	for (int j = 0; j < _ids.size(); j++)
		//	{
		//		ids.insert(_ids[j]);
		//	}
		//}
		return ids;

	}

	std::set<shared_ptr<mMeshFaceData>> mMeshPartData::getMeshFaces()
	{
		std::set<shared_ptr<mMeshFaceData>> ids;
		//for (int i = 0; i < _meshData3.size(); i++)
		//{
		//	QVector<shared_ptr<mMeshFaceData>> _ids = _meshData3[i]->getFace();
		//	for (int j = 0; j < _ids.size(); j++)
		//	{
		//		ids.insert(_ids[j]);
		//	}
		//}
		return ids;
	}

	QVector3D mMeshPartData::getPartColor()
	{
		return _partColor;
	}

	bool mMeshPartData::getPartVisual()
	{
		return _isShow;
	}

	int mMeshPartData::getPartID()
	{
		return _partID;
	}

	QString mMeshPartData::getPartName()
	{
		return _partName;
	}
	void mMeshPartData::appendMeshFaceData(const QVector<shared_ptr<mMeshFaceData>>& face)
	{
		_faceData.append(face);
	}
	void mMeshPartData::setMeshFaceData(const QVector<shared_ptr<mMeshFaceData>> &face)
	{		
		_faceData = face;
	}

	void mMeshPartData::clearMeshFaceData()
	{
		QVector<shared_ptr<mMeshFaceData>>().swap(_faceData);
		//_faceData.clear();
	}

	QVector<shared_ptr<mMeshFaceData>> mMeshPartData::getMeshFaceData()
	{
		return _faceData;
	}
	Space::AABB mMeshPartData::getMeshPartAABB()
	{
		return _aabb;
	}
}
