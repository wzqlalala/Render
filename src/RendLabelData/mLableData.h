#pragma once
#include<QHash>
#include"mLableDataStruct.h"
#include"rendlabeldata_global.h"
namespace MDataLable
{
	class RENDLABELDATA_EXPORT mLableData_common : public QObject
	{
		Q_OBJECT
	public:
		/*
		* 固定文字
		*/
		QHash<QString, Fixed_Font_Common> _font_Fixed_Common_Data;
		/*
		* 通用文字（添加在节点处）
		*/
		QHash<QString, Common_Font_Common> _font_Common_Common_Data;
		/*
		* X类型图标（旋转中心）
		*/
		QHash<QString, X_Point_Common> _XPoint_Common_Data;
		/*
		* 临时高亮点
		*/
		QHash<QString, TempPoint_Common> _tempPoint_Common_Data;
		/*
		* 临时高亮网格
		*/
		QHash<QString, TempMesh_Common> _tempMesh_Common_Data;
		/*
		* 全局坐标系数据
		*/
		QHash<QString, Arrow_Font_Axis_Common> _globalAxis_Common_Data;
		/*
		* 箭头
		*/
		QHash<QString, Arrow_Common> _arrow_Common_Data;

	};
	class RENDLABELDATA_EXPORT mLableData_implicit : public QObject
	{
		Q_OBJECT
	public:
		/*
		* 坐标系
		*/
		//箭头+文字组合类型图标数据（坐标系）
		QHash<QString, Arrow_Font_Axis_Implicit> _arrow_Font_Axis_Implicit_Data;

		/*
		* 载荷
		*/
		//箭头+文字组合类型图标数据（集中力、重力、力矩、线载荷、压强、预紧力、离心力）
		QHash<QString, Arrow_Font_Load_Implicit> _arrow_Font_Load_Implicit_Data;
		//三角形+文字 组合类型图标数据（热流密度，电荷密度）
		QHash<QString, Triangle_Font_Load_Implicit> _triangle_Font_Load_Implicit_Data;
		//菱形图标+文字 组合类型图标数据（对流换热，热辐射）
		QHash<QString, Rhombus_Font_Load_Implicit> _rhombus_Font_Load_Implicit_Data;
		//十字图标+文字 组合类型图标数据(内部热源,温度载荷)
		QHash<QString, X_Font_Load_Implicit> _X_Font_Load_Implicit_Data;
		//闪电型图标+文字 组合类型图标渲染对象(电荷)
		QHash<QString, Lightning_Font_Load_Implicit> _lightning_Font_Load_Implicit_Data;

		/*
		* 边界
		*/
		//约束自由度类型图标数据（位移转角 固定/对称/反对称  //初始速度  //初始位移 虚拟约束）
		QHash<QString, DF_BC_Implicit> _df_BC_Implicit_Data;
		//房屋型 图标数据（初始边界条件(初始温度），温度边界，电势））
		QHash<QString, Home_BC_Implicit> _home_BC_Implicit_Data;

		/*
		* 连接
		*/
		//直线型图标数据（刚性、柔性、MPC链接）
		QHash<QString, Line_Connect_Implicit> _line_Connect_Implicit_Data;
		//主从面型图标数据（面面绑定连接）
		QHash<QString, FaceToFace_Connect_Implicit> _faceToFace_Connect_Implicit_Data;

		/*
		* 接触
		*/
		//主从面型图标数据（接触）
		QHash<QString, FaceToFace_Contact_Implicit> _faceToFace_Contact_Implicit_Data;

		/*
		* 刚体连接
		*/
		//旋转副，平面副、圆柱形副
		QHash<QString, Type1_RC_Implicit> _Type1_RC_Implicit_Data;
		//平移副
		QHash<QString, Type2_RC_Implicit> _Type2_RC_Implicit_Data;
		//直线型（弹簧 阻尼）
		QHash<QString, Line_RC_Implicit> _line_RC_Implicit_Data;
		//圆球铰副（圆球铰副）
		QHash<QString, Sphere_RC_Implicit> _sphere_RC_Implicit_Data;

		/*
		* 刚体约束
		*/
		//约束自由度类型图标数据（刚体约束）
		QHash<QString, DF_RBC_Implicit> _df_RBC_Implicit_Data;

		/*
		* 材料铺层
		*/
		//箭头（材料铺层坐标系）
		QHash<QString, Arrow_PlyCoordinate_Implicit> _arrow_PlyCoordinate_Implicit_Data;
		

	};
	class RENDLABELDATA_EXPORT mLableData_explicit : public QObject
	{
		Q_OBJECT
	public:
		/*
		* 坐标系
		*/
		//箭头+文字组合类型图标数据（坐标系）
		QHash<QString, Arrow_Font_Axis_Explicit> _arrow_Font_Axis_Explicit_Data;
		/*
		* 载荷
		*/
		//箭头+文字组合类型图标数据（点载荷、面载荷、重力）
		QHash<QString, Arrow_Font_Load_Explicit> _arrow_Font_Load_Explicit_Data;
		//约束自由度类型图标数据（刚体载荷）
		QHash<QString, DF_Load_Explicit> _df_Load_Explicit_Data;
		/*
		* 边界
		*/
		//约束自由度类型图标数据（位移 速度 加速度）
		QHash<QString, DF_BC_Explicit> _df_BC_Explicit_Data;
		//箭头类型图标数据（滑移）
		QHash<QString, Arrow_BC_Explicit> _arrow_BC_Explicit_Data;
		/*
		* 连接
		*/
		//直线型图标数据（电焊、节点刚体、合并刚体、额外节点、转动副、球铰、圆柱副、移动副、锁止关节、平面副、万向节、齿轮传动、皮带轮传动、齿轮-齿条传动、对接焊缝、角焊缝、铆接、运动耦合、
		//分布耦合、节点绑定、壳-实体耦合、平动约束、通用刚度、通用点焊、移动刚度、弯曲-扭转刚度）
		QHash<QString, Line_Connect_Explicit> _line_Connect_Explicit_Data;
		//主从面型图标数据（面面绑定、失效绑定连接）
		QHash<QString, FaceToFace_Connect_Explicit> _faceToFace_Connect_Explicit_Data;
		//约束自由度类型图标数据（刚体运动）
		QHash<QString, DF_Connect_Explicit> _df_Connect_Explicit_Data;
		/*
		* 接触
		*/
		//主从面型图标数据（点对面 面对面）
		QHash<QString, FaceToFace_Contact_Explicit> _faceToFace_Contact_Explicit_Data;
		/*
		* 初始条件
		*/
		//箭头+文字组合类型图标数据（初始速度）
		QHash<QString, Arrow_Font_InitCondition_Explicit> _arrow_Font_InitCondition_Explicit_Data;
		//三角形+文字 组合类型图标数据（初始应力、SPH初始应力、初始炸点）
		QHash<QString, Triangle_Font_InitCondition_Explicit> _triangle_Font_InitCondition_Explicit_Data;
		/*
		* 刚性墙
		*/
		//线
		QHash<QString, Line_RW_Explicit> _line_RW_Explicit_Data;
		//面
		QHash<QString, Face_RW_Explicit> _face_RW_Explicit_Data;
		//坐标系
		QHash<QString, Axis_RW_Explicit> _axis_RW_Explicit_Data;
		/*
		* 盒子
		*/
		//面
		QHash<QString, Face_Box_Explicit> _face_Box_Explicit_Data;
		//线
		QHash<QString, Line_Box_Explicit> _line_Box_Explicit_Data;
		/*
		* 向量
		*/
		//箭头+文字组合类型图标数据（向量）
		QHash<QString, Arrow_Vector_Explicit> _arrow_Vector_Explicit_Data;


	};
	class RENDLABELDATA_EXPORT mLableData_fluid : public QObject
	{
		Q_OBJECT
	};
	class RENDLABELDATA_EXPORT mLableData_magnet : public QObject
	{
		Q_OBJECT
	public:
		/*
		* 坐标系
		*/
		//箭头+文字组合类型图标数据（坐标系）
		QHash<QString, Arrow_Font_Axis_Magnet> _arrow_Font_Axis_Magnet_Data;
		/*
		* 载荷
		*/
		//箭头+文字组合类型图标数据（边 电荷/电流   磁化强度）
		QHash<QString, Arrow_Font_Load_Magnet> _arrow_Font_Load_Magnet_Data;
		//三角形+文字 组合类型图标数据（表面电荷密度 表面电流密度 表面磁通密度 表面电场强度）
		QHash<QString, Triangle_Font_Load_Magnet> _triangle_Font_Load_Magnet_Data;
		//闪电型图标+文字 组合类型图标渲染对象(节点/表面/体 电荷   节点/表面 电流)
		QHash<QString, Lightning_Font_Load_Magnet> _lightning_Font_Load_Magnet_Data;
		/*
		* 边界
		*/
		//箭头+文字组合类型图标数据（磁矢量位）
		QHash<QString, Arrow_BC_Magnet> _arrow_BC_Magnet_Data;
		//房屋型 图标数据（标量电位）
		QHash<QString, Home_BC_Magnet> _home_BC_Magnet_Data;
	};
	class RENDLABELDATA_EXPORT mLableData_post : public QObject
	{
		Q_OBJECT
	public:


		/*
		* 矢量箭头
		*/
		QHash<int, QHash<QString, VectorArrow_Post>> _vecArrow_Post_Data;
		/*
		* 固定文字
		*/
		QHash<int, QHash<QString, Fixed_Font_Post>> _font_Fixed_Post_Data;
		
	};
}