#pragma once
#include "rendview_global.h"
//解决中文乱码
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
		 * 构造函数
		 */
		mPostFrameText(std::shared_ptr<mFontRender> fontRender);

		/*
		 * 析构函数
		 */
		~mPostFrameText();

		/*
		 * 更新窗口大小
		 */
		void resizeWindow(int w, int h);

		/*
		 * 更新文字坐标
		 */
		void updateTextVertex();

		/*
		 * 获取右上角第一行文字
		 */
		QHash<int, FontText> getTopRightText();

		/*
		 * 设置右上角第一行文字
		 */
		void setTopRightText(int row, const QString &text);

		/*
		 * 设置右上角第一行文字显隐
		 */
		void setTopRightTextVisual(int row,bool isshow);

	private:

		//窗口大小
		int Win_WIDTH, Win_HEIGHT;

		//右上角行数对应的文字
		QHash<int, FontText> _topRightText;

		//文字渲染
		std::shared_ptr<mFontRender> _fontRender;

	};
}
