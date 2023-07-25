#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include "app.h"
#include <QObject>
#include <QHash>
#include <set>
#include <QString>

#include "SpaceTree.h"



#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

#include "mGroupRender.h"
namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Geode;
	class Texture;
}
namespace MViewBasic
{
	class mModelView;
	class mViewBase;
}
namespace MDataMesh
{
	class mMeshModelData;
	class mMeshPartData;
}
using namespace MViewBasic;
using namespace MDataMesh;
using namespace MBaseRend;
using namespace mxr;
using namespace std;
namespace MPreRend
{
	class mPreRendStatus;
	class RENDVIEW_EXPORT mPreMeshPartRender
	{
	public:

		mPreMeshPartRender(std::shared_ptr<mxr::Group> parent, std::shared_ptr<mMeshPartData> partData);

		~mPreMeshPartRender();

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet);
		void setFaceTransparentNodeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> meshlineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		//void setQuadFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

		Space::AABB getPartAABB();

		void appendPart(/*mGeoModelData1 *geoModelData*/);

		void setShowFuntion(ShowFuntion showFuntion);
		std::shared_ptr<mxr::Drawable> getFaceDrawable() { return _facerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getLineDrawable() { return _linerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getPointDrawable() { return _pointrend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getFaceTransparentNodeformationDrawable() { return _facetransparentnodeformationrend->getDrawable(); };
		std::shared_ptr<mxr::Geode> getGeode() { return _geode; }

	private:
		void appendFace(QVector<QVector3D> vertexs, QVector3D color);
		void appendLine(QVector<QVector3D> vertexs, QVector3D color);
		void appendPoint(QVector<QVector3D> vertexs, QVector3D color);
		void appendMeshLine(QVector<QVector3D> vertexs);
	private:
		shared_ptr<mxr::Group> _parent;//父节点
		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _facerend;//面
		std::shared_ptr<mGroupRender1<Vec3Array>> _facetransparentnodeformationrend;//变形前透明的面
		std::shared_ptr<mGroupRender1<Vec3Array>>  _edgelinerend;//边界线
		std::shared_ptr<mGroupRender2<Vec3Array, FloatArray>>  _facelinerend;//面线
		//std::shared_ptr<mGroupRender1<Vec3Array>>  _quadfacelinerend;//面线
		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _linerend;//线
		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _pointrend;//点

		std::shared_ptr<mMeshPartData> _partData;

		Space::AABB _aabb;
	};
	class RENDVIEW_EXPORT mPreMeshModelRender
	{
	public:

		mPreMeshModelRender(shared_ptr<mxr::Group> parent, shared_ptr<mPreRendStatus> rendStatus, std::shared_ptr<mMeshModelData> meshModelData);

		~mPreMeshModelRender();

		bool updateRender();

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet);
		void setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet);
		void setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		//void setQuadFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);
		void setNodeStateSet(std::shared_ptr<mxr::StateSet> stateset);

		//更新缓存
		//void UpdateModelBuffer();

		//更新单个部件操作
		bool updateModelOperate(QPair<MViewBasic::ModelOperateEnum, std::set<QString>> postModelOperates);

		void setShowFuntion(ShowFuntion showFuntion);

		Space::AABB getModelAABB();

		Space::AABB getPartAABB(QString partName);


	private:
		shared_ptr<mxr::Group> _parent;//父节点

		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		std::shared_ptr<mPreRendStatus> _rendStatus;

		std::shared_ptr<mMeshModelData> _meshModelData;

		std::shared_ptr<mxr::StateSet> _faceStateSet;//渲染面的状态

		std::shared_ptr<mxr::StateSet> _faceTransparentStateSet;//渲染透明面的状态

		std::shared_ptr<mxr::StateSet> _faceTransparentNodeformationStateSet;//渲染模型透明面无变形的状态

		std::shared_ptr<mxr::StateSet> _facelineStateSet;//渲染三角形面线的状态
		//std::shared_ptr<mxr::StateSet> _quadfacelineStateSet;//渲染四边形面线的状态

		std::shared_ptr<mxr::StateSet> _lineStateSet;//渲染线的状态

		std::shared_ptr<mxr::StateSet> _pointStateSet;//渲染点的状态

		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//渲染边界线的状态

		std::shared_ptr<mxr::StateSet> _nodeStateSet;//渲染节点的状态

		//零件的顺序
		QList<QString> _partOrder;
		QHash<QString, std::shared_ptr<mPreMeshPartRender>> _partRenders;
	};
}
