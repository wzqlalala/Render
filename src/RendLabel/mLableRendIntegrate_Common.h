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
		* ������Ԫ��
		*/
		friend class  mLableRendController_common;
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
		shared_ptr<mLableDataController_common> getLableDataController();


	private:

		/*
		* ������Ⱦ����
		*/
		//ͨ�ö���
		void createRendObject_common(shared_ptr<mFontTexture> ft);

	private:
		//��Ⱦ���ݶ���
		shared_ptr<mLableDataController_common> _lableDataController = nullptr;

	private:
		/*
		* ͨ��
		*/
		//�̶�������Ⱦ
		shared_ptr<mLableRendBase<mDrawTexture>> _fontRend_fixed_common = nullptr;
		//ͨ��������Ⱦ
		shared_ptr<mLableRendBase<mDrawTexture>> _fontRend_common_common = nullptr;
		//X���� ͼ����Ⱦ������ת���ģ�
		shared_ptr<mLableRendBase<mDrawArray>> _XPointRend_common = nullptr; //ͼ��
		//��ʱ������
		shared_ptr<mLableRendBase<mDrawArray>> _tempPointRend_common = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _tempPointFontRend_common = nullptr; //����
		//��ʱ��������
		shared_ptr<mLableRendBase<mDrawArray>> _tempMeshRend_common = nullptr; //ͼ��
		//��ͷ+���� �������ͼ����Ⱦ���� ��ȫ������ϵ��
		shared_ptr<mLableRendBase<mDrawElement>> _globalAxisRend_common = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _globalAxisFontRend_common = nullptr; //����
		//��ͷ
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_common = nullptr; //ͼ��
	};
}
