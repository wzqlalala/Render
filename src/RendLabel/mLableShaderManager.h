#pragma once
//Qt
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include<QMap>
//
#include"mLableRendEnum.h"
#include<memory>
using namespace std;
namespace MLableRend
{

	class mLableShaderManager : protected QOpenGLFunctions_3_3_Core
	{
	public:
		mLableShaderManager();
		/*
		* ��ȡ����ָ��
		*/
		static mLableShaderManager* getInstance();
		/*
		* ��ȡһ����ɫ��
		*/
		shared_ptr<QOpenGLShaderProgram> getShader(const ShaderType &type);

	private:
		/*
		* ����������ɫ��
		*/
		void createAllShader();
		/*
		* ����һ����ɫ��
		*/
		void createShader(const ShaderType &type, const QString &path);


	private:
		//����
		static mLableShaderManager* _instance;
		//shader����
		QMap<ShaderType, shared_ptr<QOpenGLShaderProgram>> _shader;
	};
}
