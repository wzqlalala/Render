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
		* 获取单例指针
		*/
		static mLableShaderManager* getInstance();
		/*
		* 获取一个着色器
		*/
		shared_ptr<QOpenGLShaderProgram> getShader(const ShaderType &type);

	private:
		/*
		* 创建所有着色器
		*/
		void createAllShader();
		/*
		* 创建一个着色器
		*/
		void createShader(const ShaderType &type, const QString &path);


	private:
		//单例
		static mLableShaderManager* _instance;
		//shader容器
		QMap<ShaderType, shared_ptr<QOpenGLShaderProgram>> _shader;
	};
}
