#pragma once
#include "rendview_global.h"

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>

//MViewBasic
#include "mBasicEnum.h"

namespace mxr
{
	class Drawable;
	class Shader;
	class Group;
	class Application;
}
using namespace MxFunctions;
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
		void setBackGroundMode(BackGroundMode backgroundmode);
		/*
		 * ��ȡ��������ʾģʽ
		 */
		BackGroundMode getBackGroundMode();
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
		mxr::Shader * _backgroundShader;
		std::shared_ptr<mxr::Drawable> _drawable;
		//�������εĶ�������
		QVector<QVector2D> _backGroundVertex;
		//�������ε���ɫֵ
		QVector<QVector3D> _backGroundColor;
		//��Ⱦ������ģʽ
		BackGroundMode _backGroundMode;
	};
}
