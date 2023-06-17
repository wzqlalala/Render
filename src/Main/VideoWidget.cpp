#include "VideoWidget.h"
#include "ui_VideoWidget.h"
#include <QTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QImage>
#include <QDebug>
VideoWidget::VideoWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::VideoWidget)
{
	ui->setupUi(this);
	connect(ui->OpenFile, SIGNAL(clicked()), this, SLOT(OpenFile()));
	connect(ui->Play, SIGNAL(clicked()), this, SLOT(Play()));
}
VideoWidget::~VideoWidget()
{
	delete ui;
}
void VideoWidget::OpenFile()
{
	QString filter = tr("mp4 file(*.mp4);;rmvb file(*.rmvb);;avi file(*.avi)");
	filePath_ = QFileDialog::getOpenFileName(this, tr("打开文件"), QString(), filter);
}
void VideoWidget::Play()
{
	if (filePath_.isEmpty())
	{
		QMessageBox::warning(nullptr, tr("warn"), tr("未选择文件"), QMessageBox::Ok);
		return;
	}
	std::string input_file = filePath_.toStdString();
	//注册编解码器
	//av_register_all();

	AVFormatContext *pFormatCtx = avformat_alloc_context();
	if (avformat_open_input(&pFormatCtx, input_file.c_str(), nullptr, nullptr) < 0)
	{
		qDebug() << "fail to find the stream\n";
		return;
	}
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		qDebug() << "fail to find the stream info\n";
		return;
	}
	av_dump_format(pFormatCtx,0,input_file.c_str(),0);

	int video_stream_index = -1;
	for (size_t i = 0; i < pFormatCtx->nb_streams; ++i)
	{
		if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			video_stream_index = i;
			break;
		}
	}
	if (video_stream_index == -1)
	{
		qDebug() << "fail to find the video stream";
		return;
	}
	AVCodecParameters *codecParameters = pFormatCtx->streams[video_stream_index]->codecpar;
	const AVCodec *pCodec = avcodec_find_decoder(codecParameters->codec_id);
	if (pCodec == NULL)
	{
		qDebug() << "fail to find the decoder\n";
		return;
	}
	AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);
	if (pCodecCtx == nullptr)
	{
		return;
	}
	// 设置解码器参数
	if (avcodec_parameters_to_context(pCodecCtx, codecParameters) < 0) {
		printf("无法设置解码器参数\n");
		return;
	}
	if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0)
	{
		qDebug() << "fail to open the decodec\n";
		return;
	}
	//展示图像
	AVFrame *pFrame = av_frame_alloc();
	AVFrame *pframeRGB = av_frame_alloc();
	unsigned char *out_buffer = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1));
	av_image_fill_arrays(pframeRGB->data, pframeRGB->linesize, out_buffer, AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1);

	AVPacket *pkt = av_packet_alloc();
	av_init_packet(pkt);
	SwsContext *pSwsCtx = sws_alloc_context();
	pSwsCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB32,
		SWS_BICUBIC, nullptr, nullptr, nullptr);

	int got_frame = 0;
	/*AVFrame *frame = nullptr;*/
	while (av_read_frame(pFormatCtx, pkt) >= 0)
	{
		if (pkt->stream_index == video_stream_index) {
			int  ret = avcodec_send_packet(pCodecCtx, pkt);
			if (ret < 0)
			{
				qDebug() << "fail to decode video\n";
				return;
			}
			ret = avcodec_receive_frame(pCodecCtx, pFrame);
			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
				break;
			}
			else if (ret < 0) {
				printf("无法接收解码后的视频帧\n");
				return;
			}

			//if (got_frame > 0)
			//{
				sws_scale(pSwsCtx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pframeRGB->data, pframeRGB->linesize);
				QImage image = QImage((unsigned char*)pframeRGB->data[0], pCodecCtx->width, pCodecCtx->height, QImage::Format_RGB32, nullptr, nullptr);
				ui->label->setPixmap(QPixmap::fromImage(image));
				Delay(3000);
			//}
		}
		av_packet_unref(pkt);
	}

	sws_freeContext(pSwsCtx);
	av_packet_free(&pkt);
	avformat_close_input(&pFormatCtx);
	avformat_free_context(pFormatCtx);

}
void VideoWidget::Delay(int delay)
{
	QTime deadTime = QTime::currentTime().addMSecs(delay);
	if (QTime::currentTime() < deadTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}