#include "mPreRender.h"
#include "mPreRend.h"
#include "mPreRendStatus.h"
#include "mPreGeoModelRender.h"
#include "mPreGeoHighLightRender.h"
#include "mPreMeshModelRender.h"
#include "mPreMeshHighLightRender.h"
#include "mFontRender.h"
#include "mArrowRender.h"
#include "mPreMeshPickData1.h"
#include "mPreMeshPickThread.h"

//MDataGeo
#include "mGeoPickData1.h"
#include "mPreGeoPickThread.h"

//BasicData
#include "MeshMessage.h"

#include <renderpch.h>
#include "texture.h"
#include "Space.h"

#include "mShaderManage.h"
#include "mTextureManage.h"

#include "mGeoModelData1.h"

//视图类
#include "mModelView.h"
#include "mViewBase.h"

#include <QMatrix3x3>
#include <QFileDialog>
#include <QFileInfo>
#include <QApplication>
#include <QtConcurrent/QtConcurrent>
#include <QThread>


#include <qmath.h>


using namespace mxr;
using namespace std;
namespace MPreRend
{
	mPreRender::mPreRender(std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, mPreRend *preRend):mBaseRender(app, parent, preRend)
	{
		this->makeCurrent();

		//QOpenGLContext *context = QOpenGLContext::currentContext();
		//qDebug() << "mPreRender" << QString::number(long long int(context), 16);

		_rendStatus = make_shared<mPreRendStatus>();
		_rendStatus->_showFunction = SmoothShaded;
		_pointTexture = mTextureManage::GetInstance()->GetTexture("GeoPoint.png", 3);
		_geoModelData = new mGeoModelData1();
		_geoModelRender = MakeAsset<mPreGeoModelRender>(parent, _rendStatus, _geoModelData);
		_meshModelRender = MakeAsset<mPreMeshModelRender>(parent, _rendStatus);

		/**********************************************************模型**********************************************************/
		//face
		_faceStateSet = MakeAsset<StateSet>();
		mxr::Shader *shader = mShaderManage::GetInstance()->GetShader("PreGeoMeshFace");
		_faceStateSet->setShader(shader);
		_faceStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_faceStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetFill>(0, 0), 0);
		_faceStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_faceStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_faceStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_faceStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_faceStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));

		//edgeline
		_edgelineStateSet = MakeAsset<StateSet>();
		shader = mShaderManage::GetInstance()->GetShader("PreGeoMeshEdgeLine");
		_edgelineStateSet->setShader(shader);
		_edgelineStateSet->setDrawMode(GL_LINES);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);
		_edgelineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);

		_edgelineStateSet->setUniform(MakeAsset<Uniform>("pvm", QMatrix4x4()));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("showColor", _rendStatus->_faceLineColor));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("rightToLeft", 0));
		_edgelineStateSet->setUniform(MakeAsset<Uniform>("lineWidth", 1.0f));

		//faceline
		_facelineStateSet = MakeAsset<StateSet>();
		shader = mShaderManage::GetInstance()->GetShader("PreMeshFaceLine");
		_facelineStateSet->setShader(shader);
		_facelineStateSet->setDrawMode(GL_TRIANGLES);
		_facelineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_facelineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(PolygonMode::FRONT_AND_BACK, PolygonMode::LINE), 1);
		_facelineStateSet->setAttributeAndModes(MakeAsset<PolygonOffsetLine>(-1, -1), 1);
		_facelineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 1);

		_facelineStateSet->setUniform(MakeAsset<Uniform>("pvm", QMatrix4x4()));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("showColor", _rendStatus->_faceLineColor));
		_facelineStateSet->setUniform(MakeAsset<Uniform>("rightToLeft", 0));

		//line
		_independentlineStateSet = MakeAsset<StateSet>();
		shader = mShaderManage::GetInstance()->GetShader("PreGeoMeshLine");
		_independentlineStateSet->setShader(shader);
		_independentlineStateSet->setDrawMode(GL_LINES);
		_independentlineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_independentlineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_independentlineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_independentlineStateSet->setUniform(MakeAsset<Uniform>("pvm", QMatrix4x4()));

		//dotline
		_dotlineStateSet = MakeAsset<StateSet>();
		shader = mShaderManage::GetInstance()->GetShader("PreGeoMeshDotLine");
		_dotlineStateSet->setShader(shader);
		_dotlineStateSet->setDrawMode(GL_LINES);
		_dotlineStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_dotlineStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_dotlineStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_dotlineStateSet->setUniform(MakeAsset<Uniform>("pvm", QMatrix4x4()));
		_dotlineStateSet->setUniform(MakeAsset<Uniform>("showColor", QVector3D(0.5,0.5,0.5)));

		//point
		_pointStateSet = MakeAsset<StateSet>();
		shader = mShaderManage::GetInstance()->GetShader("PreGeoPoint");
		_pointStateSet->setShader(shader);
		_pointStateSet->setDrawMode(GL_POINTS);
		_pointStateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_pointStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(), 1);
		_pointStateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);

		_pointStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_pointStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_pointStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_pointStateSet->setUniform(MakeAsset<Uniform>("PointSize", 10));
		_pointStateSet->setTexture("sprite_texture",_pointTexture);

		//node
		_nodeStateSet = MakeAsset<StateSet>();
		mxr::Shader * pointshader = mShaderManage::GetInstance()->GetShader("PostHighLightPoint");
		_nodeStateSet->setShader(pointshader);
		_nodeStateSet->setDrawMode(GL_POINTS);
		_nodeStateSet->setAttributeAndModes(MakeAsset<Depth>(), 0);
		_nodeStateSet->setAttributeAndModes(MakeAsset<PolygonMode>(mxr::PolygonMode::FRONT_AND_BACK, mxr::PolygonMode::FILL), 1);

		_nodeStateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_nodeStateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_nodeStateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_nodeStateSet->setUniform(MakeAsset<Uniform>("lightIsOn", int(1)));
		_nodeStateSet->setUniform(MakeAsset<Uniform>("viewPos", QVector3D()));
		_nodeStateSet->setUniform(MakeAsset<Uniform>("light.position", _rendStatus->_postLight.lightPosition));
		_nodeStateSet->setUniform(MakeAsset<Uniform>("light.ambient", _rendStatus->_postLight.ambient));
		_nodeStateSet->setUniform(MakeAsset<Uniform>("light.diffuse", _rendStatus->_postLight.diffuse));
		_nodeStateSet->setUniform(MakeAsset<Uniform>("light.specular", _rendStatus->_postLight.specular));
		_nodeStateSet->setUniform(MakeAsset<Uniform>("light.shiness", _rendStatus->_postLight.shiness));
		_nodeStateSet->setUniform(MakeAsset<Uniform>("PointSize", _rendStatus->_nodeSize));

		_geoModelRender->setFaceStateSet(_faceStateSet);
		_geoModelRender->setIndependentLineStateSet(_independentlineStateSet);
		_geoModelRender->setDotLineStateSet(_dotlineStateSet);
		_geoModelRender->setEdgeLineStateSet(_edgelineStateSet);
		_geoModelRender->setPointStateSet(_pointStateSet);

		_meshModelRender->setFaceStateSet(_faceStateSet);
		_meshModelRender->setEdgeLineStateSet(_edgelineStateSet);
		_meshModelRender->setFaceLineStateSet(_facelineStateSet);
		_meshModelRender->setLineStateSet(_independentlineStateSet);
		_meshModelRender->setPointStateSet(_pointStateSet);
		_meshModelRender->setNodeStateSet(_nodeStateSet);

		//初始化高亮渲染
		_geoPickData = new mGeoPickData1();
		_geoPickThread = new mPreGeoPickThread(_geoModelData, _geoPickData);
		_geoPickThread->setPickFilter(_baseRend->getPickFilter());
		_geoHighLightRender = make_shared<mPreGeoHighLightRender>(_parent ,_rendStatus, _geoPickData, _geoModelData);

		_meshPickData = new mPreMeshPickData1();
		_meshPickThread = new mPreMeshPickThread(_meshPickData);
		_meshPickThread->setPickFilter(_baseRend->getPickFilter());
		_meshHighLightRender = make_shared<mPreMeshHighLightRender>(_parent, _rendStatus, _meshPickData);

		//this->doneCurrent();
	}
	bool mPreRender::getIsDragSomething(QVector2D pos)
	{
		//判断是否有物体被拖拽
		return false;
	}
	void mPreRender::dragSomething(QVector2D pos)
	{
		//拖拽物体并更新物体
	}
	void mPreRender::startPick(QVector<QVector2D> poses)
	{
		if (*_baseRend->getCurrentPickMode() == PickMode::MultiplyPick && *_baseRend->getMultiplyPickMode() == MultiplyPickMode::NoPick)
		{
			return;
		}
		makeCurrent();
		QFuture<void> future;
		switch (*_baseRend->getPickFilter())
		{
		case PickFilter::PickAny:
		case PickFilter::PickPoint:
		case PickFilter::Pick1DMesh:
		case PickFilter::Pick2DMesh:
		case PickFilter::PickTri:
		case PickFilter::PickQuad:
		case PickFilter::PickTet:
		case PickFilter::PickWedge:
		case PickFilter::PickHex:
		case PickFilter::PickPyramid:
		case PickFilter::PickNode:
		case PickFilter::PickNodePath:
		case PickFilter::PickNodeOrder:
		case PickFilter::PickAnyMesh:
		case PickFilter::PickMeshLine:
		case PickFilter::PickMeshFace:
		case PickFilter::PickNodeByPart:
		case PickFilter::PickAnyMeshByPart:
		case PickFilter::PickMeshLineByPart:
		case PickFilter::PickMeshFaceByPart:
		case PickFilter::PickNodeByLine:
		case PickFilter::PickMeshByLine:
		case PickFilter::PickMeshLineByLine:
		case PickFilter::PickNodeByFace:
		case PickFilter::PickMeshFaceByFace:
		case PickFilter::PickMeshByFace:
		case PickFilter::PickMeshPart:
		case PickFilter::PickNodeByLineAngle:
		case PickFilter::PickNodeByFaceAngle:
		case PickFilter::Pick1DMeshByAngle:
		case PickFilter::Pick2DMeshByAngle:
		case PickFilter::PickMeshLineByAngle:
		case PickFilter::PickMeshFaceByAngle:
		//网格
		{
			_meshPickData->setMeshPickFunction(int(_baseRend->getPickFuntion()));
			_meshPickThread->setMatrix(_baseRend->getCamera()->getPVMValue());
			_meshPickThread->setWidget(_baseRend->getCamera()->SCR_WIDTH, _baseRend->getCamera()->SCR_HEIGHT);
			_meshPickThread->setPickMode(*_baseRend->getCurrentPickMode(), *_baseRend->getMultiplyPickMode());
			_meshPickThread->setPickAngleValue(_baseRend->getPickAngle());
			if (*_baseRend->getCurrentPickMode() == PickMode::SoloPick)
			{
				float depth = this->getDepth(poses.first());
				GLenum error = QOpenGLContext::currentContext()->functions()->glGetError();
				if (error != 0)
				{
					qDebug() << error;
				}
				_meshPickThread->setLocation(poses.first(), depth);
			}
			else
			{
				_meshPickThread->setLocation(poses, (_baseRend->getCamera()->_Center - _baseRend->getCamera()->_Eye).normalized());
			}

			future = QtConcurrent::run(_meshPickThread, &mPreMeshPickThread::startPick);
			QObject::connect(&w, &QFutureWatcher<void>::finished, [this] {
				_meshHighLightRender->updateHighLightRender();
				QObject::disconnect(&w, 0, 0, 0);//断开信号
				emit finishedPickSig();
				emit update();
			});
			w.setFuture(future);
		}
		break;
		case PickFilter::PickGeoPoint:
		case PickFilter::PickGeoPointOrder:
		case PickFilter::PickGeoLine:
		case PickFilter::PickGeoFace:
		case PickFilter::PickGeoSolid:
		case PickFilter::PickGeoPointByPart:
		case PickFilter::PickGeoLineByPart:
		case PickFilter::PickGeoFaceByPart:
		case PickFilter::PickGeoSolidByPart:
		case PickFilter::PickGeoPart:
		case PickFilter::pickVertexOnGeoLine:
		case PickFilter::pickVertexOnGeoFace:
		case PickFilter::pickVertexOnScreen:
		//几何
		{
			//开始拾取操作
			if (!_geoModelRender)
			{
				return;
			}
			_geoPickData->setGeoPickFunction(int(_baseRend->getPickFuntion()));
			_geoPickThread->setMatrix(_baseRend->getCamera()->getPVMValue());
			_geoPickThread->setWidget(_baseRend->getCamera()->SCR_WIDTH, _baseRend->getCamera()->SCR_HEIGHT);
			_geoPickThread->setPickMode(*_baseRend->getCurrentPickMode(), *_baseRend->getMultiplyPickMode());
			if (*_baseRend->getCurrentPickMode() == PickMode::SoloPick)
			{			
				float depth = this->getDepth(poses.first());
				_geoPickThread->setLocation(poses.first(), depth);
			}
			else
			{
				_geoPickThread->setLocation(poses, (_baseRend->getCamera()->_Center - _baseRend->getCamera()->_Eye).normalized());
			}
			
			future = QtConcurrent::run(_geoPickThread, &mPreGeoPickThread::startPick);
			QObject::connect(&w, &QFutureWatcher<void>::finished, [this] {
				_geoHighLightRender->updateHighLightRender();
				QObject::disconnect(&w, 0, 0, 0);//断开信号
				emit finishedPickSig();
				emit update();
			});
			w.setFuture(future);
		}
		break;
		default:
			return;
		}
		

	}

	bool mPreRender::getModelAABB(Space::AABB &aabb)
	{
		bool hasModel = false;
		if (_geoModelData->isShowModel())
		{
			aabb.push(_geoModelData->getModelSize());
			hasModel = true;
		}
		if (MeshMessage::getInstance()->getAllPartNames().size() > 0)
		{
			auto model = _meshModelRender->getModelAABB();
			aabb.push(model);
			hasModel = true;
		}
		return hasModel;
	}

	void mPreRender::updateHighLightRender()
	{
		this->makeCurrent();
		if (_geoHighLightRender)
			_geoHighLightRender->updateHighLightRender();
		if (_meshHighLightRender)
			_meshHighLightRender->updateHighLightRender();
		emit update();
	}
	void mPreRender::updateModelOperate(QPair<ModelOperateEnum, std::set<QString>> modelOperates)
	{
		this->makeCurrent();
		if (_geoModelRender)
		{
			_geoModelRender->updateModelOperate(modelOperates);
		}
	}
	void mPreRender::clearRender()
	{
		this->makeCurrent();
		//_dataPost.reset();
		//QOpenGLContext *context = QOpenGLContext::currentContext();
		//qDebug() << "deletecontext" << QString::number(long long int(context), 16);
		//if (_oneFrameRender)
		//{
		//	_oneFrameRender->deleteThieFrame();
		//	_oneFrameRender.reset();
		//}
		//deleteAnimation();
		_geoModelData->deleteAllGeoPart();
		_baseRend->getFontRender()->clearAllRender();
		_baseRend->getArrowRender()->clearAllRender();
	}
	
	void mPreRender::setShowFuntion(ShowFuntion showFuntion)
	{
		this->makeCurrent();
		_rendStatus->_showFunction = showFuntion;
		if (_meshModelRender)
		{
			_meshModelRender->setShowFuntion(showFuntion);
		}
	}

	void mPreRender::setNodeSize(int size)
	{
		_rendStatus->_nodeSize = size;

	}

	void mPreRender::setPointSize(int size)
	{
		_rendStatus->_pointSize = size;
		_pointStateSet->getUniform("PointSize")->SetData(_rendStatus->_pointSize);
	}

	void mPreRender::setEdgeLineWidth(float size)
	{
		_rendStatus->_lineWidth = size;
		_edgelineStateSet->getUniform("lineWidth")->SetData(_rendStatus->_lineWidth);
	}

	void mPreRender::setFaceLineColor(QVector4D color)
	{
		_rendStatus->_faceLineColor = color;
		_facelineStateSet->getUniform("showColor")->SetData(_rendStatus->_faceLineColor);
	}

	mPreRender::~mPreRender()
	{
		this->makeCurrent();
		_geoModelRender.reset();
		_meshModelRender.reset();
	}

	void mPreRender::updateUniform(shared_ptr<mViewBase> modelView, shared_ptr<mViewBase> commonView)
	{
		//检测模型数据更新模型渲染
		bool isUpdateCamera{ false };
		if (_geoModelRender)
		{
			isUpdateCamera = isUpdateCamera | _geoModelRender->updateRender();
		}
		if (_meshModelRender)
		{
			isUpdateCamera = isUpdateCamera | _meshModelRender->updateRender();
		}
		if (isUpdateCamera)
		{
			if (MeshMessage::getInstance()->IsReadFileMark() || _geoModelData->isResetCamera())
			{
				_baseRend->slotResetOrthoAndCamera();
				mBaseRend3D *rend = dynamic_cast<mBaseRend3D*>(_baseRend);
				if (rend)
				{
					rend->FitView();
					rend->SetRotateCenterToModelCenter();
				}
				MeshMessage::getInstance()->setReadFileMark(false);
				_geoModelData->setResetCamera(false);
			}
			else
			{
				_baseRend->slotUpdateOrthoAndCamera();
			}
		}

		if (_faceStateSet)
		{
			QMatrix4x4 pvm = modelView->getPVMValue();
			_faceStateSet->getUniform("projection")->SetData(modelView->_projection);
			_faceStateSet->getUniform("view")->SetData(modelView->_view);
			_faceStateSet->getUniform("model")->SetData(modelView->_model);
			_edgelineStateSet->getUniform("pvm")->SetData(pvm);
			_facelineStateSet->getUniform("pvm")->SetData(pvm);
			_independentlineStateSet->getUniform("pvm")->SetData(pvm);
			_dotlineStateSet->getUniform("pvm")->SetData(pvm);
			_pointStateSet->getUniform("projection")->SetData(modelView->_projection);
			_pointStateSet->getUniform("view")->SetData(modelView->_view);
			_pointStateSet->getUniform("model")->SetData(modelView->_model);			

			_faceStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			_pointStateSet->getUniform("viewPos")->SetData(modelView->_Eye);
			if (_rendStatus->_lightIsDependOnCamera)
			{
				_faceStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
				_pointStateSet->getUniform("light.position")->SetData(2 * modelView->_Eye - modelView->_Center);
			}
			else
			{
				_faceStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
				_pointStateSet->getUniform("light.position")->SetData(_rendStatus->_postLight.lightPosition);
			}

			_facelineStateSet->getUniform("rightToLeft")->SetData(float(modelView->_Right - modelView->_Left));
			_edgelineStateSet->getUniform("rightToLeft")->SetData(float(modelView->_Right - modelView->_Left));
		}
	
		_geoHighLightRender->updateUniform(modelView, commonView);
		_meshHighLightRender->updateUniform(modelView, commonView);
	}
}