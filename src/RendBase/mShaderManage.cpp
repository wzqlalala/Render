#include "mShaderManage.h"
#include "renderpch.h"
#include <qapplication.h>
using namespace mxr;
namespace mxr
{
	mShaderManage *mShaderManage::GetInstance()
	{
		static mShaderManage manage;
		return &manage;
	}

	mxr::Shader *mShaderManage::GetShader(QString pathname)
	{
		if (_shaders.contains(pathname))
		{
			return _shaders[pathname];
		}
		else
		{
			//asset_ref<Shader> shader = MakeAsset<Shader>(qApp->applicationDirPath().toStdString() 
			//	+ "/../GLSL/" + pathname.toStdString() + ".glsl");
			Shader *shader = new Shader(":/GLSL/" + pathname+ ".glsl");
			_shaders[pathname] = shader;
			return shader;
		}

	}

	void mShaderManage::deleteAllShaders()
	{
		for (auto shader : _shaders)
		{
			delete shader;
		}
		_shaders.clear();
	}

	mShaderManage::mShaderManage()
	{


	}

	mShaderManage::~mShaderManage()
	{
		
	}

}
