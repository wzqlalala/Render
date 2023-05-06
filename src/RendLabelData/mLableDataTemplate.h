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
			//初始化默认
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
		//显示隐藏状态
		bool _showState;
		//高亮状态
		bool _hightLightState;
		//深度缓冲状态
		bool _depthState;
		//尺寸
		float _size;
		//颜色
		QVector3D _color;
	public:
		/*
		* 获取ID
		*/
		int getLableID()
		{
			return _ID;
		}
		/*
		* 设置ID
		*/
		void setLableID(const int ID)
		{
			_ID = ID;
		}
		/*
		* 获取显示隐藏状态
		*/
		bool getLableShowState()
		{
			return _showState;
		}
		/*
		* 设置显示隐藏状态
		*/
		void setLableShowState(const bool &showstate)
		{
			_showState = showstate;
		}
		/*
		* 获取高亮状态
		*/
		bool getLableHighlightState()
		{
			return _hightLightState;
		}
		/*
		* 设置高亮状态
		*/
		void setLableHighlightState(const bool &hightlightState)
		{
			_hightLightState = hightlightState;
		}
		/*
		* 获取深度缓冲状态
		*/
		bool getLableDepthState()
		{
			return _depthState;
		}
		/*
		* 设置深度缓冲状态
		*/
		void setLableDepthState(const bool &depthState)
		{
			_depthState = depthState;
		}
		/*
		* 获取尺寸
		*/
		float getLableSize()
		{
			return _size;
		}
		/*
		* 设置尺寸
		*/
		void setLableSize(const double &size)
		{
			_size = size;
		}
		/*
		* 获取颜色
		*/
		QVector3D getLableColor()
		{
			return _color;
		}
		/*
		* 设置颜色
		*/
		void setLableColor(const QVector3D &color)
		{
			_color = color;
		}
	};
	/*
	* 参数为一个的情况
	*/
	template<class T>
	class LableData1 :public mLableDataBase
	{
	public:
		//构造函数
		LableData1() {};
		~LableData1() {};
		//获取参数
		T getParameter()
		{
			return _parameter;
		}
		//设置参数
		void setParameter(const T &para)
		{
			_parameter = para;
		}

	private:
		T _parameter;

	};
	/*
	* 参数为两个的情况
	*/
	template<class T1, class T2>
	class  LableData2 :public mLableDataBase
	{
	public:

		LableData2() {};
		~LableData2() {};
		//获取参数
		T1 getParameter1()
		{
			return _parameter1;
		}
		T2 getParameter2()
		{
			return _parameter2;
		}
		//设置参数
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
	* 参数为三个的情况
	*/
	template<class T1, class T2, class T3>
	class  LableData3 :public mLableDataBase
	{
	public:
		LableData3() {};
		~LableData3() {};
		//获取参数
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
		//设置参数
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