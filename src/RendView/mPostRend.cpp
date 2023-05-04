#include "mPostRend.h"
#include "mPostRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"

////MxRender
#include <renderpch.h>

namespace MPostRend
{

	mPostRend::mPostRend(const QString& name):mBaseRend( name)
	{
		qDebug() << "Post Struct";
	}

	void mPostRend::initializeGL()
	{
		mBaseRend::initializeGL();

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
		mBaseRend::paintGL();
		GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}

	}

	void mPostRend::resizeGL(int w, int h)
	{
		mBaseRend::resizeGL(w, h);
	}
	void mPostRend::mousePressEvent(QMouseEvent *event)
	{
		mBaseRend::mousePressEvent(event);
	}
	void mPostRend::mouseReleaseEvent(QMouseEvent *event)
	{
		mBaseRend::mouseReleaseEvent(event);
	}
	void mPostRend::mouseMoveEvent(QMouseEvent *event)
	{
		mBaseRend::mouseMoveEvent(event);
	}
	void mPostRend::wheelEvent(QWheelEvent *event)
	{
		mBaseRend::wheelEvent(event);
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