#include "mVideoRender1.h"

#include "mViewBase.h"

#include <renderpch.h>

#include "mShaderManage.h"
#include <mTextureManage.h>
#include "texture.h"

#include <QDateTime>
#include <QDebug>
#include <QOpenGLTexture>
#include <qmessagebox.h>
#include <QCoreApplication>
#include <QTimer>

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



FFMpegReader1::FFMpegReader1(QObject *parent) : Running_(false)
{

}

void FFMpegReader1::Open(const QString& Url)
{
	Url_ = Url;
	Running_ = true;
	if (Url_.isEmpty())
	{
		QMessageBox::warning(nullptr, tr("warn"), tr("未选择文件"), QMessageBox::Ok);
		return;
	}
	std::string input_file = Url_.toStdString();
	//注册编解码器
	//av_register_all();

	pFormatCtx = avformat_alloc_context();
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
	av_dump_format(pFormatCtx, 0, input_file.c_str(), 0);

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
	pCodecCtx = avcodec_alloc_context3(pCodec);
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
	pFrame = av_frame_alloc();
	pframeRGB = av_frame_alloc();
	unsigned char *out_buffer = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1));
	av_image_fill_arrays(pframeRGB->data, pframeRGB->linesize, out_buffer, AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1);
}

void FFMpegReader1::Close()
{
	Running_ = false;
	wait();
}

void FFMpegReader1::run()
{
	Running_ = true;

	AVPacket *pkt = av_packet_alloc();
	av_init_packet(pkt);
	SwsContext *pSwsCtx = sws_alloc_context();
	pSwsCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB32,
		SWS_BICUBIC, nullptr, nullptr, nullptr);

	int got_frame = 0;
	int i = 0;
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
			i++;
			if (i == 30)
			{
				emit OnFrame(pFrame);
				break;
			}
			//if (got_frame > 0)
			//{
			sws_scale(pSwsCtx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pframeRGB->data, pframeRGB->linesize);
			//QImage image = QImage((unsigned char*)pframeRGB->data[0], pCodecCtx->width, pCodecCtx->height, QImage::Format_RGB32, nullptr, nullptr);
			//ui->label->setPixmap(QPixmap::fromImage(image));
			//}
			//Delay(3000);
		}
		//av_packet_unref(pkt);
	}

	//sws_freeContext(pSwsCtx);
	//av_packet_free(&pkt);
	//avformat_close_input(&pFormatCtx);
	//avformat_free_context(pFormatCtx);
	Running_ = false;
}

void FFMpegReader1::Delay(int delay)
{
	QTime deadTime = QTime::currentTime().addMSecs(delay);
	if (QTime::currentTime() < deadTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

namespace MBaseRend
{
	mVideoRender1::mVideoRender1(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent) :mBaseRender(app, parent)
	{
		reader = new FFMpegReader1(this);
		_app = app;
		_parent = parent;

		_drawable = MakeAsset<Drawable>();
		QVector<QVector3D> pos{ QVector3D(1,1,0.98),QVector3D(1,-1,0.98), QVector3D(-1,-1,0.98), QVector3D(-1,1,0.98) };
		QVector<QVector2D> texture{ QVector2D(1,0),QVector2D(1,1), QVector2D(0,1), QVector2D(0,0) };
		QVector<uint> index{ 0,1,3,1,2,3 };
		_drawable->setVertexAttribArray(0, MakeAsset<Vec3Array>(pos));
		_drawable->setVertexAttribArray(1, MakeAsset<Vec2Array>(texture));
		_drawable->setIndexAttribute(MakeAsset<UIntArray>(index));

		_stateSet = MakeAsset<StateSet>();
		_stateSet->setDrawMode(GL_TRIANGLES);
		connect(reader, &FFMpegReader1::OnOpen, this,
			[&] {
			qDebug("OnOpen");
		}, Qt::ConnectionType::QueuedConnection);

		QTimer *ti = new QTimer(this);
		connect(ti, SIGNAL(timeout()), this, SIGNAL(update()));//定时器刷新到update()里面取
		ti->start(40);

		//纹理
		Shader *shader = mShaderManage::GetInstance()->GetShader("VideoRend");
		_stateSet->setShader(shader);

		QString Url_("C:/Users/MX03/Desktop/msedge_Y9Ym2umW7D.mp4");
		reader->Open(Url_);

		int x;
		int y;

		x = reader->pCodecCtx->width;
		y = reader->pCodecCtx->height;
		_texture_y = new Texture(GL_TEXTURE_2D, x, y, 0, GL_R8, 1);
		_texture_u = new Texture(GL_TEXTURE_2D, x / 2.0, y / 2.0, 0, GL_R8, 1);
		_texture_v = new Texture(GL_TEXTURE_2D, x / 2.0, y / 2.0, 0, GL_R8, 1);

		//纹理
		_stateSet->setTexture("texture_y", _texture_y);
		_stateSet->setTexture("texture_u", _texture_u);
		_stateSet->setTexture("texture_v", _texture_v);

		_drawable->setStateSet(_stateSet);
		_parent->addChild(_drawable);

		connect(reader, &FFMpegReader1::OnFrame, this, [&](AVFrame *pframeRGB) {
			//	_drawable = MakeAsset<Drawable>();
			//	QVector<QVector3D> pos{ QVector3D(1,1,0.98),QVector3D(1,-1,0.98), QVector3D(-1,-1,0.98), QVector3D(-1,1,0.98) };
			//	QVector<QVector2D> texture{ QVector2D(1,0),QVector2D(1,1), QVector2D(0,1), QVector2D(0,0) };
			//	QVector<uint> index{ 0,1,3,1,2,3 };
			//	_drawable->setVertexAttribArray(0, MakeAsset<Vec3Array>(pos));
			//	_drawable->setVertexAttribArray(1, MakeAsset<Vec2Array>(texture));
			//	_drawable->setIndexAttribute(MakeAsset<UIntArray>(index));

			//	_texture_y = new Texture(GL_TEXTURE_2D, VideoSize.x(), VideoSize.y(), 0, GL_RED, 1);
			//	_texture_u = new Texture(GL_TEXTURE_2D, VideoSize.x() / 2.0, VideoSize.y() / 2.0, 0, GL_RED, 1);
			//	_texture_v = new Texture(GL_TEXTURE_2D, VideoSize.x() / 2.0, VideoSize.y() / 2.0, 0, GL_RED, 1);
			x = reader->pCodecCtx->width;
			y = reader->pCodecCtx->height;
			_texture_y->SetData(0, 0, 0, x, y, pframeRGB->data[0]);
			_texture_u->SetData(0, 0, 0, x / 2, y / 2, pframeRGB->data[1]);
			_texture_v->SetData(0, 0, 0, x / 2, y / 2, pframeRGB->data[2]);
			//	_stateSet->setTexture("texture_y", _texture_y);
			//	_stateSet->setTexture("texture_u", _texture_u);
			//	_stateSet->setTexture("texture_v", _texture_v);
			emit update();
		}, Qt::ConnectionType::AutoConnection);
		reader->start();
		//if (Url_.isEmpty())
		//{
		//	QMessageBox::warning(nullptr, tr("warn"), tr("未选择文件"), QMessageBox::Ok);
		//	return;
		//}
		//std::string input_file = Url_.toStdString();
		////注册编解码器
		////av_register_all();

		//AVFormatContext *pFormatCtx = avformat_alloc_context();
		//if (avformat_open_input(&pFormatCtx, input_file.c_str(), nullptr, nullptr) < 0)
		//{
		//	qDebug() << "fail to find the stream\n";
		//	return;
		//}
		//if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
		//{
		//	qDebug() << "fail to find the stream info\n";
		//	return;
		//}
		//av_dump_format(pFormatCtx, 0, input_file.c_str(), 0);

		//int video_stream_index = -1;
		//for (size_t i = 0; i < pFormatCtx->nb_streams; ++i)
		//{
		//	if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		//	{
		//		video_stream_index = i;
		//		break;
		//	}
		//}
		//if (video_stream_index == -1)
		//{
		//	qDebug() << "fail to find the video stream";
		//	return;
		//}
		//AVCodecParameters *codecParameters = pFormatCtx->streams[video_stream_index]->codecpar;
		//const AVCodec *pCodec = avcodec_find_decoder(codecParameters->codec_id);
		//if (pCodec == NULL)
		//{
		//	qDebug() << "fail to find the decoder\n";
		//	return;
		//}
		//AVCodecContext *pCodecCtx = avcodec_alloc_context3(pCodec);
		//if (pCodecCtx == nullptr)
		//{
		//	return;
		//}
		//// 设置解码器参数
		//if (avcodec_parameters_to_context(pCodecCtx, codecParameters) < 0) {
		//	printf("无法设置解码器参数\n");
		//	return;
		//}
		//if (avcodec_open2(pCodecCtx, pCodec, nullptr) < 0)
		//{
		//	qDebug() << "fail to open the decodec\n";
		//	return;
		//}
		////展示图像
		//AVFrame *pFrame = av_frame_alloc();
		//AVFrame *pframeRGB = av_frame_alloc();
		//unsigned char *out_buffer = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1));
		//av_image_fill_arrays(pframeRGB->data, pframeRGB->linesize, out_buffer, AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1);
		////emit OnUpdate(QPoint(pCodecCtx->width, pCodecCtx->height));
		//int x = pCodecCtx->width;
		//int y = pCodecCtx->height;
		//_texture_y = new Texture(GL_TEXTURE_2D, x, y, 0, GL_R8, 1);
		//_texture_u = new Texture(GL_TEXTURE_2D, x / 2.0, y / 2.0, 0, GL_R8, 1);
		//_texture_v = new Texture(GL_TEXTURE_2D, x / 2.0, y / 2.0, 0, GL_R8, 1);

		////纹理
		//_stateSet->setTexture("texture_y", _texture_y);
		//_stateSet->setTexture("texture_u", _texture_u);
		//_stateSet->setTexture("texture_v", _texture_v);

		//_drawable->setStateSet(_stateSet);
		//_parent->addChild(_drawable);

		//AVPacket *pkt = av_packet_alloc();
		//av_init_packet(pkt);
		//SwsContext *pSwsCtx = sws_alloc_context();
		//pSwsCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		//	pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB32,
		//	SWS_BICUBIC, nullptr, nullptr, nullptr);

		//int got_frame = 0;
		///*AVFrame *frame = nullptr;*/
		//int i = 0;
		//while (av_read_frame(pFormatCtx, pkt) >= 0)
		//{
		//	if (pkt->stream_index == video_stream_index) {
		//		int  ret = avcodec_send_packet(pCodecCtx, pkt);
		//		if (ret < 0)
		//		{
		//			qDebug() << "fail to decode video\n";
		//			return;
		//		}
		//		ret = avcodec_receive_frame(pCodecCtx, pFrame);
		//		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
		//			break;
		//		}
		//		else if (ret < 0) {
		//			printf("无法接收解码后的视频帧\n");
		//			return;
		//		}
		//		i++;
		//		if (i == 30)
		//		{
		//			_texture_y->SetData(0, 0, 0, x, y, pFrame->data[0]);
		//			_texture_u->SetData(0, 0, 0, x/2, y/2, pFrame->data[1]);
		//			_texture_v->SetData(0, 0, 0, x/2, y/2, pFrame->data[2]);
		//			break;
		//			//emit OnFrame(r);
		//		}
		//		//if (got_frame > 0)
		//		//{
		//		sws_scale(pSwsCtx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pframeRGB->data, pframeRGB->linesize);
		//		//QImage image = QImage((unsigned char*)pframeRGB->data[0], pCodecCtx->width, pCodecCtx->height, QImage::Format_RGB32, nullptr, nullptr);
		//		//ui->label->setPixmap(QPixmap::fromImage(image));
		//		//}
		//		//Delay(3000);
		//	}
		//	av_packet_unref(pkt);
		//}

		//sws_freeContext(pSwsCtx);
		//av_packet_free(&pkt);
		//avformat_close_input(&pFormatCtx);
		//avformat_free_context(pFormatCtx);
	};

	void mVideoRender1::initialVideo(QString filename)
	{


	}

	void mVideoRender1::setIsShow(bool isShow)
	{
		if (_drawable)
		{
			_drawable->setNodeMask(!isShow);
		}
	}

	void mVideoRender1::updateUniform(std::shared_ptr<MViewBasic::mViewBase> modelView, shared_ptr<mViewBase> commonView)
	{
		//if (!reader->Running_ && !reader->Url_.isEmpty())
		//{
		//	reader->start();
		//}
		//reader.run();
	}
}