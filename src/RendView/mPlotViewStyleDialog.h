#pragma once

#include <QDialog>

namespace Ui
{
	class mPlotViewStyleDialog;
}

class QwtPlot;
namespace MPlotView
{
	class mPlotView;
	class mPlotViewStyleDialog : public QDialog
	{
		Q_OBJECT

	public:
		//���캯��
		mPlotViewStyleDialog(mPlotView *parent, QwtPlot *plot);

		//��������
		~mPlotViewStyleDialog();

	private slots:
		/***********************************����********************************************/
		void slot_titleChanged();

		/***********************************������********************************************/
		//������ʽ�����е���������
		void slot_clickedtype(int index);

		//�ı���ʽ�����еĶ���ֵ
		void slot_logValueChanged(double value);

		//����X�᷶Χ
		void slot_setXRange();

		//�̶������ɷ�ʽ
		void slot_scaleLinesMethod(int index);

		//�����
		void slot_axisTitle();

		//���ok��ť
		void slot_okButton();

		/***********************************ͼ��********************************************/
		//�����������ߵ�����
		void slot_updateCurveNames();

		//�޸����ߵ�����
		void slot_changeCurveName();


	protected:
		//��ʾ�¼�
		virtual void showEvent(QShowEvent *event);

	private:
		//Ui
		Ui::mPlotViewStyleDialog *_ui;

		//Qwt��ͼ
		QwtPlot *_qwtPlot;
	};
}