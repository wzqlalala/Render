#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

namespace MPlotView
{
	class mSmoothChart :public QWidget
	{
		Q_OBJECT
	public:
		/*
		 * 构造函数
		 */
		mSmoothChart(QwtPlot *w);

		/*
		 * 析构函数
		 */
		~mSmoothChart();

		/*
		 * 设置光滑曲线图数据
		 */
		void setPlotData(QString text, const QVector<double> &xdata, const QVector<double> &ydata, int colorid);

		/*
		 * 清空全部光滑曲线图数据
		 */
		void clearAllPlotData();


	private:
		//Qwt
		QwtPlot *_qwtPlot;

		////所有的光滑曲线
		//QwtPlotCurve* _smoothCurve;

		//所有的光滑曲线(数据为一次性添加)
		QHash<QString, QwtPlotCurve*> _textCurve;
	};
}

