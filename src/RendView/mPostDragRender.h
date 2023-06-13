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
using namespace mxr;
namespace MPostRend
{
	class mPostRendStatus;
	class RENDVIEW_EXPORT mPostDragRender :public QObject
	{
		Q_OBJECT

	public:
		mPostDragRender(QString name, std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, std::shared_ptr<mPostRendStatus> rendStatus);

		~mPostDragRender() {};

		QString getName() { return _name; };

		void setIsShow(bool isShow);

		virtual bool pointIsIn(QVector2D pos, float depth, QMatrix4x4 pvm, int w, int h) { return false; };

		virtual void move(QVector2D pos, QMatrix4x4 pvm, int w, int h) {};

		virtual void updateUniform(std::shared_ptr<MViewBasic::mViewBase> modelView);

	protected:

		QString _name;

		std::shared_ptr<mxr::Application> _app;
		std::shared_ptr<mxr::Group> _parent;//父节点

		std::shared_ptr<mxr::Drawable> _drawable;
		std::shared_ptr<mxr::StateSet> _stateSet;

		std::shared_ptr<mPostRendStatus> _rendStatus;

	};

	class RENDVIEW_EXPORT mPostSphereRender :public mPostDragRender
	{
		Q_OBJECT

	public:
		mPostSphereRender(QString name, std::shared_ptr<Application> app, std::shared_ptr<Group> parent, std::shared_ptr<mPostRendStatus> rendStatus);

		void setSphereData(QVector3D center, float radius);

		bool pointIsIn(QVector2D pos, float depth, QMatrix4x4 pvm, int w, int h) override;

		void move(QVector2D pos, QMatrix4x4 pvm, int w, int h) override;

		void updateUniform(std::shared_ptr<MViewBasic::mViewBase> modelView) override;

		~mPostSphereRender();

		//半径为1，坐标在原点的球
		static QVector<QVector3D> _sphereVertexs;
		static QVector<uint> _sphereIndexs;

		//QVector3D _sphereCenter;
		//float _sphereRadius;
		//拖拽点的位置相对于球心的位置，x,y代表处于乘以矩阵后的像素（范围为-1~1）
		QVector2D _relativePosition;
		//球心深度（范围为-1~1）
		float _centerDepth;
	};
}

