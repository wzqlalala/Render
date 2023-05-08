#include "mBaseRender.h"
#include "mBaseRend.h"

#include "app.h"

namespace MBaseRend
{
	mBaseRender::mBaseRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mBaseRend* baseRend):_app(app),_parent(parent),_baseRend(baseRend)
	{
	
	}

	mBaseRender::~mBaseRender()
	{
	}

	void mBaseRender::setPickFilter(MViewBasic::PickFilter pickFilter)
	{
		if (_baseRend)
		{
			_baseRend->setPickFilter(pickFilter);
		}
	}

	void mBaseRender::setMultiplyPickMode(MultiplyPickMode multiplyPickMode)
	{
		if (_baseRend)
		{
			_baseRend->setMultiplyPickMode(multiplyPickMode);
		}
	}

	void mBaseRender::makeCurrent()
	{
		_app->GLContext()->makeCurrent(_app->GLContext()->surface());
	}

	void mBaseRender::doneCurrent()
	{
		_app->GLContext()->doneCurrent();
	}

}