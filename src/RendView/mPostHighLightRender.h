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
	class mViewBase;
}
namespace MDataPost
{
	class mDataPost1;
	class mOneFrameData1;
	class mPostOneFrameRendData;
	class mPostMeshPickData;
}
using namespace MViewBasic;
using namespace MDataPost;
using namespace MBaseRend;
using namespace std;
namespace MPostRend
{
	class mPostRendStatus;
	class mPostModelRender;
	class mPostCuttingPlaneRender;
	class RENDVIEW_EXPORT mPostHighLightRender
	{
	public:

		mPostHighLightRender(shared_ptr<mPostRendStatus> rendStatus, mPostMeshPickData *meshPickData);

		~mPostHighLightRender();

		void updateHighLightRender(mOneFrameData1 *oneFrameData, mPostOneFrameRendData* oneFrameRendData);

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView);

		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> meshlineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

	private:

		void initial();
	private:
		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		std::shared_ptr<mPostRendStatus> _rendStatus;

		mOneFrameData1 *_oneFrameData;//模型数据
		mPostOneFrameRendData *_oneFrameRendData;//当前帧变量数据
		mPostMeshPickData *_meshPickData;

		std::shared_ptr<mxr::StateSet> _facelineStateSet;//渲染面线的状态
		std::shared_ptr<mxr::StateSet> _lineStateSet;//渲染线的状态
		std::shared_ptr<mxr::StateSet> _pointStateSet;//渲染点的状态

		std::shared_ptr<mGroupRender1<mxr::Vec3Array>> _lineRender;
		std::shared_ptr<mGroupRender1<mxr::Vec3Array>> _pointRender;
		std::shared_ptr<mGroupRender2<mxr::Vec3Array, mxr::FloatArray>> _facelineRender;

		std::shared_ptr<mxr::Viewer> _viewer;
	};
	
}
