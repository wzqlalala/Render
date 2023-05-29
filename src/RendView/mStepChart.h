#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")


#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

namespace MPlotView
{
	class mStepChart :public QWidget
	{
		Q_OBJECT
	public:
		/*
		 * 构造函数
		 */
		mStepChart(QwtPlot *w);

		/*
		 * 析构函数
		 */
		~mStepChart();

		/*
		 * 设置阶跃曲线图数据
		 */
		void setPlotData(const QVector<double> &xdata, const QVector<double> &ydata,int colorid);

		/*
		 * 清空全部阶跃曲线图数据
		 */
		void clearAllPlotData();


	private:
		//Qwt
		QwtPlot *_qwtPlot;

		//所有的折线图曲线
		QList<QwtPlotCurve*> _stepCurve;
	};
}

