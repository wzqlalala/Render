#pragma once
namespace MLableRend
{
	/*
	* 着色器类型
	*/
	enum ShaderType
	{
		//文字
		ShaderType_Font_Arrow,//箭头顶端的文字
		ShaderType_Font_FixShape,//固定图标的文字
		ShaderType_Font_Common,//通用类型文字
		ShaderType_Font_Fixed,//固定类型文字
		ShaderType_Font_GlobalAxis,//全局坐标系文字
		//图标
		ShaderType_GlobalAxis, //箭头类型
		ShaderType_Arrow, //箭头类型
		ShaderType_FixShape, //固定形状类型
		ShaderType_BCDF, //约束自由度类型图标
		ShaderType_Model,//模型坐标系下绘制图标的通用着色器
		ShaderType_Model_Point, //模型坐标系下绘制点
		ShaderType_FixShapeRotate,//用于固定形状+旋转类型
		ShaderType_FixShapeRotate2,//用于固定形状+旋转2次类型
	};

	/*
	* 图标操作类型（、删除、显示、隐藏、高亮、取消高亮等）
	*/
	enum  LableOperateMode
	{
		Append, //添加
		Delete, //删除
		Show,	//显示
		Hide,	//隐藏
		Highlight, //高亮
		NoHighlight, //取消高亮
		SetColor,	 //设置颜色
		SetSize		 //设置大小
	};


	/*
	* 界面类型（前处理、后处理）
	*/
	enum InterfaceType
	{
		InterfaceType_Pre, //前处理
		InterfaceType_Post, //后处理
	};

	/*
	* 文字类型
	*/
	enum FontType
	{
		FontType_Commom,  //常规文字或者固定文字
		FontType_Arrow,		//箭头末端文字
		FontType_FixShape,	//固定图标处的文字
	};
	/*
	* 局部坐标轴类型
	*/
	enum LocalAxisType
	{
		LocalAxisType_Rectangular,		//局部直角坐标系
		LocalAxisType_Cylinder,			//局部直角坐标系
		LocalAxisType_Sphere,			//局部球坐标系
	};

}