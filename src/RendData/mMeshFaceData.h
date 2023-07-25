#pragma once
#include "renddata_global.h"

#include <QString>
#include <QSet>
#include <QPair>
#include <QVector3D>

#include <memory>

using namespace std;
namespace MDataMesh
{
	class mMeshData;
	//class mMeshModelData;
	class mMeshNodeData;
	class RENDDATA_EXPORT mMeshFaceData
	{

	public:
		//���ƹ��캯��
		mMeshFaceData(QVector<int> index, std::shared_ptr<mMeshData> meshID);

		//��������
		~mMeshFaceData();

		//��ӵڶ�������ID
		void appendMeshID(std::shared_ptr<mMeshData> meshID, int startIndex);

		//��ȡ�ڶ���������Ľڵ�
		QVector<shared_ptr<mMeshNodeData>> getNodeData();

		//��ȡ��Ӧ�����������Ľڵ�
		QVector<shared_ptr<mMeshNodeData>> getNodeData(int meshID);

		//��ȡ�ڶ���������Ľڵ�
		QVector<QVector3D> getNodeVertex();

		//��ȡ��Ӧ�����������Ľڵ�
		QVector<QVector3D> getNodeVertex(int meshID);

		//��ȡ��һ������
		QPair<int, int> getFirstMesh();

		//��ȡ���ڵĲ�������
		QString getPartName();

		//��ȡ����
		std::shared_ptr<mMeshData> getMeshID();

		//��ȡ����ID1
		std::shared_ptr<mMeshData> getMeshID1();

		//��ȡ���1
		short int getMeshOrder1();

		//��ȡ����ID2
		std::shared_ptr<mMeshData> getMeshID2();

		//��ȡ���2
		short int getMeshOrder2();

		//ɾ����Ԫ���ã�Ȼ���ж��Ƿ�ɾ����Ԫ��
		bool deleteMesh(int meshID);

		//��ȡ�Ƿ���Ⱦ
		bool getVisual();

		//�����Ƿ���Ⱦ
		void setVisual(bool isShow);

		//��ȡ�Ƿ���������
		bool getMeshFaceIsTri() { return _index.size() == 3 ? true : false; };

	private:
		//��Ԫ���ϵĽڵ�����
		QVector<shared_ptr<mMeshNodeData>> _index;

		//��һ������ID
		std::shared_ptr<mMeshData> _meshID1;
		//�ڶ�������ID
		std::shared_ptr<mMeshData> _meshID2;

		//�ڶ�������ID����ʼ����λ��
		uint8_t _startIndex;
		//�Ƿ���Ⱦ
		bool _isShow;
	};
}
