#include "mBaseRend3D.h"
#include "mBaseRender.h"
#include "mShapeRender.h"
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

namespace MBaseRend
{
	mBaseRend3D::mBaseRend3D(const QString& name, MxFunctions::ViewportType type): mBaseRend(name, type)
	{
		qDebug() << "mBaseRend3D Struct";
	}

	void mBaseRend3D::initializeGL()
	{
		mBaseRend::initializeGL();

		_shapeRender = make_shared<mShapeRender>(_app, _afterroot, this);
		this->addAfterRender(_shapeRender);
	}

	void mBaseRend3D::paintGL()
	{
		mBaseRend::paintGL();
	}

	void mBaseRend3D::resizeGL(int w, int h)
	{
		mBaseRend::resizeGL(w, h);
	}
	void mBaseRend3D::mousePressEvent(QMouseEvent *event)
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
					makeCurrent();
					FBO->bind();
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
					FBO->release();
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
	void mBaseRend3D::mouseReleaseEvent(QMouseEvent *event)
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
				if (*_pickMode == PickMode::SoloPick || *_pickMode == PickMode::DragPick)
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
		if (ifGetRotateCenter)
		{
			makeCurrent();
			FBO->bind();
			int posx = event->pos().x(); //QT像素坐标原点左上角
			int posy = event->pos().y();
			QVector3D Center = mViewToolClass::PixelToModelPosition(posx, posy, _modelView->_projection, _modelView->_view, _modelView->_model, SCR_WIDTH, SCR_HEIGHT, BufDepth);
			float Depth = 0.0;
			if (abs(BufDepth - 1.0) < 0.1)
				Depth = 0;
			else
				Depth = (BufDepth - 0.5) * 2;//将深度值范围从0.1转换到-1.1
			float depth;
			glReadPixels(event->pos().x(), SCR_HEIGHT - event->pos().y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
			_center_now = Center;
			_shapeRender->appendXPointShape("RotateCenter", { Center }, QVector3D(1, 1, 0));
			FBO->release();
			ifGetRotateCenter = false;
		}
		else
		{
			//设置旋转中心隐藏
			_shapeRender->setXPointIsShow("RotateCenter", false);//隐藏

		}
		update();
	}

	void mBaseRend3D::mouseMoveEvent(QMouseEvent *event)
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
					makeCurrent();
					FBO->bind();
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
					FBO->release();
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
			else if (ifZoomByMouseMove == true && ifGetRotateCenter == false)
			{
				_modelView->ZoomAtViewCenter_ByMove(Posx_Firstmouse, Posy_Firstmouse, nowX, nowY);
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
	void mBaseRend3D::wheelEvent(QWheelEvent *event)
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

	mBaseRend3D::~mBaseRend3D()
	{
		

	}
	
	bool mBaseRend3D::event(QEvent * e)
	{
		auto view = dynamic_pointer_cast<mCameraModelView>(_modelView);
		auto view1 = dynamic_pointer_cast<mCameraCommonView>(_commonView);
		if (view)
		{
			view->handle(e);
			view1->handle(e);
		}
		return QWidget::event(e);
	}

		//设置视图*旋转中心*类型
	void mBaseRend3D::SetRotateCenterToPoint()
	{
		ifGetRotateCenter = true;
		ifRotateAtViewCenter = false;
		_shapeRender->deleteXPointShape("RotateCenter");

	}
	void mBaseRend3D::SetRotateCenterToViewCenter()
	{
		ifGetRotateCenter = false;
		ifRotateAtViewCenter = true;
		//获取屏幕中心的坐标
		QVector3D ViewCenter = mViewToolClass::NormToModelPosition(QVector3D(0, 0, 0), _modelView->_projection, _modelView->_view, _modelView->_model);
		//找新的旋转半径
		float maxRadius = mViewToolClass::GetMaxRadius(_aabb.minEdge.x(), _aabb.maxEdge.x(), _aabb.minEdge.y(), _aabb.maxEdge.x(), _aabb.minEdge.z(), _aabb.maxEdge.z(), ViewCenter);
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			view->SetRotateCenterToViewCenter(ViewCenter, maxRadius);
		}
		_center_now = ViewCenter;
		//传递旋转中心数据
		QVector3D centerPos = mViewToolClass::PixelToModelPosition(SCR_WIDTH / 2, SCR_HEIGHT / 2, _modelView->_projection, _modelView->_view, _modelView->_model, SCR_WIDTH, SCR_HEIGHT);
		_shapeRender->appendXPointShape("RotateCenter", { centerPos }, QVector3D(1, 1, 0));
		update();


	}
	void mBaseRend3D::SetRotateCenterToModelCenter()
	{
		ifGetRotateCenter = false;
		ifRotateAtViewCenter = false;
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			view->SetRotateCenterToModelCenter(_center_model, _maxRadius_model);
		}
		_center_now = _center_model;
		//传递旋转中心数据
		_shapeRender->appendXPointShape("RotateCenter", { _center_model }, QVector3D(1, 1, 0));
		update();


	}
	void mBaseRend3D::slotSetRotate_ByButton(float angle)
	{
		SetRotate_ByButton(angle);
	}
	void mBaseRend3D::SetRotate_ByButton(float angle)
	{
		auto view = dynamic_pointer_cast<mModelView>(_modelView);
		if (view)
		{
			view->Rotate_ByBotton(angle);
		}
		auto view1 = dynamic_pointer_cast<mCommonView>(_commonView);
		if (view1)
		{
			view1->Rotate_ByBotton(angle);
		}
		update();
	}

	//设置视图*旋转*类型
	void mBaseRend3D::SetRotateType(RotateType rotateType)
	{
		if (rotateType == Rotate_XY)
		{
			ifRotateAtXY = true;
			ifRotateAtZ = false;
			ifTranslateXY = false;
			ifZoomByMouseMove = false;
		}
		else if (rotateType == Rotate_Z)
		{
			ifRotateAtXY = false;
			ifRotateAtZ = true;
			ifTranslateXY = false;
			ifZoomByMouseMove = false;
		}
	}
}