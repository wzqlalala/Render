#pragma once
#include "rendview_global.h"
//�����������
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

		std::shared_ptr<mxr::StateSet> _stateSet;//������ɫ��Shader��������Ⱦ״̬���StateAttribute����������Texture��������Ⱦģʽ������uniformֵ��

		mxr::Texture * _diffuseMap;
		mxr::Texture * _normalMap;
	};
}


