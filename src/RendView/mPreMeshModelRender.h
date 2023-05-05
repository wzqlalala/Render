#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include "app.h"
#include <QObject>
#include <QHash>
#include <set>
#include <QString>

#include "SpaceTree.h"

#include "mBasicEnum.h"

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
class MXMesh;
class MeshEntity;
class MXGeoSolid;
class MXGeoFace;
class MXGeoEdge;
class MXMeshElement;
using namespace MViewBasic;
using namespace MViewBasic;
using namespace MBaseRend;
using namespace mxr;
using namespace std;
namespace MPreRend
{
	class mPreRendStatus;
	class RENDVIEW_EXPORT mPreMeshPartRender
	{
	public:

		mPreMeshPartRender(std::shared_ptr<mxr::Group> parent/*, MDataGeo::mGeoPartData1 *part*/);

		~mPreMeshPartRender();

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet);
		void setFaceTransparentNodeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> meshlineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

		Space::SpaceTree* getPartSpaceTree();
		//MDataGeo::mGeoPartData1 *getpartData() { return _partData; };

		void appendPart(/*mGeoModelData1 *geoModelData*/);

		void setShowFuntion(ShowFuntion showFuntion);
		std::shared_ptr<mxr::Drawable> getFaceDrawable() { return _facerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getLineDrawable() { return _linerend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getPointDrawable() { return _pointrend->getDrawable(); };
		std::shared_ptr<mxr::Drawable> getFaceTransparentNodeformationDrawable() { return _facetransparentnodeformationrend->getDrawable(); };
		std::shared_ptr<mxr::Geode> getGeode() { return _geode; }

	private:
		shared_ptr<mxr::Group> _parent;//���ڵ�
		std::shared_ptr<mxr::Geode> _geode;//��ǰ�ܽڵ�

		std::shared_ptr<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>  _facerend;//��
		std::shared_ptr<mGroupRender1<Vec3Array>> _facetransparentnodeformationrend;//����ǰ͸������
		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _edgelinerend;//�߽���
		std::shared_ptr<mGroupRender3<Vec3Array, Vec3Array, FloatArray>>  _facelinerend;//����
		std::shared_ptr<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>  _linerend;//��
		std::shared_ptr<mGroupRender5<Vec3Array, Vec3Array, FloatArray, FloatArray, Vec3Array>>  _pointrend;//��

		//MDataGeo::mGeoPartData1 *_partData;

		Space::SpaceTree *_spaceTree;
	};
	class RENDVIEW_EXPORT mPreMeshModelRender
	{
	public:

		mPreMeshModelRender(shared_ptr<mxr::Group> parent, shared_ptr<mPreRendStatus> rendStatus/*, mGeoModelData1 *geoModelData*/);

		~mPreMeshModelRender();

		bool updateRender();

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet);
		void setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet);
		void setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> edgelineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

		//���»���
		void UpdateModelBuffer();

		//���µ�����������
		bool updateModelOperate(QPair<MxFunctions::ModelOperateEnum, std::set<QString>> postModelOperates);

		void setShowFuntion(ShowFuntion showFuntion);

		Space::AABB getModelAABB();

		Space::SpaceTree *getPartSpaceTree(QString partName);

	private:
		shared_ptr<mxr::Group> _parent;//���ڵ�

		std::shared_ptr<mxr::Geode> _geode;//��ǰ�ܽڵ�

		std::shared_ptr<mPreRendStatus> _rendStatus;

		//mGeoModelData1 *_geoModelData;

		std::shared_ptr<mxr::StateSet> _faceStateSet;//��Ⱦ���״̬

		std::shared_ptr<mxr::StateSet> _faceTransparentStateSet;//��Ⱦ͸�����״̬

		std::shared_ptr<mxr::StateSet> _faceTransparentNodeformationStateSet;//��Ⱦģ��͸�����ޱ��ε�״̬

		std::shared_ptr<mxr::StateSet> _facelineStateSet;//��Ⱦ���ߵ�״̬

		std::shared_ptr<mxr::StateSet> _lineStateSet;//��Ⱦ�ߵ�״̬

		std::shared_ptr<mxr::StateSet> _pointStateSet;//��Ⱦ���״̬

		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//��Ⱦ�߽��ߵ�״̬

		//�����˳��
		QList<QString> _partOrder;
		QHash<QString, std::shared_ptr<mPreMeshPartRender>> _partRenders;
	};
}
