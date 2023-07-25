#pragma once
#include "renddata_global.h"

#include <QString>
#include <QSet>
#include <QPair>
#include <QVector3D>

#include <memory>

using namespace std;
namespace MDataMesh
{
	class mMeshData;
	//class mMeshModelData;
	class mMeshNodeData;
	class RENDDATA_EXPORT mMeshFaceData
	{

	public:
		//复制构造函数
		mMeshFaceData(QVector<int> index, std::shared_ptr<mMeshData> meshID);

		//析构函数
		~mMeshFaceData();

		//添加第二对网格ID
		void appendMeshID(std::shared_ptr<mMeshData> meshID, int startIndex);

		//获取第二对网格面的节点
		QVector<shared_ptr<mMeshNodeData>> getNodeData();

		//获取对应网格的网格面的节点
		QVector<shared_ptr<mMeshNodeData>> getNodeData(int meshID);

		//获取第二对网格面的节点
		QVector<QVector3D> getNodeVertex();

		//获取对应网格的网格面的节点
		QVector<QVector3D> getNodeVertex(int meshID);

		//获取第一对网格
		QPair<int, int> getFirstMesh();

		//获取所在的部件名称
		QString getPartName();

		//获取网格
		std::shared_ptr<mMeshData> getMeshID();

		//获取网格ID1
		std::shared_ptr<mMeshData> getMeshID1();

		//获取序号1
		short int getMeshOrder1();

		//获取网格ID2
		std::shared_ptr<mMeshData> getMeshID2();

		//获取序号2
		short int getMeshOrder2();

		//删除单元调用，然后判断是否删除单元面
		bool deleteMesh(int meshID);

		//获取是否渲染
		bool getVisual();

		//设置是否渲染
		void setVisual(bool isShow);

		//获取是否是三角形
		bool getMeshFaceIsTri() { return _index.size() == 3 ? true : false; };

	private:
		//单元面上的节点索引
		QVector<shared_ptr<mMeshNodeData>> _index;

		//第一对网格ID
		std::shared_ptr<mMeshData> _meshID1;
		//第二对网格ID
		std::shared_ptr<mMeshData> _meshID2;

		//第二对网格ID的起始索引位置
		uint8_t _startIndex;
		//是否渲染
		bool _isShow;
	};
}
