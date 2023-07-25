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

		//��������Ľڵ�
		virtual void setNodeIndex(QVector<int> index, QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>& _meshFace = QVector<QVector<QPair<QPair<int, int>, std::shared_ptr<mMeshFaceData>>>>{}){};

		//��ȡ����Ľڵ�
		QVector<int> getNodeIndex();

		//��ȡ����Ľڵ�
		virtual QVector<shared_ptr<mMeshNodeData>> getNodeData() { return QVector<shared_ptr<mMeshNodeData>>{}; };

		//��ȡ����Ľڵ�����
		QVector<QVector3D> getNodeVertex();

		//��ȡ�������������
		virtual MViewBasic::MeshType getMeshType() { return MViewBasic::MeshType::MeshNone; };

		//��ȡ���������
		QVector3D getMeshCenter();

		//��ȡ�����AABB
		Space::AABB getMeshAABB();

		//��������Ӧ��������
		//virtual void appendMeshFace(shared_ptr<mMeshFaceData> meshface){};

		//���������Ӧ��������
		virtual void setMeshFace(int order, shared_ptr<mMeshFaceData> meshFace){};

		//���������Ӧ��������
		//void setMeshFace(const QVector<int>& ids);

		//��ȡ�����Ӧ��������
		virtual QVector<shared_ptr<mMeshFaceData>> getMeshFace() { return QVector<shared_ptr<mMeshFaceData>>{}; };

		//��������Ӧ��������
		virtual void appendMeshLine(int meshLineID);

		//��ȡ�����Ӧ��������
		virtual QVector<int> getMeshLines();

		//���������Ӧ��������
		virtual void setMeshLine(const QVector<int>& ids);

		//������������
		virtual void setMeshVisual(bool isshow);

		//������������
		virtual void reverseMeshVisual(){};

		//��ȡ�����Ƿ�����
		bool getMeshVisual() { return _isShow; };

		//��ȡ�����ά��
		virtual int getMeshDimension() { return -1; };

		//��ȡ�����ID
		int getMeshID() { return _meshID; };

		//��ȡ���ڵĲ�������
		QString getPartName();

		//��ȡ����ָ��
		shared_ptr<mMeshPartData> getPartData() {return _partData;};

		//��ȡģ�����ݹ���ָ��
		shared_ptr<mMeshModelData> getMeshModelData();

	protected:
		//QVector<int> _index;
		//shared_ptr<mMeshData> faces[6] = { nullptr };	
		//�����µ����е�Ԫ��ID
		//QVector<int> _meshLineIDs;
		//����ID
		int _meshID;
		//�����Ƿ���ʾ
		bool _isShow;
		//ģ�����ݹ���
		//shared_ptr<mMeshModelData> _modelData;

		//����
		shared_ptr<mMeshPartData> _partData;
		//��Ĵ�С
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

		//��������Ӧ��������
		//void appendMeshFace(shared_ptr<mMeshFaceData> meshFace) override;

		//���������Ӧ��������
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
