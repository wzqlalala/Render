#include "mPreRend.h"
#include "mPreRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"

#include "mViewBase.h"
#include "mCommonView.h"
#include "mModelView.h"

//AppConfiguration
#include <mAppConfiguration.h>

////MxRender
#include <renderpch.h>

//图标渲染
#include"mLableRendIntegrate_pre.h"
#include"mLableRendController_pre.h"
#include"mLableRendIntegrate_common.h"
#include"mLableRendController_common.h"
#include"mLableDataController_common.cpp"

#include <math.h>

namespace MPreRend
{

	mPreRend::mPreRend(const QString& name):mBaseRend3D(name, Viewport3D)
	{
		*_pickFilter = PickFilter::Pick2DMeshByAngle;
		qDebug() << "Pre Struct";

		//保存单位制
		QString viewStyle = MAppConfiguration::mAppConfiguration::getInstance()->_viewStyle;

		this->clearCameraKeys();
		this->clearPickKeys();
		if (viewStyle == "Abaqus")
		{
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::MiddleButton, Qt::ControlModifier | Qt::AltModifier), CameraOperateMode::Zoom);
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ControlModifier | Qt::AltModifier), CameraOperateMode::Rotate);
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ControlModifier | Qt::AltModifier), CameraOperateMode::Translate);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::NoModifier), PickMode::SoloPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::NoModifier), PickMode::SoloPick);
		}
		else if (viewStyle == "HyperView")
		{
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::MiddleButton, Qt::ControlModifier), CameraOperateMode::Zoom);
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ControlModifier), CameraOperateMode::Rotate);
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ControlModifier), CameraOperateMode::Translate);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::NoModifier), PickMode::SoloPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::NoModifier), PickMode::SoloPick);
		}
		else
		{
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::MiddleButton, Qt::NoModifier), CameraOperateMode::Zoom);
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::NoModifier), CameraOperateMode::Rotate);
			this->setCameraKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::NoModifier), CameraOperateMode::Translate);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ShiftModifier), PickMode::MultiplyPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::LeftButton, Qt::ControlModifier), PickMode::SoloPick);
			this->setPickKeys(QPair<Qt::MouseButton, Qt::KeyboardModifiers>(Qt::RightButton, Qt::ControlModifier), PickMode::SoloPick);
		}
		
		//让其自动进入OpenGL初始化
		this->setGeometry(-1, -1, 1, 1);
		this->show();
	}

	void mPreRend::initializeGL()
	{
		mBaseRend3D::initializeGL();

		makeCurrent();

		shared_ptr<mPreRender> preRender = make_shared<mPreRender>(_app, _root, this);
		this->addRender(preRender);

		_fontRender->appendGloabalAxisFont();
		_arrowRender->appendGloabalAxisArrow();

		//图标渲染对象
		_lableRend_pre = make_shared<mLableRendIntegrate_pre>();
		_lableRend_common = make_shared<mLableRendIntegrate_common>();
		mCommonView *commonView =  dynamic_pointer_cast<mCommonView>(_commonView).get();
		mModelView *modelView = dynamic_pointer_cast<mModelView>(_modelView).get();
		_lableRend_pre->initial(commonView, modelView);
		_lableRend_common->initial(commonView, modelView);
		//图标控制对象
		_lableRendController_pre = make_shared<mLableRendController_pre>(_lableRend_pre);
		connect(_lableRendController_pre.get(), SIGNAL(updateView()), this, SLOT(update()));
		_lableRendController_common = make_shared<mLableRendController_common>(_lableRend_common);
		connect(_lableRendController_common.get(), SIGNAL(updateView()), this, SLOT(update()));
	
		//_lableRendController_common->appendLableRendData<GlobalAxis>("globalAxis", QVector2D(75, 75), 1.3);

		qDebug() << "Pre Initial";

		GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}
	}

	void mPreRend::paintGL()
	{
		makeCurrent();
		mBaseRend3D::paintGL();
		//GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
		//if (error != 0)
		//{
		//	qDebug() << error;
		//}
	}

	void mPreRend::resizeGL(int w, int h)
	{
		makeCurrent();
		mBaseRend3D::resizeGL(w, h);
		////////图标测试////////////
		_lableRend_pre->getScreenSize(w, h);
		_lableRend_common->getScreenSize(w, h);
		///////////////////////////
	}
	void mPreRend::mousePressEvent(QMouseEvent *event)
	{
		mBaseRend3D::mousePressEvent(event);
	}
	void mPreRend::mouseReleaseEvent(QMouseEvent *event)
	{
		mBaseRend3D::mouseReleaseEvent(event);
	}
	void mPreRend::mouseMoveEvent(QMouseEvent *event)
	{
		mBaseRend3D::mouseMoveEvent(event);
	}
	void mPreRend::wheelEvent(QWheelEvent *event)
	{
		mBaseRend3D::wheelEvent(event);
	}

	mPreRend::~mPreRend()
	{
		qDebug() << "Pre Distruct";
	}
	shared_ptr<mPreRender> mPreRend::getPreRender()
	{
		for (auto render : _renderArray)
		{
			auto pre = dynamic_pointer_cast<mPreRender>(render);
			if (pre)
			{
				return pre;
			}
		}
		return nullptr;
	}
	shared_ptr<mLableRendController_pre> mPreRend::getLableRendController_pre()
	{
		return _lableRendController_pre;
	}
	shared_ptr<mLableRendController_common> mPreRend::getLableRendController_common()
	{
		return _lableRendController_common;
	}
	shared_ptr<mLableDataController_pre> mPreRend::getLableDataController_pre()
	{
		return _lableRend_pre->getLableDataController();
	}
	void mPreRend::GetModelSizePara(bool isModelCenter)
	{
		Space::AABB aabb;
		if (!this->getPreRender()->getModelAABB(aabb))
		{
			aabb = Space::AABB();
		}

		//额外顶点数据
		//QVector<QVector3D> ExtraVertex = mTempHighLightData1::getInstance()->getHighLightPointVertex() + MDataMesh::mMeshModelData1::getInstance()->getTempNodeVertex()
		//	+ _lableRend_pre->getLableDataController()->getExtraVertex() + _lableRend_common->getLableDataController()->getExtraVertex();
		//if (!ExtraVertex.isEmpty())
		//{
		//	float left_extra = 0, right_extra = 0, bottom_extra = 0, top_extra = 0, back_extra = 0, front_extra = 0;
		//	//获取额外顶点数据的最大参数
		//	left_extra = right_extra = ExtraVertex.at(0).x();
		//	bottom_extra = top_extra = ExtraVertex.at(0).y();
		//	back_extra = front_extra = ExtraVertex.at(0).z();
		//	for (int i = 0; i < ExtraVertex.size(); i++)
		//	{
		//		left_extra = min(left_extra, ExtraVertex.at(i).x());
		//		right_extra = max(right_extra, ExtraVertex.at(i).x());
		//		bottom_extra = min(bottom_extra, ExtraVertex.at(i).y());
		//		top_extra = max(top_extra, ExtraVertex.at(i).y());
		//		back_extra = min(back_extra, ExtraVertex.at(i).z());
		//		front_extra = max(front_extra, ExtraVertex.at(i).z());
		//	}

		//	//获取最终的最大参数
		//	_left = min(left_mesh, left_extra);
		//	_right = max(right_mesh, right_extra);
		//	_bottom = min(bottom_mesh, bottom_extra);
		//	_top = max(top_mesh, top_extra);
		//	_back = min(back_mesh, back_extra);
		//	_front = max(front_mesh, front_extra);
		//}
		//else
		//{
		//	//获取最终的最大参数
		//	_left = left_mesh;
		//	_right = right_mesh;
		//	_bottom = bottom_mesh;
		//	_top = top_mesh;
		//	_back = back_mesh;
		//	_front = front_mesh;
		//}
		_aabb = aabb;
		//模型中心
		_center_model = (_aabb.maxEdge + _aabb.minEdge)/2.0;
		_maxRadius_model = _aabb.maxEdge.distanceToPoint(_aabb.minEdge);
		if (isinf(_maxRadius_model))
		{
			_maxRadius_model = _maxRadius_now;
		}
		if (isModelCenter)
		{
			_center_now = _center_model;
			_maxRadius_now = _maxRadius_model;
		}
		else
		{
			float t = _maxRadius_now;
			_maxRadius_now = _aabb.maxEdge.distanceToPoint(_center_now);
			if (isinf(_maxRadius_now))
			{
				_maxRadius_now = t;
			}
		}
	}
	void mPreRend::slotResetOrthoAndCamera()
	{
		GetModelSizePara(true);
		_modelView->ResetOrthoAndCamera(_center_model, _maxRadius_now);
		_modelView->SaveCurrentView();
		_commonView->SaveCurrentView();
		//_meshModelRulerRend->UpdateNum();
		update();
	}

	void mPreRend::slotUpdateOrthoAndCamera()
	{
		GetModelSizePara(false);
		_modelView->UpdateOrthoAndCamera(_maxRadius_model, _maxRadius_now);
		_modelView->SaveCurrentView();
		_commonView->SaveCurrentView();
		//_meshModelRulerRend->UpdateNum();
		//HideOrShowAllFont();
		update();

	}

	void mPreRend::slotUpdateOrthoAndCamera(QVector<QVector3D> addVertex)
	{
		GetModelSizePara(false);
		_modelView->UpdateOrthoAndCamera(_maxRadius_now, addVertex);
		_modelView->SaveCurrentView();
		_commonView->SaveCurrentView();
		//_meshModelRulerRend->UpdateNum();
		//HideOrShowAllFont();
		update();
	}
	void mPreRend::otherRend()
	{
		////////图标////////////
		_lableRend_pre->drawLable();
		_lableRend_common->drawLable();
		///////////////////////////
		GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
		if (error != 0)
		{
			qDebug() << error;
		}
	}
}