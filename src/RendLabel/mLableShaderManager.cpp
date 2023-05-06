#include "mLableShaderManager.h"
#include<qapplication.h>
namespace MLableRend
{
	mLableShaderManager::mLableShaderManager()
	{
		createAllShader();
	}
	mLableShaderManager * mLableShaderManager::getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new mLableShaderManager;
		}
		return _instance;
	}

	mLableShaderManager* mLableShaderManager::_instance = nullptr;

	void mLableShaderManager::createAllShader()
	{
		this->initializeOpenGLFunctions();
		QString filePath = QString(":/GLSLLabel/");
		//��������ɫ��������ͷ����
		createShader(ShaderType_Font_Arrow, filePath + QString("Font_Arrow"));
		//��������ɫ�����̶�ͼ������֣�
		createShader(ShaderType_Font_FixShape, filePath + QString("Font_FixShape"));
		//��������ɫ����ͨ�ã�
		createShader(ShaderType_Font_Common, filePath + QString("Font_Common"));
		//��������ɫ�����̶���
		createShader(ShaderType_Font_Fixed, filePath + QString("Font_Fixed"));
		//��ͷ����ɫ��
		createShader(ShaderType_Arrow, filePath + QString("Lable_Arrow"));
		//ȫ������ϵ
		createShader(ShaderType_GlobalAxis, filePath + QString("Lable_GlobalAxis"));
		//ȫ������ϵ����
		createShader(ShaderType_Font_GlobalAxis, filePath + QString("Font_GlobalAxis"));
		//�̶�������ɫ��
		createShader(ShaderType_FixShape, filePath + QString("Lable_FixShape"));
		//�̶�������ɫ��
		createShader(ShaderType_BCDF, filePath + QString("Lable_BCDF"));
		//ģ�ͻ���������ɫ����ģ������ϵ��
		createShader(ShaderType_Model, filePath + QString("Lable_Model"));
		//����Ⱦ��ɫ����ģ������ϵ��
		createShader(ShaderType_Model_Point, filePath + QString("Lable_Point"));
		//�̶���״����ת����
		createShader(ShaderType_FixShapeRotate, filePath + QString("Lable_FixShapeRotate"));
		createShader(ShaderType_FixShapeRotate2, filePath + QString("Lable_FixShapeRotate2"));

	}

	void mLableShaderManager::createShader(const ShaderType &type, const QString &path)
	{
		shared_ptr<QOpenGLShaderProgram> shader = make_shared<QOpenGLShaderProgram>();
		if (shader == nullptr)
		{
			return;
		}
		bool success = shader->addShaderFromSourceFile(QOpenGLShader::Vertex, path + QString(".vert"));
		if (!success) {
			qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shader->log();
			return;
		}
		success = shader->addShaderFromSourceFile(QOpenGLShader::Fragment, path+ QString(".frag"));
		if (!success) {
			qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shader->log();
			return ;
		}
		success = shader->link();
		if (!success) {
			qDebug() << "shaderProgram link failed!" << shader->log();
			return ;
		}
		_shader[type] = shader;

	}

	shared_ptr<QOpenGLShaderProgram> mLableShaderManager::getShader(const ShaderType &type)
	{
		if (_shader.find(type) != _shader.end())
		{
			return _shader[type];
		}
		return nullptr;
	}
}