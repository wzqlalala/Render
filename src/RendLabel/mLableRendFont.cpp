#pragma once
#include "mLableRendFont.h"
#include"mLableRendBase.h"
#include"mLableDataStruct.h"
#include <QOpenGLWidget>
#include<memory>
//Qt
#include<qapplication.h>
#include<QVector>
//freetype
#include "ft2build.h"
#include FT_FREETYPE_H
using namespace MDataLable;
namespace MLableRend
{
	/*
	* 加载文字纹理（把所有文字加载到一张纹理上）
	*/
	void mFontTexture::LoadFontTexture()
	{
		this->initializeOpenGLFunctions();
		//创建大纹理
		glGenTextures(1, &this->_texture);
		glBindTexture(GL_TEXTURE_2D, this->_texture);

		//设置纹理过滤的参数
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,      // 指定是二维图片
			0,                  // 指定多级渐远纹理的级别（0为基本级别）
			GL_RED,           // 纹理的使用的存储格式
			1024,				//设置1024*1024的大纹理来放字形纹理，每个字形纹理用50*50的像素方块来放（方块比字形纹理大），字形纹理左下角与方块左下角重合
			1024,
			0,                  //是否的边
			GL_RED,				 // 数据的格式，bmp中，windows,操作系统中存储的数据是bgr格式
			GL_UNSIGNED_BYTE,   //!数据是8bit数据
			NULL					//开始创建大纹理时没有数据
		);


		//将字形纹理贴在大纹理上
		FT_Library ft;
		//初始化FreeType库
		if (FT_Init_FreeType(&ft))
		{
			qDebug() << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
			return;
		}

		//创建字形
		FT_Face face;
		if (FT_New_Face(ft, (qApp->applicationDirPath() + "/arial.ttf").toLocal8Bit(), 0, &face))
		{
			qDebug() << "ERROR::FREETYPE: Failed to load font" << endl;
			return;
		}
		//设置字体大小
		FT_Set_Pixel_Sizes(face, 0, 48);	//宽度设为0即为根据高度动态计算出宽度
		//禁用字节对齐限制
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (GLubyte c = 0; c < 128; c++)
		{
			// 加载字符的字形
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				qDebug() << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
				continue;
			}

			glTexSubImage2D(
				GL_TEXTURE_2D,
				0,
				50 * (c % 20),
				50 * (c / 20),
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);

			Character character =
			{
				QVector2D(50 * (c % 20),50 * (c / 20)),					//文字纹理左下角的像素坐标
				QVector2D(face->glyph->bitmap.width,face->glyph->bitmap.rows),
				QVector2D(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x,
			};
			_characters.insert(static_cast<GLchar>(c), character);
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glBindTexture(GL_TEXTURE_2D, 0);
		FT_Done_FreeType(ft);
	}
}
