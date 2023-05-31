#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")



#include <QVector>
#include <QString>
#include <QVector2D>
#include <QHash>

//#include "mBasicStruct.h"
namespace MBaseRend
{
	class mFontRender;
}
using namespace MBaseRend;
namespace MPostRend
{
	struct FontText
	{
		QVector2D pos;
		QString value;
		bool visual;
		FontText()
		{
			pos = QVector2D(0, 0);
			value = "";
			visual = true;
		}
	};
	class RENDVIEW_EXPORT mPostFrameText
	{
	public:

		/*
		 * ���캯��
		 */
		mPostFrameText(std::shared_ptr<mFontRender> fontRender);

		/*
		 * ��������
		 */
		~mPostFrameText();

		/*
		 * ���´��ڴ�С
		 */
		void resizeWindow(int w, int h);

		/*
		 * ������������
		 */
		void updateTextVertex();

		/*
		 * ��ȡ���Ͻǵ�һ������
		 */
		QHash<int, FontText> getTopRightText();

		/*
		 * �������Ͻǵ�һ������
		 */
		void setTopRightText(int row, const QString &text);

		/*
		 * �������Ͻǵ�һ����������
		 */
		void setTopRightTextVisual(int row,bool isshow);

	private:

		//���ڴ�С
		int Win_WIDTH, Win_HEIGHT;

		//���Ͻ�������Ӧ������
		QHash<int, FontText> _topRightText;

		//������Ⱦ
		std::shared_ptr<mFontRender> _fontRender;

	};
}
