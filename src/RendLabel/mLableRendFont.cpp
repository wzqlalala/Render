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
	* ���������������������ּ��ص�һ�������ϣ�
	*/
	void mFontTexture::LoadFontTexture()
	{
		this->initializeOpenGLFunctions();
		//����������
		glGenTextures(1, &this->_texture);
		glBindTexture(GL_TEXTURE_2D, this->_texture);

		//����������˵Ĳ���
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,      // ָ���Ƕ�άͼƬ
			0,                  // ָ���༶��Զ����ļ���0Ϊ��������
			GL_RED,           // �����ʹ�õĴ洢��ʽ
			1024,				//����1024*1024�Ĵ�����������������ÿ������������50*50�����ط������ţ��������������󣩣������������½��뷽�����½��غ�
			1024,
			0,                  //�Ƿ�ı�
			GL_RED,				 // ���ݵĸ�ʽ��bmp�У�windows,����ϵͳ�д洢��������bgr��ʽ
			GL_UNSIGNED_BYTE,   //!������8bit����
			NULL					//��ʼ����������ʱû������
		);


		//�������������ڴ�������
		FT_Library ft;
		//��ʼ��FreeType��
		if (FT_Init_FreeType(&ft))
		{
			qDebug() << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
			return;
		}

		//��������
		FT_Face face;
		if (FT_New_Face(ft, (qApp->applicationDirPath() + "/arial.ttf").toLocal8Bit(), 0, &face))
		{
			qDebug() << "ERROR::FREETYPE: Failed to load font" << endl;
			return;
		}
		//���������С
		FT_Set_Pixel_Sizes(face, 0, 48);	//�����Ϊ0��Ϊ���ݸ߶ȶ�̬��������
		//�����ֽڶ�������
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (GLubyte c = 0; c < 128; c++)
		{
			// �����ַ�������
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
				QVector2D(50 * (c % 20),50 * (c / 20)),					//�����������½ǵ���������
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
