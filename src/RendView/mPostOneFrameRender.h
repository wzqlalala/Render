#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")
#include <QObject>
#include <QOpenGLContext>
#include <set>

#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

#include "mPostCuttingPlaneRender.h"

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
	class mPostContourRender;
	class mPostStreamLineRender;
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

		/**************************��ֵ�ߵ�ֵ��ͼ*********************************************/

		//ɾ����ֵ�ߵ�ֵ��ͼ
		void deleteContourGraph();

		//ɾ��ĳһ����ֵ�ߵ�ֵ��ͼ
		void deleteContourGraph(int i);

		//���ɵ�ֵ�ߵ�ֵ��ͼ
		void createContourGraph(std::shared_ptr<mxr::StateSet> lineStateSet, std::shared_ptr<mxr::StateSet> faceStateSet);

		//���ɵ�ֵ�ߵ�ֵ��ͼ	
		void createContourGraph(std::shared_ptr<mxr::StateSet> lineStateSet, std::shared_ptr<mxr::StateSet> faceStateSet, int i, double value, bool isshow = true);

		//���õ�ֵ�ߵ�ֵ��ͼ������״̬	
		void setContourGraph(int i, bool isshow);

		/**************************����ͼ*********************************************/

		//ɾ������ͼ
		void deleteStreamLine();

		//�������ߵ���ʾ��ʽ
		void setStreamLineShowForm(int streamLineShowForm = 0);//0����������1�����

		//��������ͼ���ַ���
		void setIntergrateDirection(int intergrateDirection = 0);//0����˫��1��������2������

		//��������ͼ
		void createStreamLine(std::shared_ptr<mxr::StateSet> lineStateSet, std::shared_ptr<mxr::StateSet> pointStateSet, QVector3D center, float radius, int streamLineNum = 500, float ratio = 0.1);//���ģ��뾶�������ܶȣ�������,���ֱ���

		/*************************************��ȡ����*******************************************/
			
		//���ʰȡ���ĵĵ�		
		QPair<QVector<QVector3D>, QVector<QVector3D>>  getPickingNodeData(std::set<int> nodeIds);

		//��ȡ����ʣ�µ���ͼ�ĵ�
		QPair<QVector<QVector3D>, QVector<QVector3D>>  getCuttingNodeData();

		//��ȡ��������
		template <class T>
		QPair<QVector<QVector3D>, QVector<T>> getCuttingPlaneData(int index, QHash<int, T> value, MViewBasic::NodeOrElement nodeOrElement)
		{
			QPair<QVector<QVector3D>, QVector<T>> result;

			result = _cuttingPlaneRenders[index]->getCuttingPlaneData()->getCuttingNodeData(value, nodeOrElement);
			return result;
		};

		//��ȡ��������������
		QVector<QVector3D> getCuttingPlaneData(int index);
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

		QVector<std::shared_ptr<mPostContourRender>> _postContourRenders;//��ֵ����ͼ��Ⱦ

		std::shared_ptr<mPostStreamLineRender> _postStreamLineRender;//����ͼ��Ⱦ

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

		/************************************��ֵ�ߵ�ֵ��ͼ*******************************************/
		std::shared_ptr<mxr::StateSet> _contourLineStateSet;//��Ⱦ��ֵ��ͼ��״̬(ͬ������������ͼ���ߵ�״̬)
		std::shared_ptr<mxr::StateSet> _contourFaceStateSet;//��Ⱦ��ֵ��ͼ��״̬

		/************************************����ͼ*******************************************/
		std::shared_ptr<mxr::StateSet> _streamlinePointStateSet;//��Ⱦ����ͼ���״̬

		/*************************************��ǰ֡��Ⱦ********************************************************/
		std::shared_ptr<mxr::Viewer> _viewer;
	};
	
}
