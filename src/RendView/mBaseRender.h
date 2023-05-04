#pragma once
#include "rendview_global.h"
//�����������
#pragma execution_character_set("utf-8")

#include "app.h"
#include <QObject>
#include <QVector2D>

#include "mMeshViewEnum.h"
namespace mxr
{
	class Group;
}
namespace MViewBasic
{
	class mViewBase;
}
using namespace MViewBasic;
using namespace std;
namespace MBaseRend
{
	class mBaseRend;
	class RENDVIEW_EXPORT mBaseRender : public QObject
	{
		Q_OBJECT

	public:
		mBaseRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mBaseRend *baseRend = nullptr);

		~mBaseRender();

		virtual void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView) {};

		virtual void startPick(QVector<QVector2D> poses) { _poses = poses; };

		virtual bool getIsDragSomething(QVector2D pos) { return false; };

		virtual void dragSomething(QVector2D pos) {};
	protected:

		void makeCurrent() { _app->GLContext()->makeCurrent(_app->GLContext()->surface()); };

		void doneCurrent() { _app->GLContext()->doneCurrent(); };

	signals:

		void update();

	private:



	protected:
		std::shared_ptr<mxr::Application> _app;

		std::shared_ptr<mxr::Group> _parent;

		mBaseRend *_baseRend;

		//λ��
		QVector<QVector2D> _poses;//��ѡ����ק���ǵ�ǰ�㣬���κ�Բ�ο�ѡ�������ĺ����յĵ㣬����ο�ѡ����ÿһ����
	};
}