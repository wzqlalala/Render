#pragma once
#include "rendview_global.h"
//解决中文乱码
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
		* 重写父类函数
		*/
		void initializeGL()  Q_DECL_OVERRIDE;
		void resizeGL(int w, int h) Q_DECL_OVERRIDE;
		void paintGL() Q_DECL_OVERRIDE;
		void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

	public slots:

		//导入新模型后更新视角
		virtual void slotResetOrthoAndCamera() {};
		//模型中添加/删除后更新视角（不自适应）
		virtual void slotUpdateOrthoAndCamera() {};
		//模型中添加顶点后更新视角（需要自适应）
		virtual void slotUpdateOrthoAndCamera(QVector<QVector3D> addVertex) {};

	public:

		//获取模型参数
		virtual void GetModelSizePara(bool isModelCenter) {};//是否以模型几何中心为中心

	protected:
		//模型参数
		QVector2D _center_model; //模型中心
		float _maxRadius_model;//最大旋转半径(以模型中心)
		QVector2D _center_now; //当前旋转中心
		float _maxRadius_now;//最大旋转半径（以当前中心）
		Space::AABB _aabb; //模型在X,Y,Z方向上的最大参数
	};

}
