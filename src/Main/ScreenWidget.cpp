#include "ScreenWidget.h"
#include <QMutex>
#include <QApplication>
#include <QPainter>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QEvent>
#include <QDateTime>
#include <QStringList>
#include <QMouseEvent>
#include <QTimer>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

#include "gif.h"

#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QScreen>
#endif

#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))

Screen::Screen(QSize size)
{
	maxWidth = size.width();
	maxHeight = size.height();

	startPos = QPoint(-1, -1);
	endPos = startPos;
	leftUpPos = startPos;
	rightDownPos = startPos;
	status = SELECT;
}

int Screen::width()
{
	return maxWidth;
}

int Screen::height()
{
	return maxHeight;
}

Screen::STATUS Screen::getStatus()
{
	return status;
}

void Screen::setStatus(STATUS status)
{
	this->status = status;
}

void Screen::setEnd(QPoint pos)
{
	endPos = pos;
	leftUpPos = startPos;
	rightDownPos = endPos;
	cmpPoint(leftUpPos, rightDownPos);
}

void Screen::setStart(QPoint pos)
{
	startPos = pos;
}

QPoint Screen::getEnd()
{
	return endPos;
}

QPoint Screen::getStart()
{
	return startPos;
}

QPoint Screen::getLeftUp()
{
	return leftUpPos;
}

QPoint Screen::getRightDown()
{
	return rightDownPos;
}

bool Screen::isInArea(QPoint pos)
{
	if (pos.x() > leftUpPos.x() && pos.x() < rightDownPos.x() && pos.y() > leftUpPos.y() && pos.y() < rightDownPos.y()) {
		return true;
	}

	return false;
}

void Screen::move(QPoint p)
{
	int lx = leftUpPos.x() + p.x();
	int ly = leftUpPos.y() + p.y();
	int rx = rightDownPos.x() + p.x();
	int ry = rightDownPos.y() + p.y();

	if (lx < 0) {
		lx = 0;
		rx -= p.x();
	}

	if (ly < 0) {
		ly = 0;
		ry -= p.y();
	}

	if (rx > maxWidth) {
		rx = maxWidth;
		lx -= p.x();
	}

	if (ry > maxHeight) {
		ry = maxHeight;
		ly -= p.y();
	}

	leftUpPos = QPoint(lx, ly);
	rightDownPos = QPoint(rx, ry);
	startPos = leftUpPos;
	endPos = rightDownPos;
}

void Screen::cmpPoint(QPoint &leftTop, QPoint &rightDown)
{
	QPoint l = leftTop;
	QPoint r = rightDown;

	if (l.x() <= r.x()) {
		if (l.y() <= r.y()) {
			;
		}
		else {
			leftTop.setY(r.y());
			rightDown.setY(l.y());
		}
	}
	else {
		if (l.y() < r.y()) {
			leftTop.setX(r.x());
			rightDown.setX(l.x());
		}
		else {
			QPoint tmp;
			tmp = leftTop;
			leftTop = rightDown;
			rightDown = tmp;
		}
	}
}

ScreenWidgetBase::ScreenWidgetBase(QWidget *parent) :QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::X11BypassWindowManagerHint);//�����ޱ߿򣬴����ö�����������������
	//ȡ����Ļ��С
	screen = new Screen(QApplication::desktop()->size());
	//����ȫ��ͼ��
	fullScreen = new QPixmap();
}

void ScreenWidgetBase::startScreen(ScreenType screenType, QRect rect)
{
	QScreen *pscreen = QApplication::primaryScreen();
	*fullScreen = pscreen->grabWindow(QApplication::desktop()->winId(), 0, 0, screen->width(), screen->height());
	_screenType = screenType;
	this->showFullScreen();
	this->setGeometry(0, 0, screen->width(), screen->height());
	switch (screenType)
	{
	case QuadScreen:	
		break;
	case FullScreen:
		screen->setStart(QPoint(0,0));
		screen->setEnd(QPoint(screen->width(), screen->height()));
		break;
	case OneScreen:
	{
		QPoint cursorPos = QCursor::pos(); // ��ȡ��ǰ���λ��
		QScreen *pscreen = QGuiApplication::screenAt(cursorPos); // ��ȡ������λ�õ���Ļ
		int screenNumber = QGuiApplication::screens().indexOf(pscreen); // ��ȡ����Ļ����Ļ�б��е�����
		QRect screenGeometry = QGuiApplication::screens().at(screenNumber)->geometry(); // ��ȡ��Ļ�ļ��β��������ȡ��߶ȡ�λ�õ�
		*fullScreen = pscreen->grabWindow(QApplication::desktop()->winId(), screenGeometry.x(), screenGeometry.y(), screenGeometry.width(), screenGeometry.height());
		screen->setStart(QPoint(screenGeometry.x(), screenGeometry.y()));
		screen->setEnd(QPoint(screenGeometry.x() + screenGeometry.width(), screenGeometry.y() + screenGeometry.height()));
		//ScreenRecordWidget::Instance()->slot_startScreen();
		break;
	}
	case SoftScreen:
	{
		*fullScreen = pscreen->grabWindow(QApplication::desktop()->winId(), rect.x(), rect.y(), rect.width(), rect.height());
		screen->setStart(QPoint(rect.x(), rect.y()));
		screen->setEnd(QPoint(rect.x() + rect.width(), rect.y() + rect.height()));
		break;

	}
	default:
		break;
	}

}

void ScreenWidgetBase::mouseMoveEvent(QMouseEvent *e)
{
	if (screen->getStatus() == Screen::SELECT) {
		screen->setEnd(e->pos());
	}
	else if (screen->getStatus() == Screen::MOV) {
		QPoint p(e->x() - movPos.x(), e->y() - movPos.y());
		screen->move(p);
		movPos = e->pos();
	}

	this->update();
}

void ScreenWidgetBase::mousePressEvent(QMouseEvent *e)
{
	int status = screen->getStatus();

	if (status == Screen::SELECT) {
		screen->setStart(e->pos());
	}
	else if (status == Screen::MOV) {
		if (screen->isInArea(e->pos()) == false) {
			screen->setStart(e->pos());
			screen->setStatus(Screen::SELECT);
		}
		else {
			movPos = e->pos();
			this->setCursor(Qt::SizeAllCursor);
		}
	}

	this->update();
}

void ScreenWidgetBase::mouseReleaseEvent(QMouseEvent *)
{
	if (screen->getStatus() == Screen::SELECT) {
		screen->setStatus(Screen::MOV);
	}
	else if (screen->getStatus() == Screen::MOV) {
		this->setCursor(Qt::ArrowCursor);
	}
}

void ScreenWidgetBase::paintEvent(QPaintEvent *)
{
	int x = screen->getLeftUp().x();
	int y = screen->getLeftUp().y();
	int w = screen->getRightDown().x() - x;
	int h = screen->getRightDown().y() - y;

	QPainter painter(this);

	QPen pen;
	pen.setColor(Qt::green);
	pen.setWidth(2);
	pen.setStyle(Qt::DotLine);
	painter.setPen(pen);
	int width = bgScreen->width();
	int wid = fullScreen->width();
	painter.drawPixmap(0, 0, *bgScreen);//��ģ������

	if (w != 0 && h != 0) {
		painter.drawPixmap(x, y, fullScreen->copy(x, y, w, h));//��ͼ��
	}

	painter.drawRect(x, y, w, h);//���߿�

	pen.setColor(Qt::yellow);
	painter.setPen(pen);
	painter.drawText(x + 2, y - 8, tr("��ͼ��Χ��( %1 x %2 ) - ( %3 x %4 )  ͼƬ��С��( %5 x %6 )")//������
		.arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
}

void ScreenWidgetBase::showEvent(QShowEvent *e)
{
	QPoint point(-1, -1);
	screen->setStart(point);
	screen->setEnd(point);

	//����͸����ʵ��ģ������
	QPixmap pix(screen->width(), screen->height());
	pix.fill((QColor(160, 160, 160, 200)));
	bgScreen = new QPixmap(*fullScreen);
	QPainter p(bgScreen);
	p.drawPixmap(0, 0, pix);
	//qDebug() << "leftUP" << screen->getLeftUp();
	//qDebug() << "rightDown" << screen->getRightDown();
}

QScopedPointer<ScreenWidget> ScreenWidget::self;
ScreenWidget *ScreenWidget::Instance()
{
	if (self.isNull()) {
		static QMutex mutex;
		QMutexLocker locker(&mutex);
		if (self.isNull()) {
			self.reset(new ScreenWidget);
		}
	}

	return self.data();
}

ScreenWidget::ScreenWidget(QWidget *parent) : ScreenWidgetBase(parent)
{
	//this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);

	//menu = new QMenu(this);
	//menu->addAction("���浱ǰ��ͼ", this, SLOT(saveScreen()));
	//menu->addAction("����ȫ����ͼ", this, SLOT(saveFullScreen()));
	//menu->addAction("��ͼ���Ϊ", this, SLOT(saveScreenOther()));
	//menu->addAction("ȫ�����Ϊ", this, SLOT(saveFullOther()));
	//menu->addAction("�˳���ͼ", this, SLOT(hide()));
	saveButton = new QPushButton("����", this);
	connect(saveButton, &QPushButton::clicked, this, &ScreenWidget::slot_saveButton);
	saveButton->setGeometry(-1, -1, saveButton->width(), saveButton->height());
	saveButton->hide();

	saveAsButton = new QPushButton("���Ϊ", this);
	connect(saveAsButton, &QPushButton::clicked, this, &ScreenWidget::slot_saveAsButton);
	saveAsButton->setGeometry(-1, -1, saveAsButton->width(), saveAsButton->height());
	saveAsButton->hide();

	cancelButton = new QPushButton("ȡ��", this);
	connect(cancelButton, &QPushButton::clicked, this, &ScreenWidget::slot_cancelButton);
	cancelButton->setGeometry(-1, -1, cancelButton->width(), cancelButton->height());
	cancelButton->hide();
}

void ScreenWidget::startScreen(ScreenType screenType, QRect rect)
{
	ScreenWidgetBase::startScreen(screenType, rect);
	switch (screenType)
	{
	case QuadScreen:
		break;
	case FullScreen:
	case OneScreen:
	case SoftScreen:
		saveScreen();
		break;
	default:
		break;
	}

}

void ScreenWidget::saveScreen()
{
	int x = screen->getLeftUp().x();
	int y = screen->getLeftUp().y();
	int w = screen->getRightDown().x() - x;
	int h = screen->getRightDown().y() - y;

	QString fileName = QString("%1/screen_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
	fullScreen->copy(x, y, w, h).save(fileName, "png");
	close();
}

void ScreenWidget::slot_saveButton()
{
	saveButton->hide();
	saveAsButton->hide();
	cancelButton->hide();
	saveScreen();
}

void ScreenWidget::slot_saveAsButton()
{
	saveButton->hide();
	saveAsButton->hide();
	cancelButton->hide();
	saveScreenOther();
}

void ScreenWidget::slot_cancelButton()
{
	saveButton->hide();
	cancelButton->hide();
	close();
}

void ScreenWidget::saveFullScreen()
{
	QString fileName = QString("%1/full_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
	fullScreen->save(fileName, "png");
	close();
}

void ScreenWidget::saveScreenOther()
{
	QString name = QString("%1.png").arg(STRDATETIME);
	QString fileName = QFileDialog::getSaveFileName(this, "����ͼƬ", name, "png Files (*.png)");
	if (!fileName.endsWith(".png")) {
		fileName += ".png";
	}

	if (fileName.length() > 0) {
		int x = screen->getLeftUp().x();
		int y = screen->getLeftUp().y();
		int w = screen->getRightDown().x() - x;
		int h = screen->getRightDown().y() - y;
		fullScreen->copy(x, y, w, h).save(fileName, "png");
		close();
	}
}

void ScreenWidget::saveFullOther()
{
	QString name = QString("%1.png").arg(STRDATETIME);
	QString fileName = QFileDialog::getSaveFileName(this, "����ͼƬ", name, "png Files (*.png)");
	if (!fileName.endsWith(".png")) {
		fileName += ".png";
	}

	if (fileName.length() > 0) {
		fullScreen->save(fileName, "png");
		close();
	}
}

void ScreenWidget::contextMenuEvent(QContextMenuEvent *)
{
	//this->setCursor(Qt::ArrowCursor);
	//menu->exec(cursor().pos());
}

void ScreenWidget::paintEvent(QPaintEvent * event)
{
	ScreenWidgetBase::paintEvent(event);
	int x = screen->getLeftUp().x();
	int y = screen->getRightDown().y();
	if (x == -1 && y == -1)
	{
		return;
	}
	if (saveButton->isHidden())
	{
		saveButton->show();
		saveAsButton->show();
		cancelButton->show();
	}
	int xbias = screen->getLeftUp().x() - 1;
	int ybias = screen->getRightDown().y();
	if ((screen->getRightDown().y() + saveButton->height()) > screen->height())
	{
		ybias = screen->height() - saveButton->height();
	}

	saveButton->setGeometry(xbias, ybias, saveButton->width(), saveButton->height());
	xbias += saveButton->width();
	saveAsButton->setGeometry(xbias, ybias, saveAsButton->width(), saveAsButton->height());
	xbias += saveAsButton->width();
	cancelButton->setGeometry(xbias, ybias, cancelButton->width(), cancelButton->height());
	xbias += saveButton->width();
}

GifWriter _gifWriter;

void Record::slot_start()
{
	_fileName = QString("%1/record_%2.gif").arg(qApp->applicationDirPath()).arg(STRDATETIME);
	int x = _screen->getLeftUp().x();
	int y = _screen->getLeftUp().y();
	int w = _screen->getRightDown().x() - x;
	int h = _screen->getRightDown().y() - y;
	bool bOk = GifBegin(&_gifWriter, _fileName.toLocal8Bit().data(), w, h, _gifDelay);
	if (!bOk)
	{
		return;
	}
	_timer = new QTimer;
	_timer->setInterval(10);
	connect(_timer, SIGNAL(timeout()), this, SLOT(slot_record()));
	_timer->start();
}

void Record::slot_stop()
{
	_timer->stop();
	bool OK = GifEnd(&_gifWriter);
	if (OK)
	{
		//qDebug() << "¼�����";
	}
}

void Record::slot_pause()
{
	_timer->stop();
	//qDebug() << "��ͣ";
}

void Record::slot_resume()
{
	_timer->start();
	//qDebug() << "����";
}

void Record::slot_cancel()
{
	_timer->stop();
	bool OK = GifEnd(&_gifWriter);
	QFile file(_fileName);
	file.remove();
	//qDebug() << "ȡ��";
}

void Record::slot_record()
{
	int x = _screen->getLeftUp().x();
	int y = _screen->getLeftUp().y();
	int w = _screen->getRightDown().x() - x;
	int h = _screen->getRightDown().y() - y;
	QPixmap pix = QApplication::primaryScreen()->grabWindow(0, x, y, w, h);
	//QPixmap pix = fullScreen->copy(x, y, w, h);
	QImage image = pix.toImage().convertToFormat(QImage::Format_RGBA8888);
	GifWriteFrame(&_gifWriter, image.bits(), w, h, _gifDelay);
}

RecordWidget::RecordWidget(Screen *screen)
{
	record = new Record(screen);
	record->moveToThread(&thread);
	thread.start();
	_lastTime = 0.0;
	connect(this, &RecordWidget::startsig, record, &Record::slot_start, Qt::AutoConnection);
	connect(this, &RecordWidget::stopsig, record, &Record::slot_stop, Qt::BlockingQueuedConnection);//�ܹ�����ֹͣ
	connect(this, &RecordWidget::puasesig, record, &Record::slot_pause, Qt::AutoConnection);
	connect(this, &RecordWidget::resumesig, record, &Record::slot_resume, Qt::AutoConnection);
	connect(this, &RecordWidget::cancelsig, record, &Record::slot_cancel, Qt::BlockingQueuedConnection);

	x = screen->getLeftUp().x() - 1;
	y = screen->getLeftUp().y() - 1;
	w = screen->getRightDown().x() - x + 1;
	h = screen->getRightDown().y() - y + 1;

	//����˳���ܸ�
	setAttribute(Qt::WA_TranslucentBackground, true);	// ���ô���͸��
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::X11BypassWindowManagerHint);//�����ޱ߿򣬴����ö�����������������
	setAttribute(Qt::WA_TransparentForMouseEvents, true);	// ������괩͸

	stopButton = new QPushButton("����", this);
	connect(stopButton, &QPushButton::clicked, this, &RecordWidget::stop);
	pauseButton = new QPushButton("��ͣ", this);
	connect(pauseButton, &QPushButton::clicked, this, &RecordWidget::puaseOrReusme);
	cancelButton = new QPushButton("ȡ��", this);
	connect(cancelButton, &QPushButton::clicked, this, &RecordWidget::cancel);
	//��ť����
	int xbias = screen->getLeftUp().x() - 1;
	int ybias = screen->getRightDown().y();
	if ((screen->getRightDown().y() + stopButton->height()) > screen->height())
	{
		ybias = screen->height() - stopButton->height();
	}
	stopButton->setGeometry(xbias, ybias, stopButton->width(), stopButton->height());
	stopButton->show();
	xbias += stopButton->width();
	pauseButton->setGeometry(xbias, ybias, pauseButton->width(), pauseButton->height());
	pauseButton->show();
	xbias += pauseButton->width();
	cancelButton->setGeometry(xbias, ybias, cancelButton->width(), cancelButton->height());
	cancelButton->show();
	xbias += cancelButton->width();

	//qDebug() << stopButton->pos() << pauseButton->pos() << cancelButton->pos();


	showFullScreen();//��Ҫ������λ��֮ǰ����ȫ��
	setGeometry(0, 0, screen->width(), screen->height());
}

RecordWidget::~RecordWidget()
{
	delete record;
	record = nullptr;
	if (thread.isRunning())
	{
		thread.exit(0);
		thread.wait(10);
	}
}

void RecordWidget::start()
{
	emit startsig();
	time.start();//��ʱ��

	t = new QTimer;//��ʱ��
	/* ��ʱ0.1s */
	t->start(100);
	QObject::connect(t, &QTimer::timeout, [this]() {
		//this->raise();
		this->updateTime();
		//this->activateWindow();
	});
}

void RecordWidget::puaseOrReusme()
{
	if (pauseButton->text() == "��ͣ")//��ͣ
	{
		emit puasesig();
		pauseButton->setText("����");
		//qDebug() << "��ͣ";
		//��ͣ��ʱ
		_lastTime = _elaspedTime;
		t->stop();
	}
	else
	{
		emit resumesig();
		pauseButton->setText("��ͣ");
		//qDebug() << "����";
		//������ʱ
		time.start();
		t->start();
	}
}

void RecordWidget::stop()
{
	if (t)
	{
		delete t;
		t = nullptr;
		emit stopsig();
		//qDebug() << "����";
	}
}

void RecordWidget::cancel()
{
	if (t)
	{
		delete t;
		t = nullptr;
		emit cancelsig();
		//qDebug() << "ȡ��";
	}
}

void RecordWidget::updateTime()
{
	this->update();

	_elaspedTime = _lastTime + time.elapsed();
}

void RecordWidget::paintEvent(QPaintEvent *e)
{
	// ���ƾ���
	QPainter painter(this);
	painter.setPen(Qt::red);
	painter.drawRect(x, y, w, h);

	painter.setPen(Qt::yellow);
	painter.drawText(x + 2, y - 8, tr("¼����Χ��( %1 x %2 ) - ( %3 x %4 )  ͼƬ��С��( %5 x %6 ) ʱ�䣺��%7s��")//������
		.arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h).arg(_elaspedTime / 1000.0));
	//qDebug() << x, y, w, h;
}

QScopedPointer<ScreenRecordWidget> ScreenRecordWidget::self;
ScreenRecordWidget *ScreenRecordWidget::Instance()
{
	if (self.isNull()) {
		static QMutex mutex;
		QMutexLocker locker(&mutex);
		if (self.isNull()) {
			self.reset(new ScreenRecordWidget);
		}
	}

	return self.data();
}

ScreenRecordWidget::ScreenRecordWidget(QWidget *parent) : ScreenWidgetBase(parent)
{
	startbutton = new QPushButton("��ʼ", this);
	connect(startbutton, &QPushButton::clicked, this, &ScreenRecordWidget::slot_startScreen);
	startbutton->setGeometry(-1, -1, startbutton->width(), startbutton->height());
	startbutton->hide();
}

void ScreenRecordWidget::startScreen(ScreenType screenType, QRect rect)
{
	ScreenWidgetBase::startScreen(screenType, rect);
	switch (screenType)
	{
	case QuadScreen:
		//saveScreen();
		break;
	case FullScreen:
	case OneScreen:
	case SoftScreen:
		ScreenRecordWidget::Instance()->slot_startScreen();
		break;
	default:
		break;
	}

}

ScreenRecordWidget::~ScreenRecordWidget()
{

}

void ScreenRecordWidget::paintEvent(QPaintEvent * e)
{
	ScreenWidgetBase::paintEvent(e);
	int x = screen->getLeftUp().x();
	int y = screen->getRightDown().y();
	if (x == -1 && y == -1)
	{
		return;
	}
	if (startbutton->isHidden())
	{
		startbutton->show();
	}
	int xbias = screen->getLeftUp().x() - 1;
	if ((screen->getRightDown().y() + startbutton->height()) > screen->height())
	{
		startbutton->setGeometry(xbias, screen->height() - startbutton->height(), startbutton->width(), startbutton->height());
		xbias += startbutton->width();
	}
	else
	{
		startbutton->setGeometry(xbias, screen->getRightDown().y(), startbutton->width(), startbutton->height());
		xbias += startbutton->width();
	}
}

void ScreenRecordWidget::slot_startScreen()
{
	isStart = true;
	widget = new RecordWidget(screen);
	widget->start();
	connect(widget, &RecordWidget::stopsig, this, &ScreenRecordWidget::slot_stopScreen);//���ӽ���¼���ź�
	connect(widget, &RecordWidget::cancelsig, this, &ScreenRecordWidget::slot_cancelScreen);//���ӽ���¼���ź�
	hide();
}

void ScreenRecordWidget::slot_stopScreen()
{
	//�ж��Ƿ�ʼ��
	if (isStart)
	{
		isStart = !isStart;
		close();
		startbutton->hide();
		widget->stop();
		widget->close();
		delete widget;
	}
}

void ScreenRecordWidget::slot_cancelScreen()
{
	//�ж��Ƿ�ʼ��
	if (isStart)
	{
		isStart = !isStart;
		close();
		startbutton->hide();
		widget->cancel();
		widget->close();
		delete widget;
	}
}

void ScreenRecordWidget::keyPressEvent(QKeyEvent * event)
{
	switch (event->key())
	{
	case Qt::Key_Enter:
		this->slot_startScreen();
	}
}
