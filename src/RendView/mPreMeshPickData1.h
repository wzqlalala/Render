#pragma once
#include "rendview_global.h"
//解决中文乱码
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

		//获取单例
		static mPreMeshPickData1* getInstance();

		//设置添加拾取还是减少拾取
		void setMeshPickFunction(int pickfunction);//0加1减

		//拾取时调用
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

		//一次性高亮，清除上一次高亮
		void setAllPickNodeData(set<MXMeshVertex*> nodeids);
		void setAllPickMeshData(set<MXMeshElement*> meshids);
		void setAllPickMeshLineData(set<int> meshlineids);
		void setAllPickMeshFaceData(set<int> meshfaceids);
		void setAllPickMeshPartData(set<QString> partNames);

		//累积高亮
		void setAddPickNodeData(set<MXMeshVertex*> nodeids);
		void setAddPickMeshData(set<MXMeshElement*> meshids);
		void setAddPickMeshLineData(set<int> meshlineids);
		void setAddPickMeshFaceData(set<int> meshfaceids);
		void setAddPickMeshPartData(set<QString> partNames);

		//减少高亮
		void setReducePickNodeData(set<MXMeshVertex*> nodeids);
		void setReducePickMeshData(set<MXMeshElement*> meshids);
		void setReducePickMeshLineData(set<int> meshlineids);
		void setReducePickMeshFaceData(set<int> meshfaceids);
		void setReducePickMeshPartData(set<QString> partNames);

		//单选不保留顺序拾取完成后调用
		void setSoloPickData();

		//单选保留顺序时完成后调用
		void setSoloOrderPickData();

		//获取拾取后的节点编号
		set<MXMeshVertex*> getPickNodeIDs();

		//获取拾取后的节点编号(保留拾取顺序)
		QVector<MXMeshVertex*> getPickNodeIDsOrder();

		//获取拾取后的单元编号
		set<MXMeshElement*> getPickMeshIDs();

		//获取拾取后的单元线编号
		set<int> getPickMeshLineIDs();

		//获取拾取后的单元面编号
		set<int> getPickMeshFaceIDs();

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
		QPair<PickObjectType, QPair<QString, int>> getSoloPickNodeDataByLineAngle();//拾取到的对象和其编号
		QPair<PickObjectType, QPair<QString, int>> getSoloPickNodeDataByFaceAngle();//拾取到的对象和其编号
		QPair<QString, int> getSoloPickMeshDataByAngle();
		QPair<QString, int> getSoloPickMeshLineDataByAngle();
		QPair<QString, int> getSoloPickMeshFaceDataByAngle();

	private:

		//默认构造函数
		mPreMeshPickData1();

		//析构函数
		~mPreMeshPickData1();

	private:
		//单例指针
		static mPreMeshPickData1 *_instance;

		int _pickFunction;//0加1减

		MeshVertexDepthBuffer _nodeBuffer;//当前节点编号

		MeshElementDepthBuffer _meshBuffer;//当前单元编号

		IDDepthBuffer _meshLineBuffer;//当前单元线编号

		IDDepthBuffer _meshFaceBuffer;//当前单元面编号

		NameDepthBuffer _partBuffer;//当前部件名称

		MeshVertexSetDepthBuffer _nodeBuffers;//按部件拾取节点的编号

		MeshElementSetDepthBuffer _meshBuffers;//按部件拾取单元的编号

		IDSetDepthBuffer _meshLineBuffers;//按部件拾取单元线的编号

		IDSetDepthBuffer _meshFaceBuffers;//按部件拾取单元面的编号

		IDQStringDepthBuffer _meshPartNameBuffers;//按角度拾取单元的编号

		IDQStringDepthBuffer _meshLinePartNameBuffers;//按角度拾取单元线的编号

		IDQStringDepthBuffer _meshFacePartNameBuffers;//按角度拾取单元面的编号

		//IDQStringDepthBuffer _meshPartNameBuffers;//按角度拾取单元的编号

		set<MXMeshVertex*> _pickNodes;//最终拾取到的节点
		QVector<MXMeshVertex*> _pickNodesOrder;//最终拾取到的节点（保留拾取顺序）
		set<MXMeshElement*> _pickMeshs;//最终拾取到的单元
		set<int> _pickMeshLines;//最终拾取到的单元线
		set<int> _pickMeshFaces;//最终拾取到的单元面
		set<QString> _pickParts;//最终拾取到的部件

	};
}

