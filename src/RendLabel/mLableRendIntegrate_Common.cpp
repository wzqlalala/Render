#include "mLableRendIntegrate_common.h"
#include"mLableShaderManager.h"
#include"mLableRendFont.h"
#include"mLableRendModel.h"
#include"mLableRendFixShape.h"
#include"mLableShapeVertex.h"
#include"mLableRendAxis.h"
#include"mLableRendArrow.h"
//MDataLable
#include"mLableDataController_common.cpp"
//MViewBasic
#include"mCommonView.h"
#include"mModelView.h"
namespace MLableRend
{
	mLableRendIntegrate_common::mLableRendIntegrate_common()
	{
		_lableDataController = make_shared<mLableDataController_common>();
		createRendObject();
	}
	mLableRendIntegrate_common::~mLableRendIntegrate_common()
	{
	}

	void mLableRendIntegrate_common::createRendObject()
	{
		//����������Ϣ�ഴ��
		shared_ptr<mFontTexture> ft = make_shared<mFontTexture>();
		//����ͨ����Ⱦ����
		createRendObject_common(ft);
	}
	void mLableRendIntegrate_common::drawLable()
	{
		glEnable(GL_DEPTH_TEST);
		/*
		* ģ������ϵ�»��Ƶ�ͨ��ģ����Ⱦ
		*/
		shared_ptr<QOpenGLShaderProgram> shaderModel = mLableShaderManager::getInstance()->getShader(ShaderType_Model);
		shaderModel->bind();
		shaderModel->setUniformValue("uModel_m", _modelView->_model);
		shaderModel->setUniformValue("uView_m", _modelView->_view);
		shaderModel->setUniformValue("uProjection_m", _modelView->_projection);
		shaderModel->setUniformValue("uAlpha", (float)1.0);
		shaderModel->setUniformValue("uAlpha", (float)0.8);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//����������ʱ�������Ӵ�����������ӣ�
		_tempMeshRend_common->_rendObject->draw();
		glDisable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		/*
		* ģ������ϵ�»��Ƶ�
		*/
		shared_ptr<QOpenGLShaderProgram> shaderPoint = mLableShaderManager::getInstance()->getShader(ShaderType_Model_Point);
		shaderPoint->bind();
		shaderPoint->setUniformValue("uModel_m", _modelView->_model);
		shaderPoint->setUniformValue("uView_m", _modelView->_view);
		shaderPoint->setUniformValue("uProjection_m", _modelView->_projection);
		//��ʱ������
		glPointSize(8.0);
		_tempPointRend_common->_rendObject->draw();
		glPointSize(1.0);

		/*
		* ��ͷ����Ⱦ
		*/
		shared_ptr<QOpenGLShaderProgram> shaderArrow = mLableShaderManager::getInstance()->getShader(ShaderType_Arrow);
		shaderArrow->bind();
		shaderArrow->setUniformValue("uModel_c", _commonView->_model);
		shaderArrow->setUniformValue("uView_c", _commonView->_view);
		shaderArrow->setUniformValue("uProjection_c", _commonView->_projection);
		shaderArrow->setUniformValue("uModel_m", _modelView->_model);
		shaderArrow->setUniformValue("uView_m", _modelView->_view);
		shaderArrow->setUniformValue("uProjection_m", _modelView->_projection);
		//��ͷ
		_arrowRend_common->_rendObject->draw();
		/*
		* �̶�����ͼ����Ⱦ
		*/
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shared_ptr<QOpenGLShaderProgram> shaderFixShape = mLableShaderManager::getInstance()->getShader(ShaderType_FixShape);
		shaderFixShape->bind();
		shaderFixShape->setUniformValue("uModel_c", _commonView->_model);
		QMatrix4x4 view;
		view.lookAt(QVector3D(0, 0, 1), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
		shaderFixShape->setUniformValue("uView_c", view);
		shaderFixShape->setUniformValue("uProjection_c", _commonView->_projection);
		shaderFixShape->setUniformValue("uModel_m", _modelView->_model);
		shaderFixShape->setUniformValue("uView_m", _modelView->_view);
		shaderFixShape->setUniformValue("uProjection_m", _modelView->_projection);
		//��ת����
		_XPointRend_common->_rendObject->draw();

		shared_ptr<QOpenGLShaderProgram> shaderGlobalAxis = mLableShaderManager::getInstance()->getShader(ShaderType_GlobalAxis);
		shaderGlobalAxis->bind();
		//ͨ���ӽ��µ�PVM
		shaderGlobalAxis->setUniformValue("uModel_c", _commonView->_model);
		shaderGlobalAxis->setUniformValue("uView_c", _commonView->_view);
		shaderGlobalAxis->setUniformValue("uProjection_c", _commonView->_projection);
		//��Ļ�ߴ�
		shaderGlobalAxis->setUniformValue("uScr_width", _scr_width);
		shaderGlobalAxis->setUniformValue("uScr_height", _scr_height);
		//��ǰ�Ӿ���ߴ�
		shaderGlobalAxis->setUniformValue("uPers_width", float(_commonView->_Right - _commonView->_Left));
		shaderGlobalAxis->setUniformValue("uPers_height", float(_commonView->_Top - _commonView->_Bottom));
		//��ǰ�ӵ�״̬�µ�X��Y��Z��	
		shaderGlobalAxis->setUniformValue("uAuxX_new", QVector3D::crossProduct(_commonView->_Up, _commonView->_Eye).normalized());
		shaderGlobalAxis->setUniformValue("uAuxY_new", _commonView->_Up.normalized());
		shaderGlobalAxis->setUniformValue("uAuxZ_new", _commonView->_Eye.normalized());

		_globalAxisRend_common->_rendObject->draw();

			
		/*
		* �ı���Ⱦ
		*/
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		///������Ⱦ�ӽ�mpv
		QMatrix4x4 projection, model;
		projection.ortho(0, _scr_width, 0, _scr_height, 0.1, 99);
		view.lookAt(QVector3D(0, 0, 2), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
		model.setToIdentity();

		//ͨ���ı�
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

		//��ʱ����Ⱦ�ı�
		_tempPointFontRend_common->_rendObject->draw();
		//ͨ���ı�
		_fontRend_common_common->_rendObject->draw();


		//�̶�λ���ı�
		shared_ptr<QOpenGLShaderProgram> shaderFontFixed = mLableShaderManager::getInstance()->getShader(ShaderType_Font_Fixed);
		shaderFontFixed->bind();
		shaderFontFixed->setUniformValue("uScr_width", _scr_width);
		shaderFontFixed->setUniformValue("uScr_height", _scr_height);
		shaderFontFixed->setUniformValue("uProjection_font", projection);
		shaderFontFixed->setUniformValue("uView_font", view);
		shaderFontFixed->setUniformValue("uModel_font", model);
		shaderFontFixed->setUniformValue("uRatio", _ratio);
		//�̶�λ���ı�
		_fontRend_fixed_common->_rendObject->draw();


		shared_ptr<QOpenGLShaderProgram> shaderFontGlobalAxis = mLableShaderManager::getInstance()->getShader(ShaderType_Font_GlobalAxis);
		//ȫ������ϵ�ı�
		shaderFontGlobalAxis->bind();
		//�����ӽ��µ�PVM
		shaderFontGlobalAxis->setUniformValue("uView_font", view);
		shaderFontGlobalAxis->setUniformValue("uProjection_font", projection);
		//ͨ���ӽ��µ�PVM	 
		shaderFontGlobalAxis->setUniformValue("uModel_c", _commonView->_model);
		shaderFontGlobalAxis->setUniformValue("uView_c", _commonView->_view);
		shaderFontGlobalAxis->setUniformValue("uProjection_c", _commonView->_projection);
		//��Ļ�ߴ�			
		shaderFontGlobalAxis->setUniformValue("uScr_width", _scr_width);
		shaderFontGlobalAxis->setUniformValue("uScr_height", _scr_height);
		//��ǰ�Ӿ���ߴ�
		shaderFontGlobalAxis->setUniformValue("uPers_width", float(_commonView->_Right - _commonView->_Left));
		shaderFontGlobalAxis->setUniformValue("uPers_height", float(_commonView->_Top - _commonView->_Bottom));
		//��ǰ�ӵ�״̬�µ�X��Y��Z��	
		shaderFontGlobalAxis->setUniformValue("uAuxX_new", QVector3D::crossProduct(_commonView->_Up, _commonView->_Eye).normalized());
		shaderFontGlobalAxis->setUniformValue("uAuxY_new", _commonView->_Up.normalized());
		shaderFontGlobalAxis->setUniformValue("uAuxZ_new", _commonView->_Eye.normalized());

		shaderFontGlobalAxis->setUniformValue("uRatio", _ratio);
		_globalAxisFontRend_common->_rendObject->draw();


		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);


	}
	shared_ptr<mLableDataController_common> mLableRendIntegrate_common::getLableDataController()
	{
		return _lableDataController;
	}
	void mLableRendIntegrate_common::createRendObject_common(shared_ptr<mFontTexture> ft)
	{
		shared_ptr<mLableData_common> lableData_common = _lableDataController->getLableDataBase();
		//�̶�����
		_fontRend_fixed_common = make_shared<mLableRendFont2<Fixed_Font_Common>>(lableData_common->_font_Fixed_Common_Data, ft);
		//ͨ������
		_fontRend_common_common = make_shared<mLableRendFont2<Common_Font_Common>>(lableData_common->_font_Common_Common_Data, ft);
		//X���� ͼ����Ⱦ������ת���ģ�
		_XPointRend_common = make_shared<mLableRendFixShape<X_Point_Common>>(lableData_common->_XPoint_Common_Data, mLableShapeVertex::getInstance()->_XVertices2, GL_LINES);
		//��ʱ������
		_tempPointRend_common = make_shared<mLableRendModel<TempPoint_Common>>(lableData_common->_tempPoint_Common_Data, GL_POINTS);
		_tempPointFontRend_common = make_shared<mLableRendFont2<TempPoint_Common>>(lableData_common->_tempPoint_Common_Data, ft);
		//��ʱ��������
		_tempMeshRend_common = make_shared<mLableRendModel<TempMesh_Common>>(lableData_common->_tempMesh_Common_Data, GL_TRIANGLES);
		//ȫ������ϵ��Ⱦ
		//(�ֲ�����ϵ)
		_globalAxisRend_common = make_shared<mLableRendAxis<Arrow_Font_Axis_Common>>(lableData_common->_globalAxis_Common_Data);
		_globalAxisFontRend_common = make_shared<mLableRendFont<Arrow_Font_Axis_Common>>(lableData_common->_globalAxis_Common_Data, 10, ft);
		//��ͷ��Ⱦ
		_arrowRend_common = make_shared<mLableRendArrow<Arrow_Common>>(lableData_common->_arrow_Common_Data);
	}

}
