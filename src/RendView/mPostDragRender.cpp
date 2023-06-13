#include "mPostDragRender.h"
#include "mPostRendStatus.h"

#include "mViewBase.h"

#include <renderpch.h>

#include "mShaderManage.h"

namespace MPostRend
{
	mPostDragRender::mPostDragRender(QString name, std::shared_ptr<mxr::Application> app, std::shared_ptr<mxr::Group> parent, std::shared_ptr<mPostRendStatus> rendStatus)
	{
		_app = app; 
		_parent = parent;
		_drawable = nullptr;
		_stateSet = nullptr;
		_rendStatus = rendStatus;
	};

	void mPostDragRender::setIsShow(bool isShow)
	{
		if (_drawable)
		{
			_drawable->setNodeMask(!isShow);
		}
	}

	void mPostDragRender::updateUniform(std::shared_ptr<MViewBasic::mViewBase> modelView)
	{

	}

	mPostSphereRender::mPostSphereRender(QString name, std::shared_ptr<Application> app, std::shared_ptr<Group> parent, std::shared_ptr<mPostRendStatus> rendStatus):mPostDragRender(name, app, parent, rendStatus)
	{
		_stateSet = MakeAsset<StateSet>();

		if (_sphereVertexs.empty())
		{
			_sphereVertexs.clear();
			_sphereIndexs.clear();
			int xsegment = 30;
			int ysegment = 30;
			//生成球的顶点
			float PI = 3.1415926;
			for (int y = 0; y <= xsegment; y++)
			{
				for (int x = 0; x <= ysegment; x++)
				{
					float xSegment = (float)x / (float)xsegment;
					float ySegment = (float)y / (float)ysegment;
					float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
					float yPos = std::cos(ySegment * PI);
					float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
					_sphereVertexs.push_back(QVector3D(xPos, yPos, zPos));
				}
			}

			//生成球的Indices
			for (int i = 0; i < xsegment; i++)
			{
				for (int j = 0; j < ysegment; j++)
				{
					_sphereIndexs.push_back(i * (xsegment + 1) + j);
					_sphereIndexs.push_back((i + 1) * (xsegment + 1) + j);
					_sphereIndexs.push_back((i + 1) * (xsegment + 1) + j + 1);
					_sphereIndexs.push_back(i* (xsegment + 1) + j);
					_sphereIndexs.push_back((i + 1) * (xsegment + 1) + j + 1);
					_sphereIndexs.push_back(i * (xsegment + 1) + j + 1);
				}
			}
		}

		if (_drawable)
		{
			_parent->removeChild(_drawable);
		}

		_drawable = MakeAsset<Drawable>();
		_drawable->setStateSet(_stateSet);

		rendStatus->_sphereCenter = QVector3D(0, 0, 0);
		rendStatus->_sphereRadius = 0;

		_drawable = MakeAsset<Drawable>();
		_drawable->setVertexAttribArray(0, MakeAsset<Vec3Array>(_sphereVertexs));
		_drawable->setVertexAttribArray(1, MakeAsset<FloatArray>(QVector<float>(_sphereVertexs.size(), 1)));
		_drawable->setIndexAttribute(MakeAsset<UIntArray>(_sphereIndexs));
		Shader *shader = mShaderManage::GetInstance()->GetShader("Common");
		_stateSet = MakeAsset<StateSet>();
		_stateSet->setShader(shader);
		_stateSet->setAttributeAndModes(MakeAsset<Depth>(), 1);
		_stateSet->setAttributeAndModes(MakeAsset<BlendFunc>(), 0);
		_stateSet->setAttributeAndModes(MakeAsset<PolygonMode>(PolygonMode::FRONT_AND_BACK, PolygonMode::LINE), 1);
		_stateSet->setUniform(MakeAsset<Uniform>("projection", QMatrix4x4()));
		_stateSet->setUniform(MakeAsset<Uniform>("view", QMatrix4x4()));
		_stateSet->setUniform(MakeAsset<Uniform>("model", QMatrix4x4()));
		_stateSet->setUniform(MakeAsset<Uniform>("showColor", QVector4D()));
		_drawable->setStateSet(_stateSet);

		_parent->addChild(_drawable);
	}
	mPostSphereRender::~mPostSphereRender()
	{
		if (_drawable)
		{
			_parent->removeChild(_drawable);
		}
	}

	void mPostSphereRender::setSphereData(QVector3D center, float radius)
	{
		_rendStatus->_sphereCenter = center;
		_rendStatus->_sphereRadius = radius;
	}

	bool mPostSphereRender::pointIsIn(QVector2D pos, float depth, QMatrix4x4 pvm, int w, int h)
	{
		QVector4D centerPos = (pvm * QVector4D(_rendStatus->_sphereCenter, 1.0));//求出球心世界坐标的乘以矩阵后的坐标（范围为-1~1）
		_centerDepth = centerPos.z();//求出球心的深度

		QVector4D res;
		res.setX(2 * pos.x() / w - 1);
		res.setY(1 - 2 * pos.y() / h);//求出鼠标点击的地方的坐标（换算为-1~1）

		//通过逆矩阵求出鼠标点击的地方的世界坐标
		QVector3D WorldPos = pvm.inverted() * QVector4D(res.x(), res.y(), _centerDepth, 1.0).toVector3D();//算出坐标

		//比较鼠标点击的地方的世界坐标与球心的世界坐标的距离是否小于球心的半径
		if (WorldPos.distanceToPoint(_rendStatus->_sphereCenter) < _rendStatus->_sphereRadius)
		{
			_relativePosition = (res - centerPos).toVector2D();//求鼠标的位置相对于球心的位置(x，y代表位置)
			return true;
		}
		return false;
	}

	void mPostSphereRender::move(QVector2D pos, QMatrix4x4 pvm, int w, int h)
	{
		QVector4D res;
		res.setX(2 * pos.x() / w - 1);
		res.setY(1 - 2 * pos.y() / h);//求出鼠标点击的地方的坐标（换算为-1~1）

		//通过拖拽点和球心的相对位置来计算出球心的坐标（范围为-1~1）
		QVector2D centerPos = res.toVector2D() - _relativePosition;

		//通过逆矩阵换算出球心的世界坐标
		QVector3D WorldPos = pvm.inverted() * QVector4D(centerPos.x(), centerPos.y(), _centerDepth, 1.0).toVector3D();//算出坐标

		_rendStatus->_sphereCenter = WorldPos;
	}

	void mPostSphereRender::updateUniform(std::shared_ptr<MViewBasic::mViewBase> modelView)
	{
		if (_stateSet)
		{
			_stateSet->getUniform("projection")->SetData(modelView->_projection);
			_stateSet->getUniform("view")->SetData(modelView->_view);
			QMatrix4x4 model = modelView->_model;
			model.translate(_rendStatus->_sphereCenter);
			model.scale(_rendStatus->_sphereRadius);
			_stateSet->getUniform("model")->SetData(model);
		}
	}

	QVector<QVector3D> mPostSphereRender::_sphereVertexs;
	QVector<uint> mPostSphereRender::_sphereIndexs;
}