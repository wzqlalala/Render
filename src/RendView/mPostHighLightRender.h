#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")
//#define TINYOBJLOADER_IMPLEMENTATION
#include <QObject>
#include <QOpenGLContext>
#include <set>

#include "mBasicEnum.h"
#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

#include "Array.h"

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
using namespace std;
namespace MPostRend
{
	class mPostRendStatus;
	class mPostModelRender;
	class mPostCuttingPlaneRender;
	class mPostBaseHighLightRender//����ߺͱ߽���
	{
	public:
		std::shared_ptr<mxr::Vec3Array>  _vertex0;

		mPostBaseHighLightRender(std::shared_ptr<mxr::Group> parent);
		~mPostBaseHighLightRender();
		std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
		void setStateSet(std::shared_ptr<mxr::StateSet> state);
	private:
		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::Group> _parent;
	};
	class mPostBaseHighLightRender1//����
	{
	public:
		std::shared_ptr<mxr::Vec3Array>  _vertex0;
		std::shared_ptr<mxr::FloatArray>  _vertex1;

		mPostBaseHighLightRender1(std::shared_ptr<mxr::Group> parent);
		~mPostBaseHighLightRender1();
		std::shared_ptr<mxr::Drawable> getDrawable() { return _drawable; }
		void setStateSet(std::shared_ptr<mxr::StateSet> state);
	private:
		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::Group> _parent;
	};
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
		std::shared_ptr<mxr::Geode> _geode;//��ǰ�ܽڵ�

		std::shared_ptr<mPostRendStatus> _rendStatus;

		mOneFrameData1 *_oneFrameData;//ģ������
		mPostOneFrameRendData *_oneFrameRendData;//��ǰ֡��������
		mPostMeshPickData *_meshPickData;

		std::shared_ptr<mxr::StateSet> _facelineStateSet;//��Ⱦ���ߵ�״̬
		std::shared_ptr<mxr::StateSet> _lineStateSet;//��Ⱦ�ߵ�״̬
		std::shared_ptr<mxr::StateSet> _pointStateSet;//��Ⱦ���״̬

		std::shared_ptr<mPostBaseHighLightRender> _lineRender;
		std::shared_ptr<mPostBaseHighLightRender> _pointRender;
		std::shared_ptr<mPostBaseHighLightRender1> _facelineRender;

		std::shared_ptr<mxr::Viewer> _viewer;
	};
	
}
