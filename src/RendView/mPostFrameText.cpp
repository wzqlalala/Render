#include "mPostFrameText.h"
#include "mFontRender.h"

namespace MPostRend
{
	mPostFrameText::mPostFrameText(std::shared_ptr<mFontRender> fontRender)
	{
		_fontRender = fontRender;
		//右上角第一行文字默认值
		//_topRighttext1.pos = QVector2D(0.78, 0.95);
		//_topRighttext1.value = "";
		//_topRighttext1.visual = true;

		////右上角第二行文字默认值
		//_topRighttext2.pos = QVector2D(0.78, 0.90);
		//_topRighttext2.value = "";
		//_topRighttext2.visual = true;
	}

	mPostFrameText::~mPostFrameText()
	{

	}

	void mPostFrameText::resizeWindow(int w, int h)
	{
		if (Win_WIDTH == w && Win_HEIGHT == h)
		{
			return;
		}
		Win_WIDTH = w;
		Win_HEIGHT = h;
		updateTextVertex();
	}

	void mPostFrameText::updateTextVertex()
	{
		QString text = _topRightText[1].value;
		int size = text.size();
		float cratio = 1618.0 / Win_WIDTH;

		float ratio = 0.05f;//一行的高度比例
		if (Win_HEIGHT < 600 && Win_HEIGHT > 350)
		{
			ratio = -Win_HEIGHT / 5000.0f + 0.17;
		}
		else if (Win_HEIGHT <= 350)
		{
			ratio = 0.1*350.0 / Win_HEIGHT;
		}

		QVector<QVector2D> pos;
		QVector<QString> val;
		QHashIterator<int, FontText> iter(_topRightText);
		while (iter.hasNext())
		{
			iter.next();
			_topRightText[iter.key()].pos.setX(0.98 - size * 0.01 * cratio);
			_topRightText[iter.key()].pos.setY(1 - iter.key() * ratio);
			pos.append(_topRightText[iter.key()].pos);
			val.append(iter.value().value);
		}

		_fontRender->appendFixedFont("postRightText", pos, val, QVector3D(0, 0, 0));

	}

	QHash<int, FontText> mPostFrameText::getTopRightText()
	{
		return _topRightText;
	}

	void mPostFrameText::setTopRightTextVisual(int row, bool isshow)
	{
		_topRightText[row].visual = isshow;
		updateTextVertex();
	}

	void mPostFrameText::setTopRightText(int row,const QString& text)
	{
		float ratio = 0.05f;//一行的高度比例
		if (Win_HEIGHT < 600 && Win_HEIGHT > 350)
		{
			ratio = -Win_HEIGHT / 5000.0f + 0.17;
		}
		else if (Win_HEIGHT <= 350)
		{
			ratio = 0.1*350.0 / Win_HEIGHT;
		}

		_topRightText[row].value = text;
		_topRightText[row].pos = QVector2D(0.78, 1 - row * ratio);

		updateTextVertex();
	}
}
