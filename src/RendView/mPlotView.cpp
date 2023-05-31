#include "mPlotView.h"
#include "mLineChart.h"
#include "mStepChart.h"
#include "mSmoothChart.h"
#include "mSmoothStepChart.h"
#include "mPlotViewStyleDialog.h"

//Qt
#include <QVector3D>

#include "qwt_plot_canvas.h"
#include "qwt_plot_layout.h"
#include "qwt_legend.h"
#include "qwt_legend_label.h"
#include <qwt_painter.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_scale_engine.h>
#include "qwt_plot.h"
#include "qwt_plot_canvas.h"
#include "qwt_legend.h"
#include "qwt_plot_legenditem.h"
#include <qwt_plot_grid.h>

namespace MPlotView
{
	mPlotView::mPlotView(const QString& name, ViewportType viewType):mBaseRend2D(name, viewType)
	{
		_qwtPlot = new QwtPlot();

		if (QwtPainter::isX11GraphicsSystem())
		{
#if QT_VERSION < 0x050000
			_qwtPlot->canvas()->setAttribute(Qt::WA_PaintOutsidePaintEvent, true);
#endif
			_qwtPlot->canvas()->setAttribute(Qt::WA_PaintOnScreen, true);
		}

		//设置画布
		_canvas = new QwtPlotCanvas();
		QColor c(248, 248, 255);
		QPalette palette(c);
		_canvas->setPalette(palette);
		_canvas->setBorderRadius(10);
		_qwtPlot->setCanvas(_canvas);
		_qwtPlot->plotLayout();
		_qwtPlot->plotLayout()->setAlignCanvasToScales(true);

		//图
		_lineChart = new mLineChart(_qwtPlot);
		_smoothChart = new mSmoothChart(_qwtPlot);
		_stepChart = new mStepChart(_qwtPlot);
		_smoothStepChart = new mSmoothStepChart(_qwtPlot);

		//布局
		_layout = new QHBoxLayout(this);
		_layout->setMargin(0);
		_layout->addWidget(_qwtPlot);

		//图例
		_legendItem = new QwtPlotLegendItem();
		_legendItem->attach(_qwtPlot);
		_legendItem->setMaxColumns(1);
		_legendItem->setAlignment(Qt::AlignRight | Qt::AlignTop);
		_legendItem->setBackgroundMode(QwtPlotLegendItem::BackgroundMode(QwtPlotLegendItem::LegendBackground));
		_legendItem->setBorderRadius(8);
		_legendItem->setMargin(4);
		_legendItem->setSpacing(2);
		_legendItem->setItemMargin(0);
		_legendItem->setRenderHint(QwtPlotItem::RenderAntialiased);
		QFont font = _legendItem->font();
		font.setPointSize(14);
		_legendItem->setFont(font);

		//_legend = new QwtLegend;
		//_legend->setDefaultItemMode(QwtLegendData::Checkable);//图例可被点击
		//_qwtPlot->insertLegend(_legend, QwtPlot::RightLegend);
		//connect(_legend, SIGNAL(checked(const QVariant &, bool, int)), SLOT(showItem(const QVariant &, bool, int)));//点击图例操作

		//样式弹窗
		_styleDialog = new mPlotViewStyleDialog(this,_qwtPlot);

		//栅格
		_grid = new QwtPlotGrid;

		//缩放
		QwtPlotMagnifier *plot_magnifier = new QwtPlotMagnifier(_canvas);
		QwtPlotZoomer* zoomer = new QwtPlotZoomer(_canvas);
		//zoomer->setRubberBandPen(QColor(Qt::black));
		//zoomer->setTrackerPen(QColor(Qt::black));
		//zoomer->setResizeMode(QwtPicker::KeepSize);
		//设置鼠标键为功能键
		zoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
		zoomer->setMousePattern(QwtEventPattern::MouseSelect6, Qt::RightButton, Qt::ControlModifier);
		zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ShiftModifier);
		//zoomer = new QwtPlotZoomer(_canvas);
		//zoomer->setKeyPattern(QwtEventPattern::KeyRedo, Qt::Key_I, Qt::ShiftModifier);
		//zoomer->setKeyPattern(QwtEventPattern::KeyUndo, Qt::Key_O, Qt::ShiftModifier);
		//zoomer->setKeyPattern(QwtEventPattern::KeyHome, Qt::Key_Home);

		QwtPlotPanner *panner = new QwtPlotPanner(_canvas);
		panner->setMouseButton(Qt::MidButton);
	}

	mPlotView::~mPlotView()
	{
		if (_layout != nullptr)
		{
			delete _layout;
			_layout = nullptr;
		}
		if (_legendItem != nullptr)
		{
			delete _legendItem;
			_legendItem = nullptr;
		}
		if (_lineChart!=nullptr)
		{
			delete _lineChart;
			_lineChart = nullptr;
		}
		if (_canvas != nullptr)
		{
			delete _canvas;
			_canvas = nullptr;
		}
		if (_qwtPlot!=nullptr)
		{
			delete _qwtPlot;
			_qwtPlot = nullptr;
		}
	}

	void mPlotView::setPlotType(PlotType plotType)
	{
		_plotType = plotType;
	}

	void mPlotView::setPlotTitle(QString title)
	{
		_qwtPlot->setTitle(title);
	}

	void mPlotView::setPlotBackGround(QVector3D color)
	{
		if (_canvas == nullptr)
		{
			return;
		}
		QColor c(color.x(), color.y(), color.z());
		QPalette palette(c);
		_canvas->setPalette(palette);
	}

	void mPlotView::setXText(QString text)
	{
		_qwtPlot->setAxisTitle(QwtPlot::xBottom, text);
	}

	void mPlotView::setYText(QString text)
	{
		_qwtPlot->setAxisTitle(QwtPlot::yLeft, text);
	}

	void mPlotView::clearPlotTexts()
	{
		_plotTexts.clear();
	}

	void mPlotView::setPlotData(PlotType plotType, QString text, const QVector<double> &xdata, const QVector<double> &ydata, int colorid, bool isShowLegendSymbol, bool isShowLegendLine)
	{
		_plotTexts.append(text);
		if (_plotType == plotType)
		{
			if (plotType == LineChart)
			{
				_lineChart->setPlotData(text, xdata, ydata, colorid, isShowLegendSymbol, isShowLegendLine);
			}
			else if(_plotType == StepChart)
			{
				_stepChart->setPlotData(xdata, ydata, colorid);
			}
			else if (_plotType == SmoothChart)
			{
				_smoothChart->setPlotData(text, xdata, ydata, colorid);
			}
			else if (_plotType == SmoothStepChart)
			{
				_smoothStepChart->setPlotData(text, xdata, ydata, colorid);
			}
		}

		_qwtPlot->replot();
	}

	void mPlotView::appendPlotData(MxFunctions::PlotType plotType, QString text, QPointF point)
	{
		if (_plotType == plotType)
		{
			if (plotType == LineChart)
			{
				_lineChart->appendPlotData(text, point);
			}
		}

		_qwtPlot->replot();
	}

	void mPlotView::clearAllPlot()
	{
		_lineChart->clearAllPlotData();
		_smoothChart->clearAllPlotData();
		_stepChart->clearAllPlotData();
		_smoothStepChart->clearAllPlotData();
		_plotTexts.clear();

		_qwtPlot->replot();
	}

	void mPlotView::setLegendVisual(bool isshow)
	{
		_legendItem->setVisible(isshow);
	}

	void mPlotView::setOutLegendVisual(bool isshow)
	{
		if (isshow)
		{
			_legend = new QwtLegend;
			_legend->setDefaultItemMode(QwtLegendData::Checkable);//图例可被点击
			_qwtPlot->insertLegend(_legend, QwtPlot::RightLegend);
			connect(_legend, SIGNAL(checked(const QVariant &, bool, int)), this, SLOT(showItem(const QVariant &, bool, int)));//点击图例操作

			if (_plotType == LineChart)
			{
				_lineChart->setOutLegendChecked();
			}
		}
		else
		{
			_legend = dynamic_cast<QwtLegend*>(_qwtPlot->legend());
			if (_legend != nullptr)
			{
				disconnect(_legend, SIGNAL(checked(const QVariant &, bool, int)), this, SLOT(showItem(const QVariant &, bool, int)));//点击图例操作
				delete _legend;
			}
		}
	}

	void mPlotView::setAxisScaleEngine(int xory, int linearorlog)
	{
		QwtPlot::Axis axis = QwtPlot::xBottom;

		if (xory == 1)
		{
			axis = QwtPlot::yLeft;
		}
		if (linearorlog == 0)
		{
			_qwtPlot->setAxisScaleEngine(axis, new QwtLinearScaleEngine);
		}
		else
		{
			_qwtPlot->setAxisMaxMajor(axis, 6);
			_qwtPlot->setAxisMaxMinor(axis, 9);
			_qwtPlot->setAxisScaleEngine(axis, new QwtLogScaleEngine(10));
		}
		_qwtPlot->replot();
	}

	void mPlotView::mouseDoubleClickEvent(QMouseEvent * event)
	{
		if (event->button() == Qt::LeftButton)
		{
			//if (_isEnableDialog)
			{
				_styleDialog->show();
			}
		}
	}

	void mPlotView::showItem(const QVariant& itemInfo, bool on, int i)
	{
		QwtPlotItem *plotItem = _qwtPlot->infoToItem(itemInfo);
		if (plotItem)
			plotItem->setVisible(on);

		_qwtPlot->replot();
	}

	void mPlotView::slot_clickedGrid(bool clicked)
	{
		if (clicked)
		{
			_grid->enableXMin(true);
			_grid->setMajorPen(Qt::black, 0, Qt::SolidLine);
			_grid->setMinorPen(Qt::black, 0, Qt::SolidLine);
			_grid->attach(_qwtPlot);
		}
		else
		{
			_grid->detach();
		}
		_qwtPlot->replot();
	}
}
