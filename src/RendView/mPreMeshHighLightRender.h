#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
//#define TINYOBJLOADER_IMPLEMENTATION
#include <QObject>
#include <QOpenGLContext>
#include <set>

#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

#include "mGroupRender.h"

namespace mxr
{
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Geode;
	class Texture;
	class Viewer;
}
namespace MViewBasic
{
	class mModelView;
	class mViewBase;
}
namespace MDataPre
{
	class mPreMeshPickData1;
}
using namespace MDataPre;
using namespace MViewBasic;
using namespace MBaseRend;
using namespace std;
namespace MPreRend
{
	class mPreRendStatus;
	class RENDVIEW_EXPORT mPreMeshHighLightRender
	{
	public:

		mPreMeshHighLightRender(shared_ptr<mPreRendStatus> rendStatus, mPreMeshPickData1 *meshPickData);

		~mPreMeshHighLightRender();

		void updateHighLightRender();

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView);

		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> meshlineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

	private:

		void initial();
	private:
		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		std::shared_ptr<mPreRendStatus> _rendStatus;

		mPreMeshPickData1 *_meshPickData;

		std::shared_ptr<mxr::StateSet> _facelineStateSet;//渲染面线的状态
		std::shared_ptr<mxr::StateSet> _lineStateSet;//渲染线的状态
		std::shared_ptr<mxr::StateSet> _pointStateSet;//渲染点的状态

		//std::shared_ptr<mGroupRender1<mxr::Vec3Array>> _edgelineRender;
		std::shared_ptr<mGroupRender1<mxr::Vec3Array>> _lineRender;
		std::shared_ptr<mGroupRender1<mxr::Vec3Array>> _pointRender;
		std::shared_ptr<mGroupRender2<mxr::Vec3Array, mxr::FloatArray>> _facelineRender;

		std::shared_ptr<mxr::Viewer> _viewer;
	};
	
}
