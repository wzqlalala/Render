#pragma once
namespace MDataLable
{
	//图标操作类型 （载荷、边界、连接等）
	enum  LableOperateType
	{
		LableOperateType_FixedFont,	//固定文字
		LableOperateType_CommonFont,	//通用文字
		LableOperateType_XPoint,	//旋转中心
		LableOperateType_TempPoint,	//临时高亮点
		LableOperateType_TempMesh,	//临时高亮网格
		LableOperateType_GlobalAxis,	//全局坐标系
		LableOperateType_Arrow,	//箭头

		LableOperateType_LocalAxis,//局部坐标系
		LableOperateType_Load,//载荷
		LableOperateType_BC,//边界
		LableOperateType_Connect,//连接
		LableOperateType_Contact,//接触
		LableOperateType_RigidConnect, //刚体连接
		LableOperateType_RigidConstraint, //刚体约束
		LableOperateType_RigidWall ,//刚性墙
		LableOperateType_InitCondition,//初始条件
		LableOperateType_Box,//盒子
		LableOperateType_Vector,//向量
		LableOperateType_PlyCoordinate,//材料铺层坐标系

		//后处理
		LableOperateType_VecArrow,//矢量箭头
		LableOperateType_FixedFont_Post,//固定文字（帧绑定）
		
	};
	//固定形状加旋转类型
	enum FixShapeRotateType
	{
		//刚体连接  旋转副
		FixShapeRotateType_Revolute,
		//刚体连接  平面副
		FixShapeRotateType_Planar,
		//刚体连接  圆柱形副
		FixShapeRotateType_Cylindrical,
		//刚体连接  平移副
		FixShapeRotateType_Prismatic,
	};
	/*
	* 图标数据类型（隐藏状态、高亮状态）
	*/
	enum  LableDataType
	{
		LableDataType_ShowState,
		LableDataType_HighLightState,
	};
}