#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"

#include <QObject>

namespace MViewBasic
{
	class mViewBase;
}
namespace mxr
{
	class Texture;
	class StateSet;
}
using namespace MViewBasic;
namespace MBaseRend
{
	class RENDVIEW_EXPORT mTestRender:public mBaseRender
	{
	public:

		mTestRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent);

		~mTestRender();

		void appendOnePart();

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView) override;

	private:

		std::shared_ptr<mxr::StateSet> _stateSet;//设置着色器Shader；设置渲染状态详见StateAttribute；设置纹理Texture；设置渲染模式；设置uniform值；

		mxr::Texture * _diffuseMap;
		mxr::Texture * _normalMap;
	};
}


