#pragma once
#include "renddata_global.h"

#include <QVector>
#include <QObject>
#include <set>

#include <memory>

#include "mMeshViewEnum.h"

#include "SpaceTree.h"


using namespace MViewBasic;
using namespace std;
namespace MDataMesh
{
	class mMeshNodeData;
	class mMeshFaceData;
	class mMeshModelData;
	class mMeshPartData;
	class RENDDATA_EXPORT mMeshData : public std::enable_shared_from_this<mMeshData>
	{
	public:
		
		mMeshData(int ID, shared_ptr<mMeshPartData> partData);

		~mMeshData();

		//设置网格的节点
		virtual void setNodeIndex(QVector<int> index, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>& _meshFace = QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>{}){};

		//获取网格的节点
		QVector<int> getNodeIndex();

		//获取网格的节点
		virtual QVector<shared_ptr<mMeshNodeData>> getNodeData() { return QVector<shared_ptr<mMeshNodeData>>{}; };

		//获取网格的节点坐标
		QVector<QVector3D> getNodeVertex();

		//获取网格的网格类型
		virtual MViewBasic::MeshType getMeshType() { return MViewBasic::MeshType::MeshNone; };

		//获取网格的形心
		QVector3D getMeshCenter();

		//获取网格的AABB
		Space::AABB getMeshAABB();

		//添加网格对应的网格面
		//virtual void appendMeshFace(shared_ptr<mMeshFaceData> meshface){};

		//设置网格对应的网格面
		virtual void setMeshFace(int order, shared_ptr<mMeshFaceData> meshFace){};

		//设置网格对应的网格面
		//void setMeshFace(const QVector<int>& ids);

		//获取网格对应的网格面
		virtual QVector<shared_ptr<mMeshFaceData>> getMeshFace() { return QVector<shared_ptr<mMeshFaceData>>{}; };

		//添加网格对应的网格线
		virtual void appendMeshLine(int meshLineID);

		//获取网格对应的网格面
		virtual QVector<int> getMeshLines();

		//设置网格对应的网格线
		virtual void setMeshLine(const QVector<int>& ids);

		//设置网格显隐
		virtual void setMeshVisual(bool isshow);

		//反向网格显隐
		virtual void reverseMeshVisual(){};

		//获取网格是否显隐
		bool getMeshVisual() { return _isShow; };

		//获取网格的维度
		virtual int getMeshDimension() { return -1; };

		//获取网格的ID
		int getMeshID() { return _meshID; };

		//获取所在的部件名称
		QString getPartName();

		//获取部件指针
		shared_ptr<mMeshPartData> getPartData() {return _partData;};

		//获取模型数据管理指针
		shared_ptr<mMeshModelData> getMeshModelData();

	protected:
		//QVector<int> _index;
		//shared_ptr<mMeshData> faces[6] = { nullptr };	
		//网格下的所有单元线ID
		//QVector<int> _meshLineIDs;
		//网格ID
		int _meshID;
		//网格是否显示
		bool _isShow;
		//模型数据管理
		//shared_ptr<mMeshModelData> _modelData;

		//部件
		shared_ptr<mMeshPartData> _partData;
		//面的大小
		//uint8_t _location{};


	};

	class RENDDATA_EXPORT mMesh0DimData : public mMeshData
	{
	public:
		mMesh0DimData(int ID, shared_ptr<mMeshPartData> partData);

		MViewBasic::MeshType getMeshType() override { return MeshPoint; };

		void setNodeIndex(QVector<int> index, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>& _meshFace = QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>{}) override;

		int getMeshDimension() override { return 0; };

		QVector<shared_ptr<mMeshNodeData>> getNodeData() override;

	protected:

		shared_ptr<mMeshNodeData> _index;


	};

	class RENDDATA_EXPORT mMesh1DimData : public mMeshData
	{
	public:
		mMesh1DimData(int ID, shared_ptr<mMeshPartData> partData);

		MViewBasic::MeshType getMeshType() override { return MeshBeam; };

		void setNodeIndex(QVector<int> index, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>& _meshFace = QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>{}) override;

		int getMeshDimension() override { return 1; };

		QVector<shared_ptr<mMeshNodeData>> getNodeData() override;

	protected:

		QVector<shared_ptr<mMeshNodeData>> _index;


	};

	class RENDDATA_EXPORT mMesh2DimData : public mMeshData
	{
	public:
		mMesh2DimData(int ID, shared_ptr<mMeshPartData> partData);

		void setNodeIndex(QVector<int> index, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>& _meshFace = QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>{}) override;

		int getMeshDimension() override { return 2; };

		QVector<shared_ptr<mMeshNodeData>> getNodeData() override;

	protected:

		QVector<shared_ptr<mMeshNodeData>> _index;
	};

	class RENDDATA_EXPORT mMeshTriData : public mMesh2DimData
	{
	public:
		mMeshTriData(int ID, shared_ptr<mMeshPartData> partData);

		MViewBasic::MeshType getMeshType() override { return MeshTri; };

	};

	class RENDDATA_EXPORT mMeshQuadData : public mMesh2DimData
	{
	public:
		mMeshQuadData(int ID, shared_ptr<mMeshPartData> partData);

		MViewBasic::MeshType getMeshType() override { return MeshQuad; };

	};

	class RENDDATA_EXPORT mMesh3DimData : public mMeshData
	{
	public:
		mMesh3DimData(int ID, shared_ptr<mMeshPartData> partData);

		QVector<shared_ptr<mMeshFaceData>> getMeshFace() override;

		void setNodeIndex(QVector<int> index, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>& _meshFace = QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>{}) override;

		void createMeshFace(QVector<int> index, int order, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>& _meshFace);

		int getMeshDimension() override { return 3; };

		//添加网格对应的网格面
		//void appendMeshFace(shared_ptr<mMeshFaceData> meshFace) override;

		//设置网格对应的网格面
		void setMeshFace(int order, shared_ptr<mMeshFaceData> meshFace) override;

	protected:

		QVector<shared_ptr<mMeshFaceData>> _faces;
	};

	class RENDDATA_EXPORT mMeshTetData : public mMesh3DimData
	{
	public:
		mMeshTetData(int ID, shared_ptr<mMeshPartData> partData);		

		MViewBasic::MeshType getMeshType() override { return MeshTet; };

		QVector<shared_ptr<mMeshNodeData>> getNodeData() override;
	};

	class RENDDATA_EXPORT mMeshPyramidData : public mMesh3DimData
	{
	public:
		mMeshPyramidData(int ID, shared_ptr<mMeshPartData> partData);

		MViewBasic::MeshType getMeshType() override { return MeshPyramid; };

		QVector<shared_ptr<mMeshNodeData>> getNodeData() override;
	};


	class RENDDATA_EXPORT mMeshWedgeData : public mMesh3DimData
	{
	public:
		mMeshWedgeData(int ID, shared_ptr<mMeshPartData> partData);

		MViewBasic::MeshType getMeshType() override { return MeshWedge; };

		QVector<shared_ptr<mMeshNodeData>> getNodeData() override;
	};

	class RENDDATA_EXPORT mMeshHexData : public mMesh3DimData
	{
	public:
		mMeshHexData(int ID, shared_ptr<mMeshPartData> partData);

		MViewBasic::MeshType getMeshType() { return MeshHex; };

		QVector<shared_ptr<mMeshNodeData>> getNodeData() override;
	};
}
