#pragma once
//�����������
#pragma execution_character_set("utf-8")

#include "rendbasic_global.h"

//MBasicFunction
#include "mBasicEnum.h"

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

namespace MBasicFunction
{
	class RENDBASIC_EXPORT mCommonFunctions : public QObject
	{
	public:
		/*
		* ������ɫ��ť��ɫ��
		*/
		static void setButtonColor(QPushButton* button, const QColor& color);

		/*
		* �Դ�����б���ˣ����ع��˺���б�ʹ��������ʽƥ��Ĺ��ˣ�
		*/
		static QStringList filterList(const QStringList& list1, const QString& filter);

		/*
		* ��ͨ���ˣ�����Ŀ���ַ�����
		*/
		static QStringList filterList1(const QStringList& list1, const QString& filter);

		/*
		* ���QListWidget
		*/
		static void clearStackedWidget(QStackedWidget* stackedwidge);

		/*
		* ���QListWidget
		*/
		static void clearListWidget(QListWidget* listwidget);

		/*
		* ��ȡQListWidget��ѡ����
		*/
		static QStringList getSelectedList(QListWidget* listWidget);

		/*
		* ��ȡQListWidget��δѡ����
		*/
		static QStringList getUnselectedList(QListWidget* listWidget);

		/*
		* ��ȡQListWidgetѡ���������
		*/
		static QList<int> getSelectedIndexes(QListWidget* listwidget);

		/*
		* ��ȡQListWidgetѡ���������
		*/
		static QString getSelectedString(QListWidget* listwidget);

		/*
		* ��ȡQListWidget����Checked�������
		*/
		static QStringList getCheckedItemNames(QListWidget* listwidget);

		/*
		* ����QListWidget�������Ƿ�ѡ��
		*/
		static void setListWidgetItemSelected(QListWidget* listwidget, bool isselect);

		/*
		* ����QListWidget����ѡ��
		*/
		static void setListWidgetItemSelected(QListWidget* listwidget, const QStringList& names);

		/*
		* ����QListWidgetĳЩ��Checked
		*/
		static void setListWidgetItemChecked(QListWidget* listwidget, const QStringList& names);

		/*
		* �����и�ѡ���QListWidget
		*/
		static void addCheckableListWidget(QListWidget* listwidget, const QStringList& texts);

		/*
		* ����QListWidget������ѡ�е���δѡ��
		*/
		static void setListWidgetUnchecked(QListWidget* listwidget);

		/*
		* ����QListWidget������δѡ��
		*/
		static void setListWidgetUnchecked1(QListWidget* listwidget);

		/*
		* ����QListWidget���ֻ��һ��ѡ��
		*/
		static void setListWidgetItemState(QListWidget* listwidget, QListWidgetItem* item);

		/*
		* ���ݵ�Ԫ���ͻ�ȡ��Ԫ�����Ľڵ����
		*/
		static int elementNodeCount(const QString& type);
		static int elementNodeCount(ElementType elementType);
		/*
		* ���ݵ�Ԫö�����ͻ�ȡ��Ԫ����
		*/
		static QString getElementMainType(ElementType elementType);

		/*
		* ��ID�б�ת�����ַ���
		* ����type��ʾID���ͣ��ڵ�(0)��Ԫ(1)
		*/
		static QString transformListToString(const std::set<int> ids, int type);

		/*
		* ��ID�б�ת�����ַ�����������ͣ�
		*/
		static QString transformListToString(const std::set<int> nids, const std::set<int> eids);


		/*
		* ��ID�ַ���ת�����б�
		* ����ֵΪID���ͣ��ڵ�(0)��Ԫ(1)
		* ����idsΪ�ַ���������ID�б�
		*/
		static int transformStringToList(const QString& idstring, QList<int>& ids);

		/*
		* ��ID�ַ���ת�����б�
		* ����ֵΪID���ͣ��ڵ�(0)��Ԫ(1)
		* ����idsΪ�ַ���������ID�б�
		*/
		static int transformStringToList(const QString& idstring, std::set<int>& ids);

		/*
		* ��ID�ַ���ת�����б�
		* ����idstringΪID�ַ���
		* ����nidsΪ�ڵ�ID�б�����eidsΪ��ԪID�б�
		*/
		static bool transformStringToList(const QString& idstring, std::set<int>& nids, std::set<int>& eids);

		/*
		* ��ID�ַ���ת��Ϊ�б�
		* ����1,2,5-10
		*/
		static bool transformMeshStringToSet(const QString& idstring, std::set<int>& mids);

		/*
		* ��Set<int>����ת����Qlist<int>����
		*/
		static bool transformSetToList(const std::set<int> id_set, QList<int>& id_list);
		/*
		 ��Qlist<int>����ת����Set<int>����
		*/
		static bool transformListToSet(const QList<int> id_list, std::set<int>& id_set);
		/*
		* ��QVector<int>ת��Ϊstd::set<int>
		*/
		static std::set<int> transformQVectorToSet(const QVector<int> vec);

		/*
		* ��QStringListת����QString���ö��ŷָ�
		*/
		static QString transformQStringListToQString(const QStringList& strlist);

		/*
		* ͨ���������Ƕȣ�p3Ϊ�ǵ�
		*/
		static double getThreePointAngle(const QVector3D p1, const QVector3D p2, const QVector3D p3);

		/*
		* ��ȡ������е�����
		*/
		static QVector3D getCenterPointCoord(const QVector3D p1, const QVector3D p2);

		/*
		* ��ȡ�����ε�����
		*/
		static QVector3D getCenterPointCoord(const QVector3D p1, const QVector3D p2, const QVector3D p3);

		/*
		* ��ȡ�ı��ε�����
		*/
		static QVector3D getCenterPointCoord(const QVector3D p1, const QVector3D p2, const QVector3D p3, const QVector3D p4);

		/*
		* ��ȡ��������������������
		* ����p1��p2�ֱ�Ϊ����ǰ����������
		* ����valΪ����ϵ������ΧΪ0-1��0��ʾ��������1��ʾͬʱ�������е�
		*/
		static void getShrinkPosition(QVector3D& p1, QVector3D& p2, double val);

		/*
		 * ��ö�ٵĵ�Ԫ����ת��Ϊ�ַ���
		 */
		static QString getElementString(ElementType elementType);

		/*
		 * ���ַ����ĵ�Ԫ����ת��Ϊö��
		*/
		static ElementType getElementType(QString elementstring);

		/*
		* ���ݳ�������ȷ��Ĭ�ϵ�Ԫ����(���յ����������жϣ����������ⲿ����ѭ������)
		*/
		static QHash<MeshType,ElementType> getDefaultElementType(AnalysisModule analysisModule,int order = 1);

		/*
		* ���ݳ����ͺ�����������ȷ��Ĭ�ϵ�Ԫ����(���յ����������жϣ����������ⲿ����ѭ������)
		*/
		static ElementType getDefaultElementType(AnalysisModule analysisModule, MeshType meshType, int order = 1);

		/*
		* ���ݵͽ׵�Ԫ���ͻ�ȡ�߽׵�Ԫ����
		*/
		static ElementType getHighOrderElementType(ElementType elementType);

		/*
		* ���ݸ߽׵�Ԫ���ͻ�ȡ�ͽ׵�Ԫ����
		*/
		static ElementType getLowOrderElementType(ElementType elementType);

		/*
		 * �ж�ĳ��Ԫ������һ�׻��Ƕ��ף�����Ҳ�ж���������״
		 */
		static QString getElementInfo(ElementType elementType, int &order);

		/*
		* ����ĳ���ڵ��µ������ӽڵ�ѡ��״̬
		*/
		static void setChildrenItemState(QTreeWidgetItem* pitem, bool ischeck);

		/*
		* �������Ʋ���QListWidget�е�������ø���ѡ��
		*/
		static QListWidgetItem* getListWidgetItem(QListWidget* widget, const QString& name);

		/*
		* ��ȡQListWidgetѡ�����еĵ�һ��
		*/
		static QListWidgetItem* getFirstSelectedItem(QListWidget* widget);

		/*
		* QVector3D ����ת��Ϊ QString����
		*/
		static QString transformVector3DToString(const QVector3D pos);
		/*
		* QVector(QVector3D) ����ת��Ϊ QString����
		*/
		static QString transformVector_Vector3DToString(const QVector<QVector3D> pos);

		/*
		*  QString ����ת��Ϊ QVector3D����
		*/
		static QVector3D transformStringToVector3D(const QString pos);
		/*
		*  QString ����ת��Ϊ QVector(QVector3D)����
		*/
		static  QVector<QVector3D> transformStringToVector_Vector3D(const QString pos);
		/*
		*  �ж���ά�߶��Ƿ��ཻ
		*/
		static bool LineIntersect3D(const QVector3D &line1P1, const QVector3D &line1P2, const QVector3D &line2P1, const QVector3D &line2P2, QVector3D &intersectP);

		/*
		* ���������ַ������ͻ�ȡö������
		*/
		static MeshType transformTypeStrToEnum(const QString& typestr);
		static MeshType transformTypeStrToEnum(ElementType elementType);
		/*
		* ��ȡ�����ķ����б�
		*/
		static QStringList getVariableComponents(const QString& var);

		/*
		* ���ݱ�������ȡ������Ⱦ�����ͣ�0-�ڵ㣬1-��Ԫ
		*/
		static int getPostRendType(const QString& var);

		/*
		* ���ñ����ʽ
		*/
		static void setTableWidgetStyle(QTableWidget* tw, const QStringList& headers);

		/*
		* ��ȡ����е�XY����
		*/
		static void getTableWidgetDatas(QTableWidget* tw, QList<QPair<double, double>>& datas);

		/*
		* ���ñ������
		*/
		static void setTableWidgetDatas(QTableWidget* tw, QList<QPair<double, double>>& datas);

		/*
		* ��ձ��
		*/
		static void clearTableWidgetDatas(QTableWidget* tw);

		/*
		* ͨ���������λ�û�ȡ������λ��
		*/
		static void setWidgetPosByMouse(QWidget *parnetWidget, QWidget* popWidget);

		/*
		* ͨ��ģ�͵���ֵ�ͻ����ı���ȷ��ƽ��ľ����ĵ�
		*/
		static QVector3D caculatePlaneRange(float minx,float miny,float minz,float maxx, float maxy, float maxz, QVector3D normal,float ratio);

		/*
		* ͨ��ģ�͵���ֵ��ƽ�澭���ĵ�ȷ�������ı���
		*/
		static float caculatePlaneRatio(float minx, float miny, float minz, float maxx, float maxy, float maxz, QVector3D normal, QVector3D vertex);

		/*
		* ͨ��ģ�͵���ֵȷ��ģ�͵����ĺ����뾶
		*/
		static void caculateModelCenterAndRaduis(float minx, float miny, float minz, float maxx, float maxy, float maxz, QVector3D &center, float &radius);

		/*
		* ��ȡһ����Ԫ�����
		*/
		static double getMeshVolume(QVector<QVector3D> vertexs, MeshType meshType);

		/*
		* �����Ӣ��ת����
		*/
		static QString transformOutputEToC(QString str);

		/*
		* ���������תӢ��
		*/
		static QString transformOutputCToE(QString str);

		/*
		* �ı������Ӣ��ת����
		*/
		static QString transformTextOutputEToC(QString str);

		/*
		* �ı����������תӢ��
		*/
		static QString transformTextOutputCToE(QString str);

		/*
		* ����������ö��ת��Ϊ�ַ���
		*/
		static QString transformAnalysisEnumToString(AnalysisModule am);

		/*
		* ʹ��������ʽ��ȡ�����е�����
		*/
		static QString getParenthesesContent(const QString& text);

		/*
		* ���Ʊ�����ݵ����а�
		*/
		static void copyTableToBoard(QTableWidget* tw);

		/*
		* �����а�����ճ���������
		* ����isone��ʾ�Ƿ����Ʊ���Ϊһ�У�����ģ����Ҫ��
		*/
		static void pasteBoardToTable(QTableWidget* tw, bool isone);

	private:
		//��Ԫ���ͣ��ַ�����ö�ٻ�ת
		static QStringList _elementStringList;

		//������ַ���Ӣ��ת����
		static QHash<QString,QString> _outputStringList;

		//������ַ�������תӢ��
		static QHash<QString, QString> _outputStringList1;

		//�ı�������ַ���Ӣ��ת����
		static QHash<QString, QString> _textOutputStringList;

		//�ı�������ַ�������תӢ��
		static QHash<QString, QString> _textOutputStringList1;
	};
}