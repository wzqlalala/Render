#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QFutureWatcher>

//MViewBasic
#include "mMeshViewEnum.h"

//MViewBasic
#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

#include "SpaceTree.h"

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
}
namespace MDataGeo
{
	class mGeoModelData1;
	class mGeoPickData1;
	class mPreGeoPickThread;
}

using namespace MViewBasic;
using namespace MViewBasic;
using namespace MBaseRend;
using namespace MDataGeo;
using namespace std;
namespace MPreRend
{
	class mPreRend;
	class mPreRendStatus;
	class mPreGeoModelRender;
	class mPreGeoHighLightRender;
	class mPreMeshModelRender;
	class RENDVIEW_EXPORT mPreRender :public mBaseRender
	{
		Q_OBJECT
	public:

		mPreRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mPreRend *preRend);

		bool getIsDragSomething(QVector2D pos) override;

		void dragSomething(QVector2D pos) override;

		void startPick(QVector<QVector2D> poses) override;

		bool getModelAABB(Space::AABB &aabb);

		//获取几何模型渲染数据
		mGeoModelData1 *getGeoModelData() { return _geoModelData; }; 

		//获取集合模型拾取数据
		mGeoPickData1 *getGeoPickData() { return _geoPickData; };

		//更新高亮渲染
		void updateHighLightRender();

		//更新部件操作
		void updateModelOperate(QPair<MxFunctions::ModelOperateEnum, std::set<QString>> modelOperates);
		
		//清除全部的渲染
		void clearRender();
		
		//设置当前的显示模式	
		void setShowFuntion(ShowFuntion shoFuntion);

		//设置点的大小
		void setPointSize(int size);

		//设置边界线的宽度
		void setEdgeLineWidth(float size);

		//设置线的颜色
		void setFaceLineColor(QVector4D color);


		~mPreRender();

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView) override;

	signals:
		//拾取完成信号
		void finishedPickSig();

	protected:

		shared_ptr<mPreRendStatus> _rendStatus;

		QFutureWatcher<void> w;//监控拾取多线程

		shared_ptr<mPreGeoModelRender> _geoModelRender;//几何渲染
		mGeoModelData1 *_geoModelData;//几何模型数据
		mPreGeoPickThread *_geoPickThread;//几何拾取
		mGeoPickData1 *_geoPickData;//几何拾取数据
		shared_ptr<mPreGeoHighLightRender> _geoHighLightRender;//几何高亮数据

		shared_ptr<mPreMeshModelRender> _meshModelRender;//网格渲染

		shared_ptr<mxr::Texture> _pointTexture;//几何点的纹理

		/*********************************模型*****************************************************/
		std::shared_ptr<mxr::StateSet> _faceStateSet;//渲染面的状态
		std::shared_ptr<mxr::StateSet> _independentlineStateSet;//渲染独立线的状态
		std::shared_ptr<mxr::StateSet> _dotlineStateSet;//渲染虚线的状态
		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//渲染边界线的状态
		std::shared_ptr<mxr::StateSet> _pointStateSet;//渲染点的状态
		std::shared_ptr<mxr::StateSet> _facelineStateSet;//渲染面线的状态

		//计时器
		QTimer* _aniTimer;

		//拾取线程
		////前面的部件数量少
		//mPostMeshPickThread *_thread; QFutureWatcher<void> w;
		//mPostMeshPickData *_pickData;
		//shared_ptr<mPreHighLightRender> _highLightRender;
		
	};
}
