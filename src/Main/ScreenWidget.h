#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H
//解决中文乱码
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
//截屏对象类
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
	bool isInArea(QPoint pos);          // 检测pos是否在截图区域内
	void move(QPoint p);                // 按 p 移动截图区域

private:
	QPoint leftUpPos, rightDownPos;     //记录 截图区域 左上角、右下角
	QPoint startPos, endPos;            //记录 鼠标开始位置、结束位置
	int maxWidth, maxHeight;            //记录屏幕大小
	STATUS status;                      //三个状态: 选择区域、移动区域、设置width height

	void cmpPoint(QPoint &s, QPoint &e);//比较两位置，判断左上角、右下角
};

class ScreenWidgetBase :public QWidget
{
	Q_OBJECT
public:
	explicit ScreenWidgetBase(QWidget *parent = 0);

	virtual void startScreen(ScreenType screenType, QRect rect = QRect());

protected:
	Screen *screen;         //截屏对象
	QPixmap *fullScreen;    //保存全屏图像
	QPixmap *bgScreen;      //模糊背景图
	QPoint movPos;          //坐标

	ScreenType _screenType;//截屏类型

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);
	void showEvent(QShowEvent *);

};

//截屏窗口类
class ScreenWidget : public ScreenWidgetBase
{
	Q_OBJECT
public:
	static ScreenWidget *Instance();
	explicit ScreenWidget(QWidget *parent = 0);

	void startScreen(ScreenType screenType, QRect rect = QRect()) override;

protected:
	static QScopedPointer<ScreenWidget> self;
	QMenu *menu;            //右键菜单对象
	QPushButton *saveButton;//保存
	QPushButton *saveAsButton;//另存为
	QPushButton *cancelButton;//取消

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
//录屏类
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
	//录屏
	void slot_record();



private:
	QString _fileName;
	Screen *_screen;
	//录屏计时器
	QTimer *_timer;
	//gif延迟
	int _gifDelay = 10;
};
//录屏窗口类
class RecordWidget : public QWidget
{
	Q_OBJECT
public:
	RecordWidget(Screen *screen);

	~RecordWidget();
public slots:
	void start();

	void puaseOrReusme();//暂停或继续

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

	QTime time;//时间
	QTimer *t;
	QPushButton *stopButton;//结束
	QPushButton *pauseButton;//暂停（开始）
	QPushButton *cancelButton;//取消
	Record *record; 
	QThread thread;
	float _lastTime;
	float _elaspedTime;
	int x, y, w, h;
};
//录屏获取矩形框窗口类
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
	//开始录屏
	void slot_startScreen();
	//结束录屏
	void slot_stopScreen();
	//取消录屏
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