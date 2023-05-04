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
namespace MDataGeo
{
	class mGeoModelData1;
	class mGeoPickData1;
}
using namespace MViewBasic;
using namespace MDataGeo;
using namespace MBaseRend;
using namespace std;
namespace MPreRend
{
	class mPreRendStatus;
	class RENDVIEW_EXPORT mPreGeoHighLightRender
	{
	public:

		mPreGeoHighLightRender(shared_ptr<mxr::Group> parent, shared_ptr<mPreRendStatus> rendStatus, mGeoPickData1 *meshPickData, mGeoModelData1 *geoModelRend);

		~mPreGeoHighLightRender();

		void updateHighLightRender();

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView);

		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> meshlineStateSet);
		void setFaceStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

	private:

		void initial();
	private:
		shared_ptr<mxr::Group> _parent;//���ڵ�

		std::shared_ptr<mxr::Geode> _geode;//��ǰ�ܽڵ�

		std::shared_ptr<mPreRendStatus> _rendStatus;

		//��������
		mGeoModelData1 *_geoModelData;
		mGeoPickData1 *_geoPickData;

		std::shared_ptr<mxr::StateSet> _faceStateSet;//��Ⱦ���״̬
		std::shared_ptr<mxr::StateSet> _lineStateSet;//��Ⱦ�ߵ�״̬
		std::shared_ptr<mxr::StateSet> _pointStateSet;//��Ⱦ���״̬

		std::shared_ptr<mGroupRender1<mxr::Vec3Array>> _lineRender;
		std::shared_ptr<mGroupRender1<mxr::Vec3Array>> _pointRender;
		std::shared_ptr<mGroupRender1<mxr::Vec3Array>> _faceRender;

		//std::shared_ptr<mxr::Viewer> _viewer;
	};
	
}
