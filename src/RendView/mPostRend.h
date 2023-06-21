#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_5_Core>

#include "mBaseRend3D.h"

using namespace MBaseRend;
namespace MPostRend
{
	class mPostRender;
	class RENDVIEW_EXPORT mPostRend : public mBaseRend3D
	{
		Q_OBJECT

	public:
		mPostRend(const QString& name = "");
		~mPostRend();

		shared_ptr<mPostRender> getPostRender() override;

	public:
		/*
		* 重写父类函数
		*/
		void initializeGL()  Q_DECL_OVERRIDE;
		void resizeGL(int w, int h) Q_DECL_OVERRIDE;
		void paintGL() Q_DECL_OVERRIDE;
		void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

		void GetModelSizePara(bool isModelCenter) override;

	public slots:

		//导入新模型后更新视角
		void slotResetOrthoAndCamera() override;
		//模型中添加/删除后更新视角（不自适应）
		void slotUpdateOrthoAndCamera() override;
		//模型中添加顶点后更新视角（需要自适应）
		void slotUpdateOrthoAndCamera(QVector<QVector3D> addVertex) override;


	protected:
		//std::shared_ptr<mxr::Group> _root;

		//std::shared_ptr<mxr::Viewer> _viewer;
	};
}