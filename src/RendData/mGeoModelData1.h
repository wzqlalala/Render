#pragma once
#include "renddata_global.h"

#include <QHash>
#include <set>
#include <QVector3D>

#include "mBasicEnum.h"
#include "mBasicStructor.h"

#include "SpaceTree.h"

using namespace std;
using namespace MBasicFunction;
namespace MDataGeo
{
	class mGeoPartData1;
	class mGeoPointData1;
	class mGeoLineData1;
	class mGeoFaceData1;
	class mGeoSolidData1;
	class RENDDATA_EXPORT mGeoModelData1
	{
	public:
		//Ĭ�Ϲ��캯��
		mGeoModelData1();

		//��������
		~mGeoModelData1();

		/*
		 * ģ�Ͳ���
		 */

		//��ȡ���еļ��ε�(��ȥ���صļ��β���)
		set<int> getAllGeoPointIDs();
		//��ȡһ�����������м��ε�(��ȥ���صļ��β���)
		set<int> getAllGeoPointIDsInPart(QString partName);

		//��ȡ���еļ�����(��ȥ���صļ��β���)
		set<int> getAllGeoLineIDs();
		//��ȡһ�����������м�����(��ȥ���صļ��β���)
		set<int> getAllGeoLineIDsInPart(QString partName);

		//��ȡ���еļ�����(��ȥ���صļ��β���)
		set<int> getAllGeoFaceIDs();
		//��ȡһ�����������м�����(��ȥ���صļ��β���)
		set<int> getAllGeoFaceIDsInPart(QString partName);

		//��ȡ���еļ�����(��ȥ���صļ��β���)
		set<int> getAllGeoSolidIDs();
		//��ȡһ�����������м�����(��ȥ���صļ��β���)
		set<int> getAllGeoSolidIDsInPart(QString partName);

		//��ȡȫ�������(��ȥ���صļ��β���)
		set<QString> getAllGeoPartNames();

		//ͨ�����ε��ȡ�������
		QString getPartNameByGeoPointID(int PointID);
		//ͨ�������߻�ȡ�������
		QString getPartNameByGeoLineID(int LineID);
		//ͨ���������ȡ�������
		QString getPartNameByGeoFaceID(int FaceID);
		//ͨ������ʵ���ȡ�������
		QString getPartNameByGeoSolidID(int SolidID);

		 //ͨ��ID(����)��ȡ��Ӧ������
		mGeoPartData1 *getGeoPartDataByPartName(QString partName);
		mGeoSolidData1 *getGeoSolidDataByID(int ID);
		mGeoFaceData1 *getGeoFaceDataByID(int ID);
		mGeoLineData1* getGeoLineDataByID(int ID);
		mGeoPointData1* getGeoPointDataByID(int ID);

		//ͨ��ID(����)��ȡ��Ӧ�İ�Χ��
		Space::AABB getGeoPartAABBByPartName(QString partName);
		Space::AABB getGeoSolidAABBByID(int ID);
		Space::AABB getGeoFaceAABBByID(int ID);
		Space::AABB getGeoLineAABBByID(int ID);
		Space::AABB getGeoPointAABBByID(int ID);

		//��ȡ��Ӧ�������е�ID(����)
		QList<QString> getAllPartName();
		QList<int> getAllGeoSolidID();
		QList<int> getAllGeoFaceID();
		QList<int> getAllGeoLineID();
		QList<int> getAllGeoPointID();

		//��ȡģ�͵����в���������
		QHashIterator<QString, mGeoPartData1*> getPartIterator();

		////����ģ�͵����뾶������
		//void setModelRadiusAndCenter(float modelRadius, QVector3D modelCenter);

		//��ȡģ�͵ĳߴ�
		Space::AABB getModelSize();

		//�ж��Ƿ����ģ��
		bool isExistModel();

		//�ж��Ƿ���ģ����ʾ
		bool isShowModel();

		/*
		 * ��������
		 */

		//���ļ�������ݣ����Զ�������Ⱦ
		void importGeoPartData(QString partName, mGeoPartData1 *geoPartData);

		//��������ʱ�滻����
		void replaceGeoPartData(QString partName, mGeoPartData1* geoPartData);

		//���������β���
		void renameGeoPart(QString newName, QString oldName);

		//��Ӽ�������
		void appendGeoPartData(QString partName, mGeoPartData1 *geoPartData);
		void appendGeoSolidData(int ID, mGeoSolidData1 *data);
		void appendGeoFaceData(int ID, mGeoFaceData1 *data);
		void appendGeoLineData(int ID, mGeoLineData1 *data);
		void appendGeoPointData(int ID, mGeoPointData1 *data);

		//���õ������β���������
		void setGeoPartVisual(QString partName,bool isShow);

		//���õ������β�������ɫ
		void setGeoPartColor(QString partName, QVector3D color);

		//ɾ���������β���
		void deleteGeoPart(QString partName);

		//����ȫ�����β���������
		void setAllGeoPartVisual(bool isShow);

		//����ȫ�����β�������ɫ
		void setAllGeoPartColor(QVector3D color);

		//ɾ��ȫ�����β���
		void deleteAllGeoPart();

		//��ȡ���β�������ɫ
		QVector3D getGeoPartColor(QString partName);

		//��ȡ�������β���������
		bool getGeoPartVisual(QString partName);
		/*
		 * �ж��Ƿ��иò���ͨ������
		 */
		bool isExistGeoPart(QString partName);

		//�����߲���
		void setGeoLineProperty(set<int> geoLineIDs, GeoLineProperty geoLineProperty);
	private:

		//���㼸�����ĺͰ뾶
		void caculateGeoModelRadiusAndCenter();
	public:

		//ģ�Ͳ���ö��
		QList<QPair<MBasicFunction::ModelOperateEnum, std::set<QString>>> _modelOperate;

	private:
		QHash<int, mGeoPointData1*> _geoPointData;
		QHash<int, mGeoLineData1*> _geoLineData;
		QHash<int, mGeoFaceData1*> _geoFaceData;
		QHash<int, mGeoSolidData1*> _geoSolidData;
		QHash<QString, mGeoPartData1*> _geoPartData;

		//ģ������
		//QVector3D _modelCenter;

		//ģ�Ͱ뾶
		//float _modelRadius;

		//ģ�͵ĳߴ�
		Space::AABB _modelSize;

		//�Ƿ��ģ�ͽ���ɾ���������,�������¼��㼸��ģ�͵İ뾶������
		bool _isAddOrDeletePart;
	};
}

