#include "mTextureManage.h"
#include "renderpch.h"
#include <qapplication.h>
using namespace mxr;
namespace mxr
{
	mTextureManage *mTextureManage::GetInstance()
	{
		static mTextureManage manage;
		return &manage;
	}

	mxr::Texture *mTextureManage::GetTexture(QString pathname, GLuint levels)
	{
		if (_textures.contains(pathname))
		{
			return _textures[pathname];
		}
		else
		{
			//asset_ref<Texture> texture = MakeAsset<Texture>(qApp->applicationDirPath().toStdString()
			//	+ "/../Texture/" + pathname.toStdString(), levels);
			Texture *texture = new Texture(":/Icon/" + pathname, levels);
			_textures[pathname] = texture;
			return texture;
		}

	}

	void mTextureManage::deleteAllTextures()
	{
		for (auto texture : _textures)
		{
			delete texture;
		}
		_textures.clear();
	}

	mTextureManage::mTextureManage()
	{


	}

	mTextureManage::~mTextureManage()
	{

	}

}
