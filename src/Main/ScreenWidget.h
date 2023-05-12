#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H
//�����������
#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QMenu>
#include <QPoint>
#include <QSize>
#include <QThread>
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QPushButton>


enum ScreenType
{
	QuadScreen,

	FullScreen,

	OneScreen,

	SoftScreen,

};
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

class ScreenWidgetBase :public QWidget
{
	Q_OBJECT
public:
	explicit ScreenWidgetBase(QWidget *parent = 0);

	virtual void startScreen(ScreenType screenType, QRect rect = QRect());

protected:
	Screen *screen;         //��������
	QPixmap *fullScreen;    //����ȫ��ͼ��
	QPixmap *bgScreen;      //ģ������ͼ
	QPoint movPos;          //����

	ScreenType _screenType;//��������

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);
	void showEvent(QShowEvent *);

};

//����������
class ScreenWidget : public ScreenWidgetBase
{
	Q_OBJECT
public:
	static ScreenWidget *Instance();
	explicit ScreenWidget(QWidget *parent = 0);

	void startScreen(ScreenType screenType, QRect rect = QRect()) override;

protected:
	static QScopedPointer<ScreenWidget> self;
	QMenu *menu;            //�Ҽ��˵�����
	QPushButton *saveButton;//����
	QPushButton *saveAsButton;//���Ϊ
	QPushButton *cancelButton;//ȡ��

protected:
	void contextMenuEvent(QContextMenuEvent *);

	void paintEvent(QPaintEvent *event) override;

public slots:

	void slot_saveButton();

	void slot_saveAsButton();

	void slot_cancelButton();


private slots:
	void saveScreen();
	void saveFullScreen();
	void saveScreenOther();
	void saveFullOther();
};
//¼����
class Record :public QObject
{
	Q_OBJECT
public:
	Record(Screen *screen) { _screen = screen; };

public slots:
	void slot_start();

	void slot_stop();

	void slot_pause();

	void slot_resume();

	void slot_cancel();

private slots:
	//¼��
	void slot_record();



private:
	QString _fileName;
	Screen *_screen;
	//¼����ʱ��
	QTimer *_timer;
	//gif�ӳ�
	int _gifDelay = 10;
};
//¼��������
class RecordWidget : public QWidget
{
	Q_OBJECT
public:
	RecordWidget(Screen *screen);

	~RecordWidget();
public slots:
	void start();

	void puaseOrReusme();//��ͣ�����

	void stop();

	void cancel();

	void updateTime();

signals:
	void startsig();
	void stopsig();
	void puasesig();
	void resumesig();
	void cancelsig();

protected:
	void paintEvent(QPaintEvent *event) override;

	QTime time;//ʱ��
	QTimer *t;
	QPushButton *stopButton;//����
	QPushButton *pauseButton;//��ͣ����ʼ��
	QPushButton *cancelButton;//ȡ��
	Record *record; 
	QThread thread;
	float _lastTime;
	float _elaspedTime;
	int x, y, w, h;
};
//¼����ȡ���ο򴰿���
class ScreenRecordWidget : public ScreenWidgetBase
{
	Q_OBJECT
public:
	static ScreenRecordWidget *Instance();
	explicit ScreenRecordWidget(QWidget *parent = 0);

	void startScreen(ScreenType screenType, QRect rect = QRect()) override;

	~ScreenRecordWidget();

protected:

	void paintEvent(QPaintEvent *e);

public slots:
	//��ʼ¼��
	void slot_startScreen();
	//����¼��
	void slot_stopScreen();
	//ȡ��¼��
	void slot_cancelScreen();

signals:

	void startsig();

	void stopsig();

private:
	static QScopedPointer<ScreenRecordWidget> self;
	RecordWidget *widget;
	QPushButton *startbutton;
	bool isStart = false;

protected:
	void keyPressEvent(QKeyEvent *event) override;
};
#endif // SCREENWIDGET_H