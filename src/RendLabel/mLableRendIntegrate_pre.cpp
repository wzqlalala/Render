#include "mLableRendIntegrate_pre.h"
#include"mLableShaderManager.h"
#include"mLableRendArrow.h"
#include"mLableRendFixShape.h"
#include"mLableRendBCDF.h"
#include"mLableRendFont.h"
#include"mLableShapeVertex.h"
#include"mLableRendModel.h"
#include"mLableRendFaceToFace.h"
#include"mLableRendFixShapeRotate.h"
#include"mLableRendFixShapeRotate2.h"
#include"mLableRendAxis.h"
//MDataLable
#include"mLableDataController_pre.cpp"

//MViewBasic
#include"mCommonView.h"
#include"mModelView.h"
namespace MLableRend
{
	mLableRendIntegrate_pre::mLableRendIntegrate_pre()
	{
		_lableDataController = make_shared<mLableDataController_pre>();
		createRendObject();
	}
	mLableRendIntegrate_pre::~mLableRendIntegrate_pre()
	{
	}

	void mLableRendIntegrate_pre::createRendObject()
	{
		//����������Ϣ�ഴ��
		shared_ptr<mFontTexture> ft = make_shared<mFontTexture>();
		//������ʽ��Ⱦ����
		createRendObject_implicit(ft);
		//������ʾ��Ⱦ����
		createRendObject_explicit(ft);
		//���������Ⱦ����
		createRendObject_magnet(ft);		
	}
	void mLableRendIntegrate_pre::drawLable()
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
		shaderModel->setUniformValue("uAlpha",(float)1.0);
		//���ԡ����ԡ�mpc����
		_lineRend_connect_i->_rendObject->draw();
		//�纸���ڵ���塢�ϲ����塢����ڵ㡢ת��������¡�Բ�������ƶ�������ֹ�ؽڡ�ƽ�渱������ڡ����ִ�����Ƥ���ִ��������� - �����������ԽӺ��졢�Ǻ��졢í�ӡ��˶���ϡ�
		//�ֲ���ϡ��ڵ�󶨡���-ʵ����ϡ�ƽ��Լ����ͨ�øնȡ�ͨ�õ㺸���ƶ��նȡ�����-Ťת�ն�
		_lineRend_connect_e->_rendObject->draw();
		//����� ʧЧ������
		_faceTofaceRend_connect_i->_rendObject->draw();
		//��ʽ�Ӵ�
		_faceTofaceRend_contact_i->_rendObject->draw();
		//��ʾ�Ӵ�������� ����棩
		_faceTofaceRend_contact_e->_rendObject->draw();
		//���� ����
		_lineRend_RC_i->_rendObject->draw();

				
		//����ǽ�е���
		glLineWidth(2);
		_lineRend_RW_e->_rendObject->draw();
		glLineWidth(1);

		shaderModel->setUniformValue("uAlpha", (float)0.8);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//������
		glLineWidth(2);
		_lineRend_box_e->_rendObject->draw();
		glLineWidth(1);
		glEnable(GL_POLYGON_OFFSET_FILL);//���������ƫ��

		glPolygonOffset(1.0, 1.0);//����ƫ������һ��-1,1����
		//����ǽ�е���
		_faceRend_RW_e->_rendObject->draw();
		//������
		_faceRend_box_e->_rendObject->draw();
		glDisable(GL_POLYGON_OFFSET_POINT);//�رն����ƫ��

		glDisable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			
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
		//�������������������ء����غɡ�ѹǿ��Ԥ�����������������غɡ����غ� ��
		_arrowRend_load_i->_rendObject->draw();
		_arrowRend_load_e->_rendObject->draw();
		//�ߵ��/�ߵ���   �Ż�ǿ��
		_arrowRend_load_mag->_rendObject->draw();
		//����
		_arrowRend_BC_e->_rendObject->draw();
		//��ʸ��λ
		_arrowRend_BC_mag->_rendObject->draw();
		//�ֲ�����ϵ
		_arrowRend_axis_i->_rendObject->draw();
		_arrowRend_axis_e->_rendObject->draw();
		_arrowRend_axis_mag->_rendObject->draw();
		//�����е�����ϵ
		_axisRend_RW_e->_rendObject->draw();
		//��ʼ�ٶ�
		_arrowRend_initCondition_e->_rendObject->draw();
		//����
		_arrowRend_vector_e->_rendObject->draw();
		//�����̲�����ϵ
		_arrowRend_plyCoordinate_i->_rendObject->draw();

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
	
		_triangleRend_load_i->_rendObject->draw();
		//�������ܶ� ��������ܶ� �����ͨ�ܶ� ����糡ǿ��
		_triangleRend_load_mag->_rendObject->draw();
		_rhombusRend_load_i->_rendObject->draw();
		glLineWidth(3.0);
		_XRend_load_i->_rendObject->draw();
		glLineWidth(1.0);
		_lightningRend_load_i->_rendObject->draw();
		//�ڵ�/����/�� ���   �ڵ�/���� ����
		_lightningRend_load_mag->_rendObject->draw();
		glLineWidth(2.0);
		_homeRend_BC_i->_rendObject->draw();
		_homeRend_BC_mag->_rendObject->draw();
		glLineWidth(1.0);
		//��ʼ����
		_triangleRend_initCondition_e->_rendObject->draw();

		//����󶨡�ʧЧ��
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		_faceTofaceRend_connect_e->_rendObject->draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//��������  ��ת��
		shaderFixShape->setUniformValue("uView_c", _commonView->_view);
		_sphereRend_RC_i->_rendObject->draw();


		/*
		* �̶�����+��תͼ����Ⱦ
		*/
		shared_ptr<QOpenGLShaderProgram> shaderFixShapeRotate = mLableShaderManager::getInstance()->getShader(ShaderType_FixShapeRotate);
		shaderFixShapeRotate->bind();
		shaderFixShapeRotate->setUniformValue("uModel_c", _commonView->_model);
		shaderFixShapeRotate->setUniformValue("uView_c", _commonView->_view);
		shaderFixShapeRotate->setUniformValue("uProjection_c", _commonView->_projection);
		shaderFixShapeRotate->setUniformValue("uModel_m", _modelView->_model);
		shaderFixShapeRotate->setUniformValue("uView_m", _modelView->_view);
		shaderFixShapeRotate->setUniformValue("uProjection_m", _modelView->_projection);
		_RCRend_type1_i->_rendObject->draw();
		/*
		* �̶�����+��תͼ����Ⱦ2����ת����������ƽ�Ƹ���
		*/
		shared_ptr<QOpenGLShaderProgram> shaderFixShapeRotate2 = mLableShaderManager::getInstance()->getShader(ShaderType_FixShapeRotate2);
		shaderFixShapeRotate2->bind();
		shaderFixShapeRotate2->setUniformValue("uModel_c", _commonView->_model);
		shaderFixShapeRotate2->setUniformValue("uView_c", _commonView->_view);
		shaderFixShapeRotate2->setUniformValue("uProjection_c", _commonView->_projection);
		shaderFixShapeRotate2->setUniformValue("uModel_m", _modelView->_model);
		shaderFixShapeRotate2->setUniformValue("uView_m", _modelView->_view);
		shaderFixShapeRotate2->setUniformValue("uProjection_m", _modelView->_projection);
		_RCRend_type2_i->_rendObject->draw();


		/*
		* ��ͷ��ϣ�Լ�����ɶ�����ͼ�꣩
		*/
		shared_ptr<QOpenGLShaderProgram> shaderBCDF = mLableShaderManager::getInstance()->getShader(ShaderType_BCDF);
		shaderBCDF->bind();
		shaderBCDF->setUniformValue("uModel_c", _commonView->_model);
		shaderBCDF->setUniformValue("uView_c", _commonView->_view);
		shaderBCDF->setUniformValue("uProjection_c", _commonView->_projection);
		shaderBCDF->setUniformValue("uModel_m", _modelView->_model);
		shaderBCDF->setUniformValue("uView_m", _modelView->_view);
		shaderBCDF->setUniformValue("uProjection_m", _modelView->_projection);
		//Լ��
		_dfRend_BC_i->_rendObject->draw();
		//����Լ��
		_dfRend_RBC_i->_rendObject->draw();
		//�����غ�
		_dfRend_load_e->_rendObject->draw();
		//�ٶ� ���ٶ� �Ǽ��ٶ�
		_dfRend_BC_e->_rendObject->draw();
		//�����˶�
		_dfRend_connect_e->_rendObject->draw();
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
		//��ͷ���˵��ı�
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
		//�������������������ء����غɡ�ѹǿ��Ԥ�����������������غɡ����غ� ���� ���/�������Ż�ǿ�ȣ�����
		_arrowFontRend_load_i->_rendObject->draw();
		_arrowFontRend_load_e->_rendObject->draw();
		_arrowFontRend_load_mag->_rendObject->draw();
		//����ϵ����
		_arrowFontRend_axis_i->_rendObject->draw();
		_arrowFontRend_axis_e->_rendObject->draw();
		_arrowFontRend_axis_mag->_rendObject->draw();
		//��ʼ�ٶ�����
		_arrowFontRend_initCondition_e->_rendObject->draw();
		//����ǽ����ϵ����
		_axisFontRend_RW_e->_rendObject->draw();


		//�̶���״ͼ���ı�
		shared_ptr<QOpenGLShaderProgram> shaderFontFixShape = mLableShaderManager::getInstance()->getShader(ShaderType_Font_FixShape);
		shaderFontFixShape->bind();
		shaderFontFixShape->setUniformValue("uProjection_m", _modelView->_projection);
		shaderFontFixShape->setUniformValue("uView_m", _modelView->_view);
		shaderFontFixShape->setUniformValue("uModel_m", _modelView->_model);
		shaderFontFixShape->setUniformValue("uScr_width", _scr_width);
		shaderFontFixShape->setUniformValue("uScr_height", _scr_height);
		shaderFontFixShape->setUniformValue("uProjection_font", projection);
		shaderFontFixShape->setUniformValue("uView_font", view);
		shaderFontFixShape->setUniformValue("uModel_font", model);
		shaderFontFixShape->setUniformValue("uRatio", _ratio);

		_triangleFontRend_load_i->_rendObject->draw();
		_triangleFontRend_load_mag->_rendObject->draw();
		_rhombusFontRend_load_i->_rendObject->draw();
		_XFontRend_load_i->_rendObject->draw();
		_lightningFontRend_load_i->_rendObject->draw();
		_lightningFontRend_load_mag->_rendObject->draw();
		//��ʼ��������
		_triangleFontRend_initCondition_e->_rendObject->draw();



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
		//���ԡ����ԡ�mpc�����ı�
		_lineFontRend_connect_i->_rendObject->draw();
		//�纸���ڵ���塢�ϲ����塢����ڵ㡢ת��������¡�Բ�������ƶ�������ֹ�ؽڡ�ƽ�渱������ڡ����ִ�����Ƥ���ִ���������-�����������ԽӺ��졢�Ǻ��졢í�ӡ��˶���ϡ�
		//�ֲ���ϡ��ڵ�󶨡���-ʵ����ϡ�ƽ��Լ����ͨ�øնȡ�ͨ�õ㺸���ƶ��նȡ�����-Ťת�ն��ı�
		_lineFontRend_connect_e->_rendObject->draw();

		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);


	}
	shared_ptr<mLableDataController_pre> mLableRendIntegrate_pre::getLableDataController()
	{
		return _lableDataController;
	}

	void mLableRendIntegrate_pre::createRendObject_implicit(shared_ptr<mFontTexture> ft)
	{
		shared_ptr<mLableData_implicit> lableData_i = _lableDataController->getLableDataBase<mLableData_implicit>();
		
		/*
		* ��ͷ+�����������
		*/
		//�������������������ء����غɡ�ѹǿ��Ԥ��������������
		_arrowRend_load_i = make_shared<mLableRendArrow<Arrow_Font_Load_Implicit>>(lableData_i->_arrow_Font_Load_Implicit_Data);
		_arrowFontRend_load_i = make_shared<mLableRendFont<Arrow_Font_Load_Implicit>>(lableData_i->_arrow_Font_Load_Implicit_Data, 10, ft);
		//(�ֲ�����ϵ)
		_arrowRend_axis_i = make_shared<mLableRendAxis<Arrow_Font_Axis_Implicit>>(lableData_i->_arrow_Font_Axis_Implicit_Data);
		_arrowFontRend_axis_i = make_shared<mLableRendFont<Arrow_Font_Axis_Implicit>>(lableData_i->_arrow_Font_Axis_Implicit_Data, 10, ft);
		//��������ϵ
		_arrowRend_plyCoordinate_i = make_shared<mLableRendArrow<Arrow_PlyCoordinate_Implicit>>(lableData_i->_arrow_PlyCoordinate_Implicit_Data);

		/*
		* �̶���״����
		*/
		//������+���� �������ͼ����Ⱦ���� �������ܶȣ�����ܶȣ�
		_triangleRend_load_i = make_shared<mLableRendFixShape<Triangle_Font_Load_Implicit>>(lableData_i->_triangle_Font_Load_Implicit_Data, mLableShapeVertex::getInstance()->_TriVertices, GL_TRIANGLES);
		_triangleFontRend_load_i = make_shared<mLableRendFont<Triangle_Font_Load_Implicit>>(lableData_i->_triangle_Font_Load_Implicit_Data, 9, ft);

		//����ͼ��+���� �������ͼ����Ⱦ���󣨶������ȣ��ȷ��䣩
		_rhombusRend_load_i = make_shared<mLableRendFixShape<Rhombus_Font_Load_Implicit>>(lableData_i->_rhombus_Font_Load_Implicit_Data, mLableShapeVertex::getInstance()->_RhombusVertices, GL_QUADS);
		_rhombusFontRend_load_i = make_shared<mLableRendFont<Rhombus_Font_Load_Implicit>>(lableData_i->_rhombus_Font_Load_Implicit_Data, 9, ft);

		//ʮ��ͼ��+���� �������ͼ����Ⱦ����(�ڲ���Դ,�¶��غ�)
		_XRend_load_i = make_shared<mLableRendFixShape<X_Font_Load_Implicit>>(lableData_i->_X_Font_Load_Implicit_Data, mLableShapeVertex::getInstance()->_XVertices, GL_LINES);
		_XFontRend_load_i = make_shared<mLableRendFont<X_Font_Load_Implicit>>(lableData_i->_X_Font_Load_Implicit_Data, 9, ft);

		//������ͼ��+���� �������ͼ����Ⱦ����(���)
		_lightningRend_load_i = make_shared<mLableRendFixShape<Lightning_Font_Load_Implicit>>(lableData_i->_lightning_Font_Load_Implicit_Data, mLableShapeVertex::getInstance()->_LightningVertices, GL_TRIANGLES);
		_lightningFontRend_load_i = make_shared<mLableRendFont<Lightning_Font_Load_Implicit>>(lableData_i->_lightning_Font_Load_Implicit_Data, 9, ft);
		
		//������ ͼ����Ⱦ���󣨳�ʼ�߽�����(��ʼ�¶ȣ����¶ȱ߽磬���ƣ�
		_homeRend_BC_i = make_shared<mLableRendFixShape<Home_BC_Implicit>>(lableData_i->_home_BC_Implicit_Data, mLableShapeVertex::getInstance()->_HomeVertices, GL_LINES);

		//����������Ⱦ����  ����ת����ƽ�渱��Բ���θ���
		_RCRend_type1_i = make_shared<mLableRendFixShapeRotate<Type1_RC_Implicit>>(lableData_i->_Type1_RC_Implicit_Data, GL_LINES);

		//����������Ⱦ����  ��ƽ�Ƹ���
		_RCRend_type2_i = make_shared<mLableRendFixShapeRotate2<Type2_RC_Implicit>>(lableData_i->_Type2_RC_Implicit_Data, GL_LINES);

		//������Ⱦ����Բ��¸���
		_sphereRend_RC_i = make_shared<mLableRendFixShape<Sphere_RC_Implicit>>(lableData_i->_sphere_RC_Implicit_Data, mLableShapeVertex::getInstance()->_Sphere_Vertex, GL_LINES);

		/*
		* ��ͷ����������ɶ�Լ������ 
		*/
		//�̶� �Գ�/���Գ� λ��ת�� ��ʼ���� ����Լ��
		_dfRend_BC_i = make_shared<mLableRendBCDF<DF_BC_Implicit>>(lableData_i->_df_BC_Implicit_Data);
		// ����Լ��
		_dfRend_RBC_i = make_shared<mLableRendBCDF<DF_RBC_Implicit>>(lableData_i->_df_RBC_Implicit_Data);

		/*
		* ģ������ϵ�»��Ƶĸ���ͼ�꣨���ӡ��Ӵ���
		*/
		//ֱ���ͣ��������� �������� mpc���ӣ�
		_lineRend_connect_i = make_shared<mLableRendModel<Line_Connect_Implicit>>(lableData_i->_line_Connect_Implicit_Data, GL_LINES);
		_lineFontRend_connect_i = make_shared<mLableRendFont2<Line_Connect_Implicit>>(lableData_i->_line_Connect_Implicit_Data, ft);
		//������ͣ���������ӣ�
		_faceTofaceRend_connect_i = make_shared<mLableRendFaceToFace<FaceToFace_Connect_Implicit>>(lableData_i->_faceToFace_Connect_Implicit_Data, GL_LINES);
		//������ͣ��Ӵ���
		_faceTofaceRend_contact_i = make_shared<mLableRendFaceToFace<FaceToFace_Contact_Implicit>>(lableData_i->_faceToFace_Contact_Implicit_Data, GL_LINES); 
		//ֱ���ͣ����� ���ᣩ
		_lineRend_RC_i = make_shared<mLableRendModel<Line_RC_Implicit>>(lableData_i->_line_RC_Implicit_Data, GL_LINES);

		




	}
	void mLableRendIntegrate_pre::createRendObject_explicit(shared_ptr<mFontTexture> ft)
	{
		shared_ptr<mLableData_explicit> lableData_e = _lableDataController->getLableDataBase<mLableData_explicit>();
		/*
		* ��ͷ+�����������
		*/
		//������ǽ�е�����ϵ��
		_axisRend_RW_e = make_shared<mLableRendAxis<Axis_RW_Explicit>>(lableData_e->_axis_RW_Explicit_Data);
		_axisFontRend_RW_e = make_shared<mLableRendFont<Axis_RW_Explicit>>(lableData_e->_axis_RW_Explicit_Data, 10, ft);
		//(�ֲ�����ϵ)
		_arrowRend_axis_e = make_shared<mLableRendAxis<Arrow_Font_Axis_Explicit>>(lableData_e->_arrow_Font_Axis_Explicit_Data);
		_arrowFontRend_axis_e = make_shared<mLableRendFont<Arrow_Font_Axis_Explicit>>(lableData_e->_arrow_Font_Axis_Explicit_Data, 10, ft);
		//�����غɡ����غɡ�������
		_arrowRend_load_e = make_shared<mLableRendArrow<Arrow_Font_Load_Explicit>>(lableData_e->_arrow_Font_Load_Explicit_Data);
		_arrowFontRend_load_e = make_shared<mLableRendFont<Arrow_Font_Load_Explicit>>(lableData_e->_arrow_Font_Load_Explicit_Data, 10, ft);
		//�����ƣ�
		_arrowRend_BC_e = make_shared<mLableRendArrow<Arrow_BC_Explicit>>(lableData_e->_arrow_BC_Explicit_Data);
		//����ʼ�ٶȣ�
		_arrowRend_initCondition_e = make_shared<mLableRendArrow<Arrow_Font_InitCondition_Explicit>>(lableData_e->_arrow_Font_InitCondition_Explicit_Data);
		_arrowFontRend_initCondition_e = make_shared<mLableRendFont<Arrow_Font_InitCondition_Explicit>>(lableData_e->_arrow_Font_InitCondition_Explicit_Data, 10, ft);
		//����
		_arrowRend_vector_e = make_shared<mLableRendArrow<Arrow_Vector_Explicit>>(lableData_e->_arrow_Vector_Explicit_Data);
		
		/*
		* �̶���״����
		*/
		//������+���� �������ͼ����Ⱦ���� ����ʼӦ����SPH��ʼӦ������ʼը�㣩
		_triangleRend_initCondition_e = make_shared<mLableRendFixShape<Triangle_Font_InitCondition_Explicit>>(lableData_e->_triangle_Font_InitCondition_Explicit_Data, mLableShapeVertex::getInstance()->_TriVertices, GL_TRIANGLES);
		_triangleFontRend_initCondition_e = make_shared<mLableRendFont<Triangle_Font_InitCondition_Explicit>>(lableData_e->_triangle_Font_InitCondition_Explicit_Data, 9, ft);


		/*
		* ��ͷ����������ɶ�Լ������
		*/
		//�����غ�
		_dfRend_load_e = make_shared<mLableRendBCDF<DF_Load_Explicit>>(lableData_e->_df_Load_Explicit_Data);
		//λ�� �ٶ� ���ٶ�
		_dfRend_BC_e = make_shared<mLableRendBCDF<DF_BC_Explicit>>(lableData_e->_df_BC_Explicit_Data);
		//�����˶�
		_dfRend_connect_e = make_shared<mLableRendBCDF<DF_Connect_Explicit>>(lableData_e->_df_Connect_Explicit_Data);
		/*
		* ģ������ϵ�»��Ƶĸ���ͼ�꣨���ӡ��Ӵ���
		*/
		//ֱ���ͣ��������� �������� mpc���ӣ�
		_lineRend_connect_e = make_shared<mLableRendModel<Line_Connect_Explicit>>(lableData_e->_line_Connect_Explicit_Data, GL_LINES);
		_lineFontRend_connect_e = make_shared<mLableRendFont2<Line_Connect_Explicit>>(lableData_e->_line_Connect_Explicit_Data, ft);
		//����ǽ�еģ��ߡ��棩
		_lineRend_RW_e = make_shared<mLableRendModel<Line_RW_Explicit>>(lableData_e->_line_RW_Explicit_Data, GL_LINES);
		_faceRend_RW_e = make_shared<mLableRendModel<Face_RW_Explicit>>(lableData_e->_face_RW_Explicit_Data, GL_QUADS);
		//������ͣ�����󶨡�ʧЧ�����ӣ�
		_faceTofaceRend_connect_e = make_shared<mLableRendFaceToFace<FaceToFace_Connect_Explicit>>(lableData_e->_faceToFace_Connect_Explicit_Data, mLableShapeVertex::getInstance()->_QuadVertices, GL_QUADS);
		//������ͣ������ �����Ӵ���
		_faceTofaceRend_contact_e = make_shared<mLableRendFaceToFace<FaceToFace_Contact_Explicit>>(lableData_e->_faceToFace_Contact_Explicit_Data, mLableShapeVertex::getInstance()->_QuadVertices, GL_QUADS);
		//����
		_faceRend_box_e = make_shared<mLableRendModel<Face_Box_Explicit>>(lableData_e->_face_Box_Explicit_Data, GL_QUADS);
		_lineRend_box_e = make_shared<mLableRendModel<Line_Box_Explicit>>(lableData_e->_line_Box_Explicit_Data, GL_LINES);

	}
	void mLableRendIntegrate_pre::createRendObject_magnet(shared_ptr<mFontTexture> ft)
	{
		shared_ptr<mLableData_magnet> lableData_mag = _lableDataController->getLableDataBase<mLableData_magnet>();
		/*
		* ��ͷ+�����������
		*/	
		//(�ֲ�����ϵ)
		_arrowRend_axis_mag = make_shared<mLableRendAxis<Arrow_Font_Axis_Magnet>>(lableData_mag->_arrow_Font_Axis_Magnet_Data);
		_arrowFontRend_axis_mag = make_shared<mLableRendFont<Arrow_Font_Axis_Magnet>>(lableData_mag->_arrow_Font_Axis_Magnet_Data, 10, ft);
		//���� ���/����   �Ż�ǿ�ȣ�
		_arrowRend_load_mag = make_shared<mLableRendArrow<Arrow_Font_Load_Magnet>>(lableData_mag->_arrow_Font_Load_Magnet_Data);
		_arrowFontRend_load_mag = make_shared<mLableRendFont<Arrow_Font_Load_Magnet>>(lableData_mag->_arrow_Font_Load_Magnet_Data, 10, ft);
		//��ʸ��λ
		_arrowRend_BC_mag = make_shared<mLableRendArrow<Arrow_BC_Magnet>>(lableData_mag->_arrow_BC_Magnet_Data);
		/*
		* �̶���״����
		*/
		//������+���� �������ͼ����Ⱦ���� ���������ܶ� ��������ܶ� �����ͨ�ܶ� ����糡ǿ�ȣ�
		_triangleRend_load_mag = make_shared<mLableRendFixShape<Triangle_Font_Load_Magnet>>(lableData_mag->_triangle_Font_Load_Magnet_Data, mLableShapeVertex::getInstance()->_TriVertices, GL_TRIANGLES);
		_triangleFontRend_load_mag = make_shared<mLableRendFont<Triangle_Font_Load_Magnet>>(lableData_mag->_triangle_Font_Load_Magnet_Data, 9, ft);

		//������ͼ��+���� �������ͼ����Ⱦ����(�ڵ�/����/�� ���   �ڵ�/���� ����)
		_lightningRend_load_mag = make_shared<mLableRendFixShape<Lightning_Font_Load_Magnet>>(lableData_mag->_lightning_Font_Load_Magnet_Data, mLableShapeVertex::getInstance()->_LightningVertices, GL_TRIANGLES);
		_lightningFontRend_load_mag = make_shared<mLableRendFont<Lightning_Font_Load_Magnet>>(lableData_mag->_lightning_Font_Load_Magnet_Data, 9, ft);
		//������ ͼ����Ⱦ���󣨱�����λ��
		_homeRend_BC_mag = make_shared<mLableRendFixShape<Home_BC_Magnet>>(lableData_mag->_home_BC_Magnet_Data, mLableShapeVertex::getInstance()->_HomeVertices, GL_LINES);
	}

}
