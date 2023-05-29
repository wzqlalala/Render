#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

namespace MPlotView
{
	class mSmoothChart :public QWidget
	{
		Q_OBJECT
	public:
		/*
		 * ���캯��
		 */
		mSmoothChart(QwtPlot *w);

		/*
		 * ��������
		 */
		~mSmoothChart();

		/*
		 * ���ù⻬����ͼ����
		 */
		void setPlotData(QString text, const QVector<double> &xdata, const QVector<double> &ydata, int colorid);

		/*
		 * ���ȫ���⻬����ͼ����
		 */
		void clearAllPlotData();


	private:
		//Qwt
		QwtPlot *_qwtPlot;

		////���еĹ⻬����
		//QwtPlotCurve* _smoothCurve;

		//���еĹ⻬����(����Ϊһ�������)
		QHash<QString, QwtPlotCurve*> _textCurve;
	};
}

