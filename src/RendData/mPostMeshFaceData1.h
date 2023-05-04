#pragma once
#include "renddata_global.h"

#include <QString>
#include <QSet>
#include <QPair>

namespace MDataPost
{
	class RENDDATA_EXPORT mPostMeshFaceData1
	{

	public:
		//���ƹ��캯��
		mPostMeshFaceData1(int ID, QVector<int> index, int meshID, int order, QString partName);

		//��������
		~mPostMeshFaceData1();

		//��ӵڶ�������ID
		void appendMeshID(int meshID, int order, int startIndex);

		//��ȡ�ڶ���������Ľڵ�����
		QVector<int> getNodeIndex();

		//��ȡ��Ӧ�����������Ľڵ�����
		QVector<int> getNodeIndex(int meshID);

		//��ȡ��һ������
		QPair<int, int> getFirstMesh();

		//��ȡ���ڵĲ�������
		QString getPartName();

		//��ȡ����ID1
		int getMeshID1();

		//��ȡ���1
		short int getMeshOrder1();

		//��ȡ����ID2
		int getMeshID2();

		//��ȡ���2
		short int getMeshOrder2();

		//��ȡ��Ԫ���ID
		int getMeshFaceID();

		//ɾ����Ԫ���ã�Ȼ���ж��Ƿ�ɾ����Ԫ��
		bool deleteMesh(int meshID);

		//��ȡ�Ƿ���Ⱦ
		bool getVisual();

		//�����Ƿ���Ⱦ
		void setVisual(bool isShow);

	private:
		//��Ԫ��ID
		int _ID;
		//��Ԫ���ϵĽڵ�����
		int _index[4] = { -1 };

		//��һ������ID
		int _meshID1;	
		//�ڶ�������ID
		int _meshID2;

		//�ڶ�������ID����ʼ����λ��
		uint8_t _startIndex;
		
		uint8_t _order1;
		uint8_t _order2;
		//�Ƿ���Ⱦ
		bool _isShow;
	};
}
