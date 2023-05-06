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
		* ������Ⱦ����
		*/
		void createRendObject();
		/*
		* ��ʼ��Ⱦ
		*/
		void drawLable();
		/*
		* ��ȡͼ����Ⱦ���ݿ�
		*/
		shared_ptr<mLableDataController_post> getLableDataController();
	private:
		//��Ⱦ���ݶ���
		shared_ptr<mLableDataController_post> _lableDataController = nullptr;
	private:

		/*
		* ������Ⱦ����
		*/
		void createRendObject_post(shared_ptr<mFontTexture> ft);
	private:
		//ʸ����ͷ
		shared_ptr<mLableRendBase<mDrawElement>> _vecArrow_post = nullptr; //ͼ��
		//�̶�����
		shared_ptr<mLableRendBase<mDrawTexture>> _fontRend_fixed_post = nullptr; //ͼ��
	};
}

