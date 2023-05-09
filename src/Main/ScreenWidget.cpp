#include "screenwidget.h"
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

ScreenWidget::ScreenWidget(QWidget *parent) : QWidget(parent)
{
	//this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);

	menu = new QMenu(this);
	menu->addAction("±£´æµ±Ç°½ØÍ¼", this, SLOT(saveScreen()));
	menu->addAction("±£´æÈ«ÆÁ½ØÍ¼", this, SLOT(saveFullScreen()));
	menu->addAction("½ØÍ¼Áí´æÎª", this, SLOT(saveScreenOther()));
	menu->addAction("È«ÆÁÁí´æÎª", this, SLOT(saveFullOther()));
	menu->addAction("ÍË³ö½ØÍ¼", this, SLOT(hide()));

	//È¡µÃÆÁÄ»´óÐ¡
	screen = new Screen(QApplication::desktop()->size());
	//±£´æÈ«ÆÁÍ¼Ïñ
	fullScreen = new QPixmap();
}

void ScreenWidget::startScreen()
{
	this->showFullScreen();
	this->setGeometry(0, 0, screen->width(), screen->height());
}

void ScreenWidget::paintEvent(QPaintEvent *)
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
	painter.drawPixmap(0, 0, *bgScreen);//»­Ä£ºý±³¾°

	if (w != 0 && h != 0) {
		painter.drawPixmap(x, y, fullScreen->copy(x, y, w, h));//»­Í¼Ïñ
	}

	painter.drawRect(x, y, w, h);//»­±ß¿ò

	pen.setColor(Qt::yellow);
	painter.setPen(pen);
	painter.drawText(x + 2, y - 8, tr("½ØÍ¼·¶Î§£º( %1 x %2 ) - ( %3 x %4 )  Í¼Æ¬´óÐ¡£º( %5 x %6 )")//»­ÎÄ×Ö
		.arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
}

void ScreenWidget::showEvent(QShowEvent *e)
{
	//QApplication::screenAt()
	//QPoint point(pos());
	//screen->setStart(point);
	//screen->setEnd(point);
	QPoint point(-1, -1);
	screen->setStart(point);
	screen->setEnd(point);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
	*fullScreen = fullScreen->grabWindow(QApplication::desktop()->winId(), 0, 0, screen->width(), screen->height());
#else
	QScreen *pscreen = QApplication::primaryScreen();
	*fullScreen = pscreen->grabWindow(QApplication::desktop()->winId(), 0, 0, screen->width(), screen->height());
#endif

	//ÉèÖÃÍ¸Ã÷¶ÈÊµÏÖÄ£ºý±³¾°
	QPixmap pix(screen->width(), screen->height());
	pix.fill((QColor(160, 160, 160, 200)));
	bgScreen = new QPixmap(*fullScreen);
	QPainter p(bgScreen);
	p.drawPixmap(0, 0, pix);
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

void ScreenWidget::saveFullScreen()
{
	QString fileName = QString("%1/full_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
	fullScreen->save(fileName, "png");
	close();
}

void ScreenWidget::saveScreenOther()
{
	QString name = QString("%1.png").arg(STRDATETIME);
	QString fileName = QFileDialog::getSaveFileName(this, "±£´æÍ¼Æ¬", name, "png Files (*.png)");
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
	QString fileName = QFileDialog::getSaveFileName(this, "±£´æÍ¼Æ¬", name, "png Files (*.png)");
	if (!fileName.endsWith(".png")) {
		fileName += ".png";
	}

	if (fileName.length() > 0) {
		fullScreen->save(fileName, "png");
		close();
	}
}

void ScreenWidget::mouseMoveEvent(QMouseEvent *e)
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

void ScreenWidget::mousePressEvent(QMouseEvent *e)
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

void ScreenWidget::mouseReleaseEvent(QMouseEvent *)
{
	if (screen->getStatus() == Screen::SELECT) {
		screen->setStatus(Screen::MOV);
	}
	else if (screen->getStatus() == Screen::MOV) {
		this->setCursor(Qt::ArrowCursor);
	}
}

void ScreenWidget::contextMenuEvent(QContextMenuEvent *)
{
	this->setCursor(Qt::ArrowCursor);
	menu->exec(cursor().pos());
}

GifWriter _gifWriter;
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

ScreenRecordWidget::ScreenRecordWidget(QWidget *parent) : QWidget(parent)
{
	//È¡µÃÆÁÄ»´óÐ¡
	screen = new Screen(QApplication::desktop()->size());
	record = new Record(screen);
	record->moveToThread(&thread);
	thread.start();
	connect(this, &ScreenRecordWidget::startsig, record, &Record::slot_startScreen, Qt::AutoConnection);
	connect(this, &ScreenRecordWidget::stopsig, record, &Record::slot_stopScreen, Qt::AutoConnection);
	//±£´æÈ«ÆÁÍ¼Ïñ
	fullScreen = new QPixmap();
}

ScreenRecordWidget::~ScreenRecordWidget()
{
	thread.exit(0);
	thread.wait(1);
	delete record;
	record = nullptr;
}

void ScreenRecordWidget::startScreen()
{
	this->showFullScreen();
	this->setGeometry(0, 0, screen->width(), screen->height());
}

void ScreenRecordWidget::stopScreen()
{
	isStart = !isStart;
	emit stopsig();
	close();
}

void ScreenRecordWidget::paintEvent(QPaintEvent *)
{
	if (isStart)
	{
		return;
	}
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
	painter.drawPixmap(0, 0, *bgScreen);//»­Ä£ºý±³¾°
	if (w != 0 && h != 0) {
		painter.drawPixmap(x, y, fullScreen->copy(x, y, w, h));//»­Í¼Ïñ
	}

	painter.drawRect(x, y, w, h);//»­±ß¿ò

	pen.setColor(Qt::yellow);
	painter.setPen(pen);
	painter.drawText(x + 2, y - 8, tr("½ØÍ¼·¶Î§£º( %1 x %2 ) - ( %3 x %4 )  Í¼Æ¬´óÐ¡£º( %5 x %6 )")//»­ÎÄ×Ö
		.arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
}

void ScreenRecordWidget::showEvent(QShowEvent *e)
{
	//QApplication::screenAt()
	//QPoint point(pos());
	//screen->setStart(point);
	//screen->setEnd(point);
	QPoint point(-1, -1);
	screen->setStart(point);
	screen->setEnd(point);

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
	*fullScreen = fullScreen->grabWindow(QApplication::desktop()->winId(), 0, 0, screen->width(), screen->height());
#else
	QScreen *pscreen = QApplication::primaryScreen();
	*fullScreen = pscreen->grabWindow(QApplication::desktop()->winId(), 0, 0, screen->width(), screen->height());
#endif

	//ÉèÖÃÍ¸Ã÷¶ÈÊµÏÖÄ£ºý±³¾°
	QPixmap pix(screen->width(), screen->height());
	pix.fill((QColor(160, 160, 160, 200)));
	bgScreen = new QPixmap(*fullScreen);
	QPainter p(bgScreen);
	p.drawPixmap(0, 0, pix);
}

void ScreenRecordWidget::saveScreen()
{
	int x = screen->getLeftUp().x();
	int y = screen->getLeftUp().y();
	int w = screen->getRightDown().x() - x;
	int h = screen->getRightDown().y() - y;

	QString fileName = QString("%1/screen_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
	fullScreen->copy(x, y, w, h).save(fileName, "png");
	close();
}

void ScreenRecordWidget::saveFullScreen()
{
	QString fileName = QString("%1/full_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
	fullScreen->save(fileName, "png");
	close();
}

void ScreenRecordWidget::saveScreenOther()
{
	QString name = QString("%1.png").arg(STRDATETIME);
	QString fileName = QFileDialog::getSaveFileName(this, "±£´æÍ¼Æ¬", name, "png Files (*.png)");
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

void ScreenRecordWidget::saveFullOther()
{
	QString name = QString("%1.png").arg(STRDATETIME);
	QString fileName = QFileDialog::getSaveFileName(this, "±£´æÍ¼Æ¬", name, "png Files (*.png)");
	if (!fileName.endsWith(".png")) {
		fileName += ".png";
	}

	if (fileName.length() > 0) {
		fullScreen->save(fileName, "png");
		close();
	}
}

void ScreenRecordWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (isStart)
	{
		return;
	}
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

void ScreenRecordWidget::mousePressEvent(QMouseEvent *e)
{
	if (isStart)
	{
		return;
	}
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

void ScreenRecordWidget::mouseReleaseEvent(QMouseEvent *)
{
	if (isStart)
	{
		return;
	}
	if (screen->getStatus() == Screen::SELECT) {
		screen->setStatus(Screen::MOV);
	}
	else if (screen->getStatus() == Screen::MOV) {
		this->setCursor(Qt::ArrowCursor);
	}
}

void ScreenRecordWidget::keyPressEvent(QKeyEvent * event)
{
	switch (event->key())
	{
	case Qt::Key_Enter:
		if (!isStart)
		{	
			isStart = true;
			emit startsig();
			hide();
		}
		else
		{
			stopScreen();
		}

	}
}

void ScreenRecordWidget::contextMenuEvent(QContextMenuEvent *)
{
	//this->setCursor(Qt::ArrowCursor);
	//menu->exec(cursor().pos());
}

void Record::slot_startScreen()
{
	QString fileName = QString("%1/record_%2.gif").arg(qApp->applicationDirPath()).arg(STRDATETIME);
	int x = _screen->getLeftUp().x();
	int y = _screen->getLeftUp().y();
	int w = _screen->getRightDown().x() - x;
	int h = _screen->getRightDown().y() - y;
	bool bOk = GifBegin(&_gifWriter, fileName.toLocal8Bit().data(), w, h, _gifDelay);
	if (!bOk)
	{
		return;
	}
	_timer = new QTimer;
	_timer->setInterval(10);
	connect(_timer, SIGNAL(timeout()), this, SLOT(slot_record()));
	_timer->start();
}

void Record::slot_stopScreen()
{
	_timer->stop();
	bool OK = GifEnd(&_gifWriter);
	if (OK)
	{
		qDebug() << "Â¼ÖÆÍê³É";
	}
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
