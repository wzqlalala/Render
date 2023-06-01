#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

//MViewBasic
#include "mMeshViewEnum.h"

//MViewBasic
#include "mMeshViewEnum.h"
#include "mBasicStructor.h"

#include <QVector4D>
#include <QVector>

using namespace MViewBasic;
using namespace MViewBasic;
namespace MPostRend
{
	class RENDVIEW_EXPORT mPostRendStatus
	{

	public:
		//渲染模式
		PostMode _postMode = OneFrame;

		//是否显示变形前的图
		bool _isShowInitialShape{false};

		//节点或者网格
		NodeOrElement _nodeOrElement = PostNode;

		//显示模式
		ShowFuntion _showFunction = ElementFace;

		//拾取模式
		PickFilter _pickFilter;

		//拾取的单元类型过滤器
		std::set<int> _pickElementTypeFilter;

		//框选还是点选
		//MViewBasic::PickSoloOrMutiply _pickSoloOrMutiply;

		//渲染范围（0-全部部件范围（自定义）；1-渲染部件范围）
		int _rangeType = 0;

		//云图-离散阶数
		int _cloudDiscreteNum = 12;

		//是否离散
		bool _isdispersed = true;

		//是否等差
		bool _isEquivariance = true;

		//是否渲染颜色表
		bool _isShowPostColorTable = false;

		//是否开启光照
		bool _lightIsOn = true;

		//光照位置跟随相机位置
		bool _lightIsDependOnCamera = true;

		//光照参数
		PostLight _postLight;

		//点的大小
		int _pointSize = 1;

		//线宽
		float _lineWidth = 1.0;

		//网格面线的颜色
		QVector4D _faceLineColor = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);

		//颜色表文字大小
		float _postColorTableFontSize = 1.0;

		//颜色表位置行高度比例
		float _postColorTableRatio = 0.05f;

		/*************************************变形图********************************/

		//变形图-变形系数
		QVector3D _deformFactor = QVector3D(1,1,1);

		/*************************************切面********************************/

		//切平面的参数
		QVector<QVector4D> _cuttingPlanes;

		//是否实时显示平面
		bool _isShowTransparentPlane = true;

		//是否只显示被切的面，不显示模型
		bool _isOnlyShowCuttingPlane = false;

		/**************************************动画**********************************/

		//动画帧数
		//动画-是否生成
		bool _generateAnimate;

		//动画-生成的动画帧数
		int _aniFrameAmount;

		//动画-类型
		QString _aniType;

		//动画-范围
		QString _aniRange;

		//动画-控制变量
		QString _aniControl;

		//动画-范围起始值
		QString _aniRangeVal1;

		//动画-范围结束值
		QString _aniRangeVal2;

		//动画-线性总帧数
		int _aniLinearCount = 20;

		//动画-当前帧
		int _aniCurrentFrame = 1;

		//动画-起始帧
		int _aniStartFrame = 1;

		//动画-结束帧
		int _aniEndFrame = 20;

		//动画-帧间隔
		int _aniFrameInterval = 1;

		//动画-帧速率
		int _aniFrameRate = 40;

		//动画-是否循环播放
		bool _aniLoopPlay = true;

		//动画-渲染范围
		QString _aniEveryRange;

		//动画-最小值（仅自定义范围可用）
		float _aniRangeMin;

		//动画-最大值（仅自定义范围可用）
		float _aniRangeMax;

		//动画-是否开启
		bool _aniIsStart;

		/***********************************矢量图**********************************/

		//矢量图-节点类型（全部或者拾取）
		QString _vectorArrowNodeType = QString("全部");

		//矢量图-拾取节点ID
		std::set<int> _vectorArrowNodeIDs;

		//矢量图-矢量图的箭头类型和颜色
		QVector<QPair<QString, QVector3D>> _vectorArrowTypeColor;

		//箭头大小的方式
		QString _vectorArrowMethod;

		//矢量图-矢量图的箭头大小
		double _vectorArrowSize = 1.0f;
	};
}

