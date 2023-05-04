#pragma once
#include "rendview_global.h"

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>

//MViewBasic
#include "mMeshViewEnum.h"

#include "app.h"

namespace mxr
{
	class Drawable;
	class Shader;
	class Group;
}

namespace MBaseRend
{
	class RENDVIEW_EXPORT mBackGroundRender : public QObject
	{
		Q_OBJECT

	public:
		/*
		* Ĭ�Ϲ��캯��
		*/
		mBackGroundRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> root);
		/*
		* ��������
		*/
		~mBackGroundRender();
		/*
		 * ���ñ�������ʾģʽ
		 */
		void setBackGroundMode(MViewBasic::BackGroundMode backgroundmode);
		/*
		 * ��ȡ��������ʾģʽ
		 */
		MViewBasic::BackGroundMode getBackGroundMode();
		/*
		 * �������»������ҵ���ɫ
		 */
		void setGradientColor(QVector3D color1,QVector3D color2);
		/*
		 * ��ȡ���»������ҵ���ɫ
		 */
		void getGradientColor(QVector3D &color1, QVector3D &color2);
		/*
		 * ���ô�ɫ����ɫ
		 */
		void setPureColor(QVector3D color);
		/*
		 * ��ȡ��ɫ����ɫ
		 */
		void getPureColor(QVector3D &color);
	private:	
		std::shared_ptr<mxr::Shader> _backgroundShader;
		std::shared_ptr<mxr::Drawable> _drawable;
		//�������εĶ�������
		QVector<QVector2D> _backGroundVertex;
		//�������ε���ɫֵ
		QVector<QVector3D> _backGroundColor;
		//��Ⱦ������ģʽ
		MViewBasic::BackGroundMode _backGroundMode;
	};
}
