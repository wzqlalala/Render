#pragma once
//�����������
#pragma execution_character_set("utf-8")
#include "rendbasic_global.h"

namespace MAppConfiguration
{
	class mFileConfiguration;
	//class mUnitConfiguration;

	class RENDBASIC_EXPORT mAppConfigurationSingleton
	{
	public:
		/*
		* ��ȡ����ָ��
		*/
		static mAppConfigurationSingleton* getInstance();

		/*
		* ��ȡ�ļ�����
		*/
		mFileConfiguration* getFileConfiguration();

		/*
		* ��ȡ��λ������
		*/
		//mUnitConfiguration* getUnitConfiguration();

	private:
		/*
		* Ĭ�Ϲ��캯��
		*/
		mAppConfigurationSingleton();

		/*
		* ��������
		*/
		~mAppConfigurationSingleton();

	private:
		//����ָ��
		static mAppConfigurationSingleton* _instance;

		//�����ļ�
		mFileConfiguration* _fileConfiguration;

		//��λ��
		//mUnitConfiguration* _unitConfiguration;
	};
}
