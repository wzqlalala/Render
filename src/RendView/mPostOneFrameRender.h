#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")
#include <QObject>
#include <QOpenGLContext>
#include <set>

#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

namespace mxr
{
	class Application;
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Geode;
	class Texture;
	class Viewer;
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
namespace MBaseRend
{
	class mFontRender;
	class mArrowRender;
}
using namespace MViewBasic;
using namespace MDataPost;
using namespace std;
namespace MPostRend
{
	class mPostRendStatus;
	class mPostModelRender;
	class mPostCuttingPlaneRender;
	class mPostColorTableRender;
	class RENDVIEW_EXPORT mPostOneFrameRender
	{
	public:

		mPostOneFrameRender(std::shared_ptr<mxr::Application> app, shared_ptr<mPostRendStatus> rendStatus, mOneFrameData1 *oneFrameData, mPostOneFrameRendData *oneFrameRendData);

		~mPostOneFrameRender();

		void bufferThisFrame(QOpenGLContext *context);

		void deleteThieFrame();

		mOneFrameData1 *getOneFrameData() { return _oneFrameData; };

		mPostOneFrameRendData *getOneFrameRendData() { return _oneFrameRendData; };

		std::shared_ptr<mPostModelRender> getModelRender() {return _modelRender;};

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView);

		void resizeWindow(int w, int h);

		//void UpdateModelBuffer();

		//���µ�����������
		void updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>> postModelOperates);
		//����ȫ����������
		void updateAllModelOperate(MViewBasic::PostModelOperateEnum postModelOperate);

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet);
		void setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet);
		void setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> meshlineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

		void setTexture(mxr::Texture * texture);

		//���ñ��α���	
		void setDeformationScale(QVector3D deformationScale);

		//������������ϵ��	
		void setTextureCoordScale(float textureCoordScale);

		//���������Сֵ	
		void setMinMaxData(float maxValue, float minValue);

		/*********************************����ͼ*******************************************/

		//ɾ������ͼ	
		bool deleteCuttingPlane(int num);

		//����ͼ������ȡ��
		bool reverseCuttingPlaneNormal(int num);

		//����ֻ��ʾ�����ͼ������ʾ������Ԫ����ͼ
		void setOnlyShowCuttingPlane(bool isOnlyShowCuttingPlane);

		//�����Ƿ���ĳһ����������и�ͼ
		void setIsShowCuttingPlane(int num, bool isShow);

		// ��������ͼ
		bool createCuttingPlane(std::shared_ptr<mxr::StateSet> cuttingPlaneStateSet, std::shared_ptr<mxr::StateSet> transparentPlaneStateSet, int num, QVector3D normal = QVector3D(0, 0, 1), QVector3D vertex = QVector3D(0, 0, 0), bool hasVector = false);

		// ʵʱ�����и���Ǹ�ƽ����Ϣ
		void setPlaneData(int num, QVector3D normal = QVector3D(0, 0, 1), QVector3D centervertex = QVector3D(0, 0, 0), float maxR = 0.0);

		//�����Ƿ�ʵʱ��ʾƽ��
		void setIsShowPlane(bool isShow);

		/*********************************ʸ��ͼ*******************************************/

		//����ʸ��ͼ
		void createVectorGraph(QVector<QPair<QString, QVector3D>> type_color, double size);

		//ָ���ڵ�ID����ʸ��ͼ		
		void createVectorGraph(std::set<int> nodeIDs, QVector<QPair<QString, QVector3D>> type_color, double size);

		//ɾ��ʸ��ͼ
		void deleteVectorGraph();

		//����
		void createVectorGraph(QPair<QVector<QVector3D>, QVector<QVector3D>> res);


		/*************************************��ȡ����*******************************************/
			
		//���ʰȡ���ĵĵ�		
		QPair<QVector<QVector3D>, QVector<QVector3D>>  getPickingNodeData(std::set<int> nodeIds);


	private:

		void initial();

		void makeCurrent();

		void doneCurrent();
	private:
		std::shared_ptr<mxr::Application> _app;

		std::shared_ptr<mxr::Geode> _geode;//��ǰ�ܽڵ�

		std::shared_ptr<mPostRendStatus> _rendStatus;

		mOneFrameData1 *_oneFrameData;//ģ������
		mPostOneFrameRendData *_oneFrameRendData;//��ǰ֡��������

		std::shared_ptr<mPostModelRender> _modelRender;	//ģ����Ⱦ

		std::shared_ptr<mPostColorTableRender> _colorTableRender;//��ɫ����Ⱦ

		QVector<std::shared_ptr<mPostCuttingPlaneRender>> _cuttingPlaneRenders;//������Ⱦ

		std::shared_ptr<MBaseRend::mFontRender> _fontRender;//������Ⱦ

		std::shared_ptr<MBaseRend::mArrowRender> _arrowRender;//��ͷ��Ⱦ

		mxr::Texture * _texture;//��ɫ������

		/*********************************ģ��*****************************************************/
		std::shared_ptr<mxr::StateSet> _faceStateSet;//��Ⱦ���״̬
		std::shared_ptr<mxr::StateSet> _faceTransparentStateSet;//��Ⱦ͸�����״̬
		std::shared_ptr<mxr::StateSet> _faceTransparentNodeformationStateSet;//��Ⱦģ��͸�����ޱ��ε�״̬
		std::shared_ptr<mxr::StateSet> _facelineStateSet;//��Ⱦ���ߵ�״̬
		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//��Ⱦ�߽��ߵ�״̬
		std::shared_ptr<mxr::StateSet> _lineStateSet;//��Ⱦ�ߵ�״̬
		std::shared_ptr<mxr::StateSet> _pointStateSet;//��Ⱦ���״̬

		/***********************************����******************************************************/
		std::shared_ptr<mxr::StateSet> _cuttingPlaneStateSet;//��Ⱦ���״̬
		std::shared_ptr<mxr::StateSet> _transparentPlaneStateSet;//��Ⱦ͸�����״̬

		/*************************************��ǰ֡��Ⱦ********************************************************/
		std::shared_ptr<mxr::Viewer> _viewer;
	};
	
}
