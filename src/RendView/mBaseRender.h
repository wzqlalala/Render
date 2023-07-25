#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QVector2D>

#include "mMeshViewEnum.h"

namespace mxr
{
	class Application;
	class Group;
}
namespace MViewBasic
{
	class mViewBase;
}
using namespace MViewBasic;
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

		virtual void startPick(QVector<QVector2D> poses) { };

		virtual bool getIsDragSomething(QVector2D pos) { return false; };

		virtual void dragSomething(QVector2D pos) {};

		virtual void resizeWindow(int w, int h) {};

		//设置拾取过滤模式
		void setPickFilter(PickFilter pickFilter);

		//设置框选模式
		void setMultiplyPickMode(MultiplyPickMode multiplyPickMode);
	protected:

		void makeCurrent();

		void doneCurrent();

		//通过容差范围内的矩形像素深度获取深度
		float getDepth(QVector2D pos);

	signals:

		void update();

	private:



	protected:
		std::shared_ptr<mxr::Application> _app;

		std::shared_ptr<mxr::Group> _parent;

		mBaseRend *_baseRend;
	};
}