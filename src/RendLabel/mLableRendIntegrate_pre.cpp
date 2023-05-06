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
		//文字纹理信息类创建
		shared_ptr<mFontTexture> ft = make_shared<mFontTexture>();
		//创建隐式渲染对象
		createRendObject_implicit(ft);
		//创建显示渲染对象
		createRendObject_explicit(ft);
		//创建电磁渲染对象
		createRendObject_magnet(ft);		
	}
	void mLableRendIntegrate_pre::drawLable()
	{
		glEnable(GL_DEPTH_TEST);
		/*
		* 模型坐标系下绘制的通用模型渲染
		*/
		shared_ptr<QOpenGLShaderProgram> shaderModel = mLableShaderManager::getInstance()->getShader(ShaderType_Model);
		shaderModel->bind();
		shaderModel->setUniformValue("uModel_m", _modelView->_model);
		shaderModel->setUniformValue("uView_m", _modelView->_view);
		shaderModel->setUniformValue("uProjection_m", _modelView->_projection);
		shaderModel->setUniformValue("uAlpha",(float)1.0);
		//刚性、柔性、mpc连接
		_lineRend_connect_i->_rendObject->draw();
		//电焊、节点刚体、合并刚体、额外节点、转动副、球铰、圆柱副、移动副、锁止关节、平面副、万向节、齿轮传动、皮带轮传动、齿轮 - 齿条传动、对接焊缝、角焊缝、铆接、运动耦合、
		//分布耦合、节点绑定、壳-实体耦合、平动约束、通用刚度、通用点焊、移动刚度、弯曲-扭转刚度
		_lineRend_connect_e->_rendObject->draw();
		//面面绑定 失效绑定连接
		_faceTofaceRend_connect_i->_rendObject->draw();
		//隐式接触
		_faceTofaceRend_contact_i->_rendObject->draw();
		//显示接触（点对面 面对面）
		_faceTofaceRend_contact_e->_rendObject->draw();
		//弹簧 阻尼
		_lineRend_RC_i->_rendObject->draw();

				
		//刚性墙中的线
		glLineWidth(2);
		_lineRend_RW_e->_rendObject->draw();
		glLineWidth(1);

		shaderModel->setUniformValue("uAlpha", (float)0.8);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//盒子线
		glLineWidth(2);
		_lineRend_box_e->_rendObject->draw();
		glLineWidth(1);
		glEnable(GL_POLYGON_OFFSET_FILL);//开启多边形偏移

		glPolygonOffset(1.0, 1.0);//设置偏移量，一般-1,1即可
		//刚性墙中的面
		_faceRend_RW_e->_rendObject->draw();
		//盒子面
		_faceRend_box_e->_rendObject->draw();
		glDisable(GL_POLYGON_OFFSET_POINT);//关闭多边形偏移

		glDisable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			
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
		//（集中力、重力、力矩、线载荷、压强、预紧力、离心力、点载荷、面载荷 ）
		_arrowRend_load_i->_rendObject->draw();
		_arrowRend_load_e->_rendObject->draw();
		//边电荷/边电流   磁化强度
		_arrowRend_load_mag->_rendObject->draw();
		//滑移
		_arrowRend_BC_e->_rendObject->draw();
		//磁矢量位
		_arrowRend_BC_mag->_rendObject->draw();
		//局部坐标系
		_arrowRend_axis_i->_rendObject->draw();
		_arrowRend_axis_e->_rendObject->draw();
		_arrowRend_axis_mag->_rendObject->draw();
		//刚性中的坐标系
		_axisRend_RW_e->_rendObject->draw();
		//初始速度
		_arrowRend_initCondition_e->_rendObject->draw();
		//向量
		_arrowRend_vector_e->_rendObject->draw();
		//材料铺层坐标系
		_arrowRend_plyCoordinate_i->_rendObject->draw();

		/*
		* 固定类型图标渲染
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
		//表面电荷密度 表面电流密度 表面磁通密度 表面电场强度
		_triangleRend_load_mag->_rendObject->draw();
		_rhombusRend_load_i->_rendObject->draw();
		glLineWidth(3.0);
		_XRend_load_i->_rendObject->draw();
		glLineWidth(1.0);
		_lightningRend_load_i->_rendObject->draw();
		//节点/表面/体 电荷   节点/表面 电流
		_lightningRend_load_mag->_rendObject->draw();
		glLineWidth(2.0);
		_homeRend_BC_i->_rendObject->draw();
		_homeRend_BC_mag->_rendObject->draw();
		glLineWidth(1.0);
		//初始条件
		_triangleRend_initCondition_e->_rendObject->draw();

		//面面绑定、失效绑定
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		_faceTofaceRend_connect_e->_rendObject->draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//刚体连接  旋转副
		shaderFixShape->setUniformValue("uView_c", _commonView->_view);
		_sphereRend_RC_i->_rendObject->draw();


		/*
		* 固定类型+旋转图标渲染
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
		* 固定类型+旋转图标渲染2次旋转（刚体连接平移副）
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
		* 箭头组合（约束自由度类型图标）
		*/
		shared_ptr<QOpenGLShaderProgram> shaderBCDF = mLableShaderManager::getInstance()->getShader(ShaderType_BCDF);
		shaderBCDF->bind();
		shaderBCDF->setUniformValue("uModel_c", _commonView->_model);
		shaderBCDF->setUniformValue("uView_c", _commonView->_view);
		shaderBCDF->setUniformValue("uProjection_c", _commonView->_projection);
		shaderBCDF->setUniformValue("uModel_m", _modelView->_model);
		shaderBCDF->setUniformValue("uView_m", _modelView->_view);
		shaderBCDF->setUniformValue("uProjection_m", _modelView->_projection);
		//约束
		_dfRend_BC_i->_rendObject->draw();
		//刚体约束
		_dfRend_RBC_i->_rendObject->draw();
		//刚体载荷
		_dfRend_load_e->_rendObject->draw();
		//速度 加速度 角加速度
		_dfRend_BC_e->_rendObject->draw();
		//刚体运动
		_dfRend_connect_e->_rendObject->draw();
		/*
		* 文本渲染
		*/
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		///文字渲染视角mpv
		QMatrix4x4 projection, model;
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
		//（集中力、重力、力矩、线载荷、压强、预紧力、离心力、点载荷、面载荷 、边 电荷/电流、磁化强度）文字
		_arrowFontRend_load_i->_rendObject->draw();
		_arrowFontRend_load_e->_rendObject->draw();
		_arrowFontRend_load_mag->_rendObject->draw();
		//坐标系文字
		_arrowFontRend_axis_i->_rendObject->draw();
		_arrowFontRend_axis_e->_rendObject->draw();
		_arrowFontRend_axis_mag->_rendObject->draw();
		//初始速度文字
		_arrowFontRend_initCondition_e->_rendObject->draw();
		//刚性墙坐标系文字
		_axisFontRend_RW_e->_rendObject->draw();


		//固定形状图标文本
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
		//初始条件文字
		_triangleFontRend_initCondition_e->_rendObject->draw();



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
		//刚性、柔性、mpc连接文本
		_lineFontRend_connect_i->_rendObject->draw();
		//电焊、节点刚体、合并刚体、额外节点、转动副、球铰、圆柱副、移动副、锁止关节、平面副、万向节、齿轮传动、皮带轮传动、齿轮-齿条传动、对接焊缝、角焊缝、铆接、运动耦合、
		//分布耦合、节点绑定、壳-实体耦合、平动约束、通用刚度、通用点焊、移动刚度、弯曲-扭转刚度文本
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
		* 箭头+文字组合类型
		*/
		//（集中力、重力、力矩、线载荷、压强、预紧力、离心力）
		_arrowRend_load_i = make_shared<mLableRendArrow<Arrow_Font_Load_Implicit>>(lableData_i->_arrow_Font_Load_Implicit_Data);
		_arrowFontRend_load_i = make_shared<mLableRendFont<Arrow_Font_Load_Implicit>>(lableData_i->_arrow_Font_Load_Implicit_Data, 10, ft);
		//(局部坐标系)
		_arrowRend_axis_i = make_shared<mLableRendAxis<Arrow_Font_Axis_Implicit>>(lableData_i->_arrow_Font_Axis_Implicit_Data);
		_arrowFontRend_axis_i = make_shared<mLableRendFont<Arrow_Font_Axis_Implicit>>(lableData_i->_arrow_Font_Axis_Implicit_Data, 10, ft);
		//材料坐标系
		_arrowRend_plyCoordinate_i = make_shared<mLableRendArrow<Arrow_PlyCoordinate_Implicit>>(lableData_i->_arrow_PlyCoordinate_Implicit_Data);

		/*
		* 固定形状类型
		*/
		//三角形+文字 组合类型图标渲染对象 （热流密度，电荷密度）
		_triangleRend_load_i = make_shared<mLableRendFixShape<Triangle_Font_Load_Implicit>>(lableData_i->_triangle_Font_Load_Implicit_Data, mLableShapeVertex::getInstance()->_TriVertices, GL_TRIANGLES);
		_triangleFontRend_load_i = make_shared<mLableRendFont<Triangle_Font_Load_Implicit>>(lableData_i->_triangle_Font_Load_Implicit_Data, 9, ft);

		//菱形图标+文字 组合类型图标渲染对象（对流换热，热辐射）
		_rhombusRend_load_i = make_shared<mLableRendFixShape<Rhombus_Font_Load_Implicit>>(lableData_i->_rhombus_Font_Load_Implicit_Data, mLableShapeVertex::getInstance()->_RhombusVertices, GL_QUADS);
		_rhombusFontRend_load_i = make_shared<mLableRendFont<Rhombus_Font_Load_Implicit>>(lableData_i->_rhombus_Font_Load_Implicit_Data, 9, ft);

		//十字图标+文字 组合类型图标渲染对象(内部热源,温度载荷)
		_XRend_load_i = make_shared<mLableRendFixShape<X_Font_Load_Implicit>>(lableData_i->_X_Font_Load_Implicit_Data, mLableShapeVertex::getInstance()->_XVertices, GL_LINES);
		_XFontRend_load_i = make_shared<mLableRendFont<X_Font_Load_Implicit>>(lableData_i->_X_Font_Load_Implicit_Data, 9, ft);

		//闪电型图标+文字 组合类型图标渲染对象(电荷)
		_lightningRend_load_i = make_shared<mLableRendFixShape<Lightning_Font_Load_Implicit>>(lableData_i->_lightning_Font_Load_Implicit_Data, mLableShapeVertex::getInstance()->_LightningVertices, GL_TRIANGLES);
		_lightningFontRend_load_i = make_shared<mLableRendFont<Lightning_Font_Load_Implicit>>(lableData_i->_lightning_Font_Load_Implicit_Data, 9, ft);
		
		//房屋型 图标渲染对象（初始边界条件(初始温度），温度边界，电势）
		_homeRend_BC_i = make_shared<mLableRendFixShape<Home_BC_Implicit>>(lableData_i->_home_BC_Implicit_Data, mLableShapeVertex::getInstance()->_HomeVertices, GL_LINES);

		//刚体连接渲染对象  （旋转副，平面副、圆柱形副）
		_RCRend_type1_i = make_shared<mLableRendFixShapeRotate<Type1_RC_Implicit>>(lableData_i->_Type1_RC_Implicit_Data, GL_LINES);

		//刚体连接渲染对象  （平移副）
		_RCRend_type2_i = make_shared<mLableRendFixShapeRotate2<Type2_RC_Implicit>>(lableData_i->_Type2_RC_Implicit_Data, GL_LINES);

		//球形渲染对象（圆球铰副）
		_sphereRend_RC_i = make_shared<mLableRendFixShape<Sphere_RC_Implicit>>(lableData_i->_sphere_RC_Implicit_Data, mLableShapeVertex::getInstance()->_Sphere_Vertex, GL_LINES);

		/*
		* 箭头组合类型自由度约束类型 
		*/
		//固定 对称/反对称 位移转角 初始条件 虚拟约束
		_dfRend_BC_i = make_shared<mLableRendBCDF<DF_BC_Implicit>>(lableData_i->_df_BC_Implicit_Data);
		// 刚体约束
		_dfRend_RBC_i = make_shared<mLableRendBCDF<DF_RBC_Implicit>>(lableData_i->_df_RBC_Implicit_Data);

		/*
		* 模型坐标系下绘制的各类图标（连接、接触）
		*/
		//直线型（刚性连接 柔性连接 mpc连接）
		_lineRend_connect_i = make_shared<mLableRendModel<Line_Connect_Implicit>>(lableData_i->_line_Connect_Implicit_Data, GL_LINES);
		_lineFontRend_connect_i = make_shared<mLableRendFont2<Line_Connect_Implicit>>(lableData_i->_line_Connect_Implicit_Data, ft);
		//面对面型（面面绑定连接）
		_faceTofaceRend_connect_i = make_shared<mLableRendFaceToFace<FaceToFace_Connect_Implicit>>(lableData_i->_faceToFace_Connect_Implicit_Data, GL_LINES);
		//面对面型（接触）
		_faceTofaceRend_contact_i = make_shared<mLableRendFaceToFace<FaceToFace_Contact_Implicit>>(lableData_i->_faceToFace_Contact_Implicit_Data, GL_LINES); 
		//直线型（弹簧 阻尼）
		_lineRend_RC_i = make_shared<mLableRendModel<Line_RC_Implicit>>(lableData_i->_line_RC_Implicit_Data, GL_LINES);

		




	}
	void mLableRendIntegrate_pre::createRendObject_explicit(shared_ptr<mFontTexture> ft)
	{
		shared_ptr<mLableData_explicit> lableData_e = _lableDataController->getLableDataBase<mLableData_explicit>();
		/*
		* 箭头+文字组合类型
		*/
		//（刚性墙中的坐标系）
		_axisRend_RW_e = make_shared<mLableRendAxis<Axis_RW_Explicit>>(lableData_e->_axis_RW_Explicit_Data);
		_axisFontRend_RW_e = make_shared<mLableRendFont<Axis_RW_Explicit>>(lableData_e->_axis_RW_Explicit_Data, 10, ft);
		//(局部坐标系)
		_arrowRend_axis_e = make_shared<mLableRendAxis<Arrow_Font_Axis_Explicit>>(lableData_e->_arrow_Font_Axis_Explicit_Data);
		_arrowFontRend_axis_e = make_shared<mLableRendFont<Arrow_Font_Axis_Explicit>>(lableData_e->_arrow_Font_Axis_Explicit_Data, 10, ft);
		//（点载荷、面载荷、重力）
		_arrowRend_load_e = make_shared<mLableRendArrow<Arrow_Font_Load_Explicit>>(lableData_e->_arrow_Font_Load_Explicit_Data);
		_arrowFontRend_load_e = make_shared<mLableRendFont<Arrow_Font_Load_Explicit>>(lableData_e->_arrow_Font_Load_Explicit_Data, 10, ft);
		//（滑移）
		_arrowRend_BC_e = make_shared<mLableRendArrow<Arrow_BC_Explicit>>(lableData_e->_arrow_BC_Explicit_Data);
		//（初始速度）
		_arrowRend_initCondition_e = make_shared<mLableRendArrow<Arrow_Font_InitCondition_Explicit>>(lableData_e->_arrow_Font_InitCondition_Explicit_Data);
		_arrowFontRend_initCondition_e = make_shared<mLableRendFont<Arrow_Font_InitCondition_Explicit>>(lableData_e->_arrow_Font_InitCondition_Explicit_Data, 10, ft);
		//向量
		_arrowRend_vector_e = make_shared<mLableRendArrow<Arrow_Vector_Explicit>>(lableData_e->_arrow_Vector_Explicit_Data);
		
		/*
		* 固定形状类型
		*/
		//三角形+文字 组合类型图标渲染对象 （初始应力、SPH初始应力、初始炸点）
		_triangleRend_initCondition_e = make_shared<mLableRendFixShape<Triangle_Font_InitCondition_Explicit>>(lableData_e->_triangle_Font_InitCondition_Explicit_Data, mLableShapeVertex::getInstance()->_TriVertices, GL_TRIANGLES);
		_triangleFontRend_initCondition_e = make_shared<mLableRendFont<Triangle_Font_InitCondition_Explicit>>(lableData_e->_triangle_Font_InitCondition_Explicit_Data, 9, ft);


		/*
		* 箭头组合类型自由度约束类型
		*/
		//刚体载荷
		_dfRend_load_e = make_shared<mLableRendBCDF<DF_Load_Explicit>>(lableData_e->_df_Load_Explicit_Data);
		//位移 速度 加速度
		_dfRend_BC_e = make_shared<mLableRendBCDF<DF_BC_Explicit>>(lableData_e->_df_BC_Explicit_Data);
		//刚体运动
		_dfRend_connect_e = make_shared<mLableRendBCDF<DF_Connect_Explicit>>(lableData_e->_df_Connect_Explicit_Data);
		/*
		* 模型坐标系下绘制的各类图标（连接、接触）
		*/
		//直线型（刚性连接 柔性连接 mpc连接）
		_lineRend_connect_e = make_shared<mLableRendModel<Line_Connect_Explicit>>(lableData_e->_line_Connect_Explicit_Data, GL_LINES);
		_lineFontRend_connect_e = make_shared<mLableRendFont2<Line_Connect_Explicit>>(lableData_e->_line_Connect_Explicit_Data, ft);
		//刚性墙中的（线、面）
		_lineRend_RW_e = make_shared<mLableRendModel<Line_RW_Explicit>>(lableData_e->_line_RW_Explicit_Data, GL_LINES);
		_faceRend_RW_e = make_shared<mLableRendModel<Face_RW_Explicit>>(lableData_e->_face_RW_Explicit_Data, GL_QUADS);
		//面对面型（面面绑定、失效绑定连接）
		_faceTofaceRend_connect_e = make_shared<mLableRendFaceToFace<FaceToFace_Connect_Explicit>>(lableData_e->_faceToFace_Connect_Explicit_Data, mLableShapeVertex::getInstance()->_QuadVertices, GL_QUADS);
		//面对面型（点对面 面对面接触）
		_faceTofaceRend_contact_e = make_shared<mLableRendFaceToFace<FaceToFace_Contact_Explicit>>(lableData_e->_faceToFace_Contact_Explicit_Data, mLableShapeVertex::getInstance()->_QuadVertices, GL_QUADS);
		//盒子
		_faceRend_box_e = make_shared<mLableRendModel<Face_Box_Explicit>>(lableData_e->_face_Box_Explicit_Data, GL_QUADS);
		_lineRend_box_e = make_shared<mLableRendModel<Line_Box_Explicit>>(lableData_e->_line_Box_Explicit_Data, GL_LINES);

	}
	void mLableRendIntegrate_pre::createRendObject_magnet(shared_ptr<mFontTexture> ft)
	{
		shared_ptr<mLableData_magnet> lableData_mag = _lableDataController->getLableDataBase<mLableData_magnet>();
		/*
		* 箭头+文字组合类型
		*/	
		//(局部坐标系)
		_arrowRend_axis_mag = make_shared<mLableRendAxis<Arrow_Font_Axis_Magnet>>(lableData_mag->_arrow_Font_Axis_Magnet_Data);
		_arrowFontRend_axis_mag = make_shared<mLableRendFont<Arrow_Font_Axis_Magnet>>(lableData_mag->_arrow_Font_Axis_Magnet_Data, 10, ft);
		//（边 电荷/电流   磁化强度）
		_arrowRend_load_mag = make_shared<mLableRendArrow<Arrow_Font_Load_Magnet>>(lableData_mag->_arrow_Font_Load_Magnet_Data);
		_arrowFontRend_load_mag = make_shared<mLableRendFont<Arrow_Font_Load_Magnet>>(lableData_mag->_arrow_Font_Load_Magnet_Data, 10, ft);
		//磁矢量位
		_arrowRend_BC_mag = make_shared<mLableRendArrow<Arrow_BC_Magnet>>(lableData_mag->_arrow_BC_Magnet_Data);
		/*
		* 固定形状类型
		*/
		//三角形+文字 组合类型图标渲染对象 （表面电荷密度 表面电流密度 表面磁通密度 表面电场强度）
		_triangleRend_load_mag = make_shared<mLableRendFixShape<Triangle_Font_Load_Magnet>>(lableData_mag->_triangle_Font_Load_Magnet_Data, mLableShapeVertex::getInstance()->_TriVertices, GL_TRIANGLES);
		_triangleFontRend_load_mag = make_shared<mLableRendFont<Triangle_Font_Load_Magnet>>(lableData_mag->_triangle_Font_Load_Magnet_Data, 9, ft);

		//闪电型图标+文字 组合类型图标渲染对象(节点/表面/体 电荷   节点/表面 电流)
		_lightningRend_load_mag = make_shared<mLableRendFixShape<Lightning_Font_Load_Magnet>>(lableData_mag->_lightning_Font_Load_Magnet_Data, mLableShapeVertex::getInstance()->_LightningVertices, GL_TRIANGLES);
		_lightningFontRend_load_mag = make_shared<mLableRendFont<Lightning_Font_Load_Magnet>>(lableData_mag->_lightning_Font_Load_Magnet_Data, 9, ft);
		//房屋型 图标渲染对象（标量电位）
		_homeRend_BC_mag = make_shared<mLableRendFixShape<Home_BC_Magnet>>(lableData_mag->_home_BC_Magnet_Data, mLableShapeVertex::getInstance()->_HomeVertices, GL_LINES);
	}

}
