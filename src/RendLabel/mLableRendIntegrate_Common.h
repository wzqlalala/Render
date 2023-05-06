#pragma once
#include <QObject>
#include<memory>
#include"mLableRendIntegrateBase.h"
#include"mLableRendBase.h"
using namespace std;
namespace MDataLable
{
	class mLableDataController_common;
}
using namespace MDataLable;
namespace MLableRend
{
	class mFontTexture;
	class RENDLABEL_EXPORT mLableRendIntegrate_common : public mLableRendIntegrateBase
	{
		Q_OBJECT
	public:
		mLableRendIntegrate_common();
		virtual ~mLableRendIntegrate_common() override;
		/*
		* 声明友元类
		*/
		friend class  mLableRendController_common;
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
		shared_ptr<mLableDataController_common> getLableDataController();


	private:

		/*
		* 创建渲染对象
		*/
		//通用对象
		void createRendObject_common(shared_ptr<mFontTexture> ft);

	private:
		//渲染数据对象
		shared_ptr<mLableDataController_common> _lableDataController = nullptr;

	private:
		/*
		* 通用
		*/
		//固定文字渲染
		shared_ptr<mLableRendBase<mDrawTexture>> _fontRend_fixed_common = nullptr;
		//通用文字渲染
		shared_ptr<mLableRendBase<mDrawTexture>> _fontRend_common_common = nullptr;
		//X类型 图标渲染对象（旋转中心）
		shared_ptr<mLableRendBase<mDrawArray>> _XPointRend_common = nullptr; //图标
		//临时高亮点
		shared_ptr<mLableRendBase<mDrawArray>> _tempPointRend_common = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _tempPointFontRend_common = nullptr; //文字
		//临时高亮网格
		shared_ptr<mLableRendBase<mDrawArray>> _tempMeshRend_common = nullptr; //图标
		//箭头+文字 组合类型图标渲染对象 （全局坐标系）
		shared_ptr<mLableRendBase<mDrawElement>> _globalAxisRend_common = nullptr; //图标
		shared_ptr<mLableRendBase<mDrawTexture>> _globalAxisFontRend_common = nullptr; //文字
		//箭头
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_common = nullptr; //图标
	};
}
