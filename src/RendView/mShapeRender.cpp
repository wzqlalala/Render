#include "mShapeRender.h"

#include <QApplication>
#include <QDesktopWidget>

#include <renderpch.h>
#include "mShaderManage.h"
#include "mTextureManage.h"

#include "mModelView.h"
#include "mViewBase.h"

//freetype
#include "ft2build.h"
#include FT_FREETYPE_H

using namespace mxr;
namespace MBaseRend
{
	//×ø±êÖá¶¥µã
	QVector<QVector3D> _XPointVertices;

	mShapeRender::mShapeRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mBaseRend * baseRend) :mBaseRender(app, parent, baseRend)
	{
		makeCurrent();

		_commonPointTextureState = MakeAsset<StateSet>();
		Shader* shader = mShaderManage::GetInstance()->GetShader("CommonTextureShape");
		_commonPointTextureState->setShader(shader);
		_commonPointTextureState->setDrawMode(GL_POINTS);
		_commonPointTextureState->setUniform(MakeAsset<Uniform>("pvm", QMatrix4x4()));
		_commonPointTextureState->setUniform(MakeAsset<Uniform>("PointSize", int(10)));
		_commonPointTextureState->setAttributeAndModes(MakeAsset<Depth>(), 0);

		_XPointTexture = mTextureManage::GetInstance()->GetTexture("XPoint");
		_commonPointTextureState->setTexture("texture",_XPointTexture);
	}
	mShapeRender::~mShapeRender()
	{

	}
	void mShapeRender::clearAllRender()
	{
		QHash<QString, std::shared_ptr<mBaseShape>>().swap(_commonShapes);
	}
	void mShapeRender::updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView)
	{
		_commonPointTextureState->getUniform("pvm")->SetData(modelView->getPVMValue());
	}
	void mShapeRender::appendXPointShape(QString key, QVector<QVector3D> poses, QVector3D color, float size)
	{
		makeCurrent();
		std::shared_ptr<mBaseShape> shape = MakeAsset<mBaseShape>(_parent);
		shape->setStateSet(_commonPointTextureState);

		shape->getDrawAble()->setVertexAttribArray(0, MakeAsset<Vec3Array>(poses));
		shape->getDrawAble()->setVertexAttribArray(1, MakeAsset<Vec3Array>(QVector<QVector3D>(poses.size(),color)));

		_commonShapes[key] = shape;
	}
	void mShapeRender::setXPointIsShow(QString key, bool isShow)
	{
		auto value = _commonShapes.value(key);
		if (value)
		{
			value->setIsShow(isShow);
		}
	}

	void mShapeRender::deleteXPointShape(QString key)
	{
		_commonShapes.remove(key);
	}

	mBaseShape::mBaseShape(std::shared_ptr<mxr::Group> parent)
	{
		_parent = parent;

		_drawable = MakeAsset<Drawable>();

		_parent->addChild(_drawable);
	}

	mBaseShape::~mBaseShape()
	{
		if (_parent)
		{
			_parent->removeChild(_drawable);
		}
	}

	void mBaseShape::setStateSet(std::shared_ptr<mxr::StateSet> stateSet)
	{
		 _drawable->setStateSet(stateSet);
	}

	void mBaseShape::setIsShow(bool isShow)
	{
		if (_drawable)
		{
			_drawable->setNodeMask(isShow?0:1);
		}
	}
}
