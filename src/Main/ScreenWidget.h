#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H
//�����������
#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QMenu>
#include <QPoint>
#include <QSize>
#include <QThread>

//����������
class Screen
{
public:
	enum STATUS { SELECT, MOV, SET_W_H };
	Screen() {}
	Screen(QSize size);

	void setStart(QPoint pos);
	void setEnd(QPoint pos);
	QPoint getStart();
	QPoint getEnd();

	QPoint getLeftUp();
	QPoint getRightDown();

	STATUS getStatus();
	void setStatus(STATUS status);

	int width();
	int height();
	bool isInArea(QPoint pos);          // ���pos�Ƿ��ڽ�ͼ������
	void move(QPoint p);                // �� p �ƶ���ͼ����

private:
	QPoint leftUpPos, rightDownPos;     //��¼ ��ͼ���� ���Ͻǡ����½�
	QPoint startPos, endPos;            //��¼ ��꿪ʼλ�á�����λ��
	int maxWidth, maxHeight;            //��¼��Ļ��С
	STATUS status;                      //����״̬: ѡ�������ƶ���������width height

	void cmpPoint(QPoint &s, QPoint &e);//�Ƚ���λ�ã��ж����Ͻǡ����½�
};

//����������
class ScreenWidget : public QWidget
{
	Q_OBJECT
public:
	static ScreenWidget *Instance();
	explicit ScreenWidget(QWidget *parent = 0);

	void startScreen();

private:
	static QScopedPointer<ScreenWidget> self;
	QMenu *menu;            //�Ҽ��˵�����
	Screen *screen;         //��������
	QPixmap *fullScreen;    //����ȫ��ͼ��
	QPixmap *bgScreen;      //ģ������ͼ
	QPoint movPos;          //����

protected:
	void contextMenuEvent(QContextMenuEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);
	void showEvent(QShowEvent *);

private slots:
	void saveScreen();
	void saveFullScreen();
	void saveScreenOther();
	void saveFullOther();
};
class Record :public QObject
{
	Q_OBJECT
public:
	Record(Screen *screen) { _screen = screen; };

	void slot_startScreen();

	void slot_stopScreen();

private slots:
	//¼��
	void slot_record();



private:
	Screen *_screen;
	//¼����ʱ��
	QTimer *_timer;
	//gif�ӳ�
	int _gifDelay = 10;
};
//¼��������
class ScreenRecordWidget : public QWidget
{
	Q_OBJECT
public:
	static ScreenRecordWidget *Instance();
	explicit ScreenRecordWidget(QWidget *parent = 0);

	~ScreenRecordWidget();

	void startScreen();
	void stopScreen();

signals:

	void startsig();

	void stopsig();

private:
	static QScopedPointer<ScreenRecordWidget> self;
	//QMenu *menu;            //�Ҽ��˵�����
	Screen *screen;         //��������
	QPixmap *fullScreen;    //����ȫ��ͼ��
	QPixmap *bgScreen;      //ģ������ͼ
	QPoint movPos;          //����
	Record *record; QThread thread;
	bool isStart = false;

protected:
	void keyPressEvent(QKeyEvent *event) override;
	void contextMenuEvent(QContextMenuEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);
	void showEvent(QShowEvent *);

private slots:
	void saveScreen();
	void saveFullScreen();
	void saveScreenOther();
	void saveFullOther();
};
#endif // SCREENWIDGET_H