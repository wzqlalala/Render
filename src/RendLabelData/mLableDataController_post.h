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
		* ��ȡ���ݿ�
		*/
		shared_ptr<mLableData_post> getLableDataBase()
		{
			return _lableData_post;
		}

		////////////////////////�������ݲ���////////////////////////
		/*
		* �ж������Ƿ����
		*/
		template<class T>
		bool isLableDataExist(const int frameID, const QString& name)
		{
			auto &lableData = getLableData<T>();
			return lableData.find(frameID) != lableData.end() && lableData[frameID].find(name) != lableData[frameID].end();
		}

		/*
		* ��ӵ�������
		*/
		template<class T>
		void appendLableData(const int frameID, const QString& name, T &data)
		{
			auto &lableData = getLableData<T>();
			lableData[frameID][name] = data;
		}

		/*
		* ɾ����������
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
		* ��ȡ�������ݣ��ػ���cpp��ʵ�֣�
		*/
		template<class T>
		QHash<int, QHash<QString, T>>& getLableData();

		///////////////////////ĳһ֡ĳһ�������ݲ���////////////////////////////
		/*
		* ɾ��ĳһ֡ĳһ����������Ⱦ 
		*/
		void deleteAllLableData_OneFrame_SameType(const int frameID, const LableOperateType type)
		{
			switch (type)
			{
			case LableOperateType_VecArrow:_lableData_post->_vecArrow_Post_Data.remove(frameID); //ʸ����ͷ
				break;
			case LableOperateType_FixedFont_Post:_lableData_post->_font_Fixed_Post_Data.remove(frameID); //ʸ����ͷ
				break;
			default:
				break;
			}
	
		}
		///////////////////////ĳһ֡�������ݲ���////////////////////////////
		/*
		* ɾ��ĳһ֡֡������Ⱦ 
		*/
		void deleteAllLableData_OneFrame(const int frameID)
		{
			deleteAllLableData_OneFrame_SameType(frameID, LableOperateType_VecArrow);
			deleteAllLableData_OneFrame_SameType(frameID, LableOperateType_FixedFont_Post);
		}
		///////////////////////����֡�������ݲ���////////////////////////////
		/*
		* ɾ������֡������Ⱦ
		*/
		void deleteAllLableData_AllFrame()
		{
			_lableData_post->_vecArrow_Post_Data.clear();
			_lableData_post->_font_Fixed_Post_Data.clear();
		}

	private:
		//��������
		shared_ptr<mLableData_post> _lableData_post = nullptr;
	};
}
