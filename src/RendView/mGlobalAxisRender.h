//#pragma once
////�����������
//#pragma execution_character_set("utf-8")
//
//#include <memory>
//
//#include "app.h"
//
//#include "mBaseRender.h"
//
//namespace mxr
//{
//	class Drawable;
//	class StateSet;
//	class Texture;
//	class Group;
//	class Uniform;
//	class Shader;
//}
//namespace MViewBasic
//{
//	class mCommonView;
//	class mModelView;
//}
//using namespace MViewBasic;
//namespace MBaseRend
//{
//	class mFontRender;
//	class mGlobalAxisRender :public mBaseRender
//	{
//	public:
//		mGlobalAxisRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, std::shared_ptr<mFontRender> fontRender);
//
//		~mGlobalAxisRender();
//
//		void updateUniform(std::shared_ptr<mViewBase> modelView, std::shared_ptr<mViewBase> commonView) override;
//
//	protected:
//
//		static void InitialAxisData();
//
//	protected:
//
//		//VBO��������
//		static QVector<float> _VBOGlobalAxisData_Type;  //���� 0����X�� 1ΪY�� 2ΪZ��
//		static QVector<QVector3D>   _VBOGlobalAxisData_Vertex;
//		//EBO��������
//		static QVector<GLuint> _EBOGlobalAxisData;
//
//		std::shared_ptr<mxr::Drawable> _drawable;
//		std::shared_ptr<mxr::StateSet> _state;
//		std::shared_ptr<mFontRender> _fontRender;
//	};
//}
//
