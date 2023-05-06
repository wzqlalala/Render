#pragma once
#include"rendlabeldata_global.h"
#include<QMap>
#include<QVector3D>

namespace MDataLable
{
	class RENDLABELDATA_EXPORT mLableDataBase
	{
	public:
		mLableDataBase()
		{
			//��ʼ��Ĭ��
			_showState = true;
			_hightLightState = false;
			_depthState = true;
			_size = 1.0;
			_color = QVector3D(255, 255, 255);
		}
		~mLableDataBase() {};
	protected:
		//ID
		int _ID;
		//��ʾ����״̬
		bool _showState;
		//����״̬
		bool _hightLightState;
		//��Ȼ���״̬
		bool _depthState;
		//�ߴ�
		float _size;
		//��ɫ
		QVector3D _color;
	public:
		/*
		* ��ȡID
		*/
		int getLableID()
		{
			return _ID;
		}
		/*
		* ����ID
		*/
		void setLableID(const int ID)
		{
			_ID = ID;
		}
		/*
		* ��ȡ��ʾ����״̬
		*/
		bool getLableShowState()
		{
			return _showState;
		}
		/*
		* ������ʾ����״̬
		*/
		void setLableShowState(const bool &showstate)
		{
			_showState = showstate;
		}
		/*
		* ��ȡ����״̬
		*/
		bool getLableHighlightState()
		{
			return _hightLightState;
		}
		/*
		* ���ø���״̬
		*/
		void setLableHighlightState(const bool &hightlightState)
		{
			_hightLightState = hightlightState;
		}
		/*
		* ��ȡ��Ȼ���״̬
		*/
		bool getLableDepthState()
		{
			return _depthState;
		}
		/*
		* ������Ȼ���״̬
		*/
		void setLableDepthState(const bool &depthState)
		{
			_depthState = depthState;
		}
		/*
		* ��ȡ�ߴ�
		*/
		float getLableSize()
		{
			return _size;
		}
		/*
		* ���óߴ�
		*/
		void setLableSize(const double &size)
		{
			_size = size;
		}
		/*
		* ��ȡ��ɫ
		*/
		QVector3D getLableColor()
		{
			return _color;
		}
		/*
		* ������ɫ
		*/
		void setLableColor(const QVector3D &color)
		{
			_color = color;
		}
	};
	/*
	* ����Ϊһ�������
	*/
	template<class T>
	class LableData1 :public mLableDataBase
	{
	public:
		//���캯��
		LableData1() {};
		~LableData1() {};
		//��ȡ����
		T getParameter()
		{
			return _parameter;
		}
		//���ò���
		void setParameter(const T &para)
		{
			_parameter = para;
		}

	private:
		T _parameter;

	};
	/*
	* ����Ϊ���������
	*/
	template<class T1, class T2>
	class  LableData2 :public mLableDataBase
	{
	public:

		LableData2() {};
		~LableData2() {};
		//��ȡ����
		T1 getParameter1()
		{
			return _parameter1;
		}
		T2 getParameter2()
		{
			return _parameter2;
		}
		//���ò���
		void setParameter1(const T1 &para)
		{
			_parameter1 = para;
		}
		void setParameter2(const T2 &para)
		{
			_parameter2 = para;
		}
	private:
		T1 _parameter1;
		T2 _parameter2;
	};
	/*
	* ����Ϊ���������
	*/
	template<class T1, class T2, class T3>
	class  LableData3 :public mLableDataBase
	{
	public:
		LableData3() {};
		~LableData3() {};
		//��ȡ����
		T1 getParameter1()
		{
			return _parameter1;
		}
		T2 getParameter2()
		{
			return _parameter2;
		}
		T3 getParameter3()
		{
			return _parameter3;
		}
		//���ò���
		void setParameter1(const T1 &para)
		{
			_parameter1 = para;
		}
		void setParameter2(const T2 &para)
		{
			_parameter2 = para;
		}
		void setParameter3(const T3 &para)
		{
			_parameter3 = para;
		}
	private:
		T1 _parameter1;
		T2 _parameter2;
		T3 _parameter3;
	};


}