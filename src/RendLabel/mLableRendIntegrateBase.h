#pragma once

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QApplication>
#include <QDesktopWidget>
#include"rendlabel_global.h"

namespace MViewBasic
{
	class mModelView;
	class mCommonView;
}
using namespace MViewBasic;

namespace MLableRend
{
	class RENDLABEL_EXPORT mLableRendIntegrateBase : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
	{
		Q_OBJECT

	public:
		mLableRendIntegrateBase() 
		{
			//获取屏幕分辨率
			int ScreenWidth = QApplication::desktop()->width();
			int ScreenHeight = QApplication::desktop()->height();
			//以小的为基准
			int standard = 1080; //开发基准为1080
			int smallsize = ScreenWidth > ScreenHeight ? ScreenHeight : ScreenWidth;
			_ratio = (float)standard / smallsize;
		};
		virtual ~mLableRendIntegrateBase() {};

		/*
		* 获取屏幕像素大小
		*/
		void getScreenSize(const float scr_width, const float scr_height)
		{
			this->_scr_width = scr_width;
			this->_scr_height = scr_height;
		};
		/*
		* 初始化
		*/
		void initial(mCommonView *commonView, mModelView *modelView)
		{
			this->initializeOpenGLFunctions();
			if (commonView == nullptr || modelView == nullptr)
			{
				return;
			}
			_commonView = commonView;
			_modelView = modelView;
		}

	protected:
		//屏幕像素大小
		float  _scr_width, _scr_height;
		//通用视角
		const mCommonView *_commonView;
		//模型视角
		const mModelView *_modelView;
		//屏幕基准比例
		float _ratio;


	};
}
