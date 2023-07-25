#include "mGeoFaceData1.h"
#include "mGeoModelData1.h"

//MViewBasic
#include "mGlobalVarible.h"

using namespace MViewBasic;
namespace MDataGeo
{
	mGeoFaceData1::mGeoFaceData1(mGeoModelData1 *geoModelData, QString partName, int ID)
	{
		_geoModelData = geoModelData;
		_isShow = true;
		_partName = partName;
		_ID = ID;
		_aabb = Space::AABB();
		geoModelData->appendGeoFaceData(ID, this);
		_globalFaceId = std::max(_globalFaceId, ID);
	}

	mGeoFaceData1::~mGeoFaceData1()
	{

	}

	void mGeoFaceData1::appendGeoFaceData(int ID, QVector<QVector3D> vertex, QVector<QVector3D> normal)
	{
		_ID = ID;
		_geoFaceVertex = vertex;
		_aabb.push(vertex);
		//_geoFaceNormal = normal;
	}
	QVector<QVector3D> mGeoFaceData1::getGeoFaceVertex()
	{
		return _geoFaceVertex;
	}
	//QVector<QVector3D> mGeoFaceData1::getGeoFaceNormal()
	//{
	//	//return _geoFaceNormal;
	//}
	int mGeoFaceData1::getGeoFaceID()
	{
		return _ID;
	}

	QString mGeoFaceData1::getPartName()
	{
		return _partName;
	}

	void mGeoFaceData1::setPartName(const QString & name)
	{
		_partName = name;
	}
}
