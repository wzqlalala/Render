#include "mPlotViewStyleDialog.h"
#include "ui_mPlotViewStyleDialog.h"

#include "mPlotView.h"

#include <qwt_plot.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_curve.h>

namespace MPlotView
{
	mPlotViewStyleDialog::mPlotViewStyleDialog(mPlotView *parent, QwtPlot *plot):QDialog(parent), _qwtPlot(plot)
	{
		_ui = new Ui::mPlotViewStyleDialog();
		_ui->setupUi(this);
		//标题
		connect(_ui->lineEdit_8, SIGNAL(returnPressed()), this, SLOT(slot_titleChanged()));

		//坐标轴
		connect(_ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_clickedtype(int)));
		connect(_ui->checkBox, SIGNAL(clicked(bool)), parent, SLOT(slot_clickedGrid(bool)));
		connect(_ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_logValueChanged(double)));
		connect(_ui->lineEdit_4, SIGNAL(returnPressed()), this, SLOT(slot_setXRange()));
		connect(_ui->lineEdit_5, SIGNAL(returnPressed()), this, SLOT(slot_setXRange()));
		connect(_ui->lineEdit_7, SIGNAL(returnPressed()), this, SLOT(slot_axisTitle()));
		connect(_ui->comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_scaleLinesMethod(int)));
		connect(_ui->pushButton_8, SIGNAL(clicked()), this, SLOT(slot_okButton()));
		connect(_ui->lineEdit_9, SIGNAL(returnPressed()), this, SLOT(slot_changeCurveName()));

	}

	
	mPlotViewStyleDialog::~mPlotViewStyleDialog()
	{

	}

	void mPlotViewStyleDialog::slot_titleChanged()
	{
		_qwtPlot->setTitle(_ui->lineEdit_8->text());
	}

	void mPlotViewStyleDialog::slot_clickedtype(int index)
	{
		QwtPlot::Axis axis = QwtPlot::xBottom;

		if (_ui->comboBox_2->currentIndex() == 1)
		{
			axis = QwtPlot::yLeft;
		}
		if (index == 0)
		{
			_qwtPlot->setAxisScaleEngine(axis, new QwtLinearScaleEngine);
		}
		else
		{
			_qwtPlot->setAxisMaxMajor(axis, 6);
			_qwtPlot->setAxisMaxMinor(axis, 9);
			_qwtPlot->setAxisScaleEngine(axis, new QwtLogScaleEngine(_ui->doubleSpinBox->value()));
		}
		_qwtPlot->replot();
	}

	void mPlotViewStyleDialog::slot_logValueChanged(double value)
	{
		slot_clickedtype(1);
	}

	void mPlotViewStyleDialog::slot_setXRange()
	{
		QwtPlot::Axis axis = QwtPlot::xBottom;

		if (_ui->comboBox_2->currentIndex() == 1)
		{
			axis = QwtPlot::yLeft;
		}

		double min = _ui->lineEdit_4->text().toDouble();
		double max = _ui->lineEdit_5->text().toDouble();
		_qwtPlot->setAxisScale(axis, min, max);

		_qwtPlot->replot();
	}
	void mPlotViewStyleDialog::slot_scaleLinesMethod(int index)
	{
		if (index == 0 || index == 1)
		{
			_ui->stackedWidget_2->setCurrentIndex(index);
		}
		else
		{
			_ui->stackedWidget_2->setCurrentIndex(index - 1);
		}
	}

	void mPlotViewStyleDialog::slot_axisTitle()
	{
		QString text = _ui->lineEdit_7->text();
		QwtPlot::Axis axis = QwtPlot::xBottom;

		if (_ui->comboBox_2->currentIndex() == 1)
		{
			axis = QwtPlot::yLeft;
		}
		_qwtPlot->setAxisTitle(axis, text);
		_qwtPlot->replot();
	}

	void mPlotViewStyleDialog::slot_okButton()
	{
		QwtPlot::Axis axis = QwtPlot::xBottom;

		if (_ui->comboBox_2->currentIndex() == 1)
		{
			axis = QwtPlot::yLeft;
		}

		//设置轴标题
		QString text = _ui->lineEdit_7->text();
		_qwtPlot->setAxisTitle(axis, text);

		QwtScaleEngine *scaleEngine;
		double value = _ui->doubleSpinBox->value();
		if (_ui->comboBox->currentIndex() == 0)
		{
			scaleEngine = new QwtLinearScaleEngine(value);
			_qwtPlot->setAxisScaleEngine(axis, scaleEngine);
		}
		else
		{
			scaleEngine = new QwtLogScaleEngine(value);
			_qwtPlot->setAxisMaxMajor(axis, 10);
			_qwtPlot->setAxisMaxMinor(axis, 10);
			_qwtPlot->setAxisScaleEngine(axis, scaleEngine);
		}


		bool rangeIsEmpty{ false };
		if (_ui->lineEdit_4->text().isEmpty() || _ui->lineEdit_5->text().isEmpty())
		{
			_qwtPlot->setAxisAutoScale(QwtPlot::xBottom, true);
			_qwtPlot->setAxisAutoScale(QwtPlot::yLeft, true);
			rangeIsEmpty = true;
		}
		else
		{
			_qwtPlot->setAxisAutoScale(QwtPlot::xBottom, true);
			_qwtPlot->setAxisAutoScale(QwtPlot::yLeft, true);
		}
		double minValue = _ui->lineEdit_4->text().toDouble();
		double maxValue = _ui->lineEdit_5->text().toDouble();

		int method = _ui->comboBox_3->currentIndex();
		if (method != 0 && rangeIsEmpty)
		{
			_qwtPlot->replot();
			return;
		}
		if (method == 0)//自动生成
		{
			if (!rangeIsEmpty)
			{
				_qwtPlot->setAxisScale(axis, minValue, maxValue);
			}
			//QwtScaleDiv scaleDiv(minValue, maxValue);
			//_qwtPlot->setAxisScaleDiv(axis, scaleDiv);
		}
		else if (method == 1)//主刻度等差步长
		{
			QList<double> ticks[QwtScaleDiv::NTickTypes];
			QwtInterval interval(minValue, maxValue);
			
			//设置值
			double stepLength = _ui->lineEdit->text().toDouble();
			int stepNum = _ui->lineEdit_2->text().toInt();
			double mediumStepLength = stepLength / stepNum;

			int num = (maxValue - minValue) / stepLength;
			double lastValue = minValue;
			ticks[QwtScaleDiv::MajorTick].append(lastValue);
			for (int i = 0; i < num; i++)
			{
				double tempValue = lastValue;
				for (int j = 0; j < stepNum; j++)
				{
					tempValue += mediumStepLength;
					ticks[QwtScaleDiv::MediumTick].append(tempValue);
				}
				lastValue += stepLength;
				ticks[QwtScaleDiv::MajorTick].append(lastValue);
			}

			QwtScaleDiv scaleDiv(interval, ticks);
			_qwtPlot->setAxisScaleDiv(axis, scaleDiv);
		}
		else if (method == 2)//主刻度等比步长
		{
			QList<double> ticks[QwtScaleDiv::NTickTypes];
			QwtInterval interval(minValue, maxValue);

			//设置值
			double stepLength = _ui->lineEdit->text().toDouble();
			int stepNum = _ui->lineEdit_2->text().toInt();
			double mediumStepLength = stepLength / stepNum;

			int num = maxValue / (minValue * stepLength);
			double lastValue = minValue;
			ticks[QwtScaleDiv::MajorTick].append(lastValue);
			for (int i = 0; i < num; i++)
			{
				double tempValue = lastValue;
				for (int j = 0; j < stepNum; j++)
				{
					tempValue *= mediumStepLength;
					ticks[QwtScaleDiv::MediumTick].append(tempValue);
				}
				lastValue *= stepLength;
				ticks[QwtScaleDiv::MajorTick].append(lastValue);
			}

			QwtScaleDiv scaleDiv(interval, ticks);
			_qwtPlot->setAxisScaleDiv(axis, scaleDiv);

		}
		else//获取用户输入的表格数据
		{
			QList<double> ticks[QwtScaleDiv::NTickTypes];
			QwtInterval interval(minValue, maxValue);

			QString mainStepLengthText = _ui->lineEdit_3->text().simplified();
			QString mediumStepNumText = _ui->lineEdit_6->text().simplified();

			QStringList mainStepLengths = mainStepLengthText.split(",");
			QStringList mediumStepNums = mediumStepNumText.split(",");
			if (mainStepLengths.size() != (mediumStepNums.size() + 1))
			{
				return;
			}

			for (int i = 0; i < mainStepLengths.size(); i++)
			{
				ticks[QwtScaleDiv::MajorTick].append(mainStepLengths.at(i).toDouble());
			}

			for (int i = 0; i < mediumStepNums.size(); i++)
			{
				int stepNum = mediumStepNums.at(i).toInt();
				double firstValue = ticks[QwtScaleDiv::MajorTick].at(i);
				double lastValue = ticks[QwtScaleDiv::MajorTick].at(i + 1);
				double length = (lastValue - firstValue) / stepNum;
				for (int j = 1; j < stepNum; j++)
				{
					firstValue += length;
					ticks[QwtScaleDiv::MediumTick].append(firstValue);
				}
			}

			QwtScaleDiv scaleDiv(interval, ticks);
			_qwtPlot->setAxisScaleDiv(axis, scaleDiv);

		}

		_qwtPlot->replot();


	}
	void mPlotViewStyleDialog::slot_updateCurveNames()
	{
		QStringList names;
		QList<QwtPlotItem *> list = _qwtPlot->itemList();
		for (int i = 0; i < list.size(); i++)
		{
			QwtPlotCurve *curve = dynamic_cast<QwtPlotCurve*>(list.at(i));
			if (curve == nullptr)
			{
				continue;
			}
			names.append(curve->title().text());	
		}
		_ui->comboBox_4->clear();
		_ui->comboBox_4->addItems(names);
	}
	void mPlotViewStyleDialog::slot_changeCurveName()
	{
		const QString & oldName = _ui->comboBox_4->currentText();
		const QString & newName = _ui->lineEdit_9->text();
		QList<QwtPlotItem *> list = _qwtPlot->itemList();
		for (int i = 0; i < list.size(); i++)
		{
			QwtPlotCurve *curve = dynamic_cast<QwtPlotCurve*>(list.at(i));
			if (curve == nullptr)
			{
				continue;
			}
			if (curve->title().text() == oldName)
			{
				curve->setTitle(newName);
			}
		}
		_qwtPlot->replot();
		slot_updateCurveNames();
	}
	void mPlotViewStyleDialog::showEvent(QShowEvent * event)
	{
		slot_updateCurveNames();
	}
}