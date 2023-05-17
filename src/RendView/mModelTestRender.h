#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

#include "mBaseRender.h"
//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <QObject>

namespace mxr
{
	class Application;
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
		std::shared_ptr<mxr::Drawable> _drawable; //设置顶点属性；设置StateSet；将其添加进父节点中;
		std::shared_ptr<mxr::StateSet> _stateSet;//设置着色器Shader；设置渲染状态详见StateAttribute；设置纹理Texture；设置渲染模式；设置uniform值；

		mxr::Texture * _diffuseMap;
		mxr::Texture * _normalMap;
	};
}
