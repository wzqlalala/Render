#pragma once
#include "renddata_global.h"

#include <QVector3D>
#include <QVector>
#include <QString>

#include "SpaceTree.h"

#include "mBasicEnum.h"

using namespace MBasicFunction;
namespace MDataGeo
{
	class mGeoModelData1;
	class RENDDATA_EXPORT mGeoLineData1
	{
	public:
		mGeoLineData1(mGeoModelData1 *geoModelData, QString partName, int ID);

		~mGeoLineData1();

		//���ü����ߵ�����
		void appendGeoLineData(int ID, QVector<QVector3D> vertex);

		//���ü����ߵ�����
		void setGeoLineProperty(GeoLineProperty lineProperty);

		//��ȡ�����ߵ���������
		QVector<QVector3D> getGeoLineVertex();

		//��ȡ�����ߵ�����
		GeoLineProperty getGeoLineProperty();

		//��ȡ�����ߵ�ID
		int getGeoLineID();

		//��ȡ���ε����ڵĲ���
		QString getPartName();

		//���ò�������
		void setPartName(const QString &name);

		//��ȡ�����ߵ���ֵ
		Space::AABB getGeoLineAABB() { return _aabb; };
	private:
		mGeoModelData1 *_geoModelData;

		int _ID;

		//���������ڵĲ���
		QString _partName;

		//����
		bool _isShow;

		//�����ߵ�����
		GeoLineProperty _lineProperty;

		//�����ߵ�����
		QVector<QVector3D> _geoLineVertex;

		//�����ߵİ�Χ��
		Space::AABB _aabb;
	};
}

