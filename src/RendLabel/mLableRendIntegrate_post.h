#pragma once
#include <QObject>
#include<memory>
#include"mLableRendIntegrateBase.h"
#include"mLableRendBase.h"
namespace MDataLable
{
	class mLableDataController_post;
}
using namespace MDataLable;
namespace MLableRend
{
	class mFontTexture;
	class RENDLABEL_EXPORT mLableRendIntegrate_post : public mLableRendIntegrateBase
	{
		Q_OBJECT
	public:
		mLableRendIntegrate_post();
		virtual ~mLableRendIntegrate_post() override;
		friend class mLableRendController_post;
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
		shared_ptr<mLableDataController_post> getLableDataController();
	private:
		//渲染数据对象
		shared_ptr<mLableDataController_post> _lableDataController = nullptr;
	private:

		/*
		* 创建渲染对象
		*/
		void createRendObject_post(shared_ptr<mFontTexture> ft);
	private:
		//矢量箭头
		shared_ptr<mLableRendBase<mDrawElement>> _vecArrow_post = nullptr; //图标
		//固定文字
		shared_ptr<mLableRendBase<mDrawTexture>> _fontRend_fixed_post = nullptr; //图标
	};
}

