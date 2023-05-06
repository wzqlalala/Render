#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>

#include "mBaseRend3D.h"

namespace MLableRend
{
	class mLableRendIntegrate_pre;
	class mLableRendController_pre;
	class mLableRendIntegrate_common;
	class mLableRendController_common;
}
namespace MDataLable
{
	class mLableDataController_pre;
}
using namespace MBaseRend;
using namespace MLableRend;
using namespace MDataLable;
namespace MPreRend
{
	class mPreRender;
	class RENDVIEW_EXPORT mPreRend : public mBaseRend3D
	{
		Q_OBJECT

	public:
		mPreRend(const QString& name = "");
		~mPreRend();

		shared_ptr<mPreRender> getPreRender() override;

		//��ȡͼ����Ⱦ���ƶ���
		shared_ptr<mLableRendController_pre> getLableRendController_pre();
		shared_ptr<mLableRendController_common> getLableRendController_common();
		//��ȡͼ�����ݿ��ƶ���
		shared_ptr<mLableDataController_pre> getLableDataController_pre();

		void GetModelSizePara(bool isModelCenter) override;

	public slots:

		//������ģ�ͺ�����ӽ�
		void slotResetOrthoAndCamera() override;
		//ģ�������/ɾ��������ӽǣ�������Ӧ��
		void slotUpdateOrthoAndCamera() override;
		//ģ������Ӷ��������ӽǣ���Ҫ����Ӧ��
		void slotUpdateOrthoAndCamera(QVector<QVector3D> addVertex) override;
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

		void otherRend() override;
	protected:
		//std::shared_ptr<mxr::Group> _root;

		//std::shared_ptr<mxr::Viewer> _viewer;
				//ͼ����Ⱦ����
		shared_ptr<mLableRendIntegrate_pre> _lableRend_pre = nullptr;
		shared_ptr<mLableRendIntegrate_common> _lableRend_common = nullptr;

		//ͼ����ƶ���
		shared_ptr<mLableRendController_pre> _lableRendController_pre = nullptr;
		shared_ptr<mLableRendController_common> _lableRendController_common = nullptr;

	};
}