#include "mLineChart.h"
#include "mPlotBasic.h"

#include "qwt_plot_canvas.h"
#include "qwt_symbol.h"
#include "qwt_legend.h"
#include "qwt_legend_label.h"
#include "qwt_plot_curve.h"
#include "qwt_legend.h"


namespace MPlotView
{
	mLineChart::mLineChart(QwtPlot *w)
	{
		_qwtPlot = w;
		maxValue = QPointF(INT_MIN, INT_MIN);
		minValue = QPointF(INT_MAX, INT_MAX);
		_isShowOutLegend = false;
	}

	mLineChart::~mLineChart()
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

	void mLineChart::setPlotData(QString text, const QVector<double> &xdata, const QVector<double> &ydata, int id, bool isShowLegendSymbol, bool isShowLegendLine)
	{
		if (_qwtPlot==nullptr)
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
			QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Style(id % 14 + 1), QBrush(Qt::GlobalColor(id % 17 + 2)), QPen(Qt::GlobalColor(id % 17 + 2), 3), QSize(3, 3));//设置样本点的颜色、大小
			curve->setSymbol(symbol);//添加样本点形状
			curve->setLegendAttribute(QwtPlotCurve::LegendShowSymbol, isShowLegendSymbol);//显示图例的标志，这里显示图例的颜色。true代表同时开启
			curve->setLegendAttribute(QwtPlotCurve::LegendShowLine, isShowLegendLine);//显示图例的标志，这里显示线的颜色。true代表同时开启
			if (!isShowLegendSymbol)
			{
				symbol->setStyle(QwtSymbol::NoSymbol);
			}
			if (isShowLegendSymbol && !isShowLegendLine)
			{
				curve->setStyle(QwtPlotCurve::CurveStyle::NoCurve);
			}

			curve->setLegendIconSize(QSize(30, 3));//设置图例的宽度和高度
			curve->setPen(PlotColor.at(id % PlotColor.size()), 3, Qt::PenStyle(id % 5 + 1));//设置曲线颜色 粗细
			curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);//线条光滑化
			curve->attach(_qwtPlot);
			if (_isShowOutLegend)
			{
				QwtLegend *lgd = dynamic_cast<QwtLegend *>(_qwtPlot->legend());

				QList<QWidget *> legendWidgets =
					lgd->legendWidgets(_qwtPlot->itemToInfo(curve));

				if (legendWidgets.size() == 1)
				{
					QwtLegendLabel *legendLabel =
						dynamic_cast<QwtLegendLabel *>(legendWidgets[0]);

					if (legendLabel)
						legendLabel->setChecked(true);
				}
			}
		}
		//绘图		
		_textCurve[text] = curve;
		curve->setSamples(xdata, ydata);

		//设置坐标系
		_qwtPlot->setAxisAutoScale(QwtPlot::xBottom);
		_qwtPlot->setAxisAutoScale(QwtPlot::yLeft);
	}

	void mLineChart::appendPlotData(QString text, QPointF point)
	{
		if (_qwtPlot == nullptr)
		{
			return;
		}
		QwtPlotDirectPainter *painter;
		QwtPlotCurve *curve;
		if (_textDirectPainter.contains(text))
		{
			painter = _textDirectPainter[text];
			curve = _textCurve[text];
		}
		else
		{
			painter = new QwtPlotDirectPainter(_qwtPlot);
			curve = new QwtPlotCurve(text);
			curve->setData(new mCurveData());
			curve->setStyle(QwtPlotCurve::Lines);
			//QwtSymbol *symbol = new QwtSymbol(QwtSymbol::Ellipse, QBrush(Qt::darkMagenta), QPen(Qt::black, 2), QSize(6, 6));//设置样本点的颜色、大小
			//curve->setSymbol(symbol);//添加样本点形状
			curve->attach(_qwtPlot);
			_textDirectPainter[text] = painter;
			_textCurve[text] = curve;
		}

		mCurveData *curveData = static_cast<mCurveData*>(curve->data());
		curveData->append(point);

		//const bool doClip = !_qwtPlot->canvas()->testAttribute(Qt::WA_PaintOnScreen);
		//if (doClip)
		//{
		//	/*
		//	   Depending on the platform setting a clip might be an important
		//	   performance issue. F.e. for Qt Embedded this reduces the
		//	   part of the backing store that has to be copied out - maybe
		//	   to an unaccelerated frame buffer device.
		//	 */
		//	const QwtScaleMap xMap = _qwtPlot->canvasMap(curve->xAxis());
		//	const QwtScaleMap yMap = _qwtPlot->canvasMap(curve->yAxis());

		//	QRegion clipRegion;

		//	const QSize symbolSize = curve->symbol()->size();
		//	QRect r(0, 0, symbolSize.width() + 2, symbolSize.height() + 2);

		//	const QPointF center =
		//		QwtScaleMap::transform(xMap, yMap, point);
		//	r.moveCenter(center.toPoint());
		//	clipRegion += r;

		//	painter->setClipRegion(clipRegion);
		//}

		maxValue.setX(std::max(point.x(), maxValue.x()));
		maxValue.setY(std::max(point.y(), maxValue.y()));
		minValue.setX(std::min(point.x(), minValue.x()));
		minValue.setY(std::min(point.y(), minValue.y()));
		
		painter->drawSeries(curve, curveData->size() - 1, curveData->size() - 1);
		_qwtPlot->setAxisScale(QwtPlot::Axis::xBottom, minValue.x(), maxValue.x());
		_qwtPlot->setAxisScale(QwtPlot::Axis::yLeft, minValue.y(), maxValue.y());
	}

	void mLineChart::clearAllPlotData()
	{
		QHashIterator<QString, QwtPlotCurve*> iter(_textCurve);
		while (iter.hasNext())
		{
			iter.next();
			QwtPlotCurve *curve = iter.value();
			delete curve;
			curve = nullptr;
		}
		QHashIterator<QString, QwtPlotDirectPainter*> it(_textDirectPainter);
		while (it.hasNext())
		{
			it.next();
			QwtPlotDirectPainter *painter = it.value();
			delete painter;
			painter = nullptr;
		}
		_textCurve.clear();

		_qwtPlot->replot();
	}
	void mLineChart::setOutLegendChecked()
	{
		QHashIterator<QString, QwtPlotCurve*> iter(_textCurve);
		while (iter.hasNext())
		{
			iter.next();
			QwtPlotCurve *curve = iter.value();

			QwtLegend *lgd = dynamic_cast<QwtLegend *>(_qwtPlot->legend());

			QList<QWidget *> legendWidgets =
				lgd->legendWidgets(_qwtPlot->itemToInfo(curve));

			if (legendWidgets.size() == 1)
			{
				QwtLegendLabel *legendLabel =
					dynamic_cast<QwtLegendLabel *>(legendWidgets[0]);

				if (legendLabel)
					legendLabel->setChecked(true);
			}
		}
		_isShowOutLegend = true;
	}
}
