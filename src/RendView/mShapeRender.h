#pragma once
#include "rendview_global.h"
//解决中文乱码
#pragma execution_character_set("utf-8")

#include <QVector2D>
#include <QVector>
#include <QVector3D>
#include <QHash>
#include <qopengl.h>

#include "mBaseRender.h"

#include <memory>

namespace mxr
{
	class Drawable;
	class StateSet;
	class Texture;
	class Group;
	class Uniform;
	class Shader;
}
namespace MBaseRend 
{

	class mBaseShape
	{
	public:
		mBaseShape(std::shared_ptr<mxr::Group> parent);

		std::shared_ptr<mxr::Drawable> getDrawAble() {
			return _drawable;
		};

		~mBaseShape();

		void setStateSet(std::shared_ptr<mxr::StateSet> stateSet);

		void setIsShow(bool isShow);
	protected:

		std::shared_ptr<mxr::Group> _parent;

		std::shared_ptr<mxr::Drawable> _drawable;
	};

	class mBaseRend;
	class RENDVIEW_EXPORT mShapeRender : public mBaseRender
	{
	public:

		mShapeRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mBaseRend *baseRend = nullptr);

		~mShapeRender();

		void clearAllRender();

		void updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView) override;

		void appendXPointShape(QString key, QVector<QVector3D> pos, QVector3D color = QVector3D(1,1,1), float size = 1.0);
		void setXPointIsShow(QString key, bool isShow);
		void deleteXPointShape(QString key);

	protected:

		std::shared_ptr<mxr::StateSet> _commonPointTextureState;//支持旋转、平移、缩放等的事件

		mxr::Texture *_XPointTexture;

		QHash<QString, std::shared_ptr<mBaseShape>> _commonShapes;
	};
}

