#pragma once
//解决中文乱码
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
	* 通过鼠标点击的位置获取弹窗的位置
	*/
	extern void RENDLABEL_EXPORT setWidgetPosByMouse(QWidget *parnetWidget, QWidget* popWidget);

	/**********************************************QPushButton*************************************************/
	/*
	* 设置颜色按钮的色块
	*/
	extern void RENDLABEL_EXPORT setButtonColor(QPushButton* button, const QColor& color);

	/**********************************************QStackedWidget*************************************************/
	/*
	* 清空QStackedWidget
	*/
	extern void RENDLABEL_EXPORT clearStackedWidget(QStackedWidget* stackedwidge);

	/**********************************************QTreeWidget*************************************************/
	/*
	* 设置某父节点下的所有子节点选中状态
	*/
	extern void RENDLABEL_EXPORT setChildrenItemState(QTreeWidgetItem* pitem, bool ischeck);

	/**********************************************QListWidget*************************************************/
	/*
	* 清空QListWidget
	*/
	extern void RENDLABEL_EXPORT clearListWidget(QListWidget* listwidget);

	/*
	* 获取QListWidget的选中项
	*/
	extern QStringList RENDLABEL_EXPORT getSelectedList(QListWidget* listWidget);

	/*
	* 获取QListWidget的未选中项
	*/
	extern QStringList RENDLABEL_EXPORT getUnselectedList(QListWidget* listWidget);

	/*
	* 获取QListWidget选中项的索引
	*/
	extern QList<int> RENDLABEL_EXPORT getSelectedIndexes(QListWidget* listwidget);

	/*
	* 获取QListWidget选中项的名称
	*/
	extern QString RENDLABEL_EXPORT getSelectedString(QListWidget* listwidget);

	/*
	* 获取QListWidget所有Checked项的名称
	*/
	extern QStringList RENDLABEL_EXPORT getCheckedItemNames(QListWidget* listwidget);

	/*
	* 设置QListWidget所有项是否选中
	*/
	extern void RENDLABEL_EXPORT setListWidgetItemSelected(QListWidget* listwidget, bool isselect);

	/*
	* 设置QListWidget部分选中
	*/
	extern void RENDLABEL_EXPORT setListWidgetItemSelected(QListWidget* listwidget, const QStringList& names);

	/*
	* 设置QListWidget某些项Checked
	*/
	extern void RENDLABEL_EXPORT setListWidgetItemChecked(QListWidget* listwidget, const QStringList& names);

	/*
	* 创建有复选框的QListWidget
	*/
	extern void RENDLABEL_EXPORT addCheckableListWidget(QListWidget* listwidget, const QStringList& texts);

	/*
	* 设置QListWidget所有已选中的项未选中
	*/
	extern void RENDLABEL_EXPORT setListWidgetUnchecked(QListWidget* listwidget);

	/*
	* 设置QListWidget所有项未选中
	*/
	extern void RENDLABEL_EXPORT setListWidgetUnchecked1(QListWidget* listwidget);

	/*
	* 设置QListWidget最多只有一项选中
	*/
	extern void RENDLABEL_EXPORT setListWidgetItemState(QListWidget* listwidget, QListWidgetItem* item);

	/*
	* 根据名称查找QListWidget中的项，并设置该项选中
	*/
	extern RENDLABEL_EXPORT QListWidgetItem* getListWidgetItem(QListWidget* widget, const QString& name);

	/*
	* 获取QListWidget选中项中的第一个
	*/
	extern RENDLABEL_EXPORT QListWidgetItem* getFirstSelectedItem(QListWidget* widget);

	/**********************************************QTableWidget*************************************************/
	/*
	* 设置表格样式
	*/
	extern void RENDLABEL_EXPORT setTableWidgetStyle(QTableWidget* tw, const QStringList& headers);

	/*
	* 获取表格中的XY数据
	*/
	extern void RENDLABEL_EXPORT getTableWidgetDatas(QTableWidget* tw, QList<QPair<double, double>>& datas);

	/*
	* 设置表格数据
	*/
	extern void RENDLABEL_EXPORT setTableWidgetDatas(QTableWidget* tw, QList<QPair<double, double>>& datas);

	/*
	* 清空表格
	*/
	extern void RENDLABEL_EXPORT clearTableWidgetDatas(QTableWidget* tw);

	/*
	* 复制表格内容到剪切板
	*/
	extern void RENDLABEL_EXPORT copyTableToBoard(QTableWidget* tw);

	/*
	* 将剪切板内容粘贴到表格中
	* 参数isone表示是否限制表格仅为一行（材料模块需要）
	*/
	extern void RENDLABEL_EXPORT pasteBoardToTable(QTableWidget* tw, bool isone);
}