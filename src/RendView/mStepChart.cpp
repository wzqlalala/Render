#include "mStepChart.h"
#include "mPlotBasic.h"

#include "qwt_plot_canvas.h"
#include "qwt_symbol.h"
#include "qwt_legend.h"
#include "qwt_legend_label.h"
#include "qwt_plot_curve.h"

namespace MPlotView
{
	mStepChart::mStepChart(QwtPlot *w)
	{
		_qwtPlot = w;
	}

	mStepChart::~mStepChart()
	{
		for (QwtPlotCurve *curve:_stepCurve)
		{
			if (curve!=nullptr)
			{
				delete curve;
				curve = nullptr;
			}
		}
		_stepCurve.clear();
	}

	void mStepChart::setPlotData(const QVector<double> &xdata, const QVector<double> &ydata,int colorid)
	{
		if (_qwtPlot == nullptr)
		{
			return;
		}

		double miny, maxy;
		for (int i = 0;i<xdata.size();++i)
		{
			QwtPlotCurve *curve = new QwtPlotCurve();
			_stepCurve.append(curve);
			//绘图
			curve->setPen(PlotColor.at(colorid%PlotColor.size()), 2, Qt::PenStyle(Qt::SolidLine));//设置曲线颜色 粗细
			curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);//线条光滑化
			if (i == 0)
			{
				QVector<double> x{ 0,xdata.at(i) };
				QVector<double> y{ ydata.at(i),ydata.at(i) };
				curve->setSamples(x, y);
				curve->attach(_qwtPlot);
				miny = ydata.at(i);
				maxy = ydata.at(i);
			}
			else
			{
				QVector<double> x{ xdata.at(i-1),xdata.at(i) };
				QVector<double> y{ ydata.at(i),ydata.at(i) };
				curve->setSamples(x, y);
				curve->attach(_qwtPlot);
				miny = std::min(miny,ydata.at(i));
				maxy = std::max(maxy, ydata.at(i));
			}
		}
		double quad = maxy - miny;
		_qwtPlot->setAxisScale(QwtPlot::yLeft, miny - 0.2*quad, maxy + 0.2*quad);
		_qwtPlot->setAxisAutoScale(QwtPlot::xBottom);
		// QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::darkMagenta), QPen(Qt::black, 2), QSize(6, 6));//设置样本点的颜色、大小
		// curve->setSymbol(symbol);//添加样本点形状


	}

	void mStepChart::clearAllPlotData()
	{
		for (QwtPlotCurve *curve : _stepCurve)
		{
			if (curve != nullptr)
			{
				delete curve;
				curve = nullptr;
			}
		}
		_stepCurve.clear();


		_qwtPlot->replot();
	}

}