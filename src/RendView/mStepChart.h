#pragma once
//�����������
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
		 * ���캯��
		 */
		mStepChart(QwtPlot *w);

		/*
		 * ��������
		 */
		~mStepChart();

		/*
		 * ���ý�Ծ����ͼ����
		 */
		void setPlotData(const QVector<double> &xdata, const QVector<double> &ydata,int colorid);

		/*
		 * ���ȫ����Ծ����ͼ����
		 */
		void clearAllPlotData();


	private:
		//Qwt
		QwtPlot *_qwtPlot;

		//���е�����ͼ����
		QList<QwtPlotCurve*> _stepCurve;
	};
}

