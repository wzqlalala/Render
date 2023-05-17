#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <memory>
#include <qvector.h>
#include <qmap.h>
#include "asset.h"

namespace mxr
{
	class Shader;
	class RENDBASE_EXPORT mShaderManage
	{
	public:
		static mShaderManage* GetInstance();
		mxr::Shader* GetShader(QString pathname);

		void deleteAllShaders();

	protected:
		mShaderManage();

		~mShaderManage();
		QMap<QString, mxr::Shader*> _shaders;

	};

}