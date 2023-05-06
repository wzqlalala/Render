#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "rendlabel_global.h"

//QT
#include <QObject>
#include <QPushButton>
#include <QColor>
#include <QListWidget>
#include <QVector3D>
#include <QTreeWidgetItem>
#include <set>
#include <QTableWidget>
#include <QHeaderView>
#include <QPair>
#include <QPoint>
#include <QHash>
#include <QStackedWidget>

namespace MxFunctions
{
	/**********************************************QWidget*************************************************/
	/*
	* ͨ���������λ�û�ȡ������λ��
	*/
	extern void RENDLABEL_EXPORT setWidgetPosByMouse(QWidget *parnetWidget, QWidget* popWidget);

	/**********************************************QPushButton*************************************************/
	/*
	* ������ɫ��ť��ɫ��
	*/
	extern void RENDLABEL_EXPORT setButtonColor(QPushButton* button, const QColor& color);

	/**********************************************QStackedWidget*************************************************/
	/*
	* ���QStackedWidget
	*/
	extern void RENDLABEL_EXPORT clearStackedWidget(QStackedWidget* stackedwidge);

	/**********************************************QTreeWidget*************************************************/
	/*
	* ����ĳ���ڵ��µ������ӽڵ�ѡ��״̬
	*/
	extern void RENDLABEL_EXPORT setChildrenItemState(QTreeWidgetItem* pitem, bool ischeck);

	/**********************************************QListWidget*************************************************/
	/*
	* ���QListWidget
	*/
	extern void RENDLABEL_EXPORT clearListWidget(QListWidget* listwidget);

	/*
	* ��ȡQListWidget��ѡ����
	*/
	extern QStringList RENDLABEL_EXPORT getSelectedList(QListWidget* listWidget);

	/*
	* ��ȡQListWidget��δѡ����
	*/
	extern QStringList RENDLABEL_EXPORT getUnselectedList(QListWidget* listWidget);

	/*
	* ��ȡQListWidgetѡ���������
	*/
	extern QList<int> RENDLABEL_EXPORT getSelectedIndexes(QListWidget* listwidget);

	/*
	* ��ȡQListWidgetѡ���������
	*/
	extern QString RENDLABEL_EXPORT getSelectedString(QListWidget* listwidget);

	/*
	* ��ȡQListWidget����Checked�������
	*/
	extern QStringList RENDLABEL_EXPORT getCheckedItemNames(QListWidget* listwidget);

	/*
	* ����QListWidget�������Ƿ�ѡ��
	*/
	extern void RENDLABEL_EXPORT setListWidgetItemSelected(QListWidget* listwidget, bool isselect);

	/*
	* ����QListWidget����ѡ��
	*/
	extern void RENDLABEL_EXPORT setListWidgetItemSelected(QListWidget* listwidget, const QStringList& names);

	/*
	* ����QListWidgetĳЩ��Checked
	*/
	extern void RENDLABEL_EXPORT setListWidgetItemChecked(QListWidget* listwidget, const QStringList& names);

	/*
	* �����и�ѡ���QListWidget
	*/
	extern void RENDLABEL_EXPORT addCheckableListWidget(QListWidget* listwidget, const QStringList& texts);

	/*
	* ����QListWidget������ѡ�е���δѡ��
	*/
	extern void RENDLABEL_EXPORT setListWidgetUnchecked(QListWidget* listwidget);

	/*
	* ����QListWidget������δѡ��
	*/
	extern void RENDLABEL_EXPORT setListWidgetUnchecked1(QListWidget* listwidget);

	/*
	* ����QListWidget���ֻ��һ��ѡ��
	*/
	extern void RENDLABEL_EXPORT setListWidgetItemState(QListWidget* listwidget, QListWidgetItem* item);

	/*
	* �������Ʋ���QListWidget�е�������ø���ѡ��
	*/
	extern RENDLABEL_EXPORT QListWidgetItem* getListWidgetItem(QListWidget* widget, const QString& name);

	/*
	* ��ȡQListWidgetѡ�����еĵ�һ��
	*/
	extern RENDLABEL_EXPORT QListWidgetItem* getFirstSelectedItem(QListWidget* widget);

	/**********************************************QTableWidget*************************************************/
	/*
	* ���ñ����ʽ
	*/
	extern void RENDLABEL_EXPORT setTableWidgetStyle(QTableWidget* tw, const QStringList& headers);

	/*
	* ��ȡ����е�XY����
	*/
	extern void RENDLABEL_EXPORT getTableWidgetDatas(QTableWidget* tw, QList<QPair<double, double>>& datas);

	/*
	* ���ñ������
	*/
	extern void RENDLABEL_EXPORT setTableWidgetDatas(QTableWidget* tw, QList<QPair<double, double>>& datas);

	/*
	* ��ձ��
	*/
	extern void RENDLABEL_EXPORT clearTableWidgetDatas(QTableWidget* tw);

	/*
	* ���Ʊ�����ݵ����а�
	*/
	extern void RENDLABEL_EXPORT copyTableToBoard(QTableWidget* tw);

	/*
	* �����а�����ճ���������
	* ����isone��ʾ�Ƿ����Ʊ���Ϊһ�У�����ģ����Ҫ��
	*/
	extern void RENDLABEL_EXPORT pasteBoardToTable(QTableWidget* tw, bool isone);
}