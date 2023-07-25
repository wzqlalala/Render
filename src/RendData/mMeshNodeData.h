#pragma once
#include "renddata_global.h"

#include <QVector3D>
#include <QVector>

#include <set>

namespace MDataMesh
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

	class RENDDATA_EXPORT mMeshNodeData
	{

	public:
		PointCell pointcell;

		PointLine pointline;

		mMeshNodeData(int ID, QVector3D vertex);
		~mMeshNodeData();

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

		//set�����д�����������ָ��ʱ,�Զ���ȽϺ�����ʽ1:friend��ʽ����<
		friend	bool operator <(const std::shared_ptr<mMeshNodeData>& left, const std::shared_ptr<mMeshNodeData>&  right);

	private:
		//�ڵ�ID
		int _nodeID;
		//�ڵ�����
		QVector3D _nodeVertex;


	};
}
