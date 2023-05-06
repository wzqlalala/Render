#pragma once
#include"mLableDataTemplate.h"
#include"mLableDataEnum.h"
#include <QObject>
#include<QPair>
namespace MDataLable
{
	typedef QPair<QVector3D, QVector3D> POS_DIR;
	typedef QPair<QVector<QVector3D>, QVector3D> POS_COLOR;
	typedef QPair<QVector3D, QPair<QString, QVector3D>> POS_TXT_DIR;
	typedef QPair<QVector3D, QString> POS_TXT;
	struct POS_DIR_SIZE_COLOR
	{
		QVector3D pos = { 0,0,0 };
		QVector3D dir = { 0,0,0 };
		float size = 1;
		QVector3D color = { 0,0,0 };
	};
	struct POS_DIR_COLOR
	{
		QVector3D pos = { 0,0,0 };
		QVector3D dir = { 0,0,0 };
		QVector3D color = { 0,0,0 };
	};
	struct  Font {};
	struct  Lable {};
	struct  Lable_Lable {};
	struct  Lable_Font {};
	struct  Lable_Lable_Lable {};
	struct  Lable_Lable_Lable_Font {};

	//***这里不能起别名，后面会影响判断***
	/*
	* 通用模块
	*/
	//固定文字
	struct Fixed_Font_Common
	{
		LableData1<QVector<POS_TXT>> fontData;
	};
	//通用文字（显示在节点处）
	struct Common_Font_Common
	{
		LableData1<QVector<POS_TXT>> fontData;
	};
	//X类型图标数据（旋转中心）
	struct X_Point_Common
	{
		LableData1<QVector<QVector3D>> lableData;
	};
	//临时高亮点
	struct TempPoint_Common
	{
		LableData1<QVector<QVector3D>> lableData; //顶点
		LableData1<QVector<POS_TXT>> fontData;	  //文字
	};
	//临时高亮网格（接触临时高亮，面面绑定连接临时高亮）
	struct TempMesh_Common
	{
		LableData1<QVector<QVector3D>> lableData; //顶点
	};

	////箭头+文字 组合类型图标数据（全局坐标系）
	struct Arrow_Font_Axis_Common
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData; //X
		LableData1<QVector<POS_DIR>> lableData2; //Y
		LableData1<QVector<POS_DIR>> lableData3; //Z
		//文字  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef GlobalAxis;

	//箭头
	struct Arrow_Common
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
	};
	/*
	* 隐式模块
	*/
	///////////////////////////////////坐标系//////////////////////////////////////////
	////箭头+文字 组合类型图标数据（局部坐标系）
	struct Arrow_Font_Axis_Implicit
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData; //X
		LableData1<QVector<POS_DIR>> lableData2; //Y
		LableData1<QVector<POS_DIR>> lableData3; //Z
		//文字  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef LocalAxis_i;

	////////////////////////////////////载荷//////////////////////////////////////////
	//箭头+文字 组合类型图标数据 （集中力、重力、力矩、线载荷、压强、预紧力、离心力）
	struct Arrow_Font_Load_Implicit
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
		//文字  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef Force, Gravity, Moment, LineForce, Pressure, Pretension, CentrifugalForce;
	
	//三角形图标+文字 (热流密度、电荷密度）
	struct Triangle_Font_Load_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef Flux, ChargeDensity;

	//菱形图标+文字 （对流换热，热辐射）
	struct Rhombus_Font_Load_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef Conv,HeatRadiation;


	//十字图标+文字 （内部热源,温度载荷）
	struct X_Font_Load_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef InterHeat, TempLoad;

	//闪电型图标+文字(电荷)
	struct Lightning_Font_Load_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef Charge;
	
	////////////////////////////////////边界//////////////////////////////////////////
	//约束自由度类型图标（位移转角 固定/对称/反对称  //初始边界条件(初始位移，初始速度)  //初始位移 虚拟约束）
	struct DF_BC_Implicit
	{
		LableData2<QVector<QVector3D>, QMap<unsigned int, float>> lableData;  //QVector<QVector3D>位置  QMap<unsigned int, float>  key表示为 1 2 3 4 5 6  
	}typedef Displacement, InitialVelocity, InitialDisplacement, VirtualConstraints;

	//房屋型 图标（初始边界条件(初始温度），温度边界，电势）
	struct Home_BC_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;  
	}typedef InitialTemperature, Temperature, Potential;
	
	////////////////////////////////////连接//////////////////////////////////////////
	//在模型坐标系下绘制的各类图标 （刚性连接、柔性连接、MPC连接）
	struct Line_Connect_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef RigidConnect, FlexConnect, MPCConnect;

	//在模型坐标系下绘制的各类图标 （面面绑定）
	struct FaceToFace_Connect_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;  //主面
		LableData1<QVector<QVector3D>> lableData2;  //从面
	}typedef TieConnect;

	////////////////////////////////////接触//////////////////////////////////////////

	//在模型坐标系下绘制的各类图标 （接触）
	struct FaceToFace_Contact_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;  //主面
		LableData1<QVector<QVector3D>> lableData2;  //从面
	}typedef Contact_i;

	////////////////////////////////刚体连接//////////////////////////////////////////
	//组合图标1 (旋转副，平面副、圆柱形副) 
	struct Type1_RC_Implicit
	{
		LableData2<QVector<POS_DIR>, FixShapeRotateType> lableData;  
	}typedef RevoluteRC, CylindricalRC, PlanarRC;
	//组合图标2 （平移副）
	struct Type2_RC_Implicit
	{
		LableData3<QVector<QVector3D>,QPair<QVector3D, QVector3D>,FixShapeRotateType> lableData;  // pos Pair<dir1, dir2>  type
	}typedef PrismaticRC;
	//直线类型(弹簧  阻尼)
	struct Line_RC_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
	}typedef SpringRC, DamperRC;
	//圆球铰副（圆球铰副）
	struct Sphere_RC_Implicit
	{
		LableData1<QVector<QVector3D>> lableData;
	}typedef SphericalRC;
	////////////////////////////////////刚体约束//////////////////////////////////////////
	struct DF_RBC_Implicit
	{
		LableData2<QVector<QVector3D>, QMap<unsigned int, float>> lableData;  //QVector<QVector3D>位置  QMap<unsigned int, float>  key表示为 1 2 3 4 5 6  
	}typedef DisplacementRBC;

	////////////////////////////////////材料铺层矢量箭头//////////////////////////////////////////
	//箭头（材料铺层坐标系）
	struct Arrow_PlyCoordinate_Implicit
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
	}typedef ArrowPlyCoordinate;

	/*
	* 显示模块
	*/
	///////////////////////////////////坐标系//////////////////////////////////////////
	////箭头+文字 组合类型图标数据（局部坐标系）
	struct Arrow_Font_Axis_Explicit
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData; //X
		LableData1<QVector<POS_DIR>> lableData2; //Y
		LableData1<QVector<POS_DIR>> lableData3; //Z
		//文字  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef LocalAxis_e;
	////////////////////////////////////载荷//////////////////////////////////////////

	//箭头+文字 组合类型图标数据 （点载荷、面载荷、重力）
	struct Arrow_Font_Load_Explicit
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
		//文字  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef PointLoad, FaceLoad, Gravity_e;

	//约束自由度类型图标（刚体载荷）
	struct DF_Load_Explicit
	{
		LableData2<QVector<QVector3D>, QMap<unsigned int, float>> lableData;  //QVector<QVector3D>位置  QMap<unsigned int, float>  key表示为 1 2 3 4 5 6  
	}typedef RigidBodyLoad;

	////////////////////////////////////边界//////////////////////////////////////////
	//约束自由度类型图标（位移、速度、加速度）
	struct DF_BC_Explicit
	{
		LableData2<QVector<QVector3D>, QMap<unsigned int, float>> lableData;  //QVector<QVector3D>位置  QMap<unsigned int, float>  key表示为 1 2 3 4 5 6  
	}typedef Displacement_e, Velocity_e, Acceleration_e;
	//箭头 类型图标数据 （滑移）
	struct Arrow_BC_Explicit
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
	}typedef SlidingPlane_e;

	////////////////////////////////////连接//////////////////////////////////////////
	//在模型坐标系下绘制的各类图标 （电焊、节点刚体、合并刚体、额外节点、转动副、球铰、圆柱副、移动副、锁止关节、平面副、万向节、齿轮传动、皮带轮传动、齿轮-齿条传动、对接焊缝、角焊缝、铆接、运动耦合、
	//分布耦合、节点绑定、壳-实体耦合、平动约束、通用刚度、通用点焊、移动刚度、弯曲-扭转刚度）
	struct Line_Connect_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef Spotweld_e, NodeRigidBody_e, MergeRigidBody_e, ExtraNode_e, RevoluteJoint_e, SphericalJoint_e, CylindricalJoint_e, TranslationalJoint_e, LockingJoint_e, PlanarJoint_e, UniversalJoint_e,
		GearJoint_e, PulleyJoint_e, RackPinionJoint_e, GenButtWeld_e, GenFilletWeld_e, Rivet_e, KinematicCoupling_e, DistributingCoupling_e, TieNode_e, ShellToSolid_e, Translation_e, GeneralizedStiffness_e,
		GenSpotWeld_e, TranslationalStiffness_e, SphericalStiffness_e;
	//面面绑定、失效绑定、壳边-壳边绑定
	struct FaceToFace_Connect_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;  //主面
		LableData1<QVector<QVector3D>> lableData2;  //从面
	}typedef Tie_e, TieBreak_e, TieEdge_e;
	
	//约束自由度类型图标（刚体运动）
	struct DF_Connect_Explicit
	{
		LableData2<QVector<QVector3D>, QMap<unsigned int, float>> lableData;  //QVector<QVector3D>位置  QMap<unsigned int, float>  key表示为 1 2 3 
	}typedef RigidBodyStoppers_e;
	////////////////////////////////////初始条件//////////////////////////////////////////
	//箭头+文字 组合类型图标数据 （初始速度）
	struct Arrow_Font_InitCondition_Explicit
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
		//文字  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef InitVelocity_e;

	//三角形图标+文字 (初始应力、SPH初始应力、初始炸点）
	struct Triangle_Font_InitCondition_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef InitStress_e, SPHInitStress_e, InitFryingPoint_e;

	////////////////////////////////////接触//////////////////////////////////////////
	//面对面  点对面
	struct FaceToFace_Contact_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;  //主面
		LableData1<QVector<QVector3D>> lableData2;  //从面
	}typedef PointToFace_e, FaceToFace_e;
	////////////////////////////////////刚性墙//////////////////////////////////////////
	//线
	struct Line_RW_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;	
	};
	//面
	struct Face_RW_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;
	};
	//坐标系
	struct Axis_RW_Explicit
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData; //X
		LableData1<QVector<POS_DIR>> lableData2; //Y
		LableData1<QVector<POS_DIR>> lableData3; //Z
		//文字  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	};
	////////////////////////////////////盒子//////////////////////////////////////////
	//盒子
	struct Face_Box_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;
	};
	struct Line_Box_Explicit
	{
		LableData1<QVector<QVector3D>> lableData;
	};	
	////////////////////////////////////向量//////////////////////////////////////////
	//箭头 类型图标数据 （向量）
	struct Arrow_Vector_Explicit
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
	}typedef Vector_e;

	/*
	* 电磁模块
	*/
	///////////////////////////////////载荷//////////////////////////////////////////

	//闪电型图标+文字(节点/表面/体 电荷   节点/表面 电流)
	struct Lightning_Font_Load_Magnet
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef QSource_mag, JSource_mag;
	//箭头+文字 组合类型图标数据 （边 电荷/电流   磁化强度）
	struct Arrow_Font_Load_Magnet
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
		//文字  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef LineQSource_mag, LineJSource_mag, BIntensity_mag;

	//三角形图标+文字 (表面电荷密度 表面电流密度 表面磁通密度 表面电场强度）
	struct Triangle_Font_Load_Magnet
	{
		LableData1<QVector<QVector3D>> lableData;
		LableData1<QVector<POS_TXT>> fontData;
	}typedef QDensity_mag, JDensity_mag, BDensity_mag, EIntensity_mag;

	///////////////////////////////////边界//////////////////////////////////////////
	//箭头 类型图标数据 （磁矢量位）
	struct Arrow_BC_Magnet
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData;
	}typedef MagneticVector_mag;
	
	//房屋型 图标（标量电位）
	struct Home_BC_Magnet
	{
		LableData1<QVector<QVector3D>> lableData;
	}typedef Potential_mag;
		

	///////////////////////////////////坐标系//////////////////////////////////////////
	////箭头+文字 组合类型图标数据（局部坐标系）
	struct Arrow_Font_Axis_Magnet
	{
		//图标 QVector<QPair<pos, dir>>
		LableData1<QVector<POS_DIR>> lableData; //X
		LableData1<QVector<POS_DIR>> lableData2; //Y
		LableData1<QVector<POS_DIR>> lableData3; //Z
		//文字  QVector<QPair<pos, QPair<txt, dir>>
		LableData1<QVector<POS_TXT_DIR>> fontData;
	}typedef LocalAxis_mag;


	/*
	* 后处理模块
	*/
	//箭头 （矢量箭头）
	struct VectorArrow_Post
	{
		LableData1<QVector<POS_DIR_SIZE_COLOR>> lableData;
	};
	//固定文字
	struct Fixed_Font_Post
	{
		LableData1<QVector<POS_TXT>> fontData;
	};
}
