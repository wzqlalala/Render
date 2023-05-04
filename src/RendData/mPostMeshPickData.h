#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "renddata_global.h"

//MBasicFunction
#include "mBasicStructor.h"
#include "mBasicEnum.h"

//Qt
#include <QVector>
#include <QVector4D>

namespace MDataPost
{
	using namespace std;
	using namespace MBasicFunction;
	class RENDDATA_EXPORT mPostMeshPickData
	{
	public:
		mPostMeshPickData();

		~mPostMeshPickData();

		//�������ʰȡ���Ǽ���ʰȡ
		void setMeshPickFunction(int pickfunction);//0��1��

		//ʰȡʱ����
		void setSoloPickNodeData(int nodeid, float depth);
		void setSoloPickMeshData(int meshid, float depth);
		void setSoloPickMeshFaceData(int meshfaceid, float depth);
		void setSoloPickMeshPartData(QString partName, float depth);
		void setSoloPickNodeByPartData(set<int> nodeids, float depth);
		void setSoloPickMeshByPartData(set<int> meshids, float depth);
		void setSoloPickMeshDataByAngle(int meshid, QString partName, float depth);
		void setSoloPickMeshLineDataByAngle(int meshlineid, QString partName, float depth);
		void setSoloPickMeshFaceDataByAngle(int meshfaceid, QString partName, float depth);

		void setMultiplyPickNodeData(set<int> nodeids);
		void setMultiplyPickMeshData(set<int> meshids);
		void setMultiplyPickMeshFaceData(set<int> meshfaceids);
		void setMultiplyPickMeshPartData(QString partNames);

		//һ���Ը����������һ�θ���
		void setAllPickNodeData(set<int> nodeids);
		void setAllPickMeshData(set<int> meshids);
		void setAllPickMeshFaceData(set<int> meshfaceids);
		void setAllPickMeshPartData(set<QString> partNames);
		void setAllPickAnyPosition(QVector3D vertex, float value);

		//�ۻ�����
		void setAddPickNodeData(set<int> nodeids);
		void setAddPickMeshData(set<int> meshids);
		void setAddPickMeshFaceData(set<int> meshfaceids);
		void setAddPickMeshPartData(set<QString> partNames);
		void setAddPickAnyPosition(QVector3D vertex, float value);

		//���ٸ���
		void setReducePickNodeData(set<int> nodeids);
		void setReducePickMeshData(set<int> meshids);
		void setReducePickMeshFaceData(set<int> meshfaceids);
		void setReducePickMeshPartData(set<QString> partNames);

		//��ѡ������˳��ʰȡ��ɺ����
		void setSoloPickData();

		//��ѡ����˳��ʱ��ɺ����
		void setSoloOrderPickData();

		//��ȡʰȡ��Ľڵ���
		set<int> getPickNodeIDs();

		//��ȡʰȡ��Ľڵ���(����ʰȡ˳��)
		QVector<int> getPickNodeIDsOrder();

		//��ȡʰȡ��ĵ�Ԫ���
		set<int> getPickMeshIDs();

		//��ȡʰȡ��ĵ�Ԫ����
		set<int> getPickMeshFaceIDs();

		//��ȡʰȡ������񲿼�����
		set<QString> getPickMeshPartNames();

		//��ȡ����λ�õ�ֵ
		QVector<QVector4D> getAnyPositionValue();

		//���ȫ��ʰȡ
		void clearAllPickData();

		void clearPickNodeData();
		void clearPickMeshData();
		void clearPickMeshLineData();
		void clearPickMeshFaceData();
		void clearPickMeshPartData();

		//ͨ���Ƕ�ʰȡ
		QPair<PickObjectType, QPair<QString, int>> getSoloPickNodeDataByLineAngle();//ʰȡ���Ķ��������
		QPair<PickObjectType, QPair<QString, int>> getSoloPickNodeDataByFaceAngle();//ʰȡ���Ķ��������
		QPair<QString, int> getSoloPickMeshDataByAngle();
		QPair<QString, int> getSoloPickMeshLineDataByAngle();
		QPair<QString, int> getSoloPickMeshFaceDataByAngle();

	private:

		int _pickFunction;//0��1��

		IDDepthBuffer _nodeBuffer;//��ǰ�ڵ���

		IDDepthBuffer _meshBuffer;//��ǰ��Ԫ���

		IDDepthBuffer _meshLineBuffer;//��ǰ��Ԫ�߱��

		IDDepthBuffer _meshFaceBuffer;//��ǰ��Ԫ����

		NameDepthBuffer _partBuffer;//��ǰ��������

		IDSetDepthBuffer _nodeBuffers;//������ʰȡ�ڵ�ı��

		IDSetDepthBuffer _meshFaceBuffers;//������ʰȡ��Ԫ��ı��

		IDSetDepthBuffer _meshBuffers;//������ʰȡ��Ԫ�ı��

		IDQStringDepthBuffer _meshPartNameBuffers;//���Ƕ�ʰȡ��Ԫ�ı��

		IDQStringDepthBuffer _meshLinePartNameBuffers;//���Ƕ�ʰȡ��Ԫ�ߵı��

		IDQStringDepthBuffer _meshFacePartNameBuffers;//���Ƕ�ʰȡ��Ԫ��ı��

		set<int> _pickNodes;//����ʰȡ���Ľڵ�
		QVector<int> _pickNodesOrder;//����ʰȡ���Ľڵ㣨����ʰȡ˳��
		set<int> _pickMeshs;//����ʰȡ���ĵ�Ԫ
		set<int> _pickMeshFaces;//����ʰȡ���ĵ�Ԫ��
		set<QString> _pickParts;//����ʰȡ���Ĳ���
		QVector<QVector4D> _pickAnyPosition;//���յ�����λ�õĵ�
	};
}

