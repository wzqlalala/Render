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
		shared_ptr<mxr::Group> _parent;//���ڵ�
		std::shared_ptr<mxr::Geode> _geode;//��ǰ�ܽڵ�

		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _facerend;//��
		std::shared_ptr<mGroupRender1<Vec3Array>> _facetransparentnodeformationrend;//����ǰ͸������
		std::shared_ptr<mGroupRender1<Vec3Array>>  _edgelinerend;//�߽���
		std::shared_ptr<mGroupRender2<Vec3Array, FloatArray>>  _facelinerend;//����
		//std::shared_ptr<mGroupRender1<Vec3Array>>  _quadfacelinerend;//����
		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _linerend;//��
		std::shared_ptr<mGroupRender2<Vec3Array, Vec3Array>>  _pointrend;//��

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

		//���»���
		//void UpdateModelBuffer();

		//���µ�����������
		bool updateModelOperate(QPair<MViewBasic::ModelOperateEnum, std::set<QString>> postModelOperates);

		void setShowFuntion(ShowFuntion showFuntion);

		Space::AABB getModelAABB();

		Space::AABB getPartAABB(QString partName);


	private:
		shared_ptr<mxr::Group> _parent;//���ڵ�

		std::shared_ptr<mxr::Geode> _geode;//��ǰ�ܽڵ�

		std::shared_ptr<mPreRendStatus> _rendStatus;

		std::shared_ptr<mMeshModelData> _meshModelData;

		std::shared_ptr<mxr::StateSet> _faceStateSet;//��Ⱦ���״̬

		std::shared_ptr<mxr::StateSet> _faceTransparentStateSet;//��Ⱦ͸�����״̬

		std::shared_ptr<mxr::StateSet> _faceTransparentNodeformationStateSet;//��Ⱦģ��͸�����ޱ��ε�״̬

		std::shared_ptr<mxr::StateSet> _facelineStateSet;//��Ⱦ���������ߵ�״̬
		//std::shared_ptr<mxr::StateSet> _quadfacelineStateSet;//��Ⱦ�ı������ߵ�״̬

		std::shared_ptr<mxr::StateSet> _lineStateSet;//��Ⱦ�ߵ�״̬

		std::shared_ptr<mxr::StateSet> _pointStateSet;//��Ⱦ���״̬

		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//��Ⱦ�߽��ߵ�״̬

		std::shared_ptr<mxr::StateSet> _nodeStateSet;//��Ⱦ�ڵ��״̬

		//�����˳��
		QList<QString> _partOrder;
		QHash<QString, std::shared_ptr<mPreMeshPartRender>> _partRenders;
	};
}
