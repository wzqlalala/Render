#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include "app.h"

#include <QObject>

namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Texture;
}
namespace MViewBasic
{
	class mViewBase;
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

		std::shared_ptr<mxr::StateSet> _stateSet;//������ɫ��Shader��������Ⱦ״̬���StateAttribute����������Texture��������Ⱦģʽ������uniformֵ��

		shared_ptr<mxr::Texture> _diffuseMap;
		shared_ptr<mxr::Texture> _normalMap;
	};
}


