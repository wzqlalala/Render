#pragma once
//解决中文乱码
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

		//默认构造函数
		mPreMeshPickData1();

		//析构函数
		~mPreMeshPickData1();

		//设置添加拾取还是减少拾取
		void setMeshPickFunction(int pickfunction);//0加1减

		//拾取时调用
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

		//一次性高亮，清除上一次高亮
		void setAllPickNodeData(set<shared_ptr<mMeshNodeData>> nodeids);
		void setAllPickMeshData(set<shared_ptr<mMeshData>> meshids);
		void setAllPickMeshLineData(set<MEdge*> meshlineids);
		void setAllPickMeshFaceData(set<shared_ptr<mMeshFaceData>> meshfaceids);
		void setAllPickMeshPartData(set<QString> partNames);

		//累积高亮
		void setAddPickNodeData(set<shared_ptr<mMeshNodeData>> nodeids);
		void setAddPickMeshData(set<shared_ptr<mMeshData>> meshids);
		void setAddPickMeshLineData(set<MEdge*> meshlineids);
		void setAddPickMeshFaceData(set<shared_ptr<mMeshFaceData>> meshfaceids);
		void setAddPickMeshPartData(set<QString> partNames);

		//减少高亮
		void setReducePickNodeData(set<shared_ptr<mMeshNodeData>> nodeids);
		void setReducePickMeshData(set<shared_ptr<mMeshData>> meshids);
		void setReducePickMeshLineData(set<MEdge*> meshlineids);
		void setReducePickMeshFaceData(set<shared_ptr<mMeshFaceData>> meshfaceids);
		void setReducePickMeshPartData(set<QString> partNames);

		//单选不保留顺序拾取完成后调用
		void setSoloPickData();

		//单选保留顺序时完成后调用
		void setSoloOrderPickData();

		//获取拾取后的节点编号
		set<shared_ptr<mMeshNodeData>> getPickNodeIDs();

		//获取拾取后的节点编号(保留拾取顺序)
		QVector<shared_ptr<mMeshNodeData>> getPickNodeIDsOrder();

		//获取拾取后的单元编号
		set<shared_ptr<mMeshData>> getPickMeshIDs();

		//获取拾取后的单元线编号
		set<MEdge*> getPickMeshLineIDs();

		//获取拾取后的单元面编号
		set<shared_ptr<mMeshFaceData>> getPickMeshFaceIDs();

		//获取拾取后的网格部件名称
		set<QString> getPickMeshPartNames();

		//清空全部拾取
		void clearAllPickData();


		void clearPickNodeData();
		void clearPickMeshData();
		void clearPickMeshLineData();
		void clearPickMeshFaceData();
		void clearPickMeshPartData();

		//通过角度拾取
		QPair<PickObjectType, QPair<QString, shared_ptr<void>>> getSoloPickNodeDataByLineAngle();//拾取到的对象和其编号
		QPair<PickObjectType, QPair<QString, shared_ptr<void>>> getSoloPickNodeDataByFaceAngle();//拾取到的对象和其编号
		QPair<QString,shared_ptr<void>> getSoloPickMeshDataByAngle();
		QPair<QString,shared_ptr<void>> getSoloPickMeshLineDataByAngle();
		QPair<QString,shared_ptr<void>> getSoloPickMeshFaceDataByAngle();

	private:

		int _pickFunction;//0加1减

		MeshVertexDepthBuffer _nodeBuffer;//当前节点编号

		MeshElementDepthBuffer _meshBuffer;//当前单元编号

		MeshLineDepthBuffer _meshLineBuffer;//当前单元线编号

		MeshFaceDepthBuffer _meshFaceBuffer;//当前单元面编号

		NameDepthBuffer _partBuffer;//当前部件名称

		MeshVertexSetDepthBuffer _nodeBuffers;//按部件拾取节点的编号

		MeshElementSetDepthBuffer _meshBuffers;//按部件拾取单元的编号

		MeshLineSetDepthBuffer _meshLineBuffers;//按部件拾取单元线的编号

		MeshFaceSetDepthBuffer _meshFaceBuffers;//按部件拾取单元面的编号

		PointerQStringDepthBuffer _meshPartNameBuffers;//按角度拾取单元的编号

		PointerQStringDepthBuffer _meshLinePartNameBuffers;//按角度拾取单元线的编号

		PointerQStringDepthBuffer _meshFacePartNameBuffers;//按角度拾取单元面的编号

		//IDQStringDepthBuffer _meshPartNameBuffers;//按角度拾取单元的编号

		set<shared_ptr<mMeshNodeData>> _pickNodes;//最终拾取到的节点
		QVector<shared_ptr<mMeshNodeData>> _pickNodesOrder;//最终拾取到的节点（保留拾取顺序）
		set<shared_ptr<mMeshData>> _pickMeshs;//最终拾取到的单元
		set<MEdge*> _pickMeshLines;//最终拾取到的单元线
		set<shared_ptr<mMeshFaceData>> _pickMeshFaces;//最终拾取到的单元面
		set<QString> _pickParts;//最终拾取到的部件

	};
}

