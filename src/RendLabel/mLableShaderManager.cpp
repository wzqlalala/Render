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
		//文字类着色器（带箭头方向）
		createShader(ShaderType_Font_Arrow, filePath + QString("Font_Arrow"));
		//文字类着色器（固定图标的文字）
		createShader(ShaderType_Font_FixShape, filePath + QString("Font_FixShape"));
		//文字类着色器（通用）
		createShader(ShaderType_Font_Common, filePath + QString("Font_Common"));
		//文字类着色器（固定）
		createShader(ShaderType_Font_Fixed, filePath + QString("Font_Fixed"));
		//箭头类着色器
		createShader(ShaderType_Arrow, filePath + QString("Lable_Arrow"));
		//全局坐标系
		createShader(ShaderType_GlobalAxis, filePath + QString("Lable_GlobalAxis"));
		//全局坐标系文字
		createShader(ShaderType_Font_GlobalAxis, filePath + QString("Font_GlobalAxis"));
		//固定类型着色器
		createShader(ShaderType_FixShape, filePath + QString("Lable_FixShape"));
		//固定类型着色器
		createShader(ShaderType_BCDF, filePath + QString("Lable_BCDF"));
		//模型绘制类型着色器（模型坐标系）
		createShader(ShaderType_Model, filePath + QString("Lable_Model"));
		//点渲染着色器（模型坐标系）
		createShader(ShaderType_Model_Point, filePath + QString("Lable_Point"));
		//固定形状加旋转类型
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