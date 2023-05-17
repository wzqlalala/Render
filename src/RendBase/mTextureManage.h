#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <memory>
#include <qvector.h>
#include <qmap.h>
#include "asset.h"

namespace mxr
{
	class Texture;
	class RENDBASE_EXPORT mTextureManage
	{
	public:
		static mTextureManage* GetInstance();
		mxr::Texture *GetTexture(QString pathname, GLuint levels = 0);
		void deleteAllTextures();
	protected:
		mTextureManage();
		~mTextureManage();
		QMap<QString, mxr::Texture*> _textures;

	};

}