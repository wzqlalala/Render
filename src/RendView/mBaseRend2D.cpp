#include "mBaseRend2D.h"
#include "mBackGroundRender.h"
#include "mQuadRender.h"
#include "mBaseRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"
//工具类
#include"mViewToolClass.h"
//视图类
#include "mModelView.h"
#include "mCommonView.h"
#include "mViewBase.h"
#include "mCameraModelView.h"
#include "mCameraCommonView.h"

//Qt
#include <QMouseEvent>
#include <QGuiApplication>
#include <QOpenGLFramebufferObject>

////MxRender
#include <renderpch.h>
#include "mShaderManage.h"
#include "mTextureManage.h"
#include "appInstance.h"

namespace MBaseRend
{
	mBaseRend2D::mBaseRend2D(const QString& name, MxFunctions::ViewportType type): mBaseRend(name, type)
	{
		setMouseTracking(true);

		_viewOperateMode = new ViewOperateMode; *_viewOperateMode = ViewOperateMode::NoViewOperate;
		_cameraMode = new CameraOperateMode; *_cameraMode = CameraOperateMode::NoCameraOperate;
		_pickMode = new PickMode;*_pickMode = PickMode::NoPick;//当前拾取模式
		_multiplyPickMode = new MultiplyPickMode; *_multiplyPickMode = MultiplyPickMode::QuadPick;//框选拾取模式
		_pickFilter = new PickFilter; *_pickFilter = PickFilter::PickGeoPoint;

		//QOpenGLContext *context = QOpenGLContext::currentContext();
		_app = MakeAsset<mxr::Application>();
		//_app->setContext(context);
		mxr::ApplicationInstance::GetInstance().appendApplication(name, _app);
	
		_beforeroot = MakeAsset<mxr::Group>();
		_root = MakeAsset<mxr::Group>();
		_afterroot = MakeAsset<mxr::Group>();

		//QSurfaceFormat format;
		//format.setMajorVersion(4);
		//format.setMinorVersion(5);
		//format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
		//format.setSamples(4);
		//setFormat(format);
		//this->showMaximized();
		qDebug() << "Base Struct";
	}

	void mBaseRend2D::initializeGL()
	{
		mBaseRend::initializeGL();
	}

	void mBaseRend2D::paintGL()
	{
		mBaseRend::paintGL();
	}

	void mBaseRend2D::resizeGL(int w, int h)
	{
		mBaseRend::resizeGL(w, h);
	}
	void mBaseRend2D::mousePressEvent(QMouseEvent *event)
	{
		Posx_Firstmouse = event->pos().x();
		Posy_Firstmouse = event->pos().y();
		nowX = event->pos().x();
		nowY = event->pos().y();
		lastX = nowX;
		lastY = nowY;
		_mouseButton = event->button();
		Qt::KeyboardModifiers keyModifiers = event->modifiers();
		*_cameraMode = this->getCameraMode(_mouseButton, keyModifiers);
		if (*_cameraMode == CameraOperateMode::NoCameraOperate)
		{
			*_viewOperateMode = ViewOperateMode::NoViewOperate;
			*_pickMode = this->getPickMode(_mouseButton, keyModifiers);
			if (*_pickMode != PickMode::NoPick)
			{
				*_viewOperateMode = ViewOperateMode::PickOperate;
				if (*_pickMode == PickMode::SoloPick)
				{
					for (auto render : _beforeRenderArray)
					{
						if (render->getIsDragSomething(QVector2D(nowX, nowY)))
						{
							*_pickMode = PickMode::DragPick;
							break;
						}
					}
					for (auto render : _renderArray)
					{
						if (render->getIsDragSomething(QVector2D(nowX, nowY)))
						{
							*_pickMode = PickMode::DragPick;
							break;
						}
					}
					for (auto render : _afterRenderArray)
					{
						if (render->getIsDragSomething(QVector2D(nowX, nowY)))
						{
							*_pickMode = PickMode::DragPick;
							break;
						}
					}
				}
				else if (*_pickMode == PickMode::MultiplyPick)
				{
					_polygonVertexs.append(QVector2D(nowX, nowY));
				}
			}
		}
		else
		{
			*_viewOperateMode = ViewOperateMode::CameraOperate;
			if (*_cameraMode == CameraOperateMode::Zoom)
			{
				_polygonVertexs.append(QVector2D(nowX, nowY));
			}
		}

		update();
	}
	void mBaseRend2D::mouseReleaseEvent(QMouseEvent *event)
	{
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			if (*_viewOperateMode == ViewOperateMode::CameraOperate)
			{
				if (*_cameraMode == CameraOperateMode::Zoom)
				{
					view->ZoomAtFrameCenter((Posx_Firstmouse + nowX) / 2, (Posy_Firstmouse + nowY) / 2);
				}
			}
			else if (*_viewOperateMode == ViewOperateMode::PickOperate)
			{
				if (*_pickMode == PickMode::SoloPick)
				{
					_polygonVertexs.append(QVector2D(nowX, nowY));
					for (auto render : _beforeRenderArray)
					{
						makeCurrent();
						/*
						FBO->bind();
						float depth = 0.0;
						QOpenGLContext::currentContext()->functions()->glReadPixels(nowX, SCR_HEIGHT - nowY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
						if (depth != 0)
						{
							qDebug() << "depth" << depth;
						}
						GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
						if (error != 0)
						{
							qDebug() << error;
						}
						render->startPick(_polygonVertexs);
						FBO->release();
						*/
						FBO->bind();
						render->startPick(_polygonVertexs);
						FBO->release();
					}
					for (auto render : _renderArray)
					{
						makeCurrent();
						FBO->bind();
						render->startPick(_polygonVertexs);
						FBO->release();
					}
					for (auto render : _afterRenderArray)
					{
						makeCurrent();
						FBO->bind();
						render->startPick(_polygonVertexs);
						FBO->release();
					}
				}
				else if (*_pickMode == PickMode::MultiplyPick)
				{
					for (auto render : _beforeRenderArray)
					{
						render->startPick(_polygonVertexs);
					}
					for (auto render : _renderArray)
					{
						render->startPick(_polygonVertexs);
					}
					for (auto render : _afterRenderArray)
					{
						render->startPick(_polygonVertexs);
					}
				}
			}
			_polygonVertexs.clear();
			*_viewOperateMode = ViewOperateMode::NoViewOperate;
			*_cameraMode = CameraOperateMode::NoCameraOperate;
			*_pickMode = PickMode::NoPick;
		}
		update();
	}

	void mBaseRend2D::mouseMoveEvent(QMouseEvent *event)
	{
		isMouseMove = true;
		nowX = event->pos().x();
		nowY = event->pos().y();
		//QCursor::setPos(geometry().center());
		GLint xoffset = lastX - nowX;//计算X方向偏移量(移动像素)
		GLint yoffset = nowY - lastY;// 计算Y方向偏移量
		lastX = nowX;
		lastY = nowY;

		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		auto view1 = dynamic_pointer_cast<mCommonView>(_commonView);
		if (view)
		{
			if (*_viewOperateMode == ViewOperateMode::CameraOperate)
			{
				if (*_cameraMode == CameraOperateMode::Rotate)
				{
					if (ifRotateAtXY == true && ifGetRotateCenter == false)
					{
						view->Rotate(xoffset, yoffset, Rotate_XY);
						view1->Rotate(xoffset, yoffset, Rotate_XY);
					}
					else if (ifRotateAtZ == true && ifGetRotateCenter == false)
					{
						view->Rotate(xoffset, yoffset, Rotate_Z);
						view1->Rotate(xoffset, yoffset, Rotate_Z);
					}
				}
				else if (*_cameraMode == CameraOperateMode::Translate)
				{
					view->Translate(xoffset, yoffset);
				}
				else if (*_cameraMode == CameraOperateMode::Zoom)
				{
					float distance = 0.0;
					if (_polygonVertexs.size() > 0)
					{
						distance = _polygonVertexs.last().distanceToPoint(QVector2D(nowX, nowY));
					}
					if (distance > 20)
					{
						_polygonVertexs.append(QVector2D(nowX, nowY));
					}
				}
			}
			else if (*_viewOperateMode == ViewOperateMode::PickOperate)
			{
				if (*_pickMode == PickMode::DragPick)
				{
					for (auto render : _beforeRenderArray)
					{
						render->dragSomething(QVector2D(nowX, nowY));
					}
					for (auto render : _renderArray)
					{
						render->dragSomething(QVector2D(nowX, nowY));
					}
					for (auto render : _afterRenderArray)
					{
						render->dragSomething(QVector2D(nowX, nowY));
					}
				}
				else if (*_pickMode == PickMode::MultiplyPick)
				{
					float distance = 0.0;
					if (_polygonVertexs.size() > 0)
					{
						distance = _polygonVertexs.last().distanceToPoint(QVector2D(nowX, nowY));
					}
					if (*_multiplyPickMode == MultiplyPickMode::PolygonPick)
					{
						if (distance > 20)
						{
							_polygonVertexs.append(QVector2D(nowX, nowY));
						}
					}
					else
					{
						_polygonVertexs.append(QVector2D(nowX, nowY));
					}
				}
			}
		}
		else
		{
			//lastX = geometry().center().x();
			//lastY = geometry().center().y();
			//QCursor::setPos(geometry().center());
		}
		update();

	}
	void mBaseRend2D::wheelEvent(QWheelEvent *event)
	{
		QPoint numDegrees = event->angleDelta();//获取滚轮的转角
		if (abs(numDegrees.y() - 0) < 0.0001)
		{
			return;
		}	
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			view->ZoomAtMouse_Bywheel(nowX, nowY, numDegrees.y(), Model);
		}
		update();
	}

	mBaseRend2D::~mBaseRend2D()
	{
		

	}
}