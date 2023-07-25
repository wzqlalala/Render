#pragma once
#include "rendview_global.h"
//�����������
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

		//����ʰȡ����ģʽ
		void setPickFilter(PickFilter pickFilter);

		//���ÿ�ѡģʽ
		void setMultiplyPickMode(MultiplyPickMode multiplyPickMode);
	protected:

		void makeCurrent();

		void doneCurrent();

		//ͨ���ݲΧ�ڵľ���������Ȼ�ȡ���
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