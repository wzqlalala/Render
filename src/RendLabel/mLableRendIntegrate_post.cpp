#include "mLableRendIntegrate_post.h"
#include"mLableShaderManager.h"
#include"mLableRendArrow.h"
#include"mLableRendFont.h"
//MDataLable
#include"mLableDataController_post.cpp"
//MViewBasic
#include"mCommonView.h"
#include"mModelView.h"
namespace MLableRend
{
	mLableRendIntegrate_post::mLableRendIntegrate_post()
	{
		_lableDataController = make_shared<mLableDataController_post>();
		createRendObject();
		
	}
	mLableRendIntegrate_post::~mLableRendIntegrate_post()
	{
	}

	void mLableRendIntegrate_post::createRendObject()
	{
		//文字纹理信息类创建
		shared_ptr<mFontTexture> ft = make_shared<mFontTexture>();
		//创建后处理渲染对象
		createRendObject_post(ft);
	}

	void mLableRendIntegrate_post::drawLable()
	{
		glEnable(GL_DEPTH_TEST);

		/*
		* 箭头类渲染
		*/
		shared_ptr<QOpenGLShaderProgram> shaderArrow = mLableShaderManager::getInstance()->getShader(ShaderType_Arrow);
		shaderArrow->bind();
		shaderArrow->setUniformValue("uModel_c", _commonView->_model);
		shaderArrow->setUniformValue("uView_c", _commonView->_view);
		shaderArrow->setUniformValue("uProjection_c", _commonView->_projection);
		shaderArrow->setUniformValue("uModel_m", _modelView->_model);
		shaderArrow->setUniformValue("uView_m", _modelView->_view);
		shaderArrow->setUniformValue("uProjection_m", _modelView->_projection);
		//矢量箭头
		_vecArrow_post->_rendObject->draw();

		/*
		* 文本渲染
		*/
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		///文字渲染视角mpv
		QMatrix4x4 projection, model, view;
		projection.ortho(0, _scr_width, 0, _scr_height, 0.1, 99);
		view.lookAt(QVector3D(0, 0, 2), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
		model.setToIdentity();
		//箭头顶端的文本
		shared_ptr<QOpenGLShaderProgram> shaderFontArrow = mLableShaderManager::getInstance()->getShader(ShaderType_Font_Arrow);
		shaderFontArrow->bind();
		shaderFontArrow->setUniformValue("uProjection_m", _modelView->_projection);
		shaderFontArrow->setUniformValue("uView_m", _modelView->_view);
		shaderFontArrow->setUniformValue("uModel_m", _modelView->_model);
		shaderFontArrow->setUniformValue("uProjection_c", _commonView->_projection);
		shaderFontArrow->setUniformValue("uView_c", _commonView->_view);
		shaderFontArrow->setUniformValue("uModel_c", _commonView->_model);
		shaderFontArrow->setUniformValue("uScr_width", _scr_width);
		shaderFontArrow->setUniformValue("uScr_height", _scr_height);
		shaderFontArrow->setUniformValue("uProjection_font", projection);
		shaderFontArrow->setUniformValue("uView_font", view);
		shaderFontArrow->setUniformValue("uModel_font", model);
		shaderFontArrow->setUniformValue("uRatio", _ratio);
		

		//通用文本
		shared_ptr<QOpenGLShaderProgram> shaderFontCommon = mLableShaderManager::getInstance()->getShader(ShaderType_Font_Common);
		shaderFontCommon->bind();
		shaderFontCommon->setUniformValue("uProjection_m", _modelView->_projection);
		shaderFontCommon->setUniformValue("uView_m", _modelView->_view);
		shaderFontCommon->setUniformValue("uModel_m", _modelView->_model);
		shaderFontCommon->setUniformValue("uScr_width", _scr_width);
		shaderFontCommon->setUniformValue("uScr_height", _scr_height);
		shaderFontCommon->setUniformValue("uProjection_font", projection);
		shaderFontCommon->setUniformValue("uView_font", view);
		shaderFontCommon->setUniformValue("uModel_font", model);
		shaderFontCommon->setUniformValue("uRatio", _ratio);


		//固定位置文本
		shared_ptr<QOpenGLShaderProgram> shaderFontFixed = mLableShaderManager::getInstance()->getShader(ShaderType_Font_Fixed);
		shaderFontFixed->bind();
		shaderFontFixed->setUniformValue("uScr_width", _scr_width);
		shaderFontFixed->setUniformValue("uScr_height", _scr_height);
		shaderFontFixed->setUniformValue("uProjection_font", projection);
		shaderFontFixed->setUniformValue("uView_font", view);
		shaderFontFixed->setUniformValue("uModel_font", model);
		shaderFontFixed->setUniformValue("uRatio", _ratio);
		//固定文字
		_fontRend_fixed_post->_rendObject->draw();
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);


	}
	shared_ptr<mLableDataController_post> mLableRendIntegrate_post::getLableDataController()
	{
		return _lableDataController;
	}
	void mLableRendIntegrate_post::createRendObject_post(shared_ptr<mFontTexture> ft)
	{
		if (_lableDataController == nullptr)
		{
			return;
		}
		shared_ptr<mLableData_post> lableData_post = _lableDataController->getLableDataBase();
		//矢量箭头
		_vecArrow_post = make_shared<mLableRendArrow2<VectorArrow_Post>>(lableData_post->_vecArrow_Post_Data);
		//固定文字
		_fontRend_fixed_post = make_shared<mLableRendFont3<Fixed_Font_Post>>(lableData_post->_font_Fixed_Post_Data, ft);


	}
}