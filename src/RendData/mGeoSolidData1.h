#pragma once
#include "renddata_global.h"

#include <QVector3D>
#include <QString>
#include <set>

#include "SpaceTree.h"

namespace MDataGeo
{
	class mGeoModelData1;
	class mGeoFaceData1;
	class RENDDATA_EXPORT mGeoSolidData1
	{
	public:
		mGeoSolidData1(mGeoModelData1 *geoModelData, QString partName, int ID);

		~mGeoSolidData1();

		//��Ӽ���ʵ���ϵļ�����ID
		void appendGeoSolidFace(int GeoFaceID);

		//��ȡ����ʵ���ϵ����м�����ID
		std::set<int> getGeoSolidFaceIDs();

		//��ȡ����ʵ���ID
		int getGeoSolidID();

		//��ȡ���ε����ڵĲ���
		QString getPartName();

		//���ò�������
		void setPartName(const QString &name);

		//��ȡ����ʵ�����ֵ
		Space::AABB getGeoSolidAABB() { return _aabb; };

	private:
		mGeoModelData1 *_geoModelData;

		//����ʵ���ID
		int _ID;

		//����ʵ�����ڵĲ���
		QString _partName;

		//����
		bool _isShow;

		//����ʵ���ϵ����м�����ID
		std::set<int> _geoFaceIDs;

		//����ʵ��İ�Χ��
		Space::AABB _aabb;
	};
}

