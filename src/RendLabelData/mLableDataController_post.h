#pragma once
#include <QObject>
#include<QHash>
#include <memory>
#include"mLableData.h"
#include<type_traits>

#include"rendlabeldata_global.h"
#include"mLableDataEnum.h"

using namespace std;
//using namespace MViewBasic;
namespace MDataLable
{
	class RENDLABELDATA_EXPORT mLableDataController_post : public QObject
	{
		Q_OBJECT

	public:
		mLableDataController_post() 
		{
			_lableData_post = make_shared<mLableData_post>();
		};
		~mLableDataController_post() {};

		/*
		* 获取数据库
		*/
		shared_ptr<mLableData_post> getLableDataBase()
		{
			return _lableData_post;
		}

		////////////////////////单个数据操作////////////////////////
		/*
		* 判断数据是否存在
		*/
		template<class T>
		bool isLableDataExist(const int frameID, const QString& name)
		{
			auto &lableData = getLableData<T>();
			return lableData.find(frameID) != lableData.end() && lableData[frameID].find(name) != lableData[frameID].end();
		}

		/*
		* 添加单个数据
		*/
		template<class T>
		void appendLableData(const int frameID, const QString& name, T &data)
		{
			auto &lableData = getLableData<T>();
			lableData[frameID][name] = data;
		}

		/*
		* 删除单个数据
		*/
		template<class T>
		void deleteLableData(const int frameID, const QString& name)
		{
			auto &lableData = getLableData<T>();
			if (lableData.find(frameID) != lableData.end() && lableData[frameID].find(name) != lableData[frameID].end())
			{
				lableData[frameID].remove(name);
			}

		}

		
		/*
		* 获取单个数据（特化在cpp中实现）
		*/
		template<class T>
		QHash<int, QHash<QString, T>>& getLableData();

		///////////////////////某一帧某一类型数据操作////////////////////////////
		/*
		* 删除某一帧某一类型所有渲染 
		*/
		void deleteAllLableData_OneFrame_SameType(const int frameID, const LableOperateType type)
		{
			switch (type)
			{
			case LableOperateType_VecArrow:_lableData_post->_vecArrow_Post_Data.remove(frameID); //矢量箭头
				break;
			case LableOperateType_FixedFont_Post:_lableData_post->_font_Fixed_Post_Data.remove(frameID); //矢量箭头
				break;
			default:
				break;
			}
	
		}
		///////////////////////某一帧所有数据操作////////////////////////////
		/*
		* 删除某一帧帧所有渲染 
		*/
		void deleteAllLableData_OneFrame(const int frameID)
		{
			deleteAllLableData_OneFrame_SameType(frameID, LableOperateType_VecArrow);
			deleteAllLableData_OneFrame_SameType(frameID, LableOperateType_FixedFont_Post);
		}
		///////////////////////所有帧所有数据操作////////////////////////////
		/*
		* 删除所有帧所有渲染
		*/
		void deleteAllLableData_AllFrame()
		{
			_lableData_post->_vecArrow_Post_Data.clear();
			_lableData_post->_font_Fixed_Post_Data.clear();
		}

	private:
		//后处理数据
		shared_ptr<mLableData_post> _lableData_post = nullptr;
	};
}
