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
	//�������ݴ洢�ṹ��
	struct Character
	{
		//�����������ε�����ԭ��ʹ�С
		QVector2D  Positon;		//������ԭ�㣨���½ǣ������ִ������ϵ���������
		QVector2D  Size;       // ���δ�С(���ش�С Width Height)

		//�������û������εĶ������ݣ����ּ����
		QVector2D  Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
		GLuint     Advance;    // ԭ�����һ������ԭ��ľ���

	};

	//������
	extern std::shared_ptr<mxr::Texture> textTexture;

	//�������ݿ�
	extern QHash<GLchar, Character> Characters;

	class mBaseFont
	{
	public:
		mBaseFont(std::shared_ptr<mxr::Group> parent);

		~mBaseFont();

		void setStateSet(std::shared_ptr<mxr::StateSet> stateSet) { _drawable->setStateSet(stateSet); };

		void setIsShow(bool isShow);

		//���ֻ������ݣ�λ�ú���������
		void AppendFontVertexAndTexcoord(QVector<QString> txt, int vlocation, int tlocation);
		void AppendFontFloat(QVector<QString> txt, float size, int location);
		void AppendFontV_Vector3(QVector<QString> txt, QVector<QVector3D> pos, int location);
		void AppendFontV_Vector2(QVector<QString> txt, QVector<QVector2D> pos, int location);
		void AppendFontV_Int(QVector<QString> txt, QVector<int> type, int location);
		void AppendFontVector3(QVector<QString> txt, QVector3D color, int location);
		void AppendFontInt(QVector<QString> txt, int type, int location);

	protected:

		std::shared_ptr<mxr::Group> _parent;

		std::shared_ptr<mxr::Drawable> _drawable;
	};

	class mBaseRend;
	class RENDVIEW_EXPORT mFontRender : public mBaseRender
	{
	public:

		mFontRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mBaseRend *baseRend = nullptr);

		~mFontRender();

		void clearAllRender();

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView) override;

		void appendFixedFont(QString key, QVector<QVector2D> pos, QVector<QString> txt, QVector3D color = QVector3D(1,1,1), float size = 1.0);//��ӹ̶�������ʾ
		void setFixedFontIsShow(QString key, bool isShow);

		void appendCommonFont(QString key, QVector<QVector3D> pos, QVector<QString> txt, QVector3D color = QVector3D(1, 1, 1), float size = 1.0);//��ӹ̶�������ʾ
		void setCommonFontIsShow(QString key, bool isShow);

		void appendGloabalAxisFont();//���ȫ������ϵ������ʾ


	protected:



	private:                                                                                                                                                                
		//�������ִ�����
		void LoadFontTexture();
	protected:

		//���ݷֱ��ʵ����������ű���
		static float _ratio;

		std::shared_ptr<mxr::StateSet> _fixedFontState;//��֧����ת��ƽ�ơ����ŵȵ��¼�
		std::shared_ptr<mxr::StateSet> _fixedAxisFontState;//��֧��ƽ�ơ����ŵȵ��¼���֧����ת���¼�
		std::shared_ptr<mxr::StateSet> _commonFontState;//֧��ƽ�ơ���ת���¼�

		QHash<QString, std::shared_ptr<mBaseFont>> _fixedFonts;

		QHash<QString, std::shared_ptr<mBaseFont>> _commonFonts;

		std::shared_ptr<mBaseFont> _globalAxisFont;

	};
}

