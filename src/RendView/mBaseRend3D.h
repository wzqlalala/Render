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
	class mShapeRender;
	class RENDVIEW_EXPORT mBaseRend3D : public mBaseRend
	{
		Q_OBJECT

	public:
		mBaseRend3D(const QString& name = "", MxFunctions::ViewportType = MxFunctions::Viewport3D);
		~mBaseRend3D();	

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

		virtual bool event(QEvent *e) override;

		//设置旋转中心为视图中心
		void SetRotateCenterToViewCenter();
		//设置旋转中心为视图上一点
		void SetRotateCenterToPoint();
		//设置旋转中心为几何中心
		void SetRotateCenterToModelCenter();
		//设置点击按钮绕着垂直屏幕轴旋转
		void SetRotate_ByButton(float angle);
		//设置自定义角度旋转槽函数
		void slotSetRotate_ByButton(float angle);
		//设置点击鼠标中键进行旋转类型
		void SetRotateType(RotateType);

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

		//显示隐藏更新视角
		//void slotUpdateOrthoAndCamera();

	protected:

		//模型参数
		//QVector3D _center_model; //模型中心
		//float _maxRadius_model;//最大旋转半径(以模型中心)
		//QVector3D _center_now = QVector3D(0,0,0); //当前旋转中心
		//float _maxRadius_now = 5.0;//最大旋转半径（以当前中心）
		//Space::AABB _aabb; //模型在X,Y,Z方向上的最大参数

		std::shared_ptr<mShapeRender> _shapeRender;

	};

}
