#include "mPostPickController.h"

#include <QtConcurrent/QtConcurrent>

#include <set>

#include "mDataPost1.h"
#include "mPostMeshPickThread.h"
#include "mPostModelRender.h"
#include "mPostOneFrameRender.h"

namespace MPostRend
{
	mPostPickController::mPostPickController(MDataPost::mPostMeshPickData* pickData, QObject* parent)
		: QObject(parent), _pickData(pickData), _thread(nullptr)
	{
	}

	mPostPickController::~mPostPickController()
	{
		if (_watcher.isRunning())
		{
			_watcher.waitForFinished();
		}
		delete _thread;
	}

	void mPostPickController::initialize(MDataPost::mDataPost1* dataPost,
		mPostOneFrameRender* oneFrameRender,
		MViewBasic::PickFilter* pickFilter)
	{
		if (!dataPost || !oneFrameRender)
		{
			return;
		}

		if (_watcher.isRunning())
		{
			_watcher.waitForFinished();
		}

		delete _thread;
		_thread = new MDataPost::mPostMeshPickThread(_pickData);
		_thread->setPickFilter(pickFilter);

		std::set<QString> partNames = dataPost->getAllPostPartNames();
		for (const QString& partName : partNames)
		{
			_thread->appendPartSpaceTree(partName, oneFrameRender->getModelRender()->getPartSpaceTree(partName));
		}
	}

	MDataPost::mPostMeshPickThread* mPostPickController::thread()
	{
		return _thread;
	}

	bool mPostPickController::isReady() const
	{
		return _thread != nullptr;
	}

	void mPostPickController::start(const QTime& time, std::function<void(const QTime&)> finishedCallback)
	{
		if (!_thread || _watcher.isRunning())
		{
			return;
		}

		QFuture<void> future = QtConcurrent::run(_thread, &MDataPost::mPostMeshPickThread::startPick);
		QObject::connect(&_watcher, &QFutureWatcher<void>::finished, [this, time, finishedCallback] {
			QObject::disconnect(&_watcher, 0, 0, 0);
			if (finishedCallback)
			{
				finishedCallback(time);
			}
		});
		_watcher.setFuture(future);
	}
}
