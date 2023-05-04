#pragma once
#include "renddata_global.h"

#include <QVector3D>
#include <QVector>
#include <QString>

#include "SpaceTree.h"

namespace MDataGeo
{
	class mGeoModelData1;
	class RENDDATA_EXPORT mGeoFaceData1
	{
	public:
		mGeoFaceData1(mGeoModelData1 *geoModelData, QString partName,int ID);

		~mGeoFaceData1();

		//���ü���������
		void appendGeoFaceData(int ID, QVector<QVector3D> vertex, QVector<QVector3D> normal = QVector<QVector3D>{});

		//��ȡ���������������
		QVector<QVector3D> getGeoFaceVertex();

		//��ȡ����������з�����
		//QVector<QVector3D> getGeoFaceNormal();

		//��ȡ�������ID
		int getGeoFaceID();

		//��ȡ���������ڵĲ���
		QString getPartName();

		//���ò�������
		void setPartName(const QString &name);

		//��ȡ���������ֵ
		Space::AABB getGeoFaceAABB() { return _aabb; };

	private:
		mGeoModelData1 *_geoModelData;

		int _ID;

		//���������ڵĲ���
		QString _partName;

		bool _isShow;

		QVector<QVector3D> _geoFaceVertex;
		//QVector<QVector3D> _geoFaceNormal;

		//������İ�Χ��
		Space::AABB _aabb;
	};
}

