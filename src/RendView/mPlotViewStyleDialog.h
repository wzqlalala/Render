#pragma once

#include <QDialog>

namespace Ui
{
	class mPlotViewStyleDialog;
}

class QwtPlot;
namespace MPlotView
{
	class mPlotView;
	class mPlotViewStyleDialog : public QDialog
	{
		Q_OBJECT

	public:
		//构造函数
		mPlotViewStyleDialog(mPlotView *parent, QwtPlot *plot);

		//析构函数
		~mPlotViewStyleDialog();

	private slots:
		/***********************************标题********************************************/
		void slot_titleChanged();

		/***********************************坐标轴********************************************/
		//单击样式弹窗中的坐标类型
		void slot_clickedtype(int index);

		//改变样式弹窗中的对数值
		void slot_logValueChanged(double value);

		//设置X轴范围
		void slot_setXRange();

		//刻度线生成方式
		void slot_scaleLinesMethod(int index);

		//轴标题
		void slot_axisTitle();

		//点击ok按钮
		void slot_okButton();

		/***********************************图例********************************************/
		//更新所有曲线的名字
		void slot_updateCurveNames();

		//修改曲线的名字
		void slot_changeCurveName();


	protected:
		//显示事件
		virtual void showEvent(QShowEvent *event);

	private:
		//Ui
		Ui::mPlotViewStyleDialog *_ui;

		//Qwt绘图
		QwtPlot *_qwtPlot;
	};
}