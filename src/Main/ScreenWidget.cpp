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

HitRegion Screen::hitTest(const QPoint& pos)
{
	if (leftUpPos.x() < 0 || rightDownPos.x() < 0)
		return None;

	int x = pos.x();
	int y = pos.y();

	int l = leftUpPos.x();
	int r = rightDownPos.x();
	int t = leftUpPos.y();
	int b = rightDownPos.y();

	bool nearLeft = qAbs(x - l) <= RESIZE_MARGIN;
	bool nearRight = qAbs(x - r) <= RESIZE_MARGIN;
	bool nearTop = qAbs(y - t) <= RESIZE_MARGIN;
	bool nearBottom = qAbs(y - b) <= RESIZE_MARGIN;

	if (nearLeft && nearTop)     return TopLeft;
	if (nearRight && nearTop)    return TopRight;
	if (nearLeft && nearBottom)  return BottomLeft;
	if (nearRight && nearBottom) return BottomRight;

	if (nearLeft)   return Left;
	if (nearRight)  return Right;
	if (nearTop)    return Top;
	if (nearBottom) return Bottom;

	if (isInArea(pos))
		return Inside;

	return None;
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

void Screen::resizeLeft(int dx)
{
	int newX = leftUpPos.x() + dx;
	newX = qBound(0, newX, rightDownPos.x() - MIN_RECT_SIZE);

	leftUpPos.setX(newX);
	startPos = leftUpPos;
}


void Screen::resizeRight(int dx)
{
	int newX = rightDownPos.x() + dx;
	newX = qBound(leftUpPos.x() + MIN_RECT_SIZE, newX, maxWidth);

	rightDownPos.setX(newX);
	endPos = rightDownPos;
}


void Screen::resizeTop(int dy)
{
	int newY = leftUpPos.y() + dy;
	newY = qBound(0, newY, rightDownPos.y() - MIN_RECT_SIZE);

	leftUpPos.setY(newY);
	startPos = leftUpPos;
}


void Screen::resizeBottom(int dy)
{
	int newY = rightDownPos.y() + dy;
	newY = qBound(leftUpPos.y() + MIN_RECT_SIZE, newY, maxHeight);

	rightDownPos.setY(newY);
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
	setMouseTracking(true);
	setAttribute(Qt::WA_MouseTracking);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::X11BypassWindowManagerHint);//窗口无边框，窗口置顶，窗口任务栏隐藏
	//取得屏幕大小
	screen = new Screen(QApplication::desktop()->size());
	//保存全屏图像
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
		QPoint cursorPos = QCursor::pos(); // 获取当前鼠标位置
		QScreen *pscreen = QGuiApplication::screenAt(cursorPos); // 获取包含该位置的屏幕
		int screenNumber = QGuiApplication::screens().indexOf(pscreen); // 获取该屏幕在屏幕列表中的索引
		QRect screenGeometry = QGuiApplication::screens().at(screenNumber)->geometry(); // 获取屏幕的几何参数，如宽度、高度、位置等
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

void ScreenWidgetBase::updateCursor(HitRegion region)
{
	switch (region)
	{
	case Inside:
		setCursor(Qt::SizeAllCursor);
		break;
	case Left:
	case Right:
		setCursor(Qt::SizeHorCursor);
		break;
	case Top:
	case Bottom:
		setCursor(Qt::SizeVerCursor);
		break;
	case TopLeft:
	case BottomRight:
		setCursor(Qt::SizeFDiagCursor);
		break;
	case TopRight:
	case BottomLeft:
		setCursor(Qt::SizeBDiagCursor);
		break;
	default:
		setCursor(Qt::CrossCursor); // 新建选择
		break;
	}
}

void ScreenWidgetBase::mouseMoveEvent(QMouseEvent *e)
{
	QPoint pos = e->pos();

	if (!(e->buttons() & Qt::LeftButton))
	{
		HitRegion region = screen->hitTest(pos);
		updateCursor(region);
		return;
	}

	QPoint delta = e->pos() - _pressPos;
	_pressPos = e->pos();

	switch (_activeRegion)
	{
	case Inside:
		screen->move(delta);
		break;

	case Left:
		screen->resizeLeft(delta.x());
		break;

	case Right:
		screen->resizeRight(delta.x());
		break;

	case Top:
		screen->resizeTop(delta.y());
		break;

	case Bottom:
		screen->resizeBottom(delta.y());
		break;

	case TopLeft:
		screen->resizeLeft(delta.x());
		screen->resizeTop(delta.y());
		break;

	case TopRight:
		screen->resizeRight(delta.x());
		screen->resizeTop(delta.y());
		break;

	case BottomLeft:
		screen->resizeLeft(delta.x());
		screen->resizeBottom(delta.y());
		break;

	case BottomRight:
		screen->resizeRight(delta.x());
		screen->resizeBottom(delta.y());
		break;

	default:
		screen->setEnd(e->pos());
		break;
	}

	update();

	this->update();
}

void ScreenWidgetBase::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton)
		return;

	_pressPos = e->pos();
	_activeRegion = screen->hitTest(_pressPos);

	if (_activeRegion == None)
	{
		// 重新选择
		screen->setStart(_pressPos);
		screen->setEnd(_pressPos);
	}
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
	painter.setRenderHint(QPainter::Antialiasing);

	painter.drawPixmap(0, 0, *bgScreen);

	if (w > 0 && h > 0)
		painter.drawPixmap(x, y, fullScreen->copy(x, y, w, h));

	// ===== 绘制选择框 =====
	QPen pen(Qt::green);
	pen.setWidth(2);
	pen.setStyle(Qt::DotLine);
	painter.setPen(pen);
	painter.drawRect(x, y, w, h);

	// ===== 文字内容 =====
	QString text = tr("截图范围：( %1, %2 ) - ( %3, %4 )   大小：( %5 x %6 )")
		.arg(x).arg(y)
		.arg(x + w).arg(y + h)
		.arg(w).arg(h);

	QFontMetrics fm(painter.font());
	QRect textRect = fm.boundingRect(text);
	const int padding = 6;

	textRect.adjust(-padding, -padding, padding, padding);

	// ===== 文字位置策略 =====
	QPoint textPos;

	// 优先放在矩形上方
	if (y - textRect.height() - 4 >= 0)
	{
		textPos = QPoint(x, y - textRect.height() - 4);
	}
	else
	{
		textPos = QPoint(x + 4, y + 4);
	}

	textRect.moveTopLeft(textPos);
	if (w != 0 && h != 0)
	{
		// ===== 背景条（关键）=====
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(0, 0, 0, 160));
		painter.drawRoundedRect(textRect, 4, 4);

		// ===== 文字 =====
		painter.setPen(Qt::yellow);
		painter.drawText(textRect.adjusted(padding, padding, -padding, -padding),
			Qt::AlignLeft | Qt::AlignVCenter,
			text);
	}
}


void ScreenWidgetBase::showEvent(QShowEvent *e)
{
	QPoint point(-1, -1);
	screen->setStart(point);
	screen->setEnd(point);

	//设置透明度实现模糊背景
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
	//menu->addAction("保存当前截图", this, SLOT(saveScreen()));
	//menu->addAction("保存全屏截图", this, SLOT(saveFullScreen()));
	//menu->addAction("截图另存为", this, SLOT(saveScreenOther()));
	//menu->addAction("全屏另存为", this, SLOT(saveFullOther()));
	//menu->addAction("退出截图", this, SLOT(hide()));
	saveButton = new QPushButton("保存", this);
	connect(saveButton, &QPushButton::clicked, this, &ScreenWidget::slot_saveButton);
	saveButton->setGeometry(-1, -1, saveButton->width(), saveButton->height());
	saveButton->hide();

	saveAsButton = new QPushButton("另存为", this);
	connect(saveAsButton, &QPushButton::clicked, this, &ScreenWidget::slot_saveAsButton);
	saveAsButton->setGeometry(-1, -1, saveAsButton->width(), saveAsButton->height());
	saveAsButton->hide();

	cancelButton = new QPushButton("取消", this);
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
	QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
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
	QString fileName = QFileDialog::getSaveFileName(this, "保存图片", name, "png Files (*.png)");
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
		//qDebug() << "录制完成";
	}
}

void Record::slot_pause()
{
	_timer->stop();
	//qDebug() << "暂停";
}

void Record::slot_resume()
{
	_timer->start();
	//qDebug() << "继续";
}

void Record::slot_cancel()
{
	_timer->stop();
	bool OK = GifEnd(&_gifWriter);
	QFile file(_fileName);
	file.remove();
	//qDebug() << "取消";
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
	connect(this, &RecordWidget::stopsig, record, &Record::slot_stop, Qt::BlockingQueuedConnection);//能够立即停止
	connect(this, &RecordWidget::puasesig, record, &Record::slot_pause, Qt::AutoConnection);
	connect(this, &RecordWidget::resumesig, record, &Record::slot_resume, Qt::AutoConnection);
	connect(this, &RecordWidget::cancelsig, record, &Record::slot_cancel, Qt::BlockingQueuedConnection);

	x = screen->getLeftUp().x() - 1;
	y = screen->getLeftUp().y() - 1;
	w = screen->getRightDown().x() - x + 1;
	h = screen->getRightDown().y() - y + 1;

	//设置顺序不能改
	setAttribute(Qt::WA_TranslucentBackground, true);	// 设置窗体透明
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::X11BypassWindowManagerHint);//窗口无边框，窗口置顶，窗口任务栏隐藏
	setAttribute(Qt::WA_TransparentForMouseEvents, true);	// 设置鼠标穿透

	stopButton = new QPushButton("结束", this);
	connect(stopButton, &QPushButton::clicked, this, &RecordWidget::stop);
	pauseButton = new QPushButton("暂停", this);
	connect(pauseButton, &QPushButton::clicked, this, &RecordWidget::puaseOrReusme);
	cancelButton = new QPushButton("取消", this);
	connect(cancelButton, &QPushButton::clicked, this, &RecordWidget::cancel);
	//按钮设置
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


	showFullScreen();//需要在设置位置之前设置全屏
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
	time.start();//计时器

	t = new QTimer;//定时器
	/* 定时0.1s */
	t->start(100);
	QObject::connect(t, &QTimer::timeout, [this]() {
		//this->raise();
		this->updateTime();
		//this->activateWindow();
	});
}

void RecordWidget::puaseOrReusme()
{
	if (pauseButton->text() == "暂停")//暂停
	{
		emit puasesig();
		pauseButton->setText("继续");
		//qDebug() << "暂停";
		//暂停计时
		_lastTime = _elaspedTime;
		t->stop();
	}
	else
	{
		emit resumesig();
		pauseButton->setText("暂停");
		//qDebug() << "继续";
		//继续计时
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
		//qDebug() << "结束";
	}
}

void RecordWidget::cancel()
{
	if (t)
	{
		delete t;
		t = nullptr;
		emit cancelsig();
		//qDebug() << "取消";
	}
}

void RecordWidget::updateTime()
{
	this->update();

	_elaspedTime = _lastTime + time.elapsed();
}

void RecordWidget::paintEvent(QPaintEvent *e)
{
	// 绘制矩形
	QPainter painter(this);
	painter.setPen(Qt::red);
	painter.drawRect(x, y, w, h);

	painter.setPen(Qt::yellow);
	painter.drawText(x + 2, y - 8, tr("录屏范围：( %1 x %2 ) - ( %3 x %4 )  图片大小：( %5 x %6 ) 时间：（%7s）")//画文字
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
	startbutton = new QPushButton("开始", this);
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
	connect(widget, &RecordWidget::stopsig, this, &ScreenRecordWidget::slot_stopScreen);//连接结束录制信号
	connect(widget, &RecordWidget::cancelsig, this, &ScreenRecordWidget::slot_cancelScreen);//连接结束录制信号
	hide();
}

void ScreenRecordWidget::slot_stopScreen()
{
	//判断是否开始了
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
	//判断是否开始了
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
