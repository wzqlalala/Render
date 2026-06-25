#pragma once

#include "rendview_global.h"

#include <functional>

#include <QObject>
#include <QFutureWatcher>
#include <QTime>

#include "mMeshViewEnum.h"

namespace MDataPost
{
	class mDataPost1;
	class mPostMeshPickData;
	class mPostMeshPickThread;
}

namespace MPostRend
{
	class mPostOneFrameRender;

	class RENDVIEW_EXPORT mPostPickController : public QObject
	{
		Q_OBJECT

	public:
		explicit mPostPickController(MDataPost::mPostMeshPickData* pickData, QObject* parent = nullptr);
		~mPostPickController();

		void initialize(MDataPost::mDataPost1* dataPost,
			mPostOneFrameRender* oneFrameRender,
			MViewBasic::PickFilter* pickFilter);

		MDataPost::mPostMeshPickThread* thread();
		bool isReady() const;

		void start(const QTime& time, std::function<void(const QTime&)> finishedCallback);

	private:
		MDataPost::mPostMeshPickData* _pickData;
		MDataPost::mPostMeshPickThread* _thread;
		QFutureWatcher<void> _watcher;
	};
}
