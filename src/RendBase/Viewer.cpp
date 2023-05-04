#include "Viewer.h"
#include "Group.h"

namespace mxr
{
	QTime *time = new QTime;
	Viewer::Viewer()
	{
		_visitor = MakeAsset<NodeVisitor>();
		_clear = MakeAsset<Clear>();
	}

	void Viewer::compile()
	{
		//qDebug() << __LINE__ << time->elapsed();
		//time->start();
		//qDebug() << __LINE__ << time->elapsed();
		_sceneData->accept(_visitor);//20֡�Ķ�������100ms����ͼ����97ms
		//qDebug() << __LINE__ << time->elapsed();
		_visitor->compile();//20֡�Ķ�������43ms����ͼ����23ms
		//qDebug() << __LINE__ << time->elapsed();
		_visitor->clear();
		//qDebug() << __LINE__ << time->elapsed();
	}

	void Viewer::noClearRun()
	{
		QOpenGLContext *context = QOpenGLContext::currentContext();
		context->makeCurrent(context->surface());
		//qDebug() << "noClearRun" << QString::number(long long int(context), 16);
		//qDebug() << "noClearRun" << QString::number(long long int(context->surface()), 16);
		_sceneData->accept(_visitor);//20֡�Ķ�������100ms����ͼ����97ms
		//qDebug() << __LINE__ << time->elapsed();
		_visitor->compile();//20֡�Ķ�������43ms����ͼ����23ms
		//qDebug() << __LINE__ << time->elapsed();
		_visitor->clear();
		//context->functions()->glClearColor(0, 0, 0, 1);
		//context->functions()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//����
		_visitor->run();
		//qDebug() << __LINE__ << time->elapsed();
	}

	void Viewer::run()
	{
		this->compile();
		//����
		_clear->apply();
		_visitor->run();
		//qDebug() << __LINE__ << time->elapsed();

	}
	void Viewer::deleteAllData()
	{
		_visitor->RemoveAllData();
	}
}