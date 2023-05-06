#pragma once
#include <QObject>
#include<memory>
#include"mLableRendIntegrateBase.h"
#include"mLableRendBase.h"
using namespace std;
namespace MDataLable
{
	class mLableDataController_pre;
}
using namespace MDataLable;
namespace MLableRend
{
	class mFontTexture;
	class RENDLABEL_EXPORT mLableRendIntegrate_pre : public mLableRendIntegrateBase
	{
		Q_OBJECT
	public:
		mLableRendIntegrate_pre();
		virtual ~mLableRendIntegrate_pre() override;
		/*
		* 声明友元类
		*/
		friend class  mLableRendController_pre;

		/*
		* 创建渲染对象
		*/
		void createRendObject();
		/*
		* 开始渲染
		*/
		void drawLable();

		/*
		* 获取图标渲染数据库
		*/
		shared_ptr<mLableDataController_pre> getLableDataController();


	private:

		/*
		* 创建渲染对象
		*/

		//隐式对象
		void createRendObject_implicit(shared_ptr<mFontTexture> ft);
		//显式对象
		void createRendObject_explicit(shared_ptr<mFontTexture> ft);
		//电磁对象
		void createRendObject_magnet(shared_ptr<mFontTexture> ft);
	private:
		//渲染数据对象
		shared_ptr<mLableDataController_pre> _lableDataController = nullptr;

	private:

		/*
		* 隐式通用结构
		*/
		//////////////////坐标系/////////////////////////
		//箭头+文字 组合类型图标渲染对象 （局部坐标系）
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_axis_i = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_axis_i = nullptr; //文字

		//////////////////载荷//////////////////////////
		//箭头+文字 组合类型图标渲染对象 （集中力、重力、力矩、线载荷、压强、预紧力、离心力）
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_load_i = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_load_i = nullptr; //文字

		//三角形+文字 组合类型图标渲染对象 （热流密度，电荷密度）
		shared_ptr<mLableRendBase<mDrawArray>> _triangleRend_load_i = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _triangleFontRend_load_i = nullptr; //文字

		//菱形图标+文字 组合类型图标渲染对象（对流换热，热辐射）
		shared_ptr<mLableRendBase<mDrawArray>> _rhombusRend_load_i = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _rhombusFontRend_load_i = nullptr; //文字

		//十字图标+文字 组合类型图标渲染对象(内部热源,温度载荷)
		shared_ptr<mLableRendBase<mDrawArray>> _XRend_load_i = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _XFontRend_load_i = nullptr; //文字

		//闪电型图标+文字 组合类型图标渲染对象(电荷)
		shared_ptr<mLableRendBase<mDrawArray>> _lightningRend_load_i = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _lightningFontRend_load_i = nullptr; //文字


		//////////////////边界//////////////////////////
		//约束自由度类型图标渲染对象（位移转角 固定/对称/反对称  //初始速度  //初始位移 虚拟约束）
		shared_ptr<mLableRendBase<mDrawElement>> _dfRend_BC_i = nullptr; //图标
		
		//房屋型 图标渲染对象（初始边界条件(初始温度），温度边界，电势）
		shared_ptr<mLableRendBase<mDrawArray>> _homeRend_BC_i = nullptr; //图标

		//////////////////连接//////////////////////////

		//直线型图标渲染对象 （刚性、柔性、MPC连接）
		shared_ptr<mLableRendBase<mDrawArray>> _lineRend_connect_i = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _lineFontRend_connect_i = nullptr; //文字

		//主从面图标渲染对象 （面面绑定）
		shared_ptr<mLableRendBase<mDrawArray>> _faceTofaceRend_connect_i = nullptr; //图标 
			
		//////////////////接触//////////////////////////
		//主从面图标渲染对象 （接触）
		shared_ptr<mLableRendBase<mDrawArray>> _faceTofaceRend_contact_i = nullptr; //图标 

		//////////////////刚体连接//////////////////////////
		// 旋转副，平面副、圆柱形副
		shared_ptr<mLableRendBase<mDrawArray>> _RCRend_type1_i = nullptr; //图标 
		// 平移副
		shared_ptr<mLableRendBase<mDrawArray>> _RCRend_type2_i = nullptr; //图标 
	    //直线型图标渲染对象 （弹簧 阻尼）
		shared_ptr<mLableRendBase<mDrawArray>> _lineRend_RC_i = nullptr; //图标
		//球型图标渲染对象（圆球铰副）
		shared_ptr<mLableRendBase<mDrawArray>> _sphereRend_RC_i = nullptr; //图标

		//////////////////刚体约束//////////////////////////
		//约束自由度类型图标渲染对象（刚体约束）
		shared_ptr<mLableRendBase<mDrawElement>> _dfRend_RBC_i = nullptr; //图标

		////////////////////////////////////材料铺层//////////////////////////////////////////
		//箭头（材料铺层坐标系）
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_plyCoordinate_i = nullptr; //图标
		
		/*
		* 显式
		*/
		//////////////////坐标系/////////////////////////
		//箭头+文字 组合类型图标渲染对象 （局部坐标系）
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_axis_e = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_axis_e = nullptr; //文字

		//////////////////载荷//////////////////////////
		//箭头+文字 组合类型图标渲染对象 （点载荷、面载荷、重力）
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_load_e = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_load_e = nullptr; //文字
		//约束自由度类型图标渲染对象（刚体载荷）
		shared_ptr<mLableRendBase<mDrawElement>> _dfRend_load_e = nullptr; //图标
		
		//////////////////边界//////////////////////////
		//约束自由度类型图标渲染对象（位移 速度 加速度）
		shared_ptr<mLableRendBase<mDrawElement>> _dfRend_BC_e = nullptr; //图标
		//箭头类型图标渲染对象 （滑移）
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_BC_e = nullptr; //图标

		//////////////////连接//////////////////////////
		//直线型图标渲染对象 （电焊、节点刚体、合并刚体、额外节点、转动副、球铰、圆柱副、移动副、锁止关节、平面副、万向节、齿轮传动、皮带轮传动、齿轮-齿条传动、对接焊缝、角焊缝、铆接、运动耦合、分布耦合、节点绑定、壳-实体耦合、平动约束、通用刚度、通用点焊、移动刚度、弯曲-扭转刚度
		shared_ptr<mLableRendBase<mDrawArray>> _lineRend_connect_e = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _lineFontRend_connect_e = nullptr; //文字
		//主从面图标渲染对象 （面面绑定、失效绑定）
		shared_ptr<mLableRendBase<mDrawArray>> _faceTofaceRend_connect_e = nullptr; //图标 
		//约束自由度类型图标渲染对象（刚体运动）
		shared_ptr<mLableRendBase<mDrawElement>> _dfRend_connect_e = nullptr; //图标

		//////////////////初始条件//////////////////////////
		//箭头+文字 组合类型图标渲染对象 （初始速度）
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_initCondition_e = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_initCondition_e = nullptr; //文字
		//三角形+文字 组合类型图标渲染对象 （初始应力、SPH初始应力、初始炸点）
		shared_ptr<mLableRendBase<mDrawArray>> _triangleRend_initCondition_e = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _triangleFontRend_initCondition_e = nullptr; //文字
		
		//////////////////接触//////////////////////////
		//主从面图标渲染对象 （点对面、面对面）
		shared_ptr<mLableRendBase<mDrawArray>> _faceTofaceRend_contact_e = nullptr; //图标 

		//////////////////刚性墙//////////////////////////
		shared_ptr<mLableRendBase<mDrawArray>> _lineRend_RW_e = nullptr; //线
		shared_ptr<mLableRendBase<mDrawArray>> _faceRend_RW_e = nullptr; //面
		shared_ptr<mLableRendBase<mDrawElement>> _axisRend_RW_e = nullptr; //坐标系
		shared_ptr<mLableRendBase<mDrawTexture>> _axisFontRend_RW_e = nullptr; //坐标系文字
		
		/////////////////盒子//////////////////////////
		shared_ptr<mLableRendBase<mDrawArray>> _faceRend_box_e = nullptr; //面
		shared_ptr<mLableRendBase<mDrawArray>> _lineRend_box_e = nullptr; //线
		
		/////////////////向量//////////////////////////
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_vector_e = nullptr; //图标

		/*
		* 电磁
		*/
		//////////////////坐标系/////////////////////////
		//箭头+文字 组合类型图标渲染对象 （局部坐标系）
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_axis_mag = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_axis_mag = nullptr; //文字

		//////////////////载荷/////////////////////////
		//箭头+文字 组合类型图标渲染对象 （边 电荷/电流   磁化强度）
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_load_mag = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_load_mag = nullptr; //文字

		//三角形+文字 组合类型图标渲染对象 （表面电荷密度 表面电流密度 表面磁通密度 表面电场强度）
		shared_ptr<mLableRendBase<mDrawArray>> _triangleRend_load_mag = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _triangleFontRend_load_mag = nullptr; //文字

		//闪电型图标+文字 组合类型图标渲染对象(节点/表面/体 电荷   节点/表面 电流)
		shared_ptr<mLableRendBase<mDrawArray>> _lightningRend_load_mag = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _lightningFontRend_load_mag = nullptr; //文字

		/////////////////边界//////////////////////////
		//箭头 类型图标染对象（磁矢量位）
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_BC_mag = nullptr; //图标
		//房屋型 图标渲染对象（标量电位）
		shared_ptr<mLableRendBase<mDrawArray>> _homeRend_BC_mag = nullptr; //图标


	};
}
