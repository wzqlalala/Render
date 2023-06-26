#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")
#include "mBaseRender.h"
//#include "app.h"
#include <QObject>
#include <QTimer>
#include <QThread>
#include <QVector3D>
#include <QPair>
#include <set>
#include <QFutureWatcher>

//MViewBasic
#include "mMeshViewEnum.h"

//MViewBasic
#include "mElementFunction.h"
//#include "mBasicStructor.h"

#include "mPostOneFrameRender.h"
#include "mPostRendStatus.h"

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
	class Application;
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
	class mPostAnimationRendData;
	class mPostMeshPickThread;
	class mPostMeshPickData;
}
using namespace MViewBasic;
using namespace MDataPost;
using namespace MViewBasic;
using namespace MBaseRend;
using namespace MxFunctions;
using namespace std;
namespace MPostRend
{
	class mPostRend;
	class mPostRendStatus;
	class mPostHighLightRender;
	class mPostTempHighLightRender;
	class mPostOneFrameRender;
	class mPostAnimationRender;
	class mPostFrameText;
	class mPostDragRender;
	class RENDVIEW_EXPORT mPostRender :public mBaseRender
	{
		Q_OBJECT
	public:

		mPostRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mPostRend *postRend);

		void setPostData(shared_ptr<mDataPost1> dataPost) { _dataPost = dataPost;};

		bool getIsDragSomething(QVector2D pos) override;

		void dragSomething(QVector2D pos) override;

		void startPick(QVector<QVector2D> poses) override;

		shared_ptr<mPostOneFrameRender> getOneFrameRender() { return  _oneFrameRender; };

		mPostMeshPickData *getMeshPickData() { return _pickData; };
	
		//获取当前屏幕文字渲染的数据
		mPostFrameText* getPostFrameText();

		//更新高亮渲染
		void updateHighLightRender();

		//更新单个部件操作
		void updateOneModelOperate(QPair<MViewBasic::PostModelOperateEnum, std::set<QString>> postModelOperates);
		//更新全部部件操作
		void updateAllModelOperate(MViewBasic::PostModelOperateEnum postModelOperate);
	
		//获取动画的模型尺寸	
		Space::AABB getCurrentModelData();

		//设置后处理渲染模式
		void setPostMode(PostMode postMode);
		
		//清除全部的渲染
		void clearRender();

		//设置当前帧的渲染数据
		void setRendCurrentFrameData(mPostOneFrameRendData *postOneFrameRendData);
		
		//设置当前的显示模式	
		void setShowFuntion(ShowFuntion shoFuntion);
		
		//设置离散开关	
		void setDispersed(bool isdispersed);
		
		//设置变形倍数	
		void setDeformationScale(QVector3D deformationScale);

		//设置是否显示初始构型的透明图形
		void setIsShowInitialShape(bool isShowInitialShape);
	
		//设置纹理坐标系数	
		void setTextureCoordScale(float textureCoordScale);
	
		//设置最大最小值	
		void setMinMaxData(float maxValue, float minValue);
	
		//设置渲染范围(0-所有部件（自定义）；1-当前渲染部件)	
		void setRangeType(int index);

		//设置离散类型，是否等差	
		void setDispersIsEquivariance(bool isEquivariance);

		//设置点的大小
		void setPointSize(int size);

		//设置边界线的宽度
		void setEdgeLineWidth(float size);

		//设置线的颜色
		void setFaceLineColor(QVector4D color);

		//设置是否开启光源
		void setLightIsOn(bool isOn);
		
		//设置光源是否跟随摄像机	
		void setLightIsDependentCamera(bool lightIsDependOnCamera);
	
		//设置光源的位置
		void setLightPosition(QVector3D position);
		
		//设置光源的参数		
		void setLightParameters(QVector3D ambient, QVector3D diffuse, QVector3D specular, float shiness);

		/*********************************切面图*******************************************/
		
		//删除切面图	
		void deleteCuttingPlane(int num);
	
		//切面图法向量取反
		void reverseCuttingPlaneNormal(int num);

		//仅仅只显示切面的图，不显示其他单元的云图
		void setOnlyShowCuttingPlane(bool isOnlyShowCuttingPlane);

		//设置是否开启某一个切面或者切割图
		void setIsShowCuttingPlane(int num, bool isShow);

		// 生成切面图
		void createCuttingPlane(int num, QVector3D normal = QVector3D(0, 0, 1), QVector3D vertex = QVector3D(0, 0, 0), bool hasVector = false);

		// 实时传递切割的那个平面信息
		void setPlaneData(int num, QVector3D normal = QVector3D(0, 0, 1), QVector3D centervertex = QVector3D(0, 0, 0), float maxR = 0.0);

		//设置是否实时显示平面
		void setIsShowPlane(bool isShow);

		/**********************************动画*********************************************/

		//设置动画帧
		void setAnimationFrame(int start, int end, int current);

		//设置动画速率
		void setAnimationFrameRate(int fr);

		//设置动画间隔
		void setAnimationFrameInterval(int f);

		//设置循环播放
		void setAnimationLoopPlay(bool istrue);

		//设置瞬态动画的渲染数据	
		void setRendAnimationFrame(std::shared_ptr<mPostAnimationRendData> allFrameRendData);

		//设置瞬态动画渲染范围是否统一,真的话则是统一的渲染范围，否的话则把每帧的范围还原为初始值
		void setAnimationFrameRange(bool isAgreement);
	
		//设置瞬态动画渲染自定义范围	
		void setAnimationFrameRange(float maxValue, float minValue);

		//生成单帧动画
		void createLinearAnimation(PostMode postMode);

		//删除动画
		void deleteAnimation();
		
		//控制计时器开关		
		void setTimerOn(bool ison);

		/**************************矢量图*********************************************/
		
		//生成矢量图	
		void createVectorGraph(QVector<QPair<QString, QVector3D>> type_color, double size);
		
		//指定节点ID生成矢量图		
		void createVectorGraph(std::set<int> nodeIDs, QVector<QPair<QString, QVector3D>> type_color, double size);

		//删除矢量图
		void deleteVectorGraph();

		/**************************等值线等值面图*********************************************/
		
		//删除等值线等值面图
		void deleteContourGraph();

		//删除某一个等值线等值面图
		void deleteContourGraph(int i);
		
		//生成等值线等值面图
		void createContourGraph();
		
		//生成等值线等值面图	
		void createContourGraph(int i, double value, bool isshow = true);
		
		//设置等值线等值面图的显隐状态	
		void setContourGraph(int i, bool isshow);

		/**************************流线图*********************************************/

		//设置积分球的参数
		void setSphereData(QVector3D center, float radius);

		//删除流线图
		void deleteStreamLine();

		//设置流线的显示形式
		void setStreamLineShowForm(int streamLineShowForm = 0);//0代表线条，1代表点

		//设置流线图积分方向
		void setIntergrateDirection(int intergrateDirection = 0);//0代表双向，1代表正向，2代表反向

		//设置是否显示流线积分球
		void setIsShowSphere(bool isShow);

		//获取流线积分球的位置
		QVector3D getDragSphereCenter();

		//生成流线图
		void createStreamLine(QVector3D center, float radius, int streamLineNum = 500, float ratio = 0.1);//中心，半径，流线密度（数量）,积分比例

		/**************************最小最大值*********************************************/
		
		//设置最小值是否显示	
		void setMinIsShow(bool isshow);
		
		//设置最大值是否显示		
		void setMaxIsShow(bool isshow);
	
		//获取最小最大值的位置		
		void getMinMaxLocation();

		//计算最小最大值的线条位置（在模型发生变化时，切换帧或者变形系数时）
		void calculateMinMaxLinePosition();

		~mPostRender();

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView) override;

		void resizeWindow(int w, int h) override;

		/*********************************获取数据***************************************************/	

		//获取当前帧的节点位置	
		QVector3D getCurrentNodeVertex(int id);

		//通过零件名称获取单元ID	
		set<int> getMeshIDsByPartNames(MeshType meshType, set<QString> partNames);
	
		//获取全部单元ID
		set<int> getAllMeshIDs(MeshType meshType);
	
		//通过单元ID获取节点坐标
		QVector<QVector3D> getNodeVertexByMeshID(int meshID);

		//通过切面索引来获取切面的数据
		template <class T>
		QPair<QVector<QVector3D>, QVector<T>> getCuttingPlaneData(int cuttingPlaneIndex, QHash<int, T> value, MViewBasic::NodeOrElement nodeOrElement)
		{
			QPair<QVector<QVector3D>, QVector<T>> result;
			if (cuttingPlaneIndex >= _rendStatus->_cuttingPlanes.size())
			{
				return result;
			}
			if (_oneFrameRender != nullptr)
			{
				result = _oneFrameRender->getCuttingPlaneData(cuttingPlaneIndex, value, nodeOrElement);
			}
			return result;
		}

		//通过切面名称来获取切面的顶点数据
		QVector<QVector3D> getCuttingPlaneData(int cuttingPlaneIndex);

		//通过切面名称来获取切面的法向
		QVector3D getCuttingPlaneNormal(int cuttingPlaneIndex);

	private:

		//更新切面的uniform值
		void updateCuttingPlaneUniform();

		//初始化线程
		void initialPickThreads();

		//
	private slots:

		//计时器更新	
		void slot_aniTimer();

		//完成拖拽
		void slot_finsishedDrag();

	signals:

		//动画帧数更新
		void updateAnimationPanelSig(int);

		//拾取完成
		void finishedPickSig();

		//拖拽完成
		void finishedDragSig();

	protected:
		shared_ptr<mDataPost1> _dataPost;

		shared_ptr<mPostRendStatus> _rendStatus;

		//shared_ptr<mPostAnimationRender> _animationRender;//动画帧

		//动画帧的渲染
		QHash<int, shared_ptr<mPostOneFrameRender>> _animationRender;//动画帧
		shared_ptr<mPostAnimationRendData> _rendAnimationData;

		shared_ptr<mPostOneFrameRender> _oneFrameRender;//单帧

		shared_ptr<mPostOneFrameRender> _oneFrameAnimationRender;//单帧动画

		//拖拽的物体
		QHash<QString,shared_ptr<mPostDragRender>> _dragRenders;
		shared_ptr<mPostDragRender> _currentDragRender;

		//屏幕文字数据（除颜色表外）
		mPostFrameText *_postFrameText;

		mxr::Texture * _texture;

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

		//计时器
		QTimer* _aniTimer;

		//拾取线程
		////前面的部件数量少
		mPostMeshPickThread *_thread; QFutureWatcher<void> w;
		mPostMeshPickData *_pickData;
		shared_ptr<mPostHighLightRender> _highLightRender;

		//临时高亮
		shared_ptr<mPostTempHighLightRender> _tempHighLightRender;
		
	};
}
