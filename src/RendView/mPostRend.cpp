#include "mPostRend.h"
#include "mPostRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"
#include "mVideoRender.h"
#include "mVideoRender1.h"
#include "mPostOneFrameRender.h"
#include "mPostModelRender.h"

#include "mModelView.h"

//AppConfiguration
#include <mAppConfiguration.h>

////MxRender
#include <renderpch.h>

namespace MPostRend
{

	mPostRend::mPostRend(const QString& name):mBaseRend3D(name, ViewportPost3D)
	{
		*_pickFilter = PickFilter::PickNode;
		//qDebug() << "Post Struct";

		//保存单位制
		QString viewStyle = MAppConfiguration::mAppConfiguration::getInstance()->_viewStyle;

		this->clearCameraKeys();
		this->clearPickKeys();
		if (viewStyle == "Abaqus")
		{
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::MiddleButton, Qt::ControlModifier | Qt::AltModifier), CameraOperateMode::Zoom);
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ControlModifier | Qt::AltModifier), CameraOperateMode::Rotate);
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ControlModifier | Qt::AltModifier), CameraOperateMode::Translate);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::NoModifier), PickMode::SoloPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::NoModifier), PickMode::SoloPick);
		}
		else if (viewStyle == "HyperView")
		{
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::MiddleButton, Qt::ControlModifier), CameraOperateMode::Zoom);
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ControlModifier), CameraOperateMode::Rotate);
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ControlModifier), CameraOperateMode::Translate);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::NoModifier), PickMode::SoloPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::NoModifier), PickMode::SoloPick);
		}
		else
		{
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::MiddleButton, Qt::NoModifier), CameraOperateMode::Zoom);
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::NoModifier), CameraOperateMode::Rotate);
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::NoModifier), CameraOperateMode::Translate);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ControlModifier), PickMode::SoloPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ControlModifier), PickMode::SoloPick);
		}

		//让其自动进入OpenGL初始化
		this->setGeometry(-1, -1, 1, 1);
		this->show();
	}

	void mPostRend::initializeGL()
	{
		mBaseRend3D::initializeGL();

		makeCurrent();

		shared_ptr<mPostRender> postRender = make_shared<mPostRender>(_app, _root, this);
		this->addRender(postRender);	

		_videoRender = make_shared<mVideoRender1>(_app, _root);
		this->addRender(_videoRender);

		_fontRender->appendGloabalAxisFont();
		_arrowRender->appendGloabalAxisArrow();

		//qDebug() << "Post Initial";
	}

	void mPostRend::paintGL()
	{
		//glLineWidth(5.0);
		mBaseRend3D::paintGL();
		GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}

	}

	void mPostRend::resizeGL(int w, int h)
	{
		makeCurrent();
		mBaseRend3D::resizeGL(w, h);
	}
	void mPostRend::mousePressEvent(QMouseEvent *event)
	{
		mBaseRend3D::mousePressEvent(event);
	}
	void mPostRend::mouseReleaseEvent(QMouseEvent *event)
	{
		mBaseRend3D::mouseReleaseEvent(event);
	}
	void mPostRend::mouseMoveEvent(QMouseEvent *event)
	{
		mBaseRend3D::mouseMoveEvent(event);
	}
	void mPostRend::wheelEvent(QWheelEvent *event)
	{
		mBaseRend3D::wheelEvent(event);
	}

	mPostRend::~mPostRend()
	{

		//qDebug() << "Post Distruct";
	}
	shared_ptr<mPostRender> mPostRend::getPostRender()
	{
		for (auto render : _renderArray)
		{
			auto post = dynamic_pointer_cast<mPostRender>(render);
			if (post)
			{
				return post;
			}
		}

		return nullptr;
	}

	void mPostRend::GetModelSizePara(bool isModelCenter)
	{
		Space::AABB aabb = this->getPostRender()->getCurrentModelData();
	
		_aabb = aabb;
		//模型中心
		_center_model = (_aabb.maxEdge + _aabb.minEdge) / 2.0;
		_maxRadius_model = _aabb.maxEdge.distanceToPoint(_aabb.minEdge);
		if (isinf(_maxRadius_model) || qFuzzyCompare(0, _maxRadius_model))
		{
			_maxRadius_model = _maxRadius_now;
		}
		if (isModelCenter)
		{
			_center_now = _center_model;
			_maxRadius_now = _maxRadius_model;
		}
		else
		{
			float t = _maxRadius_now;
			_maxRadius_now = _aabb.maxEdge.distanceToPoint(_center_now);
			float max_x = std::max(std::abs(_aabb.minEdge.x() - _center_now.x()), std::abs(_aabb.maxEdge.x() - _center_now.x()));
			float max_y = std::max(std::abs(_aabb.minEdge.y() - _center_now.y()), std::abs(_aabb.maxEdge.y() - _center_now.y()));
			float max_z = std::max(std::abs(_aabb.minEdge.z() - _center_now.z()), std::abs(_aabb.maxEdge.z() - _center_now.z()));
			_maxRadius_now = sqrt(pow(max_x, 2) + pow(max_y, 2) + pow(max_z, 2));
			if (isinf(_maxRadius_now) || qFuzzyCompare(0, _maxRadius_now))
			{
				_maxRadius_now = t;
			}
		}
	}
	void mPostRend::slotResetOrthoAndCamera()
	{
		GetModelSizePara(true);
		_modelView->ResetOrthoAndCamera(_center_model, _maxRadius_model);
		_modelView->SaveCurrentView();
		_commonView->SaveCurrentView();
		//_meshModelRulerRend->UpdateNum();
		update();
	}

	void mPostRend::slotUpdateOrthoAndCamera()
	{
		GetModelSizePara(false);
		_modelView->UpdateOrthoAndCamera(_maxRadius_model, _maxRadius_now);
		_modelView->SaveCurrentView();
		_commonView->SaveCurrentView();
		//_meshModelRulerRend->UpdateNum();
		//HideOrShowAllFont();
		update();

	}

	void mPostRend::slotUpdateOrthoAndCamera(QVector<QVector3D> addVertex)
	{
		GetModelSizePara(false);
		_modelView->UpdateOrthoAndCamera(_maxRadius_now, addVertex);
		_modelView->SaveCurrentView();
		_commonView->SaveCurrentView();
		//_meshModelRulerRend->UpdateNum();
		//HideOrShowAllFont();
		update();
	}
}