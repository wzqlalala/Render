#include "mSmoothChart.h"
#include "mPlotBasic.h"
//#include "mBezierGenerate.h"

#include "qwt_plot_canvas.h"
#include "qwt_symbol.h"
#include "qwt_legend.h"
#include "qwt_legend_label.h"
#include "qwt_plot_curve.h"
#include "qwt.h"

namespace MPlotView
{
	mSmoothChart::mSmoothChart(QwtPlot *w)
	{
		_qwtPlot = w;
	}

	mSmoothChart::~mSmoothChart()
	{
		QHashIterator<QString, QwtPlotCurve*> iter(_textCurve);
		while (iter.hasNext())
		{
			iter.next();
			QwtPlotCurve *curve = iter.value();
			delete curve;
			curve = nullptr;
		}
		_textCurve.clear();
	}

	void mSmoothChart::setPlotData(QString text, const QVector<double> &xdata, const QVector<double> &ydata, int colorid)
	{
		if (_qwtPlot == nullptr)
		{
			return;
		}

		QwtPlotCurve *curve;
		if (_textCurve.contains(text))
		{
			curve = _textCurve[text];
		}
		else
		{
			curve = new QwtPlotCurve(text);
			curve->setCurveAttribute(QwtPlotCurve::Fitted);
			curve->setLegendIconSize(QSize(30, 3));//设置图例的宽度和高度
			curve->setPen(PlotColor.at(colorid%PlotColor.size()), 2, Qt::PenStyle(Qt::SolidLine));//设置曲线颜色 粗细
			curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);//线条光滑化
			curve->attach(_qwtPlot);
			//int num = xdata.size();
			//QVector<double> x, y;
			//if (xdata.size()<=3)
			//{
			//	x = xdata;
			//	y = ydata;
			//}
			//else
			//{
			//	mBezierGenerate* curve = new mBezierGenerate();
			//	curve->set_steps(100); // generate 100 interpolate points between the last 4 way points
			//	for (int i = 0; i < xdata.size(); ++i)
			//	{
			//		curve->add_way_point(QVector3D(xdata.at(i), ydata.at(i), 0));
			//	}
			//	for (int i = 0; i < curve->node_count(); ++i)
			//	{
			//		x.append(curve->node(i).x());
			//		y.append(curve->node(i).y());
			//	}
			//	delete curve;
			//}
			////绘图
			//_smoothCurve->setSamples(x, y);
		}
		_textCurve[text] = curve;
		curve->setSamples(xdata, ydata);
		_qwtPlot->setAxisAutoScale(QwtPlot::yLeft);
		_qwtPlot->setAxisAutoScale(QwtPlot::xBottom);
	}

	void mSmoothChart::clearAllPlotData()
	{
		QHashIterator<QString, QwtPlotCurve*> iter(_textCurve);
		while (iter.hasNext())
		{
			iter.next();
			QwtPlotCurve *curve = iter.value();
			delete curve;
			curve = nullptr;
		}
		_textCurve.clear();

		_qwtPlot->replot();
	}
}