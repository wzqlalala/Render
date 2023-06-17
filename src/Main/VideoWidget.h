#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#ifdef __cplusplus
}
#endif


QT_BEGIN_NAMESPACE
namespace Ui { class VideoWidget; }
QT_END_NAMESPACE

class VideoWidget : public QWidget
{
	Q_OBJECT

public:
	VideoWidget(QWidget *parent = nullptr);
	~VideoWidget();
	void Delay(int delay);

private slots:
	void OpenFile();
	void Play();
private:
	Ui::VideoWidget *ui;
	QString filePath_;
};
#endif // WIDGET_H