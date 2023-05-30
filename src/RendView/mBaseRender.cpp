#include "mBaseRender.h"
#include "mBaseRend.h"

#include "mViewBase.h"

#include "app.h"

namespace MBaseRend
{
	mBaseRender::mBaseRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mBaseRend* baseRend):_app(app),_parent(parent),_baseRend(baseRend)
	{
	
	}

	mBaseRender::~mBaseRender()
	{
	}

	void mBaseRender::setPickFilter(PickFilter pickFilter)
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

	float mBaseRender::getDepth(QVector2D pos)
	{
		int w = 9;
		int h = 9;
		float *depths = new float[w * h];//获取距离中心最近不为1.0的深度值
		//float depths[5][5];
		int x = w / 2, y = h / 2;
		float depth = 1.0;
		int minmag = pow(x, 2) + pow(y, 2);
		int height = _baseRend->getCamera()->SCR_HEIGHT;
		QOpenGLContext::currentContext()->functions()->glReadPixels(pos.x() - x, height - pos.y() - y, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, depths);
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				if (depths[i * w + j] != 1.0)
				{
					int mag = (pow((i - x), 2) + pow((j - y), 2));
					if (mag < minmag)
					{
						minmag = mag;
						depth = depths[i * w + j];
					}
				}
			}
		}
		delete depths;
		return depth;
	}

}