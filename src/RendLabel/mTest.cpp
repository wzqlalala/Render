#include "mWidgetOperation.h"
//QT
#include <QPixmap>
#include <QRegExp>
#include <QRegExpValidator>
#include <QApplication>
#include <QClipboard>

namespace MxFunctions
{
	void setWidgetPosByMouse(QWidget * mainWidget, QWidget * popWidget)
	{
		QPoint mousePos = QCursor().pos();//��ǰ����������
		QPoint mainPos = mainWidget->mapToGlobal(mainWidget->pos());//��ȡ�����ڵ�ȫ������
		int mainLeft = mainPos.x();
		int mainTop = mainPos.y();
		int mainRight = mainPos.x() + mainWidget->width();
		int mainBottom = mainPos.y() + mainWidget->height();
		//���õ����ĳ�ʼ����λ�����������
		int popLeft = mousePos.x() - popWidget->width() / 2;
		int popRight = mousePos.x() + popWidget->width() / 2;
		int popTop = mousePos.y() - popWidget->height() / 2;
		int popBottom = mousePos.y() + popWidget->height() / 2;
		// ����Ҫȷ����ֻ�����Ͻǵ�����λ��: 

		//������߳���������
		if (popLeft < mainLeft)
		{
			int temp = mainLeft - popLeft;
			popLeft = popLeft + temp;
			popRight = popRight + temp;
		}
		//�����ϲ��ֳ����������ϲ�ֵ
		if (popTop < mainTop)
		{
			int temp = mainTop - popTop;
			popTop = popTop + temp;
			popBottom = popBottom + temp;
		}

		//�������·����ұ�Ϊ�߽�

		//�����Ҳ೬���������Ҳ�߽�ֵ
		if (popRight > mainRight)
		{
			int temp = popRight - mainRight;
			popLeft = popLeft - temp;
			popRight = popRight - temp;
		}
		//�����²೬���������²�߽�ֵ
		if (popBottom > mainBottom)
		{
			int temp = popBottom - mainBottom;
			popBottom = popBottom - temp;
			popTop = popTop - temp;
		}


		popWidget->move(popLeft, popTop);
	}

	void setButtonColor(QPushButton* button, const QColor& color)
	{

		if (button == nullptr)
		{
			return;
		}
		QPixmap cpix(QSize(50, 40));
		cpix.fill(color);
		button->setIcon(cpix);
		button->setText(color.name());
	}

	void clearStackedWidget(QStackedWidget * stackedwidget)
	{
		int count = stackedwidget->count();
		for (int i = 0; i < count; ++i)
		{
			QWidget *w = stackedwidget->widget(i);
			//stackedwidget->removeWidget(w);
			if (w != nullptr)
			{
				delete w;
				w = nullptr;
			}
		}
		delete stackedwidget;
		stackedwidget = new QStackedWidget;
	}

	void setChildrenItemState(QTreeWidgetItem* pitem, bool ischeck)
	{
		if (pitem == nullptr)
		{
			return;
		}

		Qt::CheckState cs = Qt::Unchecked;
		if (ischeck)
		{
			cs = Qt::Checked;
		}

		const int ncitem = pitem->childCount();
		for (int i = 0; i < ncitem; ++i)
		{
			QTreeWidgetItem* citem = pitem->child(i);
			if (citem != nullptr)
			{
				citem->setCheckState(0, cs);
			}
		}
	}

	void clearListWidget(QListWidget* listwidget)
	{
		if (listwidget == nullptr)
		{
			return;
		}

		const int nrow = listwidget->count();
		for (int i = 0; i < nrow; ++i)
		{
			QListWidgetItem* tepItem = listwidget->item(i);
			if (tepItem != nullptr)
			{
				delete tepItem;
				tepItem = nullptr;
			}
		}
		listwidget->clear();
	}

	QStringList getSelectedList(QListWidget* listWidget)
	{
		QStringList selectedList;
		if (listWidget == nullptr)
		{
			return selectedList;
		}
		
		const int nitem = listWidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* tepItem = listWidget->item(i);
			if (tepItem != nullptr)
			{
				if (tepItem->isSelected())
				{
					QString itemname = tepItem->text();
					selectedList.append(itemname);
				}
			}
		}
		return selectedList;
	}

	QStringList getUnselectedList(QListWidget * listWidget)
	{
		QStringList unselectedList;
		if (listWidget == nullptr)
		{
			return unselectedList;
		}

		const int nitem = listWidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* tepItem = listWidget->item(i);
			if (tepItem != nullptr)
			{
				if (!tepItem->isSelected())
				{
					QString itemname = tepItem->text();
					unselectedList.append(itemname);
				}
			}
		}
		return unselectedList;
	}

	QList<int> getSelectedIndexes(QListWidget* listwidget)
	{
		QList<int> lists;
		if (listwidget == nullptr)
		{
			return lists;
		}

		const int nitem = listwidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* tepItem = listwidget->item(i);
			if (tepItem != nullptr)
			{
				if (tepItem->isSelected())
				{
					lists.append(i);
				}
			}
		}
		return lists;
	}

	QString getSelectedString(QListWidget* listwidget)
	{
		if (listwidget == nullptr)
		{
			return QString();
		}
		if (listwidget->count() == 0)
		{
			return QString();
		}
		//��ȡ��
		QListWidgetItem* item = nullptr;
		QList<QListWidgetItem*> items = listwidget->selectedItems();
		if (items.size() == 0)
		{
			//�����б��е�һ�������
			item = listwidget->item(0);
		}
		else
		{
			//����ѡ�еĵ�һ������
			item = items.at(0);
		}
		//��������
		if (item != nullptr)
		{
			return item->text();
		}
		else
		{
			return QString();
		}
	}

	QStringList getCheckedItemNames(QListWidget * listwidget)
	{
		QStringList names;
		if (listwidget == nullptr)
		{
			return names;
		}
		const int nitem = listwidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* item = listwidget->item(i);
			if (item != nullptr&&item->checkState() == Qt::Checked)
			{
				names.append(item->text());
			}
		}
		return names;
	}

	void setListWidgetItemSelected(QListWidget * listwidget, bool isselect)
	{
		if (listwidget == nullptr)
		{
			return;
		}
		const int n = listwidget->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* item = listwidget->item(i);
			if (item != nullptr)
			{
				item->setSelected(isselect);
			}
		}
	}

	void setListWidgetItemSelected(QListWidget * listwidget, const QStringList & names)
	{
		if (listwidget == nullptr)
		{
			return;
		}
		const int n = listwidget->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* item = listwidget->item(i);
			if (item != nullptr)
			{
				QString name = item->text();
				if (names.contains(name))
				{
					item->setSelected(true);
				}
				else
				{
					item->setSelected(false);
				}
			}
		}
	}

	void setListWidgetItemChecked(QListWidget * listwidget, const QStringList & names)
	{
		if (listwidget == nullptr)
		{
			return;
		}
		const int nitem = listwidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* item = listwidget->item(i);
			if (item != nullptr)
			{
				if (names.contains(item->text()))
				{
					item->setCheckState(Qt::Checked);
				}
				else
				{
					item->setCheckState(Qt::Unchecked);
				}
			}
		}
	}

	void addCheckableListWidget(QListWidget* listwidget, const QStringList& texts)
	{
		if (listwidget == nullptr)
		{
			return;
		}
		const int nitem = texts.size();
		for (int i = 0; i < nitem; ++i)
		{
			QString txt = texts.at(i);
			QListWidgetItem* item = new QListWidgetItem(txt);
			item->setCheckState(Qt::Unchecked);
			listwidget->addItem(item);
		}
	}

	void setListWidgetUnchecked(QListWidget* listwidget)
	{
		if (listwidget == nullptr)
		{
			return;
		}
		const int nitem = listwidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* item = listwidget->item(i);
			if (item != nullptr&&item->checkState() == Qt::Checked)
			{
				item->setCheckState(Qt::Unchecked);
			}
		}
	}

	void setListWidgetUnchecked1(QListWidget * listwidget)
	{
		if (listwidget == nullptr)
		{
			return;
		}
		const int nitem = listwidget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* item = listwidget->item(i);
			if (item != nullptr)
			{
				item->setCheckState(Qt::Unchecked);
			}
		}
	}

	void setListWidgetItemState(QListWidget* listwidget, QListWidgetItem* item)
	{
		if (listwidget == nullptr || item == nullptr)
		{
			return;
		}

		Qt::CheckState cs = item->checkState();
		if (cs == Qt::Checked)
		{
			item->setCheckState(Qt::Unchecked);
		}
		else
		{
			//���ø�ѡ��״̬
			const int nitem = listwidget->count();
			for (int i = 0; i < nitem; ++i)
			{
				QListWidgetItem* tepItem = listwidget->item(i);
				if (tepItem != nullptr&&tepItem != item)
				{
					tepItem->setCheckState(Qt::Unchecked);
				}
			}
			item->setCheckState(Qt::Checked);
		}
	}

	QListWidgetItem* getListWidgetItem(QListWidget* widget, const QString& name)
	{
		if (widget == nullptr)
		{
			return nullptr;
		}
		const int nitem = widget->count();
		for (int i = 0; i < nitem; ++i)
		{
			QListWidgetItem* tepitem = widget->item(i);
			if (tepitem != nullptr)
			{
				if (name == tepitem->text())
				{
					tepitem->setSelected(true);
					return tepitem;
				}
			}
		}
		return nullptr;
	}

	QListWidgetItem* getFirstSelectedItem(QListWidget* widget)
	{
		if (widget == nullptr)
		{
			return nullptr;
		}
		QList<QListWidgetItem*> items = widget->selectedItems();
		if (items.size() == 0)
		{
			return nullptr;
		}
		else
		{
			return items.at(0);
		}
	}

	void setTableWidgetStyle(QTableWidget* tw, const QStringList& headers)
	{
		if (tw == nullptr)
		{
			return;
		}
		//��ʼ�����
		tw->setColumnCount(2);
		tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
		tw->verticalHeader()->setHidden(true);
		tw->resizeColumnsToContents();

		//��ͷ
		tw->setHorizontalHeaderLabels(headers);
		tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		tw->horizontalHeader()->setEditTriggers(QAbstractItemView::NoEditTriggers);
		tw->horizontalHeader()->setHighlightSections(false);
		tw->horizontalHeader()->setStyleSheet("QHeaderView:section{background:rgb(218,218,218);}");
	}

	void getTableWidgetDatas(QTableWidget* tw, QList<QPair<double, double>>& datas)
	{
		if (tw == nullptr || tw->columnCount() != 2)
		{
			return;
		}
		const int rowcount = tw->rowCount();
		for (int i = 0; i < rowcount; ++i)
		{
			QTableWidgetItem* item1 = tw->item(i, 0);
			QTableWidgetItem* item2 = tw->item(i, 1);
			if (item1 != nullptr&&item2 != nullptr)
			{
				QPair<double, double> datapair;
				datapair.first = item1->text().toDouble();
				datapair.second = item2->text().toDouble();
				datas.append(datapair);
			}
			else if (item1 == nullptr&&item2 != nullptr)
			{
				QPair<double, double> datapair;
				datapair.first = 0;
				datapair.second = item2->text().toDouble();
				datas.append(datapair);
			}
			else if (item1 != nullptr&&item2 == nullptr)
			{
				QPair<double,double> datapair;
				datapair.first = item1->text().toDouble();
				datapair.second = 0;
				datas.append(datapair);
			}
		}
	}

	void setTableWidgetDatas(QTableWidget* tw, QList<QPair<double, double>>& datas)
	{
		if (tw == nullptr || tw->columnCount() != 2)
		{
			return;
		}
		clearTableWidgetDatas(tw);

		const int rowcount = datas.size();
		tw->setRowCount(rowcount);
		for (int i = 0; i < rowcount; ++i)
		{
			QPair<double, double> pairval = datas.at(i);
			tw->setItem(i, 0, new QTableWidgetItem(QString::number(pairval.first)));
			tw->setItem(i, 1, new QTableWidgetItem(QString::number(pairval.second)));
		}
	}

	void clearTableWidgetDatas(QTableWidget* tw)
	{
		const int count1 = tw->rowCount();
		for (int i = 0; i < count1; ++i)
		{
			QTableWidgetItem* item1 = tw->item(i, 0);
			if (item1 != nullptr)
			{
				delete item1;
			}
			QTableWidgetItem* item2 = tw->item(i, 1);
			if (item2 != nullptr)
			{
				delete item2;
			}
			QTableWidgetItem* item3 = tw->item(i, 2);
			if (item3 != nullptr)
			{
				delete item3;
			}
		}
		tw->setRowCount(0);
	}

	void copyTableToBoard(QTableWidget * tw)
	{
		if (tw == nullptr)
		{
			return;
		}
		QStringList tablelist;
		const int nrow = tw->rowCount();
		const int ncol = tw->columnCount();
		for (int i = 0; i < nrow; ++i)
		{
			QStringList rowlist;
			for (int j = 0; j < ncol; ++j)
			{
				QTableWidgetItem* item = tw->item(i, j);
				if (item != nullptr)
				{
					rowlist.append(item->text());
				}
			}
			tablelist.append(rowlist.join("\t"));
		}
		QApplication::clipboard()->setText(tablelist.join("\n"));
	}

	void pasteBoardToTable(QTableWidget * tw, bool isone)
	{
		if (tw == nullptr)
		{
			return;
		}
		//��ȡѡ�еĵ�Ԫ��λ��
		int rowi = 0;
		int coli = 0;
		QList<QTableWidgetItem*> items = tw->selectedItems();
		if (items.size() > 0)
		{
			QTableWidgetItem* item = items.at(0);
			if (item != nullptr)
			{
				rowi = tw->row(item);
				coli = tw->column(item);
			}
		}
		//��ȡ���������һ����ԣ������������ܸı䣩
		const int tablecol = tw->columnCount();
		//��ȡ���а�����
		QString cliptext = QApplication::clipboard()->text();
		//���зָ�
		QStringList tablelist = cliptext.split("\n");
		//ȥ������
		tablelist.removeAll("");
		//����ѡ�еĵ�Ԫ��λ�úͼ��а��е��������������ñ�������
		const int nrow = tablelist.size();
		if (isone)
		{
			tw->setRowCount(1);
		}
		else
		{
			tw->setRowCount(rowi + nrow);
		}
		//�ӿ�ʼ���б������а������
		for (int i = 0; i < nrow; ++i)
		{
			//��λ�ȡ�е�����
			QString rowtext = tablelist.at(i);
			QStringList rowlist = rowtext.split("\t");
			int ncol = rowlist.size();
			//д������
			int datai = 0;
			for (int j = coli; j < tablecol; ++j)
			{
				if (datai >= rowlist.size())
				{
					break;
				}
				QString itemtext = rowlist.at(datai);
				tw->setItem(rowi + i, j, new QTableWidgetItem(itemtext));

				++datai;
			}
		}
	}
}
