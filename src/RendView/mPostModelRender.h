#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include "app.h"
//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <QObject>
#include <QHash>
#include <set>
#include <QString>

#include "SpaceTree.h"

#include "mBasicEnum.h"
#include "mMeshViewEnum.h"
#include "mBasicStructor.h"
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
namespace MDataPost
{
	class mDataPost1;
	class mOneFrameData1;
	class mPostOneFrameRendData;
}
using namespace MViewBasic;
using namespace MDataPost;
using namespace MBasicFunction;
using namespace std;
namespace MPostRend
{
	class mPostRendStatus;
	class mPostPartRender;
	class RENDVIEW_EXPORT mPostModelRender
	{
	public:

		mPostModelRender(shared_ptr<mxr::Group> parent, shared_ptr<mPostRendStatus> rendStatus, mOneFrameData1 *oneFrameData, mPostOneFrameRendData *oneFrameRendData);

		~mPostModelRender();

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
		void updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>> postModelOperates);
		//����ȫ����������
		void updateAllModelOperate(MBasicFunction::PostModelOperateEnum postModelOperate);

		//void setDistancePlane(std::vector<QVector4D> cutplane);
		//void setModelView(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model);
		void setDeformationScale(QVector3D deformationScale);

		//void setLight(PostLight light, QVector3D viewpos, int lightIsOn);
		void setTextureArgument(int isEquivariance, float minValue, float maxValue, float textureCoordRatio);


		void setShowFuntion(ShowFuntion showFuntion);
		
		//�����Ƿ���ʾ��ʼ���͵�͸��ͼ��
		void setIsShowInitialShape(bool isShowInitialShape);

		Space::AABB getModelAABB();

		Space::SpaceTree *getPartSpaceTree(QString partName);

		//void append

		//void updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>> postModelOperates);

	private:
		shared_ptr<mxr::Group> _parent;//���ڵ�

		std::shared_ptr<mxr::Geode> _geode;//��ǰ�ܽڵ�

		std::shared_ptr<mPostRendStatus> _rendStatus;

		mOneFrameData1 *_oneFrameData;
		mPostOneFrameRendData *_oneFrameRendData;

		std::shared_ptr<mxr::StateSet> _faceStateSet;//��Ⱦ���״̬

		std::shared_ptr<mxr::StateSet> _faceTransparentStateSet;//��Ⱦ͸�����״̬

		std::shared_ptr<mxr::StateSet> _faceTransparentNodeformationStateSet;//��Ⱦģ��͸�����ޱ��ε�״̬

		std::shared_ptr<mxr::StateSet> _facelineStateSet;//��Ⱦ���ߵ�״̬

		std::shared_ptr<mxr::StateSet> _lineStateSet;//��Ⱦ�ߵ�״̬

		std::shared_ptr<mxr::StateSet> _pointStateSet;//��Ⱦ���״̬

		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//��Ⱦ�߽��ߵ�״̬

		//�����˳��
		QList<QString> _partOrder;
		QHash<QString, std::shared_ptr<mPostPartRender>> _partRenders;
	};
}
