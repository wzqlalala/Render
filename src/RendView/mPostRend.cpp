#include "mPostRend.h"
#include "mPostRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"
#include "mVideoRender.h"
#include "mVideoRender1.h"

//AppConfiguration
#include <mAppConfiguration.h>

////MxRender
#include <renderpch.h>

namespace MPostRend
{

	mPostRend::mPostRend(const QString& name):mBaseRend3D(name, ViewportPost3D)
	{
		*_pickFilter = PickFilter::PickNode;
		qDebug() << "Post Struct";

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

		qDebug() << "Post Initial";
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

		qDebug() << "Post Distruct";
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
}