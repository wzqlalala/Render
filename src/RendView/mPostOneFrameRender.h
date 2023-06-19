#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
#include <QObject>
#include <QOpenGLContext>
#include <set>

#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

#include "mPostCuttingPlaneRender.h"

namespace mxr
{
	class Application;
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
namespace MDataPost
{
	class mDataPost1;
	class mOneFrameData1;
	class mPostOneFrameRendData;
}
namespace MBaseRend
{
	class mFontRender;
	class mArrowRender;
}
using namespace MViewBasic;
using namespace MDataPost;
using namespace std;
namespace MPostRend
{
	class mPostRendStatus;
	class mPostModelRender;
	class mPostCuttingPlaneRender;
	class mPostColorTableRender;
	class mPostContourRender;
	class mPostStreamLineRender;
	class RENDVIEW_EXPORT mPostOneFrameRender
	{
	public:

		mPostOneFrameRender(std::shared_ptr<mxr::Application> app, shared_ptr<mPostRendStatus> rendStatus, mOneFrameData1 *oneFrameData, mPostOneFrameRendData *oneFrameRendData);

		~mPostOneFrameRender();

		void bufferThisFrame(QOpenGLContext *context);

		void deleteThieFrame();

		mOneFrameData1 *getOneFrameData() { return _oneFrameData; };

		mPostOneFrameRendData *getOneFrameRendData() { return _oneFrameRendData; };

		std::shared_ptr<mPostModelRender> getModelRender() {return _modelRender;};

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView);

		void resizeWindow(int w, int h);

		//void UpdateModelBuffer();

		//更新单个部件操作
		void updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>> postModelOperates);
		//更新全部部件操作
		void updateAllModelOperate(MViewBasic::PostModelOperateEnum postModelOperate);

		void setFaceStateSet(std::shared_ptr<mxr::StateSet> faceStateSet);
		void setFaceTransparentStateSet(std::shared_ptr<mxr::StateSet> faceTransparentStateSet);
		void setFaceTransparentNoDeformationStateSet(std::shared_ptr<mxr::StateSet> faceTransparentNoDeformationStateSet);
		void setEdgeLineStateSet(std::shared_ptr<mxr::StateSet> meshlineStateSet);
		void setFaceLineStateSet(std::shared_ptr<mxr::StateSet> facelineStateSet);
		void setLineStateSet(std::shared_ptr<mxr::StateSet> lineStateSet);
		void setPointStateSet(std::shared_ptr<mxr::StateSet> pointStateSet);

		void setTexture(mxr::Texture * texture);

		//设置变形倍数	
		void setDeformationScale(QVector3D deformationScale);

		//设置纹理坐标系数	
		void setTextureCoordScale(float textureCoordScale);

		//设置最大最小值	
		void setMinMaxData(float maxValue, float minValue);

		/*********************************切面图*******************************************/

		//删除切面图	
		bool deleteCuttingPlane(int num);

		//切面图法向量取反
		bool reverseCuttingPlaneNormal(int num);

		//仅仅只显示切面的图，不显示其他单元的云图
		void setOnlyShowCuttingPlane(bool isOnlyShowCuttingPlane);

		//设置是否开启某一个切面或者切割图
		void setIsShowCuttingPlane(int num, bool isShow);

		// 生成切面图
		bool createCuttingPlane(std::shared_ptr<mxr::StateSet> cuttingPlaneStateSet, std::shared_ptr<mxr::StateSet> transparentPlaneStateSet, int num, QVector3D normal = QVector3D(0, 0, 1), QVector3D vertex = QVector3D(0, 0, 0), bool hasVector = false);

		// 实时传递切割的那个平面信息
		void setPlaneData(int num, QVector3D normal = QVector3D(0, 0, 1), QVector3D centervertex = QVector3D(0, 0, 0), float maxR = 0.0);

		//设置是否实时显示平面
		void setIsShowPlane(bool isShow);

		/*********************************矢量图*******************************************/

		//生成矢量图
		void createVectorGraph(QVector<QPair<QString, QVector3D>> type_color, double size);

		//指定节点ID生成矢量图		
		void createVectorGraph(std::set<int> nodeIDs, QVector<QPair<QString, QVector3D>> type_color, double size);

		//删除矢量图
		void deleteVectorGraph();

		//自用
		void createVectorGraph(QPair<QVector<QVector3D>, QVector<QVector3D>> res);

		/**************************等值线等值面图*********************************************/

		//删除等值线等值面图
		void deleteContourGraph();

		//删除某一个等值线等值面图
		void deleteContourGraph(int i);

		//生成等值线等值面图
		void createContourGraph(std::shared_ptr<mxr::StateSet> lineStateSet, std::shared_ptr<mxr::StateSet> faceStateSet);

		//生成等值线等值面图	
		void createContourGraph(std::shared_ptr<mxr::StateSet> lineStateSet, std::shared_ptr<mxr::StateSet> faceStateSet, int i, double value, bool isshow = true);

		//设置等值线等值面图的显隐状态	
		void setContourGraph(int i, bool isshow);

		/**************************流线图*********************************************/

		//删除流线图
		void deleteStreamLine();

		//设置流线的显示形式
		void setStreamLineShowForm(int streamLineShowForm = 0);//0代表线条，1代表点

		//设置流线图积分方向
		void setIntergrateDirection(int intergrateDirection = 0);//0代表双向，1代表正向，2代表反向

		//生成流线图
		void createStreamLine(std::shared_ptr<mxr::StateSet> lineStateSet, std::shared_ptr<mxr::StateSet> pointStateSet, QVector3D center, float radius, int streamLineNum = 500, float ratio = 0.1);//中心，半径，流线密度（数量）,积分比例

		/*************************************获取数据*******************************************/
			
		//获得拾取到的的点		
		QPair<QVector<QVector3D>, QVector<QVector3D>>  getPickingNodeData(std::set<int> nodeIds);

		//获取被切剩下的云图的点
		QPair<QVector<QVector3D>, QVector<QVector3D>>  getCuttingNodeData();

		//获取切面数据
		template <class T>
		QPair<QVector<QVector3D>, QVector<T>> getCuttingPlaneData(int index, QHash<int, T> value, MViewBasic::NodeOrElement nodeOrElement)
		{
			QPair<QVector<QVector3D>, QVector<T>> result;

			result = _cuttingPlaneRenders[index]->getCuttingPlaneData()->getCuttingNodeData(value, nodeOrElement);
			return result;
		};

		//获取切面三角形数据
		QVector<QVector3D> getCuttingPlaneData(int index);
	private:

		void initial();

		void makeCurrent();

		void doneCurrent();
	private:
		std::shared_ptr<mxr::Application> _app;

		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		std::shared_ptr<mPostRendStatus> _rendStatus;

		mOneFrameData1 *_oneFrameData;//模型数据
		mPostOneFrameRendData *_oneFrameRendData;//当前帧变量数据

		std::shared_ptr<mPostModelRender> _modelRender;	//模型渲染

		std::shared_ptr<mPostColorTableRender> _colorTableRender;//颜色表渲染

		QVector<std::shared_ptr<mPostCuttingPlaneRender>> _cuttingPlaneRenders;//切面渲染

		QVector<std::shared_ptr<mPostContourRender>> _postContourRenders;//等值线面图渲染

		std::shared_ptr<mPostStreamLineRender> _postStreamLineRender;//流线图渲染

		std::shared_ptr<MBaseRend::mFontRender> _fontRender;//文字渲染

		std::shared_ptr<MBaseRend::mArrowRender> _arrowRender;//箭头渲染

		mxr::Texture * _texture;//颜色表纹理

		/*********************************模型*****************************************************/
		std::shared_ptr<mxr::StateSet> _faceStateSet;//渲染面的状态
		std::shared_ptr<mxr::StateSet> _faceTransparentStateSet;//渲染透明面的状态
		std::shared_ptr<mxr::StateSet> _faceTransparentNodeformationStateSet;//渲染模型透明面无变形的状态
		std::shared_ptr<mxr::StateSet> _facelineStateSet;//渲染面线的状态
		std::shared_ptr<mxr::StateSet> _edgelineStateSet;//渲染边界线的状态
		std::shared_ptr<mxr::StateSet> _lineStateSet;//渲染线的状态
		std::shared_ptr<mxr::StateSet> _pointStateSet;//渲染点的状态

		/***********************************切面******************************************************/
		std::shared_ptr<mxr::StateSet> _cuttingPlaneStateSet;//渲染面的状态
		std::shared_ptr<mxr::StateSet> _transparentPlaneStateSet;//渲染透明面的状态

		/************************************等值线等值面图*******************************************/
		std::shared_ptr<mxr::StateSet> _contourLineStateSet;//渲染等值线图的状态(同样适用于流线图的线的状态)
		std::shared_ptr<mxr::StateSet> _contourFaceStateSet;//渲染等值面图的状态

		/************************************流线图*******************************************/
		std::shared_ptr<mxr::StateSet> _streamlinePointStateSet;//渲染流线图点的状态

		/*************************************当前帧渲染********************************************************/
		std::shared_ptr<mxr::Viewer> _viewer;
	};
	
}
