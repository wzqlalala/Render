#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QVector2D>
#include <QVector3D>
#include <memory>
#include <QMatrix4x4>
#include <QString>
#include <QThread>

#include "mBaseRender.h"

namespace mxr
{
	class Application;
	class Shader;
	class Texture;
	class Uniform;
	class Drawable;
	class StateSet;
	class Group;
	class Geode;
	class Texture;
}
namespace MViewBasic
{
	class mViewBase;
}
struct AVFrame;
struct AVFormatContext;
struct AVCodecContext;
class FFMpegReader1 : public QThread
{
	Q_OBJECT
public:
	explicit FFMpegReader1(QObject *parent = nullptr);

	void Open(const QString& Url);
	void Close();
	void Delay(int delay);

	// QThread interface
	bool Running_;
	QString Url_;
	bool isOneFrame{false};

	AVFormatContext *pFormatCtx;
	AVCodecContext *pCodecCtx;
	AVFrame *pFrame;
	AVFrame *pframeRGB;
	int video_stream_index = -1;
public:
	void run();

signals:
	void OnOpen();
	void OnUpdate(const QPoint VideoSize);
	void OnFrame(AVFrame *pframeRGB);

private:
	QPoint DisplaySize_;
};


using namespace mxr;
namespace MBaseRend
{

	class RENDVIEW_EXPORT mVideoRender1 :public mBaseRender
	{
		Q_OBJECT

	public:
		mVideoRender1(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent);

		~mVideoRender1() {};

		void initialVideo(QString filename);

		void start();

		void stop();

		void setIsShow(bool isShow);

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView);

	protected:

		QString _name;

		std::shared_ptr<mxr::Application> _app;
		std::shared_ptr<mxr::Group> _parent;//父节点

		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::StateSet> _stateSet;

		FFMpegReader1 *reader;

		mxr::Texture *_texture_y;
		mxr::Texture *_texture_u;
		mxr::Texture *_texture_v;
	};
}

