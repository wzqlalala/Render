#pragma once
#include "rendview_global.h"

#include <QVector3D>
#include <QVector4D>
#include <QVector>
#include <QHash>

#include <memory>

namespace MDataPost
{
	class mPostMeshPartData1;
	class mPostMeshData1;
	class mPostMeshNodeData1;
	class mPostMeshData1;
	class mPostMeshLineData1;
	class mPostMeshFaceData1;
	class mOneFrameData1;
	class mPostOneFrameRendData;
}
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
}
using namespace MDataPost;
namespace MPostRend
{
	class mPostRendStatus;
	class RENDVIEW_EXPORT mPostStreamLineData1
	{
	public:
		mPostStreamLineData1(mOneFrameData1 *oneFrameData, mPostOneFrameRendData *currentFrameRendData = nullptr, int num = 500, float ratio = 0.1);

		~mPostStreamLineData1();

		void setSphereParameter(QVector3D center, float radius);

		void setStreamLineNum(int num = 500);

		void OrganizeModeldata();

		QVector<QVector3D> getForwardStreamLineVertex();

		QVector<float> getForwardStreamLineTexture();

		QVector<QVector3D> getBackwardStreamLineVertex();

		QVector<float> getBackwardStreamLineTexture();

	private:

		//六面体分割成六个四面体
		void splitHexahedron(QVector<int> index, MDataPost::mPostMeshData1 *meshData, MDataPost::mPostMeshPartData1 *partData);

		//三棱柱分割成三个四面体
		void splitWedge(QVector<int> index, MDataPost::mPostMeshData1 *meshData, MDataPost::mPostMeshPartData1 *partData);

		//四棱锥分割成两个四面体
		void splitPyramid(QVector<int> index, MDataPost::mPostMeshData1 *meshData, MDataPost::mPostMeshPartData1 *partData);

		//创建四面体
		void createTetrahedron(int meshID, QVector<int> index, MDataPost::mPostMeshPartData1 *partData);

		//计算一条流线，trueorfalse代表正向或者向两边积分，pmesID代表流线的起始网格ID,lineVertex代表要添加顶点进的数组，lineTexture代表要添加纹理进的数组
		void caculateStreamLineData(bool trueorfalse,int pmeshID,QVector<QVector3D> &lineVertex, QVector<float> &lineTexture);

		//判断一个点是否在一个四面体中
		bool jdugePointIsInTetrahedron(QVector<QVector3D> vertexs,QVector3D point);

		//通过插值计算一个点在四面体中的矢量值
		QVector3D caculatePointValueInTetrahedron(QVector3D point, QVector<QVector3D> vertexs, QVector<QVector3D> vectorValues);

		//通过插值计算一个点在四面体中的标量值
		float caculatePointValueInTetrahedron(QVector3D point, QVector<QVector3D> vertexs, QVector<float> values);

		//通过插值计算一个点在四面体中的标量值
		QPair<QVector3D,float> caculatePointValueInTetrahedron(QVector3D point, QVector<QVector3D>vertexs, QVector<QVector3D> vectorValues, QVector<float> values);
	private:
		mPostOneFrameRendData *_currentFrameRendData;

		//计算的矢量值
		QHash<int, QVector3D> _vectorValue;

		//计算的标量值
		QHash<int, float> _value;

		//最值
		float _maxval;
		float _minval;
		float _fenmu;

		//正向传出流线的值
		QVector<QVector3D> _forwardLineVertex;
		QVector<float> _forwardlineTexture;

		//反向传出流线的值
		QVector<QVector3D> _backwardLineVertex;
		QVector<float> _backwardLineTexture;

		//计算出来的模型数据
		QHash<int, MDataPost::mPostMeshNodeData1*> _nodeData1;
		QHash<int, MDataPost::mPostMeshData1*> _meshData1;
		QHash<QString, MDataPost::mPostMeshPartData1*> _partData1;
		//单元面数据
		QHash<QSet<int>, QVector<int>> _gongxianwangge;//共线的网格

		//积分步数
		int _integrationSteps;

		//确定初始点的球
		QVector3D _sphereCenter;
		float _sphereRadius;

		//流线密度（流线数量）
		int _streamLineNum;

		//积分比例
		float _integrateRatio;

		//后处理数据
		mOneFrameData1 *_oneFrameData;
	};


	class RENDVIEW_EXPORT mPostStreamLineRender
	{

	public:
		mPostStreamLineRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mOneFrameData1 *oneFrameData,std::shared_ptr<mPostRendStatus> rendStatus, mPostOneFrameRendData *currentFrameRendData = nullptr, int num = 500, float ratio = 0.1);

		~mPostStreamLineRender();

		void setLineStateSet(std::shared_ptr<mxr::StateSet> stateSet);

		void setPointStateSet(std::shared_ptr<mxr::StateSet> stateSet);

		void setSphereParameter(QVector3D center, float radius);

		void setStreamLineNum(int num = 500);

		void setStreamLineShowForm(int streamLineShowForm = 0);//0代表线条，1代表点

		void setIntergrateDirection(int intergrateDirection = 0);//0代表双向，1代表正向，2代表反向

		void OrganizeModeldata();

	private:

		std::shared_ptr<mPostRendStatus> _rendStatus;

		mPostStreamLineData1 *_postStreamLineData;

		std::shared_ptr<mxr::Application> _app;
		std::shared_ptr<mxr::Group> _parent;//父节点
		std::shared_ptr<mxr::Geode> _geode;//当前总节点

		//等值线
		std::shared_ptr<mxr::Drawable> _lineDrawable;
		std::shared_ptr<mxr::StateSet> _lineStateSet;
		std::shared_ptr<mxr::StateSet> _pointStateSet;
	};
}

