#pragma once
#include "renddata_global.h"

#include <QVector3D>
#include <QVector>

#include <set>

namespace MDataPost
{

	struct PointCell
	{
		int addr = 0;
		int size = 0;
	};

	struct PointLine
	{
		int addr = 0;
		int size = 0;
	};

	class RENDDATA_EXPORT mPostMeshNodeData1
	{

	public:
		PointCell pointcell;

		PointLine pointline;

		mPostMeshNodeData1(int ID, QVector3D vertex);
		~mPostMeshNodeData1();

		//��������
		//void addMesh(int meshID);

		//��������
		bool deleteMesh(int meshID);

		//��ȡ�ڵ�ID
		int getNodeID();

		//��ȡ�ڵ�����
		QVector3D getNodeVertex();

		//���ýڵ�����
		void setNodeVertex(int position, float value);

		//�洢�ڵ����ڵ�����ID
		//void setNodeInMeshIDs(const std::set<int>& ids);

		//��ȡ�ڵ����ڵ�����ID
		std::set<int> getNodeInMeshIDs();

		//��ȡ�ڵ����ڵĵ�Ԫ��ID
		std::set<int> getNodeInMeshLineIDs();

		//�жϸýڵ��ǲ������ɽڵ�
		bool getNodeIsFreeNode();

	private:
		//�ڵ�ID
		int _nodeID;
		//�ڵ�����
		QVector3D _nodeVertex;


	};
}
