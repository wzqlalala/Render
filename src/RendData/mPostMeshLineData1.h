#pragma once
#include "renddata_global.h"

#include <QString>
#include <set>

namespace MDataPost
{
	class RENDDATA_EXPORT mPostMeshLineData1
	{

	public:
		mPostMeshLineData1(int ID, std::set<int> index, int meshID, int order, QString partName);

		~mPostMeshLineData1();

		//��ȡ��Ԫ�ߵĵ�һ���ڵ������
		int getMeshLineNodeIndex1();

		//��ȡ��Ԫ�ߵĵڶ����ڵ������
		int getMeshLineNodeIndex2();

		//��ȡ��Ԫ�ߵ�ID
		int getMeshLineID();

		//��ȡ��Ԫ�����ڵĲ���������
		QString getPartName();

		//��ȡ��Ԫ�����ڵĵ�ԪID
		int getMeshID();

		//��ȡ���
		short int getMeshOrder();
	private:
		//��Ԫ��ID
		int _ID;

		//�ڵ�����1
		int _index1;

		//�ڵ�����2
		int _index2;

		//��Ԫ�����ڵ�����ID1
		int _meshID1;
		short _order;

		//��Ԫ�����ڵĲ�������
		QString _partName;
	};
}
