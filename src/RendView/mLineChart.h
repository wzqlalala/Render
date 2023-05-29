#pragma once
//�����������
#pragma execution_character_set("utf-8")


#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_directpainter.h>

namespace MPlotView
{
	class mCurveData : public QwtArraySeriesData<QPointF>
	{
	public:
		mCurveData()
		{
		}

		virtual QRectF boundingRect() const
		{
			if (d_boundingRect.width() < 0.0)
				d_boundingRect = qwtBoundingRect(*this);

			return d_boundingRect;
		}

		inline void append(const QPointF &point)
		{
			d_samples += point;
			if (d_samples.empty())
			{
				maxValue = point;
				minValue = point;
			}
			else
			{
				maxValue.setX(std::max(point.x(), maxValue.x()));
				maxValue.setY(std::max(point.y(), maxValue.y()));
				minValue.setX(std::min(point.x(), minValue.x()));
				minValue.setY(std::min(point.y(), minValue.y()));
			}
		}

		void clear()
		{
			d_samples.clear();
			d_samples.squeeze();
			d_boundingRect = QRectF(0.0, 0.0, -1.0, -1.0);
		}

		//��С���ֵ
		QPointF maxValue{ 0.0,0.0 };
		QPointF minValue{ 0.0,0.0 };
	};
	class mLineChart:public QWidget
	{
		Q_OBJECT
	public:
		/*
		 * ���캯��
		 */
		mLineChart(QwtPlot *w);

		/*
		 * ��������
		 */
		~mLineChart();

		/*
		 * ��������ͼ����
		 */
		void setPlotData(QString text, const QVector<double> &xdata, const QVector<double> &ydata,int id,bool isShowLegendSymbol = true, bool isShowLegendLine = true);

		/*
		 * �������ͼ����
		 */
		void appendPlotData(QString text, QPointF point);

		/*
		 * ���ȫ������
		 */
		void clearAllPlotData();

		/*
		* ������ͼ��ȫ����ѡ��
		*/
		void setOutLegendChecked();


	private:
		//Qwt
		QwtPlot *_qwtPlot;

		//���е�����ͼ����(����Ϊһ�������)
		QHash<QString, QwtPlotCurve*> _textCurve;

		//���е�����ͼ���ߣ�����Ϊ���ӣ�
		QHash<QString, QwtPlotDirectPainter*> _textDirectPainter;

		//��С���ֵ
		QPointF maxValue;
		QPointF minValue;

		//�Ƿ���ʾ��ͼ��
		bool _isShowOutLegend;
	};
}

