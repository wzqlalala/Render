#pragma once
//�����������
#pragma execution_character_set("utf-8")
#include "renddata_global.h"

//Qt
#include <QPair>
#include <QVector>

#include <set>

//MViewBasic
#include "mBasicStructor.h"

#include "mMeshViewEnum.h"


//class mMeshData;
//class mMeshNodeData;
//class mMeshFaceData;
class MFace;
class MEdge;
namespace MDataMesh
{	
	using namespace std;
	using namespace MViewBasic;
	class mMeshPartData;
	class mMeshData;
	class mMeshNodeData;
	class mMeshLineData;
	class mMeshFaceData;
	struct MeshVertexDepthBuffer
	{
		shared_ptr<mMeshNodeData> vertex;
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
		set<shared_ptr<mMeshNodeData>> vertexs;
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
	struct MeshFaceDepthBuffer
	{
		shared_ptr<mMeshFaceData> meshface;
		float depth;
		MeshFaceDepthBuffer()
		{
			meshface = nullptr;
			depth = FLT_MAX;
		}
		void initial()
		{
			meshface = nullptr;
			depth = FLT_MAX;
		}
	};

	struct MeshFaceSetDepthBuffer
	{
		set<shared_ptr<mMeshFaceData>> meshfaces;
		float depth;
		MeshFaceSetDepthBuffer()
		{
			depth = FLT_MAX;
		}
		void initial()
		{
			meshfaces.clear();
			depth = FLT_MAX;
		}
	};

	struct MeshLineDepthBuffer
	{
		MEdge *meshline;
		float depth;
		MeshLineDepthBuffer()
		{
			meshline = nullptr;
			depth = FLT_MAX;
		}
		void initial()
		{
			meshline = nullptr;
			depth = FLT_MAX;
		}
	};

	struct MeshLineSetDepthBuffer
	{
		set<MEdge*> meshlines;
		float depth;
		MeshLineSetDepthBuffer()
		{
			depth = FLT_MAX;
		}
		void initial()
		{
			meshlines.clear();
			depth = FLT_MAX;
		}
	};

	struct MeshElementDepthBuffer
	{
		shared_ptr<mMeshData> element;
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
		set<shared_ptr<mMeshData>> elements;
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
	struct PointerQStringDepthBuffer
	{
		shared_ptr<void> ptr;
		QString partName;
		float depth;
		PointerQStringDepthBuffer()
		{
			ptr = nullptr;
			partName = "";
			depth = FLT_MAX;
		}
		void initial()
		{
			ptr = nullptr;
			partName = "";
			depth = FLT_MAX;
		}
	};
	class RENDDATA_EXPORT mPreMeshPickData1
	{
	public:

		//Ĭ�Ϲ��캯��
		mPreMeshPickData1();

		//��������
		~mPreMeshPickData1();

		//�������ʰȡ���Ǽ���ʰȡ
		void setMeshPickFunction(int pickfunction);//0��1��

		//ʰȡʱ����
		void setSoloPickNodeData(shared_ptr<mMeshNodeData> nodeid, float depth);
		void setSoloPickMeshData(shared_ptr<mMeshData> meshid, float depth);
		void setSoloPickMeshLineData(MEdge* meshlineid, float depth);
		void setSoloPickMeshFaceData(shared_ptr<mMeshFaceData> meshfaceid, float depth);
		void setSoloPickMeshPartData(QString partName, float depth);
		void setSoloPickNodeByPartData(set<shared_ptr<mMeshNodeData>> nodeids, float depth);
		void setSoloPickMeshByPartData(set<shared_ptr<mMeshData>> meshids, float depth);
		void setSoloPickMeshLineByPartData(set<MEdge*> meshLineids, float depth);
		void setSoloPickMeshFaceByPartData(set<shared_ptr<mMeshFaceData>> meshFaceids, float depth);
		void setSoloPickMeshDataByAngle(shared_ptr<mMeshData> meshid, QString partName,float depth);
		void setSoloPickMeshLineDataByAngle(shared_ptr<void> meshlineid, QString partName, float depth);
		void setSoloPickMeshFaceDataByAngle(shared_ptr<void> meshfaceid, QString partName, float depth);

		void setMultiplyPickNodeData(set<shared_ptr<mMeshNodeData>> nodeids);
		void setMultiplyPickNodeData(QVector<shared_ptr<mMeshNodeData>> nodeids);
		void setMultiplyPickMeshData(set<shared_ptr<mMeshData>> meshids);
		void setMultiplyPickMeshLineData(set<MEdge*> meshlineids);
		void setMultiplyPickMeshFaceData(set<shared_ptr<mMeshFaceData>> meshfaceids);
		void setMultiplyPickMeshPartData(QString partNames);

		//һ���Ը����������һ�θ���
		void setAllPickNodeData(set<shared_ptr<mMeshNodeData>> nodeids);
		void setAllPickMeshData(set<shared_ptr<mMeshData>> meshids);
		void setAllPickMeshLineData(set<MEdge*> meshlineids);
		void setAllPickMeshFaceData(set<shared_ptr<mMeshFaceData>> meshfaceids);
		void setAllPickMeshPartData(set<QString> partNames);

		//�ۻ�����
		void setAddPickNodeData(set<shared_ptr<mMeshNodeData>> nodeids);
		void setAddPickMeshData(set<shared_ptr<mMeshData>> meshids);
		void setAddPickMeshLineData(set<MEdge*> meshlineids);
		void setAddPickMeshFaceData(set<shared_ptr<mMeshFaceData>> meshfaceids);
		void setAddPickMeshPartData(set<QString> partNames);

		//���ٸ���
		void setReducePickNodeData(set<shared_ptr<mMeshNodeData>> nodeids);
		void setReducePickMeshData(set<shared_ptr<mMeshData>> meshids);
		void setReducePickMeshLineData(set<MEdge*> meshlineids);
		void setReducePickMeshFaceData(set<shared_ptr<mMeshFaceData>> meshfaceids);
		void setReducePickMeshPartData(set<QString> partNames);

		//��ѡ������˳��ʰȡ��ɺ����
		void setSoloPickData();

		//��ѡ����˳��ʱ��ɺ����
		void setSoloOrderPickData();

		//��ȡʰȡ��Ľڵ���
		set<shared_ptr<mMeshNodeData>> getPickNodeIDs();

		//��ȡʰȡ��Ľڵ���(����ʰȡ˳��)
		QVector<shared_ptr<mMeshNodeData>> getPickNodeIDsOrder();

		//��ȡʰȡ��ĵ�Ԫ���
		set<shared_ptr<mMeshData>> getPickMeshIDs();

		//��ȡʰȡ��ĵ�Ԫ�߱��
		set<MEdge*> getPickMeshLineIDs();

		//��ȡʰȡ��ĵ�Ԫ����
		set<shared_ptr<mMeshFaceData>> getPickMeshFaceIDs();

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
		QPair<PickObjectType, QPair<QString, shared_ptr<void>>> getSoloPickNodeDataByLineAngle();//ʰȡ���Ķ��������
		QPair<PickObjectType, QPair<QString, shared_ptr<void>>> getSoloPickNodeDataByFaceAngle();//ʰȡ���Ķ��������
		QPair<QString,shared_ptr<void>> getSoloPickMeshDataByAngle();
		QPair<QString,shared_ptr<void>> getSoloPickMeshLineDataByAngle();
		QPair<QString,shared_ptr<void>> getSoloPickMeshFaceDataByAngle();

	private:

		int _pickFunction;//0��1��

		MeshVertexDepthBuffer _nodeBuffer;//��ǰ�ڵ���

		MeshElementDepthBuffer _meshBuffer;//��ǰ��Ԫ���

		MeshLineDepthBuffer _meshLineBuffer;//��ǰ��Ԫ�߱��

		MeshFaceDepthBuffer _meshFaceBuffer;//��ǰ��Ԫ����

		NameDepthBuffer _partBuffer;//��ǰ��������

		MeshVertexSetDepthBuffer _nodeBuffers;//������ʰȡ�ڵ�ı��

		MeshElementSetDepthBuffer _meshBuffers;//������ʰȡ��Ԫ�ı��

		MeshLineSetDepthBuffer _meshLineBuffers;//������ʰȡ��Ԫ�ߵı��

		MeshFaceSetDepthBuffer _meshFaceBuffers;//������ʰȡ��Ԫ��ı��

		PointerQStringDepthBuffer _meshPartNameBuffers;//���Ƕ�ʰȡ��Ԫ�ı��

		PointerQStringDepthBuffer _meshLinePartNameBuffers;//���Ƕ�ʰȡ��Ԫ�ߵı��

		PointerQStringDepthBuffer _meshFacePartNameBuffers;//���Ƕ�ʰȡ��Ԫ��ı��

		//IDQStringDepthBuffer _meshPartNameBuffers;//���Ƕ�ʰȡ��Ԫ�ı��

		set<shared_ptr<mMeshNodeData>> _pickNodes;//����ʰȡ���Ľڵ�
		QVector<shared_ptr<mMeshNodeData>> _pickNodesOrder;//����ʰȡ���Ľڵ㣨����ʰȡ˳��
		set<shared_ptr<mMeshData>> _pickMeshs;//����ʰȡ���ĵ�Ԫ
		set<MEdge*> _pickMeshLines;//����ʰȡ���ĵ�Ԫ��
		set<shared_ptr<mMeshFaceData>> _pickMeshFaces;//����ʰȡ���ĵ�Ԫ��
		set<QString> _pickParts;//����ʰȡ���Ĳ���

	};
}

