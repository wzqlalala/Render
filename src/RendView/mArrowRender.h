#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

#include <QVector2D>
#include <QVector>
#include <QVector3D>
#include <QHash>
#include <qopengl.h>

#include "mBaseRender.h"

#include "Drawable.h"

#include <memory>

namespace mxr
{
	class Drawable;
	class StateSet;
	class Texture;
	class Group;
	class Uniform;
	class Shader;
}
namespace MBaseRend 
{
	/*
	* ��ͷ
	*/
	extern QVector<QVector3D> _Arrow_Vertices;

	void SetVertices();

	class mBaseArrow
	{
	public:
		mBaseArrow(std::shared_ptr<mxr::Group> parent);

		~mBaseArrow();

		void setStateSet(std::shared_ptr<mxr::StateSet> stateSet) { _drawable->setStateSet(stateSet); };

		void setIsShow(bool isShow);

		//���ֻ������ݣ�λ�ú���������
		void AppendArrowFloat(int size, int num, float f, int location);//sizeΪ��ͷ�ĸ�����numΪ������ͷ�Ķ������
		void AppendArrowV_Vector3(int size, int num, QVector<QVector3D> vec3, int location);
		void AppendArrowV_Vector2(int size, int num, QVector<QVector2D> vec2, int location);
		void AppendArrowV_Float(int size, int num, QVector<float> vec2, int location);
		void AppendArrowV_Int(int size, int num, QVector<int> type, int location);
		void AppendArrowVector3(int size, int num, QVector3D color, int location);
		void AppendArrowInt(int size, int num, int type, int location);
		void AppendArrowIntIndex(int size = 3);
		void AppendArrowVertex(int size, int location);

	protected:

		std::shared_ptr<mxr::Group> _parent;

		std::shared_ptr<mxr::Drawable> _drawable;
	};

	class mBaseRend;
	class RENDVIEW_EXPORT mArrowRender : public mBaseRender
	{
	public:

		mArrowRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mBaseRend *baseRend = nullptr);

		~mArrowRender();

		void clearAllRender();

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView) override;

		void appendCommonArrow(QString key, QVector<QVector3D> pos, QVector<QVector3D> dir, QVector3D color = QVector3D(1,1,1), float size = 1.0);
		void setCommonArrowIsShow(QString key, bool isShow);

		void appendGloabalAxisArrow();//���ȫ������ϵ��ͷ��ʾ

	protected:

		std::shared_ptr<mxr::StateSet> _commonArrowState;//��֧����ת��ƽ�ơ����ŵȵ��¼�
		std::shared_ptr<mxr::StateSet> _fixedArrowFontState;//��֧��ƽ�ơ����ŵȵ��¼���֧����ת���¼�

		QHash<QString, std::shared_ptr<mBaseArrow>> _commonArrows;

		std::shared_ptr<mBaseArrow> _globalAxisArrow;
	};
}

