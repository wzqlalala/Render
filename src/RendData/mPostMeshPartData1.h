#pragma once
#include "renddata_global.h"

#include <QString>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QList>
#include <set>
#include "SpaceTree.h"
//MBasicFunction
#include "mBasicStructor.h"

namespace MDataPost
{
	class RENDDATA_EXPORT mPostMeshPartData1
	{
	public:
		//Space::SpaceTree *spaceTree;
		mPostMeshPartData1();

		mPostMeshPartData1(QString name,int ID);

		~mPostMeshPartData1();

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
		void appendMesh0(mPostMeshData1* meshID);

		//���1ά����
		void appendMesh1(mPostMeshData1* meshID);

		//���2ά����
		void appendMesh2(mPostMeshData1* meshID);

		//�����ά����
		void appendMesh3(mPostMeshData1* meshID);

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
		QVector<mPostMeshData1*> getMeshDatas0();

		//��ȡ���е�1ά����ָ��
		QVector<mPostMeshData1*> getMeshDatas1();

		//��ȡ���е�2ά����ָ��
		QVector<mPostMeshData1*> getMeshDatas2();

		//��ȡ���е�3ά����ָ��
		QVector<mPostMeshData1*> getMeshDatas3();

		//��ȡ�ò���������
		int getMeshCount();

		//��ȡ���е�������ID
		std::set<int> getMeshLineIDs();

		//��ȡ���е�������ID
		std::set<int> getMeshFaceIDs();

		//��ȡ���е�������ָ��
		std::set<mPostMeshFaceData1*> getMeshFaces();

		//��ȡ��������ɫ
		QVector3D getPartColor();

		//�жϲ����Ƿ���ʾ
		bool getPartVisual();

		//��ȡ�������
		int getPartID();

		//��ȡ��������
		QString getPartName();

		//��ӱ�����������
		void appendMeshFaceData(const QVector<mPostMeshFaceData1*> &face);

		//���ñ�����������
		void setMeshFaceData(const QVector<mPostMeshFaceData1*> &face);

		//��ձ�����������
		void clearMeshFaceData();

		//��ȡ������������
		QVector<mPostMeshFaceData1*> getMeshFaceData();

	private:
		//static int ID;
		static QList <QVector3D> Color;

		//��������
		QString _partName;
		int _partID;
		bool _isShow;
		QVector3D _partColor;

		//�����ĵ�Ԫ����Ϣ
		std::set<int> _meshLineID;
		
		//���е�������Ϣ
		QVector<mPostMeshData1*> _meshData3;
		QVector<mPostMeshData1*> _meshData2;
		QVector<mPostMeshData1*> _meshData1;
		QVector<mPostMeshData1*> _meshData0;

		//������������
		QVector<mPostMeshFaceData1*> _faceData;

	};
}

