#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

#include "mBaseRender.h"
#include "app.h"
//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
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
	class mModelView;
	class mViewBase;
	class mViewBase;
}


using namespace MViewBasic;
namespace MBaseRend
{
	class RENDVIEW_EXPORT mModelTestRender :public mBaseRender
	{
	public:

		mModelTestRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent);

		~mModelTestRender();

		QPair<QVector3D, float> setData(const tinyobj::attrib_t &attrib, const std::vector<tinyobj::shape_t> &shapes,const std::vector<tinyobj::material_t> &materials);

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView) override;

	private:
		std::shared_ptr<mxr::Drawable> _drawable; //���ö������ԣ�����StateSet��������ӽ����ڵ���;
		std::shared_ptr<mxr::StateSet> _stateSet;//������ɫ��Shader��������Ⱦ״̬���StateAttribute����������Texture��������Ⱦģʽ������uniformֵ��

		shared_ptr<mxr::Texture> _diffuseMap;
		shared_ptr<mxr::Texture> _normalMap;
	};
}
