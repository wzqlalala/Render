#pragma once
#include "rendview_global.h"
//�����������
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
		 * ���캯��
		 */
		mPlotView(const QString& name = "", MxFunctions::ViewportType = MxFunctions::Viewport2D);

		/*
		 * ��������
		 */
		~mPlotView();

		/*
		 * ���û�ͼ����
		 */
		void setPlotType(MxFunctions::PlotType plotType);

		/*
		 * ���ñ���
		 */
		void setPlotTitle(QString title);

		/*
		 * ���ñ�����ɫ
		 */
		void setPlotBackGround(QVector3D color);

		/*
		 * ���ú�������
		 */
		void setXText(QString text);

		/*
		 * ������������
		 */
		void setYText(QString text);

		/*
		* �����������
		*/
		void clearPlotTexts();

		/*
		 * ��������
		 */
		void setPlotData(MxFunctions::PlotType plotType, QString text, const QVector<double> &xdata, const QVector<double> &ydata,int colorid = 0, bool isShowLegendSymbol = true, bool isShowLegendLine = true);

		/*
		 * �������
		 */
		void appendPlotData(MxFunctions::PlotType plotType, QString text, QPointF point);

		/*
		 * ���ȫ������
		 */
		void clearAllPlot();

		/*
		* ������ͼ������
		*/
		void setLegendVisual(bool isshow);

		/*
		* ������ͼ������
		*/
		void setOutLegendVisual(bool isshow);

		/*
		* ��������Ϊ�������߶���
		*/
		void setAxisScaleEngine(int xory, int linearorlog);

	protected:

		void mouseDoubleClickEvent(QMouseEvent *event) override;
	private slots:

		void showItem(const QVariant & itemInfo, bool on, int i);

	public slots:
		//�����ʽ�����е�դ������
		void slot_clickedGrid(bool clicked);
	private:
		//��ͼ
		QwtPlot *_qwtPlot;

		//��������
		QwtPlotCanvas *_canvas;

		//�������ͼ��
		QwtLegend *_legend;

		//�����ڵ�ͼ��
		QwtPlotLegendItem *_legendItem;

		//����
		QHBoxLayout *_layout;

		//դ��
		QwtPlotGrid *_grid;

		//��ͼ����
		MxFunctions::PlotType _plotType;

		//����ͼ
		mLineChart *_lineChart;

		//�⻬����ͼ
		mSmoothChart *_smoothChart;

		//��Ծ����ͼ
		mStepChart *_stepChart;

		//�ֲ��⻬����ͼ
		mSmoothStepChart *_smoothStepChart;

		//����
		QList<QString> _plotTexts;

		//���˫������ʽ����
		mPlotViewStyleDialog *_styleDialog;
 	};

}
