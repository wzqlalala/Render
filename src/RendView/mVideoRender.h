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
class FFMS_VideoSource;
struct FFFrame
{
	FFFrame()
	{

	}

	//YUV
	QByteArray TextureY;
	QByteArray TextureU;
	QByteArray TextureV;

	//创建一个数据帧
	static FFFrame *MakeFrame()
	{
		//if (instance!=nullptr)
		//{
		//	delete instance;
		//}
		auto instance =  new FFFrame();
		return instance;
	}

	//static FFFrame *instance;
};
class QFFMpegReader : public QThread
{
	Q_OBJECT
public:
	explicit QFFMpegReader(QObject *parent = nullptr);

	void Open(const QString& Url);
	void Close();

	// QThread interface
protected:
	void run();

private:
	bool OpenImpl(FFMS_VideoSource* &videosource, int& num_frames);

signals:
	void OnOpen();
	void OnUpdate(const QPoint VideoSize);
	void OnFrame(FFFrame *Frame);

private:
	QString Url_;
	bool Running_;
	QPoint DisplaySize_;
};


using namespace mxr;
namespace MBaseRend
{

	class RENDVIEW_EXPORT mVideoRender :public mBaseRender
	{
		Q_OBJECT

	public:
		mVideoRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent);

		~mVideoRender() {};

		void initialVideo(QString filename);

		void setIsShow(bool isShow);

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView);

	protected:

		QString _name;

		std::shared_ptr<mxr::Application> _app;
		std::shared_ptr<mxr::Group> _parent;//父节点

		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::StateSet> _stateSet;

		QFFMpegReader reader;

		mxr::Texture *texture_y;
		mxr::Texture *texture_u;
		mxr::Texture *texture_v;
	};
}

