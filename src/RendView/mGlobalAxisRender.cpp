//#include "mGlobalAxisRender.h"
//#include "mArrowRender.h"
//#include "mFontRender.h"
//
//#include <renderpch.h>
//#include "mShaderManage.h"
//
//#include <QVector3D>
//
//#include "mCommonView.h"
//#include "mModelView.h"
//
//using namespace mxr;
//namespace MBaseRend
//{
//	//VBO��������
//	QVector<float> mGlobalAxisRender::_VBOGlobalAxisData_Type;  //���� 0����X�� 1ΪY�� 2ΪZ��
//	QVector<QVector3D>   mGlobalAxisRender::_VBOGlobalAxisData_Vertex;
//	//EBO��������
//	QVector<GLuint> mGlobalAxisRender::_EBOGlobalAxisData;
//
//	mGlobalAxisRender::mGlobalAxisRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, std::shared_ptr<mFontRender> fontRender) :mBaseRender(app, parent)
//	{
//		qDebug() << "mGlobalAxisRender" << QString::number(long long int(QOpenGLContext::currentContext()), 16);
//		this->makeCurrent();
//		qDebug() << "mGlobalAxisRender" << QString::number(long long int(QOpenGLContext::currentContext()), 16);
//		_state = MakeAsset<StateSet>();
//		_state->setAttributeAndModes(MakeAsset<Depth>(), 0);
//		_state->setAttributeAndModes(MakeAsset<PolygonMode>(PolygonMode::FRONT_AND_BACK, PolygonMode::LINE), 1);
//		_state->setDrawMode(GL_LINES);
//
//		asset_ref<Shader> shader = mShaderManage::GetInstance()->GetShader("GlobalAxis");
//		_state->setShader(shader);
//
//		_state->setUniform(MakeAsset<Uniform>("uModel_c", QMatrix4x4()));
//		_state->setUniform(MakeAsset<Uniform>("uView_c", QMatrix4x4()));
//		_state->setUniform(MakeAsset<Uniform>("uProjection_c", QMatrix4x4()));
//		_state->setUniform(MakeAsset<Uniform>("uScr_width", float(0)));
//		_state->setUniform(MakeAsset<Uniform>("uScr_height", float(0)));
//		_state->setUniform(MakeAsset<Uniform>("uPers_width", float(0)));
//		_state->setUniform(MakeAsset<Uniform>("uPers_height", float(0)));
//		_state->setUniform(MakeAsset<Uniform>("uAuxX_new", QVector3D()));
//		_state->setUniform(MakeAsset<Uniform>("uAuxY_new", QVector3D()));
//		_state->setUniform(MakeAsset<Uniform>("uAuxZ_new", QVector3D()));
//
//		InitialAxisData();
//
//		_drawable = MakeAsset<Drawable>();
//		asset_ref<FloatArray> bytearray = MakeAsset<FloatArray>(_VBOGlobalAxisData_Type);
//		asset_ref<Vec3Array> vec3array = MakeAsset<Vec3Array>(_VBOGlobalAxisData_Vertex);
//		asset_ref<UIntArray> uintarray = MakeAsset<UIntArray>(_EBOGlobalAxisData);
//		_drawable->setVertexAttribArray(0, bytearray);
//		_drawable->setVertexAttribArray(1, vec3array);
//		_drawable->setIndexAttribute(uintarray);
//		_drawable->setStateSet(_state);
//
//		_parent->addChild(_drawable);
//		_fontRender = fontRender;
//		_fontRender->appendGloabalAxisFont();
//	}
//
//	mGlobalAxisRender::~mGlobalAxisRender()
//	{
//		_parent->removeChild(_drawable);
//	}
//
//	void mGlobalAxisRender::updateUniform(std::shared_ptr<mViewBase> modelView, std::shared_ptr<mViewBase> commonView)
//	{
//		_state->getUniform("uModel_c")->SetData(commonView->_model);
//		_state->getUniform("uView_c")->SetData(commonView->_view);
//		_state->getUniform("uProjection_c")->SetData(commonView->_projection);
//		_state->getUniform("uScr_width")->SetData(float(modelView->SCR_WIDTH));
//		_state->getUniform("uScr_height")->SetData(float(modelView->SCR_HEIGHT));
//		_state->getUniform("uPers_width")->SetData(float(commonView->_Right - commonView->_Left));
//		_state->getUniform("uPers_height")->SetData(float(commonView->_Top - commonView->_Bottom));
//		_state->getUniform("uAuxX_new")->SetData(QVector3D::crossProduct(commonView->_Up, commonView->_Eye).normalized());
//		_state->getUniform("uAuxY_new")->SetData(commonView->_Up.normalized());
//		_state->getUniform("uAuxZ_new")->SetData(commonView->_Eye.normalized());
//
//
//		_fontRender->updateUniform(modelView, commonView); 
//	}
//
//	void mGlobalAxisRender::InitialAxisData()
//	{
//		for (int i = 0; i < 3; i++)
//		{
//			for (int j = 0; j < _Arrow_Vertices.size(); j++)
//			{
//				//type
//				_VBOGlobalAxisData_Type.push_back(i);
//				//vertex
//				_VBOGlobalAxisData_Vertex.push_back(_Arrow_Vertices[j]);
//
//			}
//
//			//EBO��������
//			int BeginIndex = _VBOGlobalAxisData_Type.size() - _Arrow_Vertices.size();//�ҵ�������ӵ���ʼ��
//
//			//ֱ��
//			_EBOGlobalAxisData.push_back(BeginIndex);//0
//			_EBOGlobalAxisData.push_back(BeginIndex + 1);//1		
//
//			_EBOGlobalAxisData.push_back(BeginIndex + 1);//1
//			_EBOGlobalAxisData.push_back(BeginIndex + 2);//2
//
//			_EBOGlobalAxisData.push_back(BeginIndex + 1);//1
//			_EBOGlobalAxisData.push_back(BeginIndex + 3);//3
//
//			_EBOGlobalAxisData.push_back(BeginIndex + 1);//1
//			_EBOGlobalAxisData.push_back(BeginIndex + 4);//4
//
//			_EBOGlobalAxisData.push_back(BeginIndex + 1);//1
//			_EBOGlobalAxisData.push_back(BeginIndex + 5);//5
//
//			_EBOGlobalAxisData.push_back(BeginIndex + 2);//2
//			_EBOGlobalAxisData.push_back(BeginIndex + 3);//3
//
//			_EBOGlobalAxisData.push_back(BeginIndex + 3);//3
//			_EBOGlobalAxisData.push_back(BeginIndex + 4);//4
//
//			_EBOGlobalAxisData.push_back(BeginIndex + 4);//4
//			_EBOGlobalAxisData.push_back(BeginIndex + 5);//5
//
//			_EBOGlobalAxisData.push_back(BeginIndex + 5);//5
//			_EBOGlobalAxisData.push_back(BeginIndex + 2);//2		
//
//
//		}
//	}
//}
