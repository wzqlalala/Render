#include "mPostRend.h"
#include "mPostRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"

////MxRender
#include <renderpch.h>

namespace MPostRend
{

	mPostRend::mPostRend(const QString& name):mBaseRend3D(name, ViewportPost3D)
	{
		*_pickFilter = PickFilter::PickNode;
		qDebug() << "Post Struct";

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