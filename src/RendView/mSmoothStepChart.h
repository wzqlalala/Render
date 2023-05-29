#pragma once
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

namespace MPlotView
{
	class mSmoothStepGenerate
	{
	public:
		void parseSmoothGenerate(const QVector<double>& x, const QVector<double>& y, int count, QVector<QPointF>& outPoints)
		{
			double step = (x.at(1) - x.at(0)) / double(count);
			double startX = x.at(0);
			double endX = x.at(1);
			double startY = y.at(0);
			double endY = y.at(1);
			for (int i = 0; i < count; i++)
			{
				double t = startX + step * i;
				double ksi = (t - startX) / (endX - startX);
				double currentValue = startY + (endY - startY) * ksi * ksi * ksi * (10 - 15 * ksi + 6 * ksi * ksi);
				outPoints.append(QPointF(t, currentValue));
			}
			outPoints.append(QPointF(endX, endY));
		}

	};
	class mSmoothStepChart :public QWidget
	{
		Q_OBJECT

	public:
		/*
		 * 构造函数
		 */
		mSmoothStepChart(QwtPlot *w);
		
		/*
		 * 析构函数
		 */
		~mSmoothStepChart();

		/*
		 * 设置分布光滑曲线图数据
		 */
		void setPlotData(QString text, const QVector<double> &xdata, const QVector<double> &ydata, int colorid);

		/*
		 * 清空全部分布光滑曲线图数据
		 */
		void clearAllPlotData();

	private:
		//Qwt
		QwtPlot *_qwtPlot;

		//所有的分布光滑曲线(数据为一次性添加)，前者为光滑曲线，后者为直线
		QHash<QString, QPair<QwtPlotCurve*, QwtPlotCurve*>> _textCurve;


	};
}