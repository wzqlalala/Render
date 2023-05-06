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

#include "mBaseRend.h"

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
	class RENDVIEW_EXPORT mBaseRend2D : public mBaseRend
	{
		Q_OBJECT

	public:
		mBaseRend2D(const QString& name = "", MxFunctions::ViewportType = MxFunctions::Viewport2D);
		~mBaseRend2D();

	public:
		/*
		* ��д���ຯ��
		*/
		void initializeGL()  Q_DECL_OVERRIDE;
		void resizeGL(int w, int h) Q_DECL_OVERRIDE;
		void paintGL() Q_DECL_OVERRIDE;
		void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

	public slots:

		//������ģ�ͺ�����ӽ�
		virtual void slotResetOrthoAndCamera() {};
		//ģ�������/ɾ��������ӽǣ�������Ӧ��
		virtual void slotUpdateOrthoAndCamera() {};
		//ģ������Ӷ��������ӽǣ���Ҫ����Ӧ��
		virtual void slotUpdateOrthoAndCamera(QVector<QVector3D> addVertex) {};

	public:

		//��ȡģ�Ͳ���
		virtual void GetModelSizePara(bool isModelCenter) {};//�Ƿ���ģ�ͼ�������Ϊ����

	protected:
		//ģ�Ͳ���
		QVector2D _center_model; //ģ������
		float _maxRadius_model;//�����ת�뾶(��ģ������)
		QVector2D _center_now; //��ǰ��ת����
		float _maxRadius_now;//�����ת�뾶���Ե�ǰ���ģ�
		Space::AABB _aabb; //ģ����X,Y,Z�����ϵ�������
	};

}
