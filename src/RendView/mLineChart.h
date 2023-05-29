#pragma once
//解决中文乱码
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

		//最小最大值
		QPointF maxValue{ 0.0,0.0 };
		QPointF minValue{ 0.0,0.0 };
	};
	class mLineChart:public QWidget
	{
		Q_OBJECT
	public:
		/*
		 * 构造函数
		 */
		mLineChart(QwtPlot *w);

		/*
		 * 析构函数
		 */
		~mLineChart();

		/*
		 * 设置折线图数据
		 */
		void setPlotData(QString text, const QVector<double> &xdata, const QVector<double> &ydata,int id,bool isShowLegendSymbol = true, bool isShowLegendLine = true);

		/*
		 * 添加折线图数据
		 */
		void appendPlotData(QString text, QPointF point);

		/*
		 * 清空全部折线
		 */
		void clearAllPlotData();

		/*
		* 设置外图例全部被选中
		*/
		void setOutLegendChecked();


	private:
		//Qwt
		QwtPlot *_qwtPlot;

		//所有的折线图曲线(数据为一次性添加)
		QHash<QString, QwtPlotCurve*> _textCurve;

		//所有的折线图曲线（数据为叠加）
		QHash<QString, QwtPlotDirectPainter*> _textDirectPainter;

		//最小最大值
		QPointF maxValue;
		QPointF minValue;

		//是否显示外图例
		bool _isShowOutLegend;
	};
}

