#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

//MBasicFunciton
#include "mBasicEnum.h"

#include "mBaseRend2D.h"

//Qt
#include <QObject>
#include <QMouseEvent>
#include <QHBoxLayout>
#include "qwt_plot.h"
#include "qwt_plot_canvas.h"
#include "qwt_legend.h"
#include "qwt_plot_legenditem.h"
#include <qwt_plot_grid.h>

using namespace MxFunctions;
namespace MPlotView
{
	class mSmoothChart;
	class mLineChart;
	class mStepChart;
	class mSmoothStepChart;
	class mPlotViewStyleDialog;
	class RENDVIEW_EXPORT mPlotView :public MBaseRend::mBaseRend2D
	{
		Q_OBJECT

	public:

		/*
		 * 构造函数
		 */
		mPlotView(const QString& name = "", MxFunctions::ViewportType = MxFunctions::Viewport2D);

		/*
		 * 析构函数
		 */
		~mPlotView();

		/*
		 * 设置绘图类型
		 */
		void setPlotType(MxFunctions::PlotType plotType);

		/*
		 * 设置标题
		 */
		void setPlotTitle(QString title);

		/*
		 * 设置背景颜色
		 */
		void setPlotBackGround(QVector3D color);

		/*
		 * 设置横轴文字
		 */
		void setXText(QString text);

		/*
		 * 设置纵轴文字
		 */
		void setYText(QString text);

		/*
		* 清空曲线名称
		*/
		void clearPlotTexts();

		/*
		 * 设置数据
		 */
		void setPlotData(MxFunctions::PlotType plotType, QString text, const QVector<double> &xdata, const QVector<double> &ydata,int colorid = 0, bool isShowLegendSymbol = true, bool isShowLegendLine = true);

		/*
		 * 添加数据
		 */
		void appendPlotData(MxFunctions::PlotType plotType, QString text, QPointF point);

		/*
		 * 清空全部数据
		 */
		void clearAllPlot();

		/*
		* 设置内图例显隐
		*/
		void setLegendVisual(bool isshow);

		/*
		* 设置外图例显隐
		*/
		void setOutLegendVisual(bool isshow);

		/*
		* 设置坐标为常数或者对数
		*/
		void setAxisScaleEngine(int xory, int linearorlog);

	protected:

		void mouseDoubleClickEvent(QMouseEvent *event) override;
	private slots:

		void showItem(const QVariant & itemInfo, bool on, int i);

	public slots:
		//点击样式弹窗中的栅格显隐
		void slot_clickedGrid(bool clicked);
	private:
		//绘图
		QwtPlot *_qwtPlot;

		//背景画布
		QwtPlotCanvas *_canvas;

		//画布外的图例
		QwtLegend *_legend;

		//画布内的图例
		QwtPlotLegendItem *_legendItem;

		//布局
		QHBoxLayout *_layout;

		//栅格
		QwtPlotGrid *_grid;

		//绘图类型
		MxFunctions::PlotType _plotType;

		//折线图
		mLineChart *_lineChart;

		//光滑曲线图
		mSmoothChart *_smoothChart;

		//阶跃曲线图
		mStepChart *_stepChart;

		//分布光滑曲线图
		mSmoothStepChart *_smoothStepChart;

		//数据
		QList<QString> _plotTexts;

		//鼠标双击的样式弹窗
		mPlotViewStyleDialog *_styleDialog;
 	};

}
