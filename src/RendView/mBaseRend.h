#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLFramebufferObject>

#include"mMeshViewEnum.h"

#include "SpaceTree.h"

#include "mBasicEnum.h"

namespace MViewBasic
{
	class mModelView;
	class mViewBase;
}
namespace mxr
{
	class Application;
	class Group;
	class Viewer;
}
using namespace std;
using namespace MViewBasic;
using namespace MxFunctions;
namespace MPostRend
{
	class mPostRender;
}
namespace MPreRend
{
	class mPreRender;
}
namespace MBaseRend
{
	class mBackGroundRender;
	class mQuadRender;
	class mBaseRender;
	class mPostRender;
	class mFontRender;
	class mArrowRender;
	typedef QVector<shared_ptr<mBaseRender>> RenderArray;
	class RENDVIEW_EXPORT mBaseRend : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
	{
		Q_OBJECT

	public:
		mBaseRend(const QString& name = "", MxFunctions::ViewportType = MxFunctions::Viewport3D);
		~mBaseRend();

		//�����ӿ�����	
		void setViewportName(const QString& name);
	
		//��ȡ�ӿ�����	
		QString getViewportName();
	
		//��ȡ�ӿ�����		
		MxFunctions::ViewportType getViewportType();

		std::shared_ptr<mxr::Application> getApplication() { return _app; };

		shared_ptr<mxr::Group> getRoot() { return _root; };

		shared_ptr<mViewBase> getCamera() {return _modelView;};

		shared_ptr<mViewBase> getCamera1() { return _commonView; };

		void setCameraType(CameraType cameratype);

		shared_ptr<mBackGroundRender> getBackGroundRender();

		shared_ptr<mBaseRender> getFirstBeforeRender() { 
			if (!_beforeRenderArray.empty()) {
				return _beforeRenderArray.first();
			}
			return nullptr;
		};

		shared_ptr<mBaseRender> getFirstRender() {
			if (!_renderArray.empty()) {
				return _renderArray.first();
			}
			return nullptr;
		};

		shared_ptr<mBaseRender> getFirstAfterRender() {
			if (!_afterRenderArray.empty()) {
				return _afterRenderArray.first();
			}
			return nullptr;
		};

		shared_ptr<mFontRender> getFontRender() { return _fontRender; };

		shared_ptr<mArrowRender> getArrowRender() { return _arrowRender; };

		QVector<QVector2D> getPickPolygonVertexs() { return _polygonVertexs; };

		virtual shared_ptr<MPostRend::mPostRender> getPostRender() { return nullptr; };

		virtual shared_ptr<MPreRend::mPreRender> getPreRender() { return nullptr; };

		void addBeforeRender(shared_ptr<mBaseRender> baseRender);

		void removeBeforeRender(shared_ptr<mBaseRender> baseRender);

		void addRender(shared_ptr<mBaseRender> baseRender);

		void removeRender(shared_ptr<mBaseRender> baseRender);

		void addAfterRender(shared_ptr<mBaseRender> baseRender);

		void removeAfterRender(shared_ptr<mBaseRender> baseRender);

		void clearRender();

		void clearCameraKeys();

		void clearPickKeys();

		void setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers> buttons, CameraOperateMode cameraOperateMode);

		void setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers> buttons, PickMode pickMode);

		void setMultiplyPickMode(MultiplyPickMode multiplyPickMode);

		void setPickFilter(PickFilter pickFilter);

		void setPickAngle(double angle);

		CameraOperateMode *getCameraOperateMode() { return _cameraMode; };

		PickMode *getCurrentPickMode() { return _pickMode; };//��ǰʰȡģʽ

		MultiplyPickMode *getMultiplyPickMode() { return _multiplyPickMode; };//��ѡʰȡģʽ

		PickFilter *getPickFilter() { return _pickFilter; };//ʰȡ����ģʽ

		double getPickAngle() { return _pickAngle; };

		PickFuntion getPickFuntion()
		{
			if (_mouseButton == Qt::LeftButton)
				return PickFuntion::AddPick;
			return PickFuntion::ReducePick;
		};//ʰȡ���˵Ĺ���

	public:
		/*
		* ��д���ຯ��
		*/
		void initializeGL()  Q_DECL_OVERRIDE;
		void resizeGL(int w, int h) Q_DECL_OVERRIDE;
		void paintGL() Q_DECL_OVERRIDE;

		virtual void otherRend() {};

	public slots:

		//������ģ�ͺ�����ӽ�
		virtual void slotResetOrthoAndCamera() {};
		//ģ�������/ɾ��������ӽǣ�������Ӧ��
		virtual void slotUpdateOrthoAndCamera() {};
		//ģ������Ӷ��������ӽǣ���Ҫ����Ӧ��
		virtual void slotUpdateOrthoAndCamera(QVector<QVector3D> addVertex) {};

	protected:
		void GetPointDepthAtMouse();

	protected:
		CameraOperateMode getCameraMode(Qt::MouseButton, Qt::KeyboardModifiers);

		PickMode getPickMode(Qt::MouseButton, Qt::KeyboardModifiers);

	public:
		//��ѡ�Ŵ�
		void SetZoomAtFrameCenter();//��������ѡȡ�����Ľ������ţ���������
		//��ͼ����Ӧ
		void FitView();
		//������Ļ��������(�����������ţ�������)
		void SetZoomAtViewCenter_ByButton(ScaleDirection);
		//������Ļ��������(���ٷֱ�)
		void SetZoomAtViewCenter_ByPercent(float Percent);
		//��ʼ���ӽǲۺ���
		void SetPerspective(Perspective);
		//���浱ǰ�ӽ�
		void SaveCurrentView();
		//���ñ�����ӽ�
		void CallSavedView();

		//������Ļ��������(����ƶ��������ţ�����)
		void SetZoomAtViewCenter_ByMove();
		//���õ������м�����ƽ��
		void SetTranslateXY();

		//��ȡģ�Ͳ���
		virtual void GetModelSizePara(bool isModelCenter) {};//�Ƿ���ģ�ͼ�������Ϊ����

		//��ʾ���ظ����ӽ�
		//void slotUpdateOrthoAndCamera();

	protected:
		QOpenGLFramebufferObjectFormat format;
		QOpenGLFramebufferObject *FBO;

		QString _name;//�ӿ�����
		MxFunctions::ViewportType _viewportType;//�ӿ�����

		std::shared_ptr<mxr::Application> _app;

		std::shared_ptr<mBackGroundRender> _bgRend;

		std::shared_ptr<mViewBase> _modelView;

		CameraType _cameraType{ CameraType::Camera1};

		std::shared_ptr<mViewBase> _commonView;

		RenderArray _beforeRenderArray;
		std::shared_ptr<mxr::Group> _beforeroot;
		std::shared_ptr<mxr::Viewer> _beforeviewer;

		RenderArray _renderArray;
		std::shared_ptr<mxr::Group> _root;
		std::shared_ptr<mxr::Viewer> _viewer;

		RenderArray _afterRenderArray;
		std::shared_ptr<mxr::Group> _afterroot;
		std::shared_ptr<mxr::Viewer> _afterviewer;

		//������Ⱦ
		std::shared_ptr<mFontRender> _fontRender;

		//��ͷ��Ⱦ
		std::shared_ptr<mArrowRender> _arrowRender;

		//���ο�
		std::shared_ptr<mQuadRender> _quadRender;

		//���ڴ�С
		int SCR_WIDTH = 800, SCR_HEIGHT = 600;
		//�������Ļ�е�����
		GLint lastX{}, lastY{}, nowX{}, nowY{};
		//��һ���������ȡ����������
		GLint Posx_Firstmouse, Posy_Firstmouse;
		//����ο�������
		QVector<QVector2D> _polygonVertexs;

		//��ȡ��ǰ��������ֵ������ģ�ͣ�//��������ĳһ����ת
		float BufDepth = 0.0; //BufDepth��Χ��0, 1��

		//ģ�Ͳ���
		QVector3D _center_model; //ģ������
		float _maxRadius_model = 5.0;//�����ת�뾶(��ģ������)
		QVector3D _center_now; //��ǰ��ת����
		float _maxRadius_now = 5.0;//�����ת�뾶���Ե�ǰ���ģ�
		Space::AABB _aabb; //ģ����X,Y,Z�����ϵ�������


		//����������ж�
		Qt::MouseButton _mouseButton = Qt::NoButton;
		//����ƶ��ж�
		bool isMouseMove = false;

		//�ӽǹ����ж�
		bool ifGetRotateCenter = false; //�Ƿ��ȡ��ת���ģ���ǰ���ĵ�����ص����꣩

		bool ifRotateAtViewCenter = false;//�Ƿ������ӽ�������ת
		bool ifRotateAtXY = true;//�Ƿ���XYƽ����ת
		bool ifRotateAtZ = false;//�Ƿ�����Z����ת

		bool ifTranslateXY = false; //�Ƿ���XYƽ�����ƶ�

		bool ifZoomByMouseMove = false; //�ǹ���������ƶ���������
		bool ifZoomAtFrameCenter = false; //�ǹ�����ѡȡ�����Ľ�������

		ViewOperateMode *_viewOperateMode;

		CameraOperateMode *_cameraMode;

		PickMode *_pickMode;//��ǰʰȡģʽ

		MultiplyPickMode *_multiplyPickMode;//��ѡʰȡģʽ

		PickFilter *_pickFilter;//��ǰʰȡ����ģʽ

		double _pickAngle = 60;//��ǰʰȡ�ĽǶ�

		static QHash<QPair<Qt::MouseButton, Qt::KeyboardModifiers>, CameraOperateMode> _cameraKeys;

		static QHash<QPair<Qt::MouseButton, Qt::KeyboardModifiers>, PickMode> _pickKeys;//������λ
	};

}
