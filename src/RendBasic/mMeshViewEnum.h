#pragma once
namespace MViewBasic
{

	/*
	 * 后处理模型操作枚举
	 */
	enum PostModelOperateEnum
	{
		//导入文件操作
		ImportOperate,

		//不操作
		NoOperate,

		//单个部件操作
		HideOnePartOperate,//隐藏
		ShowOnePartOperate,//显示
		ChangeColorOnePartOperate,//改变颜色
		ColorOnePartOperate,//显示模型颜色
		TransparentOnePartOperate,//显示透明
		TextureOnePartOperate,//显示纹理
		ReplaceOnePartOperate,//替换部件

		//全部部件操作
		HideAllPartOperate,//隐藏所有Isshow
		ShowAllPartOperate,//显示所有Isshow
		ChangeColorAllPartOperate,//改变颜色Material
		ColorAllPartOperate,//显示模型颜色isColor
		TransparentAllPartOperate,//显示透明isColor
		TextureAllPartOperate,//显示纹理isColor
		UpdateVariableValue,//更新变量值value
		UpdateMinMax,//更新最值
		UpdateNormal,//更新法向量Normal
		ShowNodeformation,//显示变形前的图
		HideNodeformation,//隐藏变形前的图
	};

	//后处理渲染模式
	enum PostMode
	{

		No,

		//单帧
		OneFrame,

		//动画
		Animation,

		//单帧线性动画
		OneFrameLinearAnimation,

		//单帧正弦动画
		OneFrameSinAnimation,
	};

	/*
	* 拾取模式,add,reduce,replace
	*/
	enum class PickFuntion
	{
		//添加拾取
		AddPick,

		//减少拾取
		ReducePick,

		//替换拾取
		ReplacePick,
	};

	enum class MultiplyPickMode
	{
		NoPick,

		RoundPick,

		QuadPick,

		PolygonPick

	};

	/*
	 * 背景色模式：纯色，从左到右渐变，从上到下渐变
	 */
	enum BackGroundMode
	{
		PureColor,
		GradientL_R,
		GradientT_B,
	};

	/*
	 * 二维曲线类型
	 */
	enum PlotType
	{
		//折线图
		LineChart,

		//光滑曲线图
		SmoothChart,

		//阶跃曲线图
		StepChart,

		//分布光滑曲线图（前段为光滑，后端为直线）
		SmoothStepChart,
	};

	/*
	* 网格形状
	*/
	enum MeshType
	{
		//无节点（特殊单元）
		MeshNone = 0,

		//点
		MeshPoint,

		//线
		MeshBeam,

		//三角形
		MeshTri,

		//四边形
		MeshQuad,

		//四面体
		MeshTet,

		//三菱柱
		MeshWedge,

		//六面体
		MeshHex,

		//四棱锥
		MeshPyramid,
	};

	/*
	 * 单元类型
	 */
	enum ElementType
	{
		/*
		* 无单元
		*/
		NoneElement,

		/*
		* 显隐式单元
		*/

		//传感器单元(无节点)
		Sensor,

		//张紧器单元（无节点）
		Pretensioner,

		//质量单元（一个节点）
		Mass1,

		//质量单元（一个节点）
		ElementSPH,

		//滑轮单元（一个节点）
		Slipring,

		//卷收器单元（一个节点）
		Retractor,

		//安全带单元（线）
		SeatBelt,

		//一阶二维杆单元
		D2R2,

		//二阶二维杆单元
		D2R3,

		//一阶三维杆单元
		D3R2,

		//二阶三维杆单元
		D3R3,

		//一阶二维梁单元
		D2B2,

		//二阶二维梁单元
		D2B3,

		//一阶三维梁单元
		D3B2,

		//二阶三维梁单元
		D3B3,

		//一阶三维管单元
		Pipe,

		//弹簧单元
		ELAS1,

		//弹簧单元
		SPRING1,

		//弹簧单元(一个节点)
		SPRING2,

		//弹簧单元
		SPRING3,

		//弹簧单元
		GAP,

		//弹簧单元
		CBUSH,

		//可拉可压杆单元
		TS2,

		//离散梁
		DB2,

		//积分梁
		HB2,

		//常截面梁
		BS2,

		//离散单元
		Discrete,

		//一阶三角形平面应变单元
		PST3,

		//二阶三角形平面应变单元
		PST6,

		//一阶三角形节点积分平面应变单元
		PST3_1,

		//一阶三角形稳定节点积分平面应变单元
		PST3_2,

		//一阶三角形边光滑平面应变单元
		PST3_3,

		//三角形壳单元TRIA3
		TRIA3,

		//三角形壳单元C0
		C0,

		//一阶三角形平面应力单元(常规)
		PET3,

		//二阶三角形平面应力单元(常规)
		PET6,

		//一阶三角形节点积分平面应变单元
		PET3_1,

		//一阶三角形稳定节点积分平面应变单元
		PET3_2,

		//一阶三角形边光滑平面应变单元
		PET3_3,

		//一阶三角形平面应力单元(杂交元H)
		PET3H,

		//二阶三角形平面应力单元(杂交元H)
		PET6H,

		//一阶三角形轴对称单元(常规)
		AST3,

		//二阶三角形轴对称单元(常规)
		AST6,

		//一阶三角形轴对称单元(杂交元H)
		AST3H,

		//二阶三角形轴对称单元(杂交元H)
		AST6H,

		//三角形壳单元CPDSG
		CPDSG,

		//三角形壳单元CPDSG
		DSG,

		//三角形壳单元SST3
		SST3,

		//三角形壳单元SST6
		SST6,

		//三角形边光滑壳单元
		CPDSG_3,

		//一阶三角形膜单元
		MBT3,

		//二阶三角形膜单元
		MBT6,

		//三角形剪力板单元
		SPT3,

		//一阶四边形平面应变单元(常规)
		PSQ4,

		//二阶四边形平面应变单元(常规)
		PSQ8,

		//一阶四边形平面应变单元(缩减积分R)
		PSQ4R,

		//二阶四边形平面应变单元(缩减积分R)
		PSQ8R,

		//一阶四边形平面应变单元(非协调元I)
		PSQ4I,

		//二阶四边形平面应变单元(非协调元I)
		PSQ8I,

		//一阶四边形平面应力单元(常规)
		PEQ4,

		//二阶四边形平面应力单元(常规)
		PEQ8,

		//一阶四边形平面应力单元(缩减积分R)
		PEQ4R,

		//二阶四边形平面应力单元(缩减积分R)
		PEQ8R,

		//一阶四边形平面应力单元(杂交元H)
		PEQ4H,

		//二阶四边形平面应力单元(杂交元H)
		PEQ8H,

		//一阶四边形平面应力单元(非协调元I)
		PEQ4I,

		//二阶四边形平面应力单元(非协调元I)
		PEQ8I,

		//一阶四边形轴对称单元(常规)
		ASQ4,

		//二阶四边形轴对称单元(常规)
		ASQ8,

		//一阶四边形轴对称单元(缩减积分R)
		ASQ4R,

		//二阶四边形轴对称单元(缩减积分R)
		ASQ8R,

		//一阶四边形轴对称单元(杂交元H)
		ASQ4H,

		//二阶四边形轴对称单元(杂交元H)
		ASQ8H,

		//一阶四边形轴对称单元 (非协调元I)
		ASQ4I,

		//二阶四边形轴对称单元 (非协调元I)
		ASQ8I,

		//四边形壳单元QPH
		QPH,

		//四边形壳单元Q4R
		Q4R,

		//四边形壳单元MITC4
		MITC4,

		//四边形壳单元SSQ4
		SSQ4,

		//四边形壳单元SSQ8
		SSQ8,

		//四边形壳单元BT
		BT,

		//四边形壳单元HL
		HL,

		//四边形壳单元CY
		CY_,

		//四边形壳单元BTSHELL
		BTShell,

		//一阶四边形膜单元
		MBQ4,

		//一阶四边形膜单元(缩减积分)
		MBQ4R,

		//二阶四边形膜单元
		MBQ8,

		//四边形剪力板单元
		SPQ4,

		//一阶四面体单元
		D3T4,

		//二阶四面体单元
		D3T10,

		//一阶四面体压电单元
		D3T4E,

		//一阶节点积分四面体单元
		D3T4_1,

		//一阶稳定节点积分四面体单元
		D3T4_2,

		//一阶边光滑四面体单元
		D3T4_3,

		//二阶四面体修正格式
		D3T10M,

		//二阶四面体压电单元
		D3T10E,

		//一阶四棱锥单元
		D3P5,

		//一阶三棱柱单元(常规)
		D3P6,

		//一阶三棱柱单元(杂交元H)
		D3P6H,

		//一阶三棱柱单元(压电单元)
		D3P6E,

		//二阶三棱柱单元(常规)
		D3P15,

		//二阶三棱柱单元(杂交元H)
		D3P15H,

		//二阶三棱柱单元(压电单元)
		D3P15E,

		//一阶三棱柱实体壳单元(缩减积分R)
		D3S6R,

		//通用实体单元
		SOLID,

		//一阶六面体单元(常规)
		D3H8,

		//二阶六面体单元(常规)
		D3H20,

		//一阶六面体单元(缩减积分R)
		D3H8R,

		//二阶六面体单元(缩减积分R)
		D3H20R,

		//一阶六面体单元(杂交元H)
		D3H8H,

		//二阶六面体单元(杂交元H)
		D3H20H,

		//一阶六面体单元(非协调元I)
		D3H8I,

		//二阶六面体单元(非协调元I)
		D3H20I,

		//一阶六面体单元(压电单元)
		D3H8E,

		//二阶六面体单元(压电单元)
		D3H20E,

		//一阶六面体实体壳单元(缩减积分R)
		D3S8R,

		/*
		*电磁模块单元
		*/

		//一阶四面体单元
		EMD3T4,

		//一阶边光滑四面体单元
		EMD3T4_3,

		//一阶三棱柱单元(常规)
		EMD3P6,

		//一阶六面体单元(常规)
		EMD3H8
	};

	/*
	* 网格和几何模型操作枚举
	*/
	enum ModelOperateEnum
	{
		//导入文件操作
		ImportPart,

		//不操作
		OperateNothing,

		//单个几何操作
		AppendOnePart,
		DeleteOnePart,
		HideOnePart,
		ShowOnePart,
		ColorOnePart,
		ReplaceOnePart,
		RenameOnePart,

		//全部几何操作
		DeleteAllPart,
		HideAllPart,
		ShowAllPart,
		ColorAllPart,
	};

	/*
	* 网格剖分枚举
	*/
	enum MeshingEnum
	{
		//未剖分
		NoMeshing,

		//正在剖分
		Meshing,

		//调整后剖分
		AdjustingMeshing,

		//偏置后剖分
		BiasingMeshing,

		//网格调整中
		Adjusting,

		//节点偏置中
		Biasing,

		//剖分完成
		FinishMeshing,

		//完成调整
		FinishAdjusting,

		//完成偏置
		FinishBiasing,
	};


	//拾取对象
	enum class PickObjectType
	{
		Mesh1D,

		MeshEdge,

		Mesh2D,

		MeshFace,
	};

	enum class GeoLineProperty
	{
		IndependentEdge,//独立边
		EdgeOnFace,//面上的边
		DottedTopology//虚拓扑边
	};

	//拾取模式
	enum class PickMode
	{
		//不拾取
		NoPick,

		//拖拽
		DragPick,

		//点选
		SoloPick,

		//框选
		MultiplyPick,



		/***************点选模式*****************/
		//


		/***************拖拽模式*****************/

		//拖拽最小值线条
		//DragMinLine,

		//拖拽最大值线条
		//DragMaxLine,
	};

	/*
		* 拾取筛选器:全部,节点,点,梁,三角形,四边形,四面体,三菱柱,六面体,不拾取,单元线,单元面，网格调整，网格偏置，四棱锥
		*/
	enum class PickFilter
	{
		//拾取任何网格（只针对网格和节点，不对网格线和网格面）
		PickAny,

		//拾取点
		PickPoint,

		//拾取一维网格
		Pick1DMesh,

		//拾取二维网格
		Pick2DMesh,

		//拾取三角形
		PickTri,

		//拾取四边形
		PickQuad,

		//拾取四面体
		PickTet,

		//拾取四棱锥
		PickPyramid,

		//拾取三菱柱
		PickWedge,

		//拾取六面体
		PickHex,

		//不选择模式
		PickNothing,

		//拾取节点
		PickNode,

		//拾取节点（保留拾取顺序）
		PickNodeOrder,

		//拾取节点(通过路径)
		PickNodePath,

		//拾取任意网格
		PickAnyMesh,

		//拾取单元线
		PickMeshLine,

		//拾取单元面
		PickMeshFace,

		//拾取节点通过部件
		PickNodeByPart,

		//拾取任意网格通过部件
		PickAnyMeshByPart,

		//拾取单元线通过部件
		PickMeshLineByPart,

		//拾取单元面通过部件
		PickMeshFaceByPart,

		//拾取节点通过线
		PickNodeByLine,

		//拾取单元通过线
		PickMeshByLine,

		//拾取单元线通过线
		PickMeshLineByLine,

		//拾取节点通过面
		PickNodeByFace,

		//拾取单元通过面
		PickMeshByFace,

		//拾取单元面通过面
		PickMeshFaceByFace,

		//拾取节点通过线角度（不区分1维单元或者单元线，内部自动判断）
		PickNodeByLineAngle,

		//拾取节点通过面角度（不区分2维单元或者单元面，内部自动判断）
		PickNodeByFaceAngle,

		//拾取1维单元通过角度
		Pick1DMeshByAngle,

		//拾取2维单元通过角度
		Pick2DMeshByAngle,

		//拾取单元线通过角度
		PickMeshLineByAngle,

		//拾取单元面通过角度
		PickMeshFaceByAngle,

		//拾取网格部件
		PickMeshPart,

		//网格剖分时候点击调整的数字
		PickAdjustNum,

		//网格剖分时候点击偏置的数字
		PickBiasNum,

		//拾取几何体
		PickGeoSolid,

		//拾取几何面
		PickGeoFace,

		//拾取几何线
		PickGeoLine,

		//拾取几何点
		PickGeoPoint,

		//顺序拾取几何点
		PickGeoPointOrder,

		//拾取几何体通过部件
		PickGeoSolidByPart,

		//拾取几何面通过部件
		PickGeoFaceByPart,

		//拾取几何线通过部件
		PickGeoLineByPart,

		//拾取几何点通过部件
		PickGeoPointByPart,

		//拾取几何部件
		PickGeoPart,

		//拾取几何线上的任意点
		pickVertexOnGeoLine,

		//拾取几何面上的任意点
		pickVertexOnGeoFace,

		//拾取屏幕上的任意点
		pickVertexOnScreen,

		//拾取后处理上的任意点
		PickAnyPositionValue,
	};

	//拾取对象
	enum PickObject
	{
		//几何
		Geometry,

		//网格
		Mesh,

		//其他
		Other,
	};

	enum class CameraType
	{
		Camera1,

		Camera2,
	};

	enum class ViewOperateMode
	{
		NoViewOperate,

		CameraOperate,

		PickOperate,
	};

	enum class CameraOperateMode
	{
		NoCameraOperate,

		Rotate,

		Translate,

		Zoom,

	};

	/*
	* 显示模式：边界线模式，透视线框模式，网格面，网格面和线
	*/
	enum ShowFuntion
	{
		WireEdge,
		WireFrame,
		ElementFace,
		SmoothShaded,
	};

	/*
	 * 云图显示模式，节点还是网格
	 */
	enum NodeOrElement
	{
		//节点
		PostNode,

		//单元
		PostElement,
	};

	/*
	* 缩放视角（以物体的视角，以人的视角）
	*/
	enum ScalePerspectice
	{
		Model,
		Eye,
	};

	/*
	* 缩放类型（放大，缩小）
	*/
	enum ScaleDirection		//缩放类型
	{
		Zoom_in,		//放大
		Zoom_out,		//缩小
	};

	/*
	* 旋转类型（XY平面旋转，绕Z旋转）
	*/
	enum RotateType
	{
		Rotate_XY,
		Rotate_Z,
	};

	/*
	* 观察视角
	*/
	enum Perspective
	{
		Front,			//前视
		Rear,			//后视
		Top,			//上视
		Bottom,			//下视
		Left,			//左视
		Right,			//右视
		ISO1,			//标准1asiAlgo_AAG
		ISO2,			//标准2
		ISO3,			//标准3
		ISO4,			//标准4

	};

}
