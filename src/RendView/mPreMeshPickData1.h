#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

//Qt
#include <QPair>
#include <QVector>

//MBasicFunction
#include "mBasicStructor.h"
#include "mBasicEnum.h"
#include "mMeshViewEnum.h"


class MXMeshElement;
class MXMeshVertex;
namespace MPreRend
{	
	using namespace std;
	using namespace MxFunctions;
	using namespace MViewBasic;
	struct MeshVertexDepthBuffer
	{
		MXMeshVertex *vertex;
		float depth;
		MeshVertexDepthBuffer()
		{
			vertex = nullptr;
			depth = FLT_MAX;
		}
		void initial()
		{
			vertex = nullptr;
			depth = FLT_MAX;
		}
	};

	struct MeshVertexSetDepthBuffer
	{
		set<MXMeshVertex*> vertexs;
		float depth;
		MeshVertexSetDepthBuffer()
		{
			depth = FLT_MAX;
		}
		void initial()
		{
			vertexs.clear();
			depth = FLT_MAX;
		}
	};
	struct MeshElementDepthBuffer
	{
		MXMeshElement *element;
		float depth;
		MeshElementDepthBuffer()
		{
			element = nullptr;
			depth = FLT_MAX;
		}
		void initial()
		{
			element = nullptr;
			depth = FLT_MAX;
		}
	};

	struct MeshElementSetDepthBuffer
	{
		set<MXMeshElement*> elements;
		float depth;
		MeshElementSetDepthBuffer()
		{
			depth = FLT_MAX;
		}
		void initial()
		{
			elements.clear();
			depth = FLT_MAX;
		}
	};
	class RENDVIEW_EXPORT mPreMeshPickData1
	{
	public:

		//��ȡ����
		static mPreMeshPickData1* getInstance();

		//�������ʰȡ���Ǽ���ʰȡ
		void setMeshPickFunction(int pickfunction);//0��1��

		//ʰȡʱ����
		void setSoloPickNodeData(MXMeshVertex* nodeid, float depth);
		void setSoloPickMeshData(MXMeshElement* meshid, float depth);
		void setSoloPickMeshLineData(int meshlineid, float depth);
		void setSoloPickMeshFaceData(int meshfaceid, float depth);
		void setSoloPickMeshPartData(QString partName, float depth);
		void setSoloPickNodeByPartData(set<MXMeshVertex*> nodeids, float depth);
		void setSoloPickMeshByPartData(set<MXMeshElement*> meshids, float depth);
		void setSoloPickMeshLineByPartData(set<int> meshLineids, float depth);
		void setSoloPickMeshFaceByPartData(set<int> meshFaceids, float depth);
		void setSoloPickMeshDataByAngle(MXMeshElement* meshid, QString partName,float depth);
		void setSoloPickMeshLineDataByAngle(int meshlineid, QString partName, float depth);
		void setSoloPickMeshFaceDataByAngle(int meshfaceid, QString partName, float depth);

		void setMultiplyPickNodeData(set<MXMeshVertex*> nodeids);
		void setMultiplyPickNodeData(QVector<MXMeshVertex*> nodeids);
		void setMultiplyPickMeshData(set<MXMeshElement*> meshids);
		void setMultiplyPickMeshLineData(set<int> meshlineids);
		void setMultiplyPickMeshFaceData(set<int> meshfaceids);
		void setMultiplyPickMeshPartData(QString partNames);

		//һ���Ը����������һ�θ���
		void setAllPickNodeData(set<MXMeshVertex*> nodeids);
		void setAllPickMeshData(set<MXMeshElement*> meshids);
		void setAllPickMeshLineData(set<int> meshlineids);
		void setAllPickMeshFaceData(set<int> meshfaceids);
		void setAllPickMeshPartData(set<QString> partNames);

		//�ۻ�����
		void setAddPickNodeData(set<MXMeshVertex*> nodeids);
		void setAddPickMeshData(set<MXMeshElement*> meshids);
		void setAddPickMeshLineData(set<int> meshlineids);
		void setAddPickMeshFaceData(set<int> meshfaceids);
		void setAddPickMeshPartData(set<QString> partNames);

		//���ٸ���
		void setReducePickNodeData(set<MXMeshVertex*> nodeids);
		void setReducePickMeshData(set<MXMeshElement*> meshids);
		void setReducePickMeshLineData(set<int> meshlineids);
		void setReducePickMeshFaceData(set<int> meshfaceids);
		void setReducePickMeshPartData(set<QString> partNames);

		//��ѡ������˳��ʰȡ��ɺ����
		void setSoloPickData();

		//��ѡ����˳��ʱ��ɺ����
		void setSoloOrderPickData();

		//��ȡʰȡ��Ľڵ���
		set<MXMeshVertex*> getPickNodeIDs();

		//��ȡʰȡ��Ľڵ���(����ʰȡ˳��)
		QVector<MXMeshVertex*> getPickNodeIDsOrder();

		//��ȡʰȡ��ĵ�Ԫ���
		set<MXMeshElement*> getPickMeshIDs();

		//��ȡʰȡ��ĵ�Ԫ�߱��
		set<int> getPickMeshLineIDs();

		//��ȡʰȡ��ĵ�Ԫ����
		set<int> getPickMeshFaceIDs();

		//��ȡʰȡ������񲿼�����
		set<QString> getPickMeshPartNames();

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

		//Ĭ�Ϲ��캯��
		mPreMeshPickData1();

		//��������
		~mPreMeshPickData1();

	private:
		//����ָ��
		static mPreMeshPickData1 *_instance;

		int _pickFunction;//0��1��

		MeshVertexDepthBuffer _nodeBuffer;//��ǰ�ڵ���

		MeshElementDepthBuffer _meshBuffer;//��ǰ��Ԫ���

		IDDepthBuffer _meshLineBuffer;//��ǰ��Ԫ�߱��

		IDDepthBuffer _meshFaceBuffer;//��ǰ��Ԫ����

		NameDepthBuffer _partBuffer;//��ǰ��������

		MeshVertexSetDepthBuffer _nodeBuffers;//������ʰȡ�ڵ�ı��

		MeshElementSetDepthBuffer _meshBuffers;//������ʰȡ��Ԫ�ı��

		IDSetDepthBuffer _meshLineBuffers;//������ʰȡ��Ԫ�ߵı��

		IDSetDepthBuffer _meshFaceBuffers;//������ʰȡ��Ԫ��ı��

		IDQStringDepthBuffer _meshPartNameBuffers;//���Ƕ�ʰȡ��Ԫ�ı��

		IDQStringDepthBuffer _meshLinePartNameBuffers;//���Ƕ�ʰȡ��Ԫ�ߵı��

		IDQStringDepthBuffer _meshFacePartNameBuffers;//���Ƕ�ʰȡ��Ԫ��ı��

		//IDQStringDepthBuffer _meshPartNameBuffers;//���Ƕ�ʰȡ��Ԫ�ı��

		set<MXMeshVertex*> _pickNodes;//����ʰȡ���Ľڵ�
		QVector<MXMeshVertex*> _pickNodesOrder;//����ʰȡ���Ľڵ㣨����ʰȡ˳��
		set<MXMeshElement*> _pickMeshs;//����ʰȡ���ĵ�Ԫ
		set<int> _pickMeshLines;//����ʰȡ���ĵ�Ԫ��
		set<int> _pickMeshFaces;//����ʰȡ���ĵ�Ԫ��
		set<QString> _pickParts;//����ʰȡ���Ĳ���

	};
}

