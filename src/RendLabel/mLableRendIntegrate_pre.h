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
		* ������Ԫ��
		*/
		friend class  mLableRendController_pre;

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
		shared_ptr<mLableDataController_pre> getLableDataController();


	private:

		/*
		* ������Ⱦ����
		*/

		//��ʽ����
		void createRendObject_implicit(shared_ptr<mFontTexture> ft);
		//��ʽ����
		void createRendObject_explicit(shared_ptr<mFontTexture> ft);
		//��Ŷ���
		void createRendObject_magnet(shared_ptr<mFontTexture> ft);
	private:
		//��Ⱦ���ݶ���
		shared_ptr<mLableDataController_pre> _lableDataController = nullptr;

	private:

		/*
		* ��ʽͨ�ýṹ
		*/
		//////////////////����ϵ/////////////////////////
		//��ͷ+���� �������ͼ����Ⱦ���� ���ֲ�����ϵ��
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_axis_i = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_axis_i = nullptr; //����

		//////////////////�غ�//////////////////////////
		//��ͷ+���� �������ͼ����Ⱦ���� �������������������ء����غɡ�ѹǿ��Ԥ��������������
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_load_i = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_load_i = nullptr; //����

		//������+���� �������ͼ����Ⱦ���� �������ܶȣ�����ܶȣ�
		shared_ptr<mLableRendBase<mDrawArray>> _triangleRend_load_i = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _triangleFontRend_load_i = nullptr; //����

		//����ͼ��+���� �������ͼ����Ⱦ���󣨶������ȣ��ȷ��䣩
		shared_ptr<mLableRendBase<mDrawArray>> _rhombusRend_load_i = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _rhombusFontRend_load_i = nullptr; //����

		//ʮ��ͼ��+���� �������ͼ����Ⱦ����(�ڲ���Դ,�¶��غ�)
		shared_ptr<mLableRendBase<mDrawArray>> _XRend_load_i = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _XFontRend_load_i = nullptr; //����

		//������ͼ��+���� �������ͼ����Ⱦ����(���)
		shared_ptr<mLableRendBase<mDrawArray>> _lightningRend_load_i = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _lightningFontRend_load_i = nullptr; //����


		//////////////////�߽�//////////////////////////
		//Լ�����ɶ�����ͼ����Ⱦ����λ��ת�� �̶�/�Գ�/���Գ�  //��ʼ�ٶ�  //��ʼλ�� ����Լ����
		shared_ptr<mLableRendBase<mDrawElement>> _dfRend_BC_i = nullptr; //ͼ��
		
		//������ ͼ����Ⱦ���󣨳�ʼ�߽�����(��ʼ�¶ȣ����¶ȱ߽磬���ƣ�
		shared_ptr<mLableRendBase<mDrawArray>> _homeRend_BC_i = nullptr; //ͼ��

		//////////////////����//////////////////////////

		//ֱ����ͼ����Ⱦ���� �����ԡ����ԡ�MPC���ӣ�
		shared_ptr<mLableRendBase<mDrawArray>> _lineRend_connect_i = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _lineFontRend_connect_i = nullptr; //����

		//������ͼ����Ⱦ���� ������󶨣�
		shared_ptr<mLableRendBase<mDrawArray>> _faceTofaceRend_connect_i = nullptr; //ͼ�� 
			
		//////////////////�Ӵ�//////////////////////////
		//������ͼ����Ⱦ���� ���Ӵ���
		shared_ptr<mLableRendBase<mDrawArray>> _faceTofaceRend_contact_i = nullptr; //ͼ�� 

		//////////////////��������//////////////////////////
		// ��ת����ƽ�渱��Բ���θ�
		shared_ptr<mLableRendBase<mDrawArray>> _RCRend_type1_i = nullptr; //ͼ�� 
		// ƽ�Ƹ�
		shared_ptr<mLableRendBase<mDrawArray>> _RCRend_type2_i = nullptr; //ͼ�� 
	    //ֱ����ͼ����Ⱦ���� ������ ���ᣩ
		shared_ptr<mLableRendBase<mDrawArray>> _lineRend_RC_i = nullptr; //ͼ��
		//����ͼ����Ⱦ����Բ��¸���
		shared_ptr<mLableRendBase<mDrawArray>> _sphereRend_RC_i = nullptr; //ͼ��

		//////////////////����Լ��//////////////////////////
		//Լ�����ɶ�����ͼ����Ⱦ���󣨸���Լ����
		shared_ptr<mLableRendBase<mDrawElement>> _dfRend_RBC_i = nullptr; //ͼ��

		////////////////////////////////////�����̲�//////////////////////////////////////////
		//��ͷ�������̲�����ϵ��
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_plyCoordinate_i = nullptr; //ͼ��
		
		/*
		* ��ʽ
		*/
		//////////////////����ϵ/////////////////////////
		//��ͷ+���� �������ͼ����Ⱦ���� ���ֲ�����ϵ��
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_axis_e = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_axis_e = nullptr; //����

		//////////////////�غ�//////////////////////////
		//��ͷ+���� �������ͼ����Ⱦ���� �����غɡ����غɡ�������
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_load_e = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_load_e = nullptr; //����
		//Լ�����ɶ�����ͼ����Ⱦ���󣨸����غɣ�
		shared_ptr<mLableRendBase<mDrawElement>> _dfRend_load_e = nullptr; //ͼ��
		
		//////////////////�߽�//////////////////////////
		//Լ�����ɶ�����ͼ����Ⱦ����λ�� �ٶ� ���ٶȣ�
		shared_ptr<mLableRendBase<mDrawElement>> _dfRend_BC_e = nullptr; //ͼ��
		//��ͷ����ͼ����Ⱦ���� �����ƣ�
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_BC_e = nullptr; //ͼ��

		//////////////////����//////////////////////////
		//ֱ����ͼ����Ⱦ���� ���纸���ڵ���塢�ϲ����塢����ڵ㡢ת��������¡�Բ�������ƶ�������ֹ�ؽڡ�ƽ�渱������ڡ����ִ�����Ƥ���ִ���������-�����������ԽӺ��졢�Ǻ��졢í�ӡ��˶���ϡ��ֲ���ϡ��ڵ�󶨡���-ʵ����ϡ�ƽ��Լ����ͨ�øնȡ�ͨ�õ㺸���ƶ��նȡ�����-Ťת�ն�
		shared_ptr<mLableRendBase<mDrawArray>> _lineRend_connect_e = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _lineFontRend_connect_e = nullptr; //����
		//������ͼ����Ⱦ���� ������󶨡�ʧЧ�󶨣�
		shared_ptr<mLableRendBase<mDrawArray>> _faceTofaceRend_connect_e = nullptr; //ͼ�� 
		//Լ�����ɶ�����ͼ����Ⱦ���󣨸����˶���
		shared_ptr<mLableRendBase<mDrawElement>> _dfRend_connect_e = nullptr; //ͼ��

		//////////////////��ʼ����//////////////////////////
		//��ͷ+���� �������ͼ����Ⱦ���� ����ʼ�ٶȣ�
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_initCondition_e = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_initCondition_e = nullptr; //����
		//������+���� �������ͼ����Ⱦ���� ����ʼӦ����SPH��ʼӦ������ʼը�㣩
		shared_ptr<mLableRendBase<mDrawArray>> _triangleRend_initCondition_e = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _triangleFontRend_initCondition_e = nullptr; //����
		
		//////////////////�Ӵ�//////////////////////////
		//������ͼ����Ⱦ���� ������桢����棩
		shared_ptr<mLableRendBase<mDrawArray>> _faceTofaceRend_contact_e = nullptr; //ͼ�� 

		//////////////////����ǽ//////////////////////////
		shared_ptr<mLableRendBase<mDrawArray>> _lineRend_RW_e = nullptr; //��
		shared_ptr<mLableRendBase<mDrawArray>> _faceRend_RW_e = nullptr; //��
		shared_ptr<mLableRendBase<mDrawElement>> _axisRend_RW_e = nullptr; //����ϵ
		shared_ptr<mLableRendBase<mDrawTexture>> _axisFontRend_RW_e = nullptr; //����ϵ����
		
		/////////////////����//////////////////////////
		shared_ptr<mLableRendBase<mDrawArray>> _faceRend_box_e = nullptr; //��
		shared_ptr<mLableRendBase<mDrawArray>> _lineRend_box_e = nullptr; //��
		
		/////////////////����//////////////////////////
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_vector_e = nullptr; //ͼ��

		/*
		* ���
		*/
		//////////////////����ϵ/////////////////////////
		//��ͷ+���� �������ͼ����Ⱦ���� ���ֲ�����ϵ��
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_axis_mag = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_axis_mag = nullptr; //����

		//////////////////�غ�/////////////////////////
		//��ͷ+���� �������ͼ����Ⱦ���� ���� ���/����   �Ż�ǿ�ȣ�
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_load_mag = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _arrowFontRend_load_mag = nullptr; //����

		//������+���� �������ͼ����Ⱦ���� ���������ܶ� ��������ܶ� �����ͨ�ܶ� ����糡ǿ�ȣ�
		shared_ptr<mLableRendBase<mDrawArray>> _triangleRend_load_mag = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _triangleFontRend_load_mag = nullptr; //����

		//������ͼ��+���� �������ͼ����Ⱦ����(�ڵ�/����/�� ���   �ڵ�/���� ����)
		shared_ptr<mLableRendBase<mDrawArray>> _lightningRend_load_mag = nullptr; //ͼ��
		shared_ptr<mLableRendBase<mDrawTexture>> _lightningFontRend_load_mag = nullptr; //����

		/////////////////�߽�//////////////////////////
		//��ͷ ����ͼ��Ⱦ���󣨴�ʸ��λ��
		shared_ptr<mLableRendBase<mDrawElement>> _arrowRend_BC_mag = nullptr; //ͼ��
		//������ ͼ����Ⱦ���󣨱�����λ��
		shared_ptr<mLableRendBase<mDrawArray>> _homeRend_BC_mag = nullptr; //ͼ��


	};
}
