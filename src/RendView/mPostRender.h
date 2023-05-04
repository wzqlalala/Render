#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include "app.h"
//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QFutureWatcher>

//MViewBasic
#include "mMeshViewEnum.h"

//MBasicFunction
#include "mBasicEnum.h"
#include "mBasicStructor.h"

namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
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
	class mPostAnimationRendData;
	class mPostMeshPickThread;
	class mPostMeshPickData;
}
using namespace MViewBasic;
using namespace MDataPost;
using namespace MBasicFunction;
using namespace MBaseRend;
using namespace std;
namespace MPostRend
{
	class mPostRend;
	class mPostRendStatus;
	class mPostHighLightRender;
	class mPostOneFrameRender;
	class mPostAnimationRender;
	class RENDVIEW_EXPORT mPostRender :public mBaseRender
	{
		Q_OBJECT
	public:

		mPostRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mPostRend *postRend);

		void setPostData(shared_ptr<mDataPost1> dataPost) { _dataPost = dataPost;};

		bool getIsDragSomething(QVector2D pos) override;

		void dragSomething(QVector2D pos) override;

		void startPick(QVector<QVector2D> poses) override;

		shared_ptr<mPostOneFrameRender> getOneFrameRender() { return  _oneFrameRender; };

		mPostMeshPickData *getMeshPickData() { return _pickData; };

		//���¸�����Ⱦ
		void updateHighLightRender();

		//���µ�����������
		void updateOneModelOperate(QPair<MBasicFunction::PostModelOperateEnum, std::set<QString>> postModelOperates);
		//����ȫ����������
		void updateAllModelOperate(MBasicFunction::PostModelOperateEnum postModelOperate);

		//���ú�����Ⱦģʽ
		void setPostMode(PostMode postMode);
		
		//���ȫ������Ⱦ
		void clearRender();

		//���õ�ǰ֡����Ⱦ����
		void setRendCurrentFrameData(mPostOneFrameRendData *postOneFrameRendData);
		
		//���õ�ǰ����ʾģʽ	
		void setShowFuntion(ShowFuntion shoFuntion);
		
		//������ɢ����	
		void setDispersed(bool isdispersed);
		
		//���ñ��α���	
		void setDeformationScale(QVector3D deformationScale);

		//�����Ƿ���ʾ��ʼ���͵�͸��ͼ��
		void setIsShowInitialShape(bool isShowInitialShape);
	
		//������������ϵ��	
		void setTextureCoordScale(float textureCoordScale);
	
		//���������Сֵ	
		void setMinMaxData(float maxValue, float minValue);
	
		//������Ⱦ��Χ(0-���в������Զ��壩��1-��ǰ��Ⱦ����)	
		void setRangeType(int index);

		//������ɢ���ͣ��Ƿ�Ȳ�	
		void setDispersIsEquivariance(bool isEquivariance);

		//���õ�Ĵ�С
		void setPointSize(int size);

		//���ñ߽��ߵĿ��
		void setEdgeLineWidth(float size);

		//�����ߵ���ɫ
		void setFaceLineColor(QVector4D color);

		/*********************************����ͼ*******************************************/
		
		//ɾ������ͼ	
		void deleteCuttingPlane(int num);
	
		//����ͼ������ȡ��
		void reverseCuttingPlaneNormal(int num);

		//����ֻ��ʾ�����ͼ������ʾ������Ԫ����ͼ
		void setOnlyShowCuttingPlane(bool isOnlyShowCuttingPlane);


		//�����Ƿ���ĳһ����������и�ͼ
		void setIsShowCuttingPlane(int num, bool isShow);

		// ��������ͼ
		void createCuttingPlane(int num, QVector3D normal = QVector3D(0, 0, 1), QVector3D vertex = QVector3D(0, 0, 0), bool hasVector = false);

		// ʵʱ�����и���Ǹ�ƽ����Ϣ
		void setPlaneData(int num, QVector3D normal = QVector3D(0, 0, 1), QVector3D centervertex = QVector3D(0, 0, 0), float maxR = 0.0);

		//�����Ƿ�ʵʱ��ʾƽ��
		void setIsShowPlane(bool isShow);

		/**********************************����*********************************************/

		//����˲̬��������Ⱦ����	
		void setRendAnimationFrame(mPostAnimationRendData *allFrameRendData);

		//���ɵ�֡����
		void createLinearAnimation(PostMode postMode);

		//ɾ������
		void deleteAnimation();
		
		//���Ƽ�ʱ������		
		void setTimerOn(bool ison);


		~mPostRender();

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView) override;

	private:

		//���������uniformֵ
		void updateCuttingPlaneUniform();

		//��ʼ���߳�
		void initialPickThreads();

		//
	private slots:

		//��ʱ������	
		void slot_aniTimer();

	protected:
		shared_ptr<mDataPost1> _dataPost;

		shared_ptr<mPostRendStatus> _rendStatus;

		//shared_ptr<mPostAnimationRender> _animationRender;//����֡

		//����֡����Ⱦ
		QHash<int, shared_ptr<mPostOneFrameRender>> _animationRender;//����֡

		shared_ptr<mPostOneFrameRender> _oneFrameRender;//��֡

		shared_ptr<mPostOneFrameRender> _oneFrameAnimationRender;//��֡����

		shared_ptr<mxr::Texture> _texture;

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

		//��ʱ��
		QTimer* _aniTimer;

		//ʰȡ�߳�
		////ǰ��Ĳ���������
		mPostMeshPickThread *_thread; QFutureWatcher<void> w;
		mPostMeshPickData *_pickData;
		shared_ptr<mPostHighLightRender> _highLightRender;
		
	};
}
