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
			//��ȡ��Ļ�ֱ���
			int ScreenWidth = QApplication::desktop()->width();
			int ScreenHeight = QApplication::desktop()->height();
			//��С��Ϊ��׼
			int standard = 1080; //������׼Ϊ1080
			int smallsize = ScreenWidth > ScreenHeight ? ScreenHeight : ScreenWidth;
			_ratio = (float)standard / smallsize;
		};
		virtual ~mLableRendIntegrateBase() {};

		/*
		* ��ȡ��Ļ���ش�С
		*/
		void getScreenSize(const float scr_width, const float scr_height)
		{
			this->_scr_width = scr_width;
			this->_scr_height = scr_height;
		};
		/*
		* ��ʼ��
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
		//��Ļ���ش�С
		float  _scr_width, _scr_height;
		//ͨ���ӽ�
		const mCommonView *_commonView;
		//ģ���ӽ�
		const mModelView *_modelView;
		//��Ļ��׼����
		float _ratio;


	};
}
