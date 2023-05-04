#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>

#include "mBaseRend.h"
#include "app.h"

namespace mxr
{
	class Group;
	class Viewer;
}
using namespace MBaseRend;
namespace MPreRend
{
	class mPreRender;
	class RENDVIEW_EXPORT mPreRend : public mBaseRend
	{
		Q_OBJECT

	public:
		mPreRend(const QString& name);
		~mPreRend();

		shared_ptr<mPreRender> getPreRender() override;

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


	protected:
		//std::shared_ptr<mxr::Group> _root;

		//std::shared_ptr<mxr::Viewer> _viewer;


	};
}