#include "mVideoRender.h"

#include "mViewBase.h"

#include <renderpch.h>

#include "mShaderManage.h"
#include <mTextureManage.h>
#include "texture.h"

#include <QDateTime>
#include <QDebug>
#include <QOpenGLTexture>

//extern "C" {
//#include <libavcodec/avcodec.h>
//
//#include <libavformat/avformat.h>
//
//#include <libavutil/imgutils.h>
//
//}

#include <ffms.h>
//FFFrame *FFFrame::instance = nullptr;

QFFMpegReader::QFFMpegReader(QObject *parent) : Running_(false)
{

}

void QFFMpegReader::Open(const QString& Url)
{
	Url_ = Url;
	Running_ = true;
	start();
}

void QFFMpegReader::Close()
{
	Running_ = false;
	wait();
}

bool QFFMpegReader::OpenImpl(FFMS_VideoSource* &videosource, int& num_frames)
{
	//https://github.com/FFMS/ffms2/blob/master/doc/ffms2-api.md
	/* Index the source file. Note that this example does not index any audio tracks. */
	char errmsg[1024];
	FFMS_ErrorInfo errinfo;
	errinfo.Buffer = errmsg;
	errinfo.BufferSize = sizeof(errmsg);
	errinfo.ErrorType = FFMS_ERROR_SUCCESS;
	errinfo.SubType = FFMS_ERROR_SUCCESS;

	FFMS_Indexer* indexer = FFMS_CreateIndexer(Url_.toStdString().c_str(), &errinfo);
	if (indexer == nullptr) {
		return false;
	}

	//Both FFMS_DoIndexing2 and FFMS_CancelIndexing destroys the indexer object and frees its memory.
	FFMS_Index* index = FFMS_DoIndexing2(indexer, FFMS_IEH_ABORT, &errinfo);
	if (index == nullptr) {
		FFMS_CancelIndexing(indexer);
		return false;
	}

	//查找视频源
	int trackno = FFMS_GetFirstTrackOfType(index, FFMS_TYPE_VIDEO, &errinfo);
	if (trackno < 0) {
		FFMS_DestroyIndex(index);
		return false;
	}

	//创建视频源
	videosource = FFMS_CreateVideoSource(Url_.toStdString().c_str(), trackno, index, 1, FFMS_SEEK_NORMAL, &errinfo);
	if (videosource == nullptr) {
		FFMS_DestroyIndex(index);
		return false;
	}

	//清除index
	FFMS_DestroyIndex(index);

	//通知事件
	OnOpen();

	//获取属性
	const FFMS_VideoProperties* videoprops = FFMS_GetVideoProperties(videosource);
	num_frames = videoprops->NumFrames;

	//读取第一帧
	const FFMS_Frame* propframe = FFMS_GetFrame(videosource, 0, &errinfo);
	DisplaySize_ = QPoint(propframe->EncodedWidth, propframe->EncodedHeight);
	OnUpdate(DisplaySize_);

	//设置播放像素格式，尺寸，和拉伸模式
	int pixfmts[2];
	pixfmts[0] = FFMS_GetPixFmt("yuv420p");
	pixfmts[1] = -1;
	if (FFMS_SetOutputFormatV2(videosource, pixfmts, DisplaySize_.x(), DisplaySize_.y(), FFMS_RESIZER_BICUBIC, &errinfo))
	{
		return false;
	}

	return true;
}


void QFFMpegReader::run()
{
	char errmsg[1024];
	FFMS_ErrorInfo errinfo;
	errinfo.Buffer = errmsg;
	errinfo.BufferSize = sizeof(errmsg);
	errinfo.ErrorType = FFMS_ERROR_SUCCESS;
	errinfo.SubType = FFMS_ERROR_SUCCESS;


	//打开文件
	FFMS_VideoSource* VideoSource = nullptr;
	int NumFrames = 0;
	if (!OpenImpl(VideoSource, NumFrames))
	{
		qDebug("Open File Failed [%s]", Url_.toStdString().c_str());
		return;
	}

	//逐帧处理
	FFMS_Track* VideoTrack = FFMS_GetTrackFromVideo(VideoSource);
	const FFMS_TrackTimeBase* TrackTimeBase = FFMS_GetTimeBase(VideoTrack);

	//初始化数据
	QDateTime StartTime = QDateTime::currentDateTime();

	//初始PTS并不是零，所以需要减去第一帧的PTS
	int64_t StartPTS = 0;
	int FrameNum = 0;

	while (Running_ && (FrameNum < NumFrames))
	{
		//取帧
		const FFMS_Frame* Frame = FFMS_GetFrame(VideoSource, FrameNum, &errinfo);
		const FFMS_FrameInfo* FrameInfo = FFMS_GetFrameInfo(VideoTrack, FrameNum);

		if (Frame)
		{
			//记录第一帧的PTS
			if (FrameNum == 0)
			{
				StartPTS = FrameInfo->PTS;
			}

			//拷贝帧
			auto FFrame = FFFrame::MakeFrame();
			FFrame->TextureY.setRawData((const char *)Frame->Data[0], Frame->Linesize[0] * Frame->ScaledHeight);
			FFrame->TextureU.setRawData((const char *)Frame->Data[1], Frame->Linesize[1] * (Frame->ScaledHeight / 2));
			FFrame->TextureV.setRawData((const char *)Frame->Data[2], Frame->Linesize[2] * (Frame->ScaledHeight / 2));

			//计算时间差，逐毫秒等待，如果一次等待，则需要等待很久
			while (Running_)
			{
				int64_t PTS = (int64_t)(((FrameInfo->PTS - StartPTS) * TrackTimeBase->Num) / (double)TrackTimeBase->Den);
				int64_t CurPTS = StartTime.msecsTo(QDateTime::currentDateTime());
				if (CurPTS < PTS)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}
				else
				{
					break;
				}
			}
			if (FrameNum == 50)
			{
				//显示帧
				OnFrame(FFrame);
			}

		}
		//计算下一帧索引
		FrameNum++;

		//如果单文件循环，则重置起始时间
		if (FrameNum >= NumFrames)
		{
			StartTime = QDateTime::currentDateTime();
			FrameNum = 0;
		}
	}

	//关闭文件
	if (VideoSource)
	{
		FFMS_DestroyVideoSource(VideoSource);
		VideoSource = nullptr;
	}
}

namespace MBaseRend
{
	mVideoRender::mVideoRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent):mBaseRender(app, parent)
	{
		_app = app; 
		_parent = parent;

		_drawable = MakeAsset<Drawable>();
		QVector<QVector3D> pos{QVector3D(1,1,0.98),QVector3D(1,-1,0.98), QVector3D(-1,-1,0.98), QVector3D(-1,1,0.98)};
		QVector<QVector2D> texture{QVector2D(1,0),QVector2D(1,1), QVector2D(0,1), QVector2D(0,0)};
		QVector<uint> index{ 0,1,3,1,2,3 };
		_drawable->setVertexAttribArray(0, MakeAsset<Vec3Array>(pos));
		_drawable->setVertexAttribArray(1, MakeAsset<Vec2Array>(texture));
		_drawable->setIndexAttribute(MakeAsset<UIntArray>(index));

		_stateSet = MakeAsset<StateSet>();
		_stateSet->setDrawMode(GL_TRIANGLES);
		connect(&reader, &QFFMpegReader::OnOpen, this,
			[&] {
			qDebug("OnOpen");
		}, Qt::ConnectionType::QueuedConnection);

		//纹理
		Shader *shader = mShaderManage::GetInstance()->GetShader("VideoRend");
		_stateSet->setShader(shader);

		//texture_y = new Texture(GL_TEXTURE_2D, 0, 0, 0, QOpenGLTexture::TextureFormat::R8_UNorm, 1);
		//texture_u = new Texture(GL_TEXTURE_2D, 0 / 2.0, 0 / 2.0, 0, QOpenGLTexture::TextureFormat::R8_UNorm, 1);
		//texture_v = new Texture(GL_TEXTURE_2D, 0 / 2.0, 0 / 2.0, 0, QOpenGLTexture::TextureFormat::R8_UNorm, 1);


		int x;
		int y;
		connect(&reader, &QFFMpegReader::OnUpdate, this, [&](const QPoint VideoSize) {
			_app->GLContext()->makeCurrent(_app->GLContext()->surface());

			x = VideoSize.x();
			y = VideoSize.y();
			texture_y = new Texture(GL_TEXTURE_2D, VideoSize.x(), VideoSize.y(),0, GL_R8, 1);
			texture_u = new Texture(GL_TEXTURE_2D, VideoSize.x() / 2.0, VideoSize.y() / 2.0, 0, GL_R8, 1);
			texture_v = new Texture(GL_TEXTURE_2D, VideoSize.x() / 2.0, VideoSize.y() / 2.0, 0, GL_R8, 1);

			//纹理
			Shader *shader = mShaderManage::GetInstance()->GetShader("VideoRend");
			_stateSet->setShader(shader);
			_stateSet->setTexture("tex_y", texture_y);
			_stateSet->setTexture("tex_u", texture_u);
			_stateSet->setTexture("tex_v", texture_v);
			
			_drawable->setStateSet(_stateSet);
			_parent->addChild(_drawable);
			qDebug("OnUpdate");

		}, Qt::ConnectionType::QueuedConnection);

		connect(&reader, &QFFMpegReader::OnFrame, this, [&](FFFrame *Frame) {
			//texture_y->SetData(0, 0, 0, x, y, Frame->TextureY.data());
			//texture_u->SetData(0, 0, 0, x/2.0, y/2.0, Frame->TextureU.data());
			//texture_v->SetData(0, 0, 0, x/2.0, y/2.0, Frame->TextureV.data());
			texture_y->SetData(0, 0, 0, x, y, Frame->TextureY.data());
			texture_u->SetData(0, 0, 0, x, y, Frame->TextureU.data());
			texture_v->SetData(0, 0, 0, x, y, Frame->TextureV.data());

			//QOpenGLTexture t(QOpenGLTexture::Target::Target2D);
			//t.setData(0, 0, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8, nullptr);

			//delete Frame;

			//update();
		}, Qt::ConnectionType::QueuedConnection);

		reader.Open(R"(C:\Users\MX03\Desktop\w0Z2Zkjr7Y.mp4)");
	};

	void mVideoRender::initialVideo(QString filename)
	{
		/*
		char *filePath = filename.toLocal8Bit().data();
		AVFormatContext* fmtCtx = nullptr;
		avformat_open_input(&fmtCtx, filePath, NULL, NULL);
		avformat_find_stream_info(fmtCtx, NULL);

		int videoStreamIndex;
		AVCodecContext* vcodecCtx = nullptr;
		for (int i = 0; i < fmtCtx->nb_streams; i++) {
			AVStream* stream = fmtCtx->streams[i];
			if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
				const AVCodec* codec = avcodec_find_decoder(stream->codecpar->codec_id);
				videoStreamIndex = i;
				vcodecCtx = avcodec_alloc_context3(codec);
				avcodec_parameters_to_context(vcodecCtx, fmtCtx->streams[i]->codecpar);
				avcodec_open2(vcodecCtx, codec, NULL);
			}
		}

		while (1) {
			AVPacket* packet = av_packet_alloc();
			int ret = av_read_frame(fmtCtx, packet);
			if (ret == 0 && packet->stream_index == videoStreamIndex) {
				ret = avcodec_send_packet(vcodecCtx, packet);
				if (ret == 0) {
					AVFrame* frame = av_frame_alloc();
					ret = avcodec_receive_frame(vcodecCtx, frame);
					if (ret == 0) {
						av_packet_unref(packet);
						avcodec_free_context(&vcodecCtx);
						avformat_close_input(&fmtCtx);
						//return frame;
					}
					else if (ret == AVERROR(EAGAIN)) {
						av_frame_unref(frame);
						continue;
					}
				}
			}

			av_packet_unref(packet);
		}
		*/


	}

	void mVideoRender::setIsShow(bool isShow)
	{
		if (_drawable)
		{
			_drawable->setNodeMask(!isShow);
		}
	}

	void mVideoRender::updateUniform(std::shared_ptr<MViewBasic::mViewBase> modelView, shared_ptr<mViewBase> commonView)
	{

	}
}