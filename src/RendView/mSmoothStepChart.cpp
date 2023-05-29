#include "mSmoothStepChart.h"
#include "mPlotBasic.h"

#include "qwt_plot_canvas.h"
#include "qwt_symbol.h"
#include "qwt_legend.h"
#include "qwt_legend_label.h"
#include "qwt_plot_curve.h"

namespace MPlotView
{
	mSmoothStepChart::mSmoothStepChart(QwtPlot *w)
	{
		_qwtPlot = w;
	}

	mSmoothStepChart::~mSmoothStepChart()
	{
		QHashIterator<QString, QPair<QwtPlotCurve*, QwtPlotCurve*>> iter(_textCurve);
		while (iter.hasNext())
		{
			iter.next();
			QwtPlotCurve *curve = iter.value().first;delete curve;
			curve = iter.value().second;delete curve;
		}
		_textCurve.clear();
	}

	void mSmoothStepChart::setPlotData(QString text, const QVector<double>& xdata, const QVector<double>& ydata, int colorid)
	{
		if (_qwtPlot == nullptr)
		{
			return;
		}

		QwtPlotCurve *curveSmooth;
		QwtPlotCurve *curveLine;
		QPair<QwtPlotCurve*, QwtPlotCurve*> pair;
		if (_textCurve.contains(text))
		{
			pair = _textCurve[text];
			curveSmooth = pair.first;
			curveLine = pair.second;
		}
		else
		{
			curveLine = new QwtPlotCurve(text);
			curveSmooth = new QwtPlotCurve(text);
			pair = QPair<QwtPlotCurve*, QwtPlotCurve*>(curveSmooth, curveLine);
			curveLine->setCurveAttribute(QwtPlotCurve::Fitted);
			curveSmooth->setCurveAttribute(QwtPlotCurve::Fitted);
			curveLine->setLegendIconSize(QSize(30, 3));//设置图例的宽度和高度
			curveSmooth->setLegendAttribute(QwtPlotCurve::LegendNoAttribute, false);
			curveLine->setPen(PlotColor.at(colorid%PlotColor.size()), 2, Qt::PenStyle(Qt::SolidLine));//设置曲线颜色 粗细
			curveSmooth->setPen(PlotColor.at(colorid%PlotColor.size()), 2, Qt::PenStyle(Qt::SolidLine));//设置曲线颜色 粗细
			curveSmooth->setRenderHint(QwtPlotItem::RenderAntialiased, true);//线条光滑化
			curveLine->attach(_qwtPlot);
			curveSmooth->attach(_qwtPlot);
			_textCurve[text] = pair;
		}
		int num = xdata.size();
		int end = num - 1;
		for (int i = 0; i < num - 1; i++)
		{
			if (ydata.at(i) == ydata.at(i + 1))
			{
				end = i;
				break;
			}
		}
		//mBezierGenerate1 b;
		QVector<QPointF> res;
		//b.parsePolyline(xdata.mid(0, end + 2), ydata.mid(0, end + 2), 10, res);
		mSmoothStepGenerate a;
		a.parseSmoothGenerate(xdata.mid(0, end + 1), ydata.mid(0, end + 1), 20, res);
		curveSmooth->setSamples(res);
		curveLine->setSamples(xdata.mid(end, num - end), ydata.mid(end, num - end));
		_qwtPlot->setAxisAutoScale(QwtPlot::yLeft);
		_qwtPlot->setAxisAutoScale(QwtPlot::xBottom);
	}
	void mSmoothStepChart::clearAllPlotData()
	{
		QHashIterator<QString, QPair<QwtPlotCurve*, QwtPlotCurve*>> iter(_textCurve);
		while (iter.hasNext())
		{
			iter.next();
			QwtPlotCurve *curve = iter.value().first; delete curve;
			curve = iter.value().second; delete curve;
		}
		_textCurve.clear();
		_qwtPlot->replot();
	}
}