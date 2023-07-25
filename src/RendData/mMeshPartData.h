#pragma once
#include "renddata_global.h"

#include <QString>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QList>
#include <set>
#include "SpaceTree.h"
//MViewBasic
#include "mBasicStructor.h"

#include <memory>

using namespace std;
namespace MDataMesh
{
	class mMeshData;
	class mMeshFaceData;
	class mMeshModelData;
	class RENDDATA_EXPORT mMeshPartData
	{
	public:
		//Space::SpaceTree *spaceTree;
		mMeshPartData();

		mMeshPartData(QString name,int ID);

		~mMeshPartData();

		//���ò�����ɫ
		void setPartColor(QVector3D color);

		//���ò�������
		void setPartVisual(bool isshow);

		//����0ά����ID
		void setMeshID0(const std::set<int>& ids);

		//����һά����ID
		void setMeshID1(const std::set<int>& ids);

		//���ö�ά����ID
		void setMeshID2(const std::set<int>& ids);

		//������ά����ID
		void setMeshID3(const std::set<int>& ids);


		//����������ID
		void setMeshLineID(const std::set<int>& ids);

		//����������ID
		void setMeshFaceID(const std::set<int>& ids);

		//���0ά����
		void appendMesh0(shared_ptr<mMeshData> meshID);

		//���1ά����
		void appendMesh1(shared_ptr<mMeshData> meshID);

		//���2ά����
		void appendMesh2(shared_ptr<mMeshData> meshID);

		//�����ά����
		void appendMesh3(shared_ptr<mMeshData> meshID);

		//���������ID
		void appendMeshLineID(int meshLineID);

		//��Ӽ�����ID
		void appendMeshFaceID(int meshFaceID);

		//ɾ��0ά����ID
		void deleteMeshID0(int meshID);

		//ɾ��1ά����ID
		void deleteMeshID1(int meshID);

		//ɾ��2ά����ID
		void deleteMeshID2(int meshID);

		//ɾ��3ά����ID
		void deleteMeshID3(int meshID);

		//ɾ��������ID
		void deleteMeshLineID(int meshLineID);

		//ɾ��������ID
		void deleteMeshFaceID(int meshFaceID);

		//��ȡ���е�0ά����ID
		std::set<int> getMeshIDs0();

		//��ȡ���е�1ά����ID
		std::set<int> getMeshIDs1();

		//��ȡ���е�2ά����ID
		std::set<int> getMeshIDs2();

		//��ȡ���е�3ά����ID
		std::set<int> getMeshIDs3();

		//��ȡ���е�0ά����ָ��
		QVector<shared_ptr<mMeshData>> getMeshDatas0();

		//��ȡ���е�1ά����ָ��
		QVector<shared_ptr<mMeshData>> getMeshDatas1();

		//��ȡ���е�2ά����ָ��
		QVector<shared_ptr<mMeshData>> getMeshDatas2();

		//��ȡ���е�3ά����ָ��
		QVector<shared_ptr<mMeshData>> getMeshDatas3();

		//��ȡ���е�����ָ��
		QVector<shared_ptr<mMeshData>> getMeshDatas();

		//��ȡ�ò���������
		int getMeshCount();

		//��ȡ���е�������ID
		std::set<int> getMeshLineIDs();

		//��ȡ���е�������ID
		std::set<int> getMeshFaceIDs();

		//��ȡ���е�������ָ��
		std::set<shared_ptr<mMeshFaceData>> getMeshFaces();

		//��ȡ��������ɫ
		QVector3D getPartColor();

		//�жϲ����Ƿ���ʾ
		bool getPartVisual();

		//��ȡ�������
		int getPartID();

		//��ȡ��������
		QString getPartName();

		//��ӱ�����������
		void appendMeshFaceData(const QVector<shared_ptr<mMeshFaceData>> &face);

		//���ñ�����������
		void setMeshFaceData(const QVector<shared_ptr<mMeshFaceData>> &face);

		//��ձ�����������
		void clearMeshFaceData();

		//��ȡ������������
		QVector<shared_ptr<mMeshFaceData>> getMeshFaceData();

		//��ȡģ�����ݹ���ָ��
		shared_ptr<mMeshModelData> getMeshModelData() {return _modelData;};

		//��ȡ������AABB
		Space::AABB getMeshPartAABB();

	private:
		//static int ID;
		static QList<QVector3D> Color;

		//��������
		QString _partName;
		int _partID;
		bool _isShow;
		QVector3D _partColor;

		//�����ĵ�Ԫ����Ϣ
		std::set<int> _meshLineID;
		
		//���е�������Ϣ
		QVector<shared_ptr<mMeshData>> _meshData3;
		QVector<shared_ptr<mMeshData>> _meshData2;
		QVector<shared_ptr<mMeshData>> _meshData1;
		QVector<shared_ptr<mMeshData>> _meshData0;

		//������������
		QVector<shared_ptr<mMeshFaceData>> _faceData;

		//ģ�����ݹ���
		shared_ptr<mMeshModelData> _modelData;

		//�����ļ��γߴ�
		Space::AABB _aabb;

	};
}

