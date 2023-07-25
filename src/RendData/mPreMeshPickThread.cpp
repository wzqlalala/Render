#include "mPreMeshPickThread.h"
#include "mPreMeshPickData1.h"

#include "mMeshModelData.h"
#include "mMeshData.h"
#include "mMeshFaceData.h"
#include "mMeshPartData.h"
#include "mMeshNodeData.h"

//MViewBasic
#include "mMeshViewEnum.h"
#include "mPickToolClass.h"

//Qt
#include <QVector2D>
#include <QVector4D>
#include <QString>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <qmath.h>

using namespace MViewBasic;
namespace MDataMesh
{
	QMutex pickMutex;

	QVector2D mBasePick::WorldvertexToScreenvertex(QVector3D Worldvertex)
	{
		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_width, _height - (res.y() / res.w() / 2.0 + 0.5)*_height);
	}

	QVector<QVector2D> mBasePick::WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs)
	{
		QVector<QVector2D> vertexs;
		for (auto Worldvertex : Worldvertexs)
		{
			vertexs.append(WorldvertexToScreenvertex(Worldvertex));
		}
		return vertexs;
	}

	QVector<QVector2D> mBasePick::getAABBToScreenVertex(QVector3D minEdge, QVector3D maxEdge)
	{
		QVector<QVector2D> ap;
		ap.append(WorldvertexToScreenvertex(QVector3D(minEdge.x(), minEdge.y(), minEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(minEdge.x(), minEdge.y(), maxEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(minEdge.x(), maxEdge.y(), maxEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(minEdge.x(), maxEdge.y(), minEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(maxEdge.x(), minEdge.y(), minEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(maxEdge.x(), minEdge.y(), maxEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(maxEdge.x(), maxEdge.y(), maxEdge.z())));
		ap.append(WorldvertexToScreenvertex(QVector3D(maxEdge.x(), maxEdge.y(), minEdge.z())));
		return ap;
	}

	QVector3D mBasePick::ScreenvertexToWorldvertex(QVector3D vertex)
	{
		QMatrix4x4 i_mvp = (_pvm).inverted();
		float ndc_x = 2.0*vertex.x() / (float)_width - 1.0f;
		float ndc_y = 1.0f - (2.0f*vertex.y() / (float)_height);
		float ndc_z = vertex.z() * 2 - 1.0;

		QVector4D world = i_mvp * QVector4D(ndc_x, ndc_y, ndc_z, 1.0);
		return QVector3D(world.x() / world.w(), world.y() / world.w(), world.z() / world.w());
	}

	QVector2D mBasePick::WorldvertexToScreenvertex(QVector3D Worldvertex, float& depth)
	{
		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
		depth = res.z();
		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_width, _height - (res.y() / res.w() / 2.0 + 0.5)*_height);
	}

	void mBasePick::WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, set<float>& depths)
	{
		for (auto Worldvertex : Worldvertexs)
		{
			float depth;
			Screenvertexs.append(WorldvertexToScreenvertex(Worldvertex, depth));
			depths.insert(depth);
		}
	}

	bool mQuadPick::getPickIsIntersectionWithAABB(AABB aabb)
	{
		QVector<QVector2D> ap = getAABBToScreenVertex(aabb.minEdge, aabb.maxEdge);
		if (!mPickToolClass::IsLineIntersectionWithQuad(ap, _multiQuad, MeshHex) && !mPickToolClass::IsPointInQuad(ap, _center, _boxXY_2))
		{
			for (auto point : _multiQuad)
			{
				if (mPickToolClass::IsPointInMesh(point, ap, MeshHex))
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}

	bool mQuadPick::get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter)
	{
		if (mPickToolClass::IsPointInQuad(WorldvertexToScreenvertex(pointCenter), _center, _boxXY_2))
		{
			return true;
		}
		return false;
	}

	bool mQuadPick::get1DMeshIsInPick(QVector<QVector3D> vertexs)
	{
		QVector<QVector2D> tempQVector2D = WorldvertexToScreenvertex(vertexs);
		if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, _multiQuad, MeshBeam) || mPickToolClass::IsPointInQuad(tempQVector2D, _center, _boxXY_2))
		{
			return true;
		}
		return false;
	}

	bool mQuadPick::isIntersectionAABBAndPick(QVector<QVector2D> ap)
	{
		if (!mPickToolClass::IsLineIntersectionWithQuad(ap, _multiQuad, MeshHex) && !mPickToolClass::IsPointInQuad(ap, _center, _boxXY_2))
		{
			for (auto point : _multiQuad)
			{
				if (mPickToolClass::IsPointInMesh(point, ap, MeshHex))
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}

	bool mQuadPick::isAABBPointIsAllInPick(QVector<QVector2D> ap)
	{
		if (mPickToolClass::IsAllPointInQuad(ap, _center, _boxXY_2))//包含
		{
			return true;
		}
		return false;
	}

	bool mPolygonPick::getPickIsIntersectionWithAABB(AABB aabb)
	{
		QVector<QVector2D> ap = getAABBToScreenVertex(aabb.minEdge, aabb.maxEdge);
		if (!mPickToolClass::IsLineIntersectionWithQuad(ap, _multiQuad, MeshHex) && !mPickToolClass::IsPointInPolygon(ap, _center, _multiQuad))
		{
			for (auto point : _multiQuad)
			{
				if (mPickToolClass::IsPointInMesh(point, ap, MeshHex))
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}

	bool mPolygonPick::get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter)
	{
		if (mPickToolClass::IsPointInPolygon(WorldvertexToScreenvertex(pointCenter), _center, _multiQuad))
		{
			return true;
		}
		return false;
	}

	bool mPolygonPick::get1DMeshIsInPick(QVector<QVector3D> vertexs)
	{
		QVector<QVector2D> tempQVector2D = WorldvertexToScreenvertex(vertexs);
		if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, _multiQuad, MeshBeam) || mPickToolClass::IsPointInPolygon(tempQVector2D, _center, _multiQuad))
		{
			return true;
		}
		return false;
	}

	bool mPolygonPick::isIntersectionAABBAndPick(QVector<QVector2D> ap)
	{
		if (!mPickToolClass::IsLineIntersectionWithQuad(ap, _multiQuad, MeshHex) && !mPickToolClass::IsPointInPolygon(ap, _center, _multiQuad))
		{
			for (auto point : _multiQuad)
			{
				if (mPickToolClass::IsPointInMesh(point, ap, MeshHex))
				{
					return true;
				}
			}
			return false;
		}
		return true;
	}

	bool mPolygonPick::isAABBPointIsAllInPick(QVector<QVector2D> ap)
	{
		if (mPickToolClass::IsAllPointInPolygon(ap, _center, _multiQuad))
		{
			return true;
		}
		return false;
	}

	bool mRoundPick::getPickIsIntersectionWithAABB(AABB aabb)
	{
		QVector<QVector2D> ap = getAABBToScreenVertex(aabb.minEdge, aabb.maxEdge);
		if (mPickToolClass::IsPointInRound(ap, _screenCenter, _screenRadius) || mPickToolClass::IsAABBIntersectionWithCircle(ap, _screenCenter, _screenRadius))
		{
			return true;
		}
		return false;
	}

	bool mRoundPick::get2DAnd3DMeshCenterIsInPick(QVector3D pointCenter)
	{
		if (mPickToolClass::IsPointInRound(pointCenter, _centerPoint, _centerDirection, _radius))
		{
			return true;
		}
		return false;
	}

	bool mRoundPick::get1DMeshIsInPick(QVector<QVector3D> vertexs)
	{
		QVector<QVector2D> tempQVector2D = WorldvertexToScreenvertex(vertexs);
		if (mPickToolClass::IsLineIntersectionWithRound(tempQVector2D.first(), tempQVector2D.last(), _screenCenter, _screenRadius))
		{
			return true;
		}
		return false;
	}

	bool mRoundPick::isIntersectionAABBAndPick(QVector<QVector2D> ap)
	{
		if (mPickToolClass::IsPointInRound(ap, _screenCenter, _screenRadius) || mPickToolClass::IsAABBIntersectionWithCircle(ap, _screenCenter, _screenRadius))
		{
			return true;
		}
		return false;
	}

	bool mRoundPick::isAABBPointIsAllInPick(QVector<QVector2D> ap)
	{
		if (mPickToolClass::IsAllPointInRound(ap, _screenCenter, _screenRadius))
		{
			return true;
		}
		return false;
	}


	mPreMeshPickThread::mPreMeshPickThread(mPreMeshPickData1 *pickData)
	{
		_isfinished = false;
		_pickData = pickData;
		_pickAngleValue = 60;

	}

	mPreMeshPickThread::~mPreMeshPickThread()
	{
	}

	void mPreMeshPickThread::setPickMode(PickMode pickMode, MultiplyPickMode multiplyPickMode)
	{
		_pickMode = pickMode;
		_multiplyPickMode = multiplyPickMode;
	}

	void mPreMeshPickThread::setPickFilter(PickFilter * pickFilter)
	{
		_pickFilter = pickFilter;
	}

	void mPreMeshPickThread::startPick()
	{
		_isfinished = false;
		QVector<QFuture<void>> futures;
		auto partNames = _meshModelData->getAllPartNameList();
		if (_pickMode == PickMode::SoloPick)
		{
			for (auto partName : partNames)
			{
				futures.append(QtConcurrent::run(this, &mPreMeshPickThread::doSoloPick, partName));
			}
			while (!futures.empty())
			{
				futures.back().waitForFinished();
				futures.takeLast();
			}
			if (*_pickFilter == PickFilter::PickNodeOrder)
			{
				_pickData->setSoloOrderPickData();
			}
			else if (*_pickFilter == PickFilter::PickNodePath)
			{
				int size = _pickData->getPickNodeIDsOrder().size();
				_pickData->setSoloOrderPickData();
				int size1 = _pickData->getPickNodeIDsOrder().size();
				if (size == size1)//没有拾取到
				{
					return;
				}
				else if (size == (size1 + 1) || size1 == 1)//减选或者第一个点
				{
					return;
				}
				else//加选
				{
					//做后续的拾取
					QtConcurrent::run(this, &mPreMeshPickThread::SoloPickNodePath).waitForFinished();
				}
			}
			else
			{
				_pickData->setSoloPickData();
				if (*_pickFilter == PickFilter::PickNodeByLineAngle || *_pickFilter == PickFilter::PickNodeByFaceAngle || *_pickFilter == PickFilter::Pick2DMeshByAngle || *_pickFilter == PickFilter::Pick1DMeshByAngle
					|| *_pickFilter == PickFilter::PickMeshLineByAngle || *_pickFilter == PickFilter::PickMeshFaceByAngle || *_pickFilter == PickFilter::PickNodePath)
				{
					//做后续的拾取
					QtConcurrent::run(this, &mPreMeshPickThread::doAnglePick).waitForFinished();
				}
			}
		}
		else
		{
			for (auto partName : partNames)
			{
				futures.append(QtConcurrent::run(this, &mPreMeshPickThread::doMultiplyPick, partName));
			}
			while (!futures.empty())
			{
				futures.back().waitForFinished();
				futures.takeLast();
			}
		}
	}

	void mPreMeshPickThread::setMatrix(QMatrix4x4 projection, QMatrix4x4 view, QMatrix4x4 model)
	{
		_projection = projection;
		_view = view;
		_model = model;
	}

	void mPreMeshPickThread::setMatrix(QMatrix4x4 pvm)
	{
		_pvm = pvm;
	}

	void MDataMesh::mPreMeshPickThread::setMeshModelData(std::shared_ptr<mMeshModelData> meshModelData)
	{
		_meshModelData = meshModelData;
	}

	void mPreMeshPickThread::setLocation(const QVector2D& pos, float depth)
	{
		_pos = pos;
		soloQuad = QVector<QVector2D>{ QVector2D(pos.x() + 5,pos.y() + 5),QVector2D(pos.x() + 5,pos.y() - 5),QVector2D(pos.x() - 5,pos.y() - 5),QVector2D(pos.x() - 5,pos.y() + 5) };
		_depth = depth;
		_p = ScreenvertexToWorldvertex(QVector3D(pos, depth));
		_origin = ScreenvertexToWorldvertex(QVector3D(pos, -1.0));
		_dir = (_p - _origin).normalized();
		//_pickSoloOrMutiply = MViewBasic::SoloPick;
	}

	void mPreMeshPickThread::setLocation(QVector<QVector2D> pickQuad, QVector3D direction)
	{
		if (pickQuad.size() < 1)
		{
			return;
		}
		//multiQuad = QVector<QVector2D>{ pickQuad.first(), QVector2D(pickQuad.first().x(), pickQuad.last().y()), pickQuad.last(), QVector2D(pickQuad.last().x(), pickQuad.first().y()) };
		//_centerBox = (pickQuad.first() + pickQuad.last()) / 2.0;
		//_boxXY_2 = pickQuad.first() - _centerBox; _boxXY_2[0] = qAbs(_boxXY_2[0]); _boxXY_2[1] = qAbs(_boxXY_2[1]);	
		//_centerX = centerX;
		//_centerY = centerY;
		//_boxW = boxW;
		//_boxY = boxY;
		//multiQuad = pickQuad;
		switch (_multiplyPickMode)
		{
		case MViewBasic::MultiplyPickMode::QuadPick:_pick = make_shared<mQuadPick>(_pvm, _Win_WIDTH, _Win_HEIGHT, pickQuad);
			break;
		case MViewBasic::MultiplyPickMode::PolygonPick:_pick = make_shared<mPolygonPick>(_pvm, _Win_WIDTH, _Win_HEIGHT, pickQuad);
			break;
		case MViewBasic::MultiplyPickMode::RoundPick:_pick = make_shared<mRoundPick>(_pvm, _Win_WIDTH, _Win_HEIGHT, pickQuad.first(), pickQuad.last(), direction);
			break;
		default:
			break;
		}

		//if (_multiplyPickMode == MultiplyPickMode::RoundPick)
		//{
		//	_centerScreenPoint = _centerBox;
		//	QVector3D centerPoint = (_pvm.inverted() * QVector4D(2 * pickQuad.last().x() / _Win_WIDTH - 1, 1 - 2 * pickQuad.last().y() / _Win_HEIGHT, 0.0, 1.0)).toVector3D();//算出圆心射线坐标
		//	_centerPoint = (_pvm.inverted() * QVector4D(2 * _centerScreenPoint.x() / _Win_WIDTH - 1, 1 - 2 * _centerScreenPoint.y() / _Win_HEIGHT, 0.0, 1.0)).toVector3D();//算出圆上一点的坐标
		//	_radius = _centerPoint.distanceToPoint(centerPoint);
		//	_screenRadius = _centerBox.distanceToPoint(pickQuad.first());
		//	_centerDirection = direction;
		//}
		//_pickSoloOrMutiply = MViewBasic::MultiplyPick;
	}

	void mPreMeshPickThread::setWidget(int w, int h)
	{
		_Win_WIDTH = w;
		_Win_HEIGHT = h;
	}

	bool mPreMeshPickThread::isFinished()
	{
		return _isfinished;
	}

	void mPreMeshPickThread::setFinished()
	{
		_isfinished = false;
	}

	void mPreMeshPickThread::doSoloPick(QString partName)
	{
		//判断该部件是否存在碰撞
		//判断点选是否在部件的包围盒内
		QVector3D worldVertex = _p;
		auto part = _meshModelData->getMeshPartDataByPartName(partName);
		Space::AABB aabb(part->getMeshPartAABB());//部件包围盒
		if (aabb.ContainPoint(_p))
		{
			switch (*_pickFilter)
			{
			case PickFilter::PickNothing:; break;
			case PickFilter::PickNode:
			case PickFilter::PickNodeOrder:
			case PickFilter::PickNodePath:SoloPickNode(part); break;
			case PickFilter::PickPoint:SoloPickMeshTypeFilter(part, QVector<MeshType>{MeshPoint}); break;
			case PickFilter::Pick1DMesh:SoloPickMeshTypeFilter(part, QVector<MeshType>{MeshBeam}); break;
			case PickFilter::Pick2DMesh:SoloPickMeshTypeFilter(part, QVector<MeshType>{MeshTri, MeshQuad}); break;
			case PickFilter::PickTri:SoloPickMeshTypeFilter(part, QVector<MeshType>{MeshTri}); break;
			case PickFilter::PickQuad:SoloPickMeshTypeFilter(part, QVector<MeshType>{MeshQuad}); break;
			case PickFilter::PickTet:SoloPickMeshTypeFilter(part, QVector<MeshType>{MeshTet}); break;
			case PickFilter::PickPyramid:SoloPickMeshTypeFilter(part, QVector<MeshType>{MeshPyramid}); break;
			case PickFilter::PickWedge:SoloPickMeshTypeFilter(part, QVector<MeshType>{MeshWedge}); break;
			case PickFilter::PickHex:SoloPickMeshTypeFilter(part, QVector<MeshType>{MeshHex}); break;
			case PickFilter::PickAnyMesh:SoloPickAnyMesh(part); break;
			case PickFilter::PickMeshLine:SoloPickMeshLine(part); break;
			case PickFilter::PickMeshFace:SoloPickMeshFace(part); break;
			case PickFilter::PickMeshPart:SoloPickMeshPart(part); break;
			case PickFilter::PickNodeByPart:SoloPickNodeByPart(part); break;
			case PickFilter::PickAnyMeshByPart:SoloPickAnyMeshByPart(part); break;
			case PickFilter::PickMeshLineByPart:SoloPickMeshLineByPart(part); break;
			case PickFilter::PickMeshFaceByPart:SoloPickMeshFaceByPart(part); break;
			case PickFilter::PickNodeByLineAngle:SoloPickNodeByLineAngle(part); break;
			case PickFilter::PickNodeByFaceAngle:SoloPickNodeByFaceAngle(part); break;
			case PickFilter::Pick1DMeshByAngle:SoloPick1DMeshByAngle(part); break;
			case PickFilter::Pick2DMeshByAngle:SoloPick2DMeshByAngle(part); break;
			case PickFilter::PickMeshLineByAngle:SoloPickMeshLineByAngle(part); break;
			case PickFilter::PickMeshFaceByAngle:SoloPickMeshFaceByAngle(part); break;
			default:break;
			}
		}
	}

	void mPreMeshPickThread::doMultiplyPick(QString partName)
	{
		auto part = _meshModelData->getMeshPartDataByPartName(partName);
		Space::AABB aabb(part->getMeshPartAABB());//部件包围盒
		QVector<QVector2D> ap = _pick->getAABBToScreenVertex(aabb.minEdge, aabb.maxEdge);
		bool isAllIn{ false };
		bool isIntersetion{ false };
		if (_pick->isAABBPointIsAllInPick(ap))//拾取到整个部件
		{
			isAllIn = true; isIntersetion = true;
		}
		else if (_pick->isIntersectionAABBAndPick(ap))//有相交
		{
			isIntersetion = true;
		}
		if (isIntersetion)
		{
			switch (*_pickFilter)
			{
			case PickFilter::PickNothing:; break;
			case PickFilter::PickNode:MultiplyPickNode(part, isAllIn); break;
			case PickFilter::PickPoint:MultiplyPickMeshTypeFilter(part, QVector<MeshType>{MeshPoint}, isAllIn); break;
			case PickFilter::Pick1DMesh:MultiplyPickMeshTypeFilter(part, QVector<MeshType>{MeshBeam}, isAllIn); break;
			case PickFilter::Pick2DMesh:MultiplyPickMeshTypeFilter(part, QVector<MeshType>{MeshTri, MeshQuad}, isAllIn); break;
			case PickFilter::PickTri:MultiplyPickMeshTypeFilter(part, QVector<MeshType>{MeshTri}, isAllIn); break;
			case PickFilter::PickQuad:MultiplyPickMeshTypeFilter(part, QVector<MeshType>{MeshQuad}, isAllIn); break;
			case PickFilter::PickTet:MultiplyPickMeshTypeFilter(part, QVector<MeshType>{MeshTet}, isAllIn); break;
			case PickFilter::PickPyramid:MultiplyPickMeshTypeFilter(part, QVector<MeshType>{MeshPyramid}, isAllIn); break;
			case PickFilter::PickWedge:MultiplyPickMeshTypeFilter(part, QVector<MeshType>{MeshWedge}, isAllIn); break;
			case PickFilter::PickHex:MultiplyPickMeshTypeFilter(part, QVector<MeshType>{MeshHex}, isAllIn); break;
			case PickFilter::PickAnyMesh:MultiplyPickAnyMesh(part, isAllIn); break;
			case PickFilter::PickMeshLine:MultiplyPickMeshLine(part, isAllIn); break;
			case PickFilter::PickMeshFace:MultiplyPickMeshFace(part, isAllIn); break;
			case PickFilter::PickMeshPart:MultiplyPickMeshPart(part, isAllIn); break;
			default:break;
			}
		}
	}

	void mPreMeshPickThread::doAnglePick()
	{
		switch (*_pickFilter)
		{
		case PickFilter::PickNothing:break;
		case PickFilter::PickNodePath:SoloPickNodePath(); break;
		case PickFilter::PickNodeByLineAngle:SoloPickNodeByLineAngle(); break;
		case PickFilter::PickNodeByFaceAngle:SoloPickNodeByFaceAngle(); break;
		case PickFilter::Pick1DMeshByAngle:SoloPick1DMeshByAngle(); break;
		case PickFilter::PickMeshLineByAngle:SoloPickMeshLineByAngle(); break;
		case PickFilter::Pick2DMeshByAngle:SoloPick2DMeshByAngle(); break;
		case PickFilter::PickMeshFaceByAngle:SoloPickMeshFaceByAngle(); break;
			//case PickAnyPositionValue:SoloPickCuttingPlaneVertex(); break;
		default:break;
		}
	}


	bool mPreMeshPickThread::isMultiplyPickMeshPart(std::shared_ptr<mMeshPartData> partData)
	{
		//获取单元面

		QVector<shared_ptr<mMeshFaceData>> mfaces = partData->getMeshFaceData();
		for (auto mface : mfaces)
		{
			if (mface == nullptr)
			{
				continue;
			}
			QVector<QVector3D> vertexs = mface->getNodeVertex();
			if (_pick->get2DAnd3DMeshCenterIsInPick(getCenter(vertexs)))
			{
				return true;
			}

		}

		//获取网格
		QVector<shared_ptr<mMeshData>> meshs = partData->getMeshDatas1() + partData->getMeshDatas0() + partData->getMeshDatas2();
		for (auto mesh : meshs)
		{
			QVector<QVector3D> vertexs = mesh->getNodeVertex();
			switch (mesh->getMeshType())
			{
			case MeshBeam:
				if (_pick->get1DMeshIsInPick(vertexs))
				{
					return true;
				}
				break;
			case MeshTri:
			case MeshQuad:
			case MeshPoint:
				if (_pick->get2DAnd3DMeshCenterIsInPick(getCenter(vertexs)))
				{
					return true;
				}
				break;
			default:
				break;
			}

		}
		return false;
	}

	QVector2D mPreMeshPickThread::WorldvertexToScreenvertex(QVector3D Worldvertex)
	{
		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_Win_WIDTH, _Win_HEIGHT - (res.y() / res.w() / 2.0 + 0.5)*_Win_HEIGHT);
	}

	QVector3D mPreMeshPickThread::ScreenvertexToWorldvertex(QVector3D vertex)
	{
		QMatrix4x4 i_mvp = (_pvm).inverted();
		float ndc_x = 2.0*vertex.x() / (float)_Win_WIDTH - 1.0f;
		float ndc_y = 1.0f - (2.0f*vertex.y() / (float)_Win_HEIGHT);
		float ndc_z = vertex.z() * 2 - 1.0;

		QVector4D world = i_mvp * QVector4D(ndc_x, ndc_y, ndc_z, 1.0);
		return QVector3D(world.x() / world.w(), world.y() / world.w(), world.z() / world.w());
	}

	QVector2D mPreMeshPickThread::WorldvertexToScreenvertex(QVector3D Worldvertex, float& depth)
	{
		QVector4D res = _pvm * QVector4D(Worldvertex, 1.0);
		depth = res.z() + 0.5;
		return QVector2D(((res.x() / res.w()) / 2.0 + 0.5)*_Win_WIDTH, _Win_HEIGHT - (res.y() / res.w() / 2.0 + 0.5)*_Win_HEIGHT);
	}

	void mPreMeshPickThread::WorldvertexToScreenvertex(QVector<QVector3D> Worldvertexs, QVector<QVector2D> &Screenvertexs, set<float>& depths)
	{
		for (auto Worldvertex : Worldvertexs)
		{
			float depth;
			Screenvertexs.append(WorldvertexToScreenvertex(Worldvertex, depth));
			depths.insert(depth);
		}
	}
	QVector3D mPreMeshPickThread::getCenter(QVector<QVector3D> vertexs)
	{
		QVector3D nodePos;
		for (auto node : vertexs)
		{
			nodePos += node;
		}
		return nodePos / vertexs.size();
	}
	set<shared_ptr<mMeshNodeData>> mPreMeshPickThread::getAllNodesByPartName(std::shared_ptr<mMeshPartData> partData)
	{
		set<shared_ptr<mMeshNodeData>> picks;
		QVector<shared_ptr<mMeshData>> meshs = partData->getMeshDatas();
		for (auto mesh : meshs)
		{
			if(!mesh->getMeshVisual())
			{
				continue;
			}
			auto vertexs = mesh->getNodeData();
			picks.insert(vertexs.begin(), vertexs.end());
			
		}
		return picks;
	}
	set<shared_ptr<mMeshData>> mPreMeshPickThread::getAllMeshsByPartName(std::shared_ptr<mMeshPartData> partData)
	{
		set<shared_ptr<mMeshData>> picks;
		QVector<shared_ptr<mMeshData>> meshs = partData->getMeshDatas();
		for (auto mesh : meshs)
		{
			if(!mesh->getMeshVisual())
			{
				continue;
			}
			picks.insert(mesh);
		}

		return picks;
	}
	set<shared_ptr<mMeshFaceData>> mPreMeshPickThread::getAllMeshFacesByPartName(std::shared_ptr<mMeshPartData> partData)
	{
		set<shared_ptr<mMeshFaceData>> picks;
		QVector<shared_ptr<mMeshFaceData>> meshfaces = partData->getMeshFaceData();
		picks.insert(meshfaces.begin(), meshfaces.end());
		return picks;
	}
	set<shared_ptr<mMeshLineData>> mPreMeshPickThread::getAllMeshLinesByPartName(std::shared_ptr<mMeshPartData> partData)
	{
		set<shared_ptr<mMeshLineData>> picks;
		//QVector<MXGeoSolid*> geoSolids = MeshMessage::getInstance()->getGeoSolidSamePart(partName);
		//for (auto geoSolid : geoSolids)
		//{
		//	QVector<MEdge*> medges = geoSolid->boundaryMeshEdges;
		//	picks.insert(medges.begin(), medges.end());
		//	//for (auto geoFace : geoSolid->getface())
		//	//{
		//	//	medges = geoFace->boundaryMeshEdgesInGface;
		//	//	picks.insert(medges.begin(), medges.end());
		//	//}
		//}
		//QVector<MXGeoFace*> geoFaces = MeshMessage::getInstance()->getFreeGFaceInPart(partName);
		//for (auto geoFace : geoFaces)
		//{
		//	QVector<MEdge*> medges = geoFace->boundaryMeshEdgesInGface;
		//	picks.insert(medges.begin(), medges.end());
		//}

		return picks;
	}
	//set<shared_ptr<mMeshNodeData>> mPreMeshPickThread::getAllNodesByGeoFace(MXGeoFace * entity)
	//{
	//	set<shared_ptr<mMeshNodeData>> datas;
	//	datas = MeshMessage::getInstance()->getAllNodesByGeoFace(entity);
	//	return datas;
	//}
	//set<shared_ptr<mMeshData>> mPreMeshPickThread::getAllMeshsByGeoFace(MXGeoFace * entity)
	//{
	//	set<shared_ptr<mMeshData>> datas;
	//	datas.insert(entity->_mTriangles.begin(), entity->_mTriangles.end());
	//	datas.insert(entity->_mQuadangles.begin(), entity->_mQuadangles.end());
	//	return datas;
	//}
	//set<shared_ptr<mMeshFaceData>> mPreMeshPickThread::getAllMeshFacesByGeoFace(MXGeoFace * entity)
	//{
	//	set<shared_ptr<mMeshFaceData>> datas;
	//	datas = MeshMessage::getInstance()->getAllMeshFacesByGeoFace(entity);
	//	return datas;
	//}
	//set<shared_ptr<mMeshNodeData>> mPreMeshPickThread::getAllNodesByGeoEdge(MXGeoEdge * entity)
	//{
	//	set<shared_ptr<mMeshNodeData>> datas;
	//	datas = MeshMessage::getInstance()->getAllNodesByGeoEdge(entity);
	//	return datas;
	//}
	//set<shared_ptr<mMeshData>> mPreMeshPickThread::getAllMeshsByGeoEdge(MXGeoEdge * entity)
	//{
	//	set<shared_ptr<mMeshData>> datas;
	//	datas.insert(entity->_mLines.begin(), entity->_mLines.end());
	//	return datas;
	//}
	//set<MEdge*> mPreMeshPickThread::getAllMeshLinesByGeoEdge(MXGeoEdge * entity)
	//{
	//	set<MEdge*> datas;
	//	datas = MeshMessage::getInstance()->getAllMeshLinesByGeoEdge(entity);
	//	return datas;
	//}
	void mPreMeshPickThread::SoloPickMeshTypeFilter(std::shared_ptr<mMeshPartData> partData, QVector<MeshType> filter)
	{
		shared_ptr<mMeshData> _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;
		float uv[2];
		float t;

		//获取单元面
		if (filter.contains(MeshTet) || filter.contains(MeshWedge) || filter.contains(MeshPyramid) || filter.contains(MeshHex))
		{
			set<shared_ptr<mMeshFaceData>> mfaces = this->getAllMeshFacesByPartName(partData);
			for (auto mface : mfaces)
			{
				/*if (!filter.contains(mface->_linkEleMents_3D[0]->getMeshType()))
				{
					continue;
				}
				MeshType meshType = MeshTri;
				int num = 3;
				if (meshType == MeshQuad)
				{
					num = 4;
				}
				QVector<QVector3D> vertexs = mface->getNodeVertex();
				if (vertexs.size() == 3 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
				{
					if (t < _meshdepth)
					{
						_meshdepth = t;
						_pickMesh = mface->_linkEleMents_3D[0];
					}
				}*/
			}
		}

		//获取二维网格
		if (filter.contains(MeshTri) || filter.contains(MeshQuad))
		{
			QVector<shared_ptr<mMeshData>> meshs = partData->getMeshDatas2();
			for (auto mesh : meshs)
			{
				int num = 3;
				if (!filter.contains(mesh->getMeshType()))
				{
					continue;
				}
				if (mesh->getMeshType() == MeshQuad)
				{
					num = 4;
				}
				QVector<QVector3D> vertexs = mesh->getNodeVertex();
				if (num == 3 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
				{
					if (t < _meshdepth)
					{
						_meshdepth = t;
						_pickMesh = mesh;
					}
				}
			}
		}

		//获取一维网格
		if (filter.contains(MeshBeam))
		{
			QVector<shared_ptr<mMeshData>> meshs = partData->getMeshDatas1();
			for (auto mesh : meshs)
			{
				int num = 2;
				if (!filter.contains(mesh->getMeshType()))
				{
					continue;
				}
				QVector<QVector2D> tempQVector2D;
				std::set<float> depthlist;
				QVector<QVector3D> vertexs = mesh->getNodeVertex();
				WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
				if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
				{
					_meshdepth = *depthlist.begin();
					_pickMesh = mesh;
				}
			}
		}


		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshData(_pickMesh, _meshdepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickNode(std::shared_ptr<mMeshPartData> partData)
	{
		std::shared_ptr<mMeshNodeData>  _picknodevertex = nullptr;
		float _nodedepth = 1;
		float depth = 1;

		//获取单元面
		QVector<shared_ptr<mMeshFaceData>> mfaces = partData->getMeshFaceData();
		for (auto mface : mfaces)
		{
			//获取所有节点
			MeshType meshType = MeshTri;
			int num = 3;
			if (meshType == MeshQuad)
			{
				num = 4;
			}
			auto vertexs = mface->getNodeData();
			for (auto vertex: vertexs)
			{
				if (vertex == nullptr)
				{
					continue;
				}
				QVector2D ap1 = WorldvertexToScreenvertex(vertex->getNodeVertex(), depth);
				if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && depth < _nodedepth)
				{
					_nodedepth = depth;
					_picknodevertex = vertex;
				}
			}
		}

		//获取网格
		QVector<shared_ptr<mMeshData>> meshs = partData->getMeshDatas2() + partData->getMeshDatas1() + partData->getMeshDatas0();
		for (auto mesh : meshs)
		{
			if (!mesh->getMeshVisual())
			{
				continue;
			}
			auto vertexs = mesh->getNodeData();
			for (auto vertex : vertexs)
			{
				if (vertex == nullptr)
				{
					continue;
				}
				QVector2D ap1 = WorldvertexToScreenvertex(vertex->getNodeVertex(), depth);
				if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && depth < _nodedepth)
				{
					_nodedepth = depth;
					_picknodevertex = vertex;
				}

			}
		}

		if (_picknodevertex == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickNodeData(_picknodevertex, _nodedepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickAnyMesh(std::shared_ptr<mMeshPartData> partData)
	{
		shared_ptr<mMeshData> _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;
		float uv[2];
		float t;

		//获取单元面
		QVector<shared_ptr<mMeshFaceData>> mfaces = partData->getMeshFaceData();
		for (auto mface : mfaces)
		{
			//获取所有节点
			QVector<QVector3D> vertexs = mface->getNodeVertex();
			if (mface->getMeshFaceIsTri() ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < _meshdepth)
				{
					_meshdepth = t;
					_pickMesh = mface->getMeshID();
				}
			}
		}

		//获取二维网格
		QVector<shared_ptr<mMeshData>> meshs = partData->getMeshDatas2();
		for (auto mesh : meshs)
		{
			if(!mesh->getMeshVisual())
			{
				continue;
			}
			QVector<QVector3D> vertexs = mesh->getNodeVertex();
			if (mesh->getMeshType() == MeshTri ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < _meshdepth)
				{
					_meshdepth = t;
					_pickMesh = mesh;
				}
			}
		}

		//获取一维网格
		meshs = partData->getMeshDatas1();
		for (auto mesh : meshs)
		{
			if(!mesh->getMeshVisual())
			{
				continue;
			}
			int num = 2;
			if (mesh->getMeshType() != MeshBeam)
			{
				continue;
			}
			QVector<QVector2D> tempQVector2D;
			std::set<float> depthlist;
			QVector<QVector3D> vertexs = mesh->getNodeVertex();
			WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
			if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
			{
				_meshdepth = *depthlist.begin();
				_pickMesh = mesh;
			}
		}

		//获取0维网格
		meshs = partData->getMeshDatas0();
		for (auto mesh : meshs)
		{
			if(!mesh->getMeshVisual())
			{
				continue;
			}
			auto vertex = mesh->getNodeVertex();
			QVector2D ap1 = WorldvertexToScreenvertex(vertex.first(), depth);
			if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && depth < _meshdepth)
			{
				_meshdepth = depth;
				_pickMesh = mesh;
			}
		}


		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshData(_pickMesh, _meshdepth);
		pickMutex.unlock();

	}
	void mPreMeshPickThread::SoloPickMeshLine(std::shared_ptr<mMeshPartData> partData)
	{
		MEdge* _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;

		//获取边界线
		//set<MEdge*> medges = partData->getMeshLineIDs();
		//for (auto edge : medges)
		//{
		//	QVector<QVector2D> tempQVector2D;
		//	std::set<float> depthlist;
		//	QVector<QVector3D> vertexs = edge->getAllVertexs();
		//	WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
		//	if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
		//	{
		//		_meshdepth = *depthlist.begin();
		//		_pickMesh = edge;
		//	}
		//}
		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshLineData(_pickMesh, _meshdepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshFace(std::shared_ptr<mMeshPartData> partData)
	{
		shared_ptr<mMeshFaceData> _pickmeshface = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;
		float uv[2];
		float t;

		//获取单元面
		set<shared_ptr<mMeshFaceData>> mfaces = partData->getMeshFaces();
		for (auto mface : mfaces)
		{
			//获取所有节点
			QVector<QVector3D> vertexs = mface->getNodeVertex();
			if (mface->getMeshFaceIsTri() ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < _meshdepth)
				{
					_meshdepth = t;
					_pickmeshface = mface;
				}
			}
		}

		if (_pickmeshface == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshFaceData(_pickmeshface, _meshdepth);
		pickMutex.unlock();
		return;
	}
	void mPreMeshPickThread::SoloPickMeshPart(std::shared_ptr<mMeshPartData> partData)
	{
		float depth = FLT_MAX;
		if (!isSoloPickMeshPart(partData, depth))
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshPartData(partData->getPartName(), depth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickNodeByPart(std::shared_ptr<mMeshPartData> partData)
	{
		float depth = FLT_MAX;
		if (!isSoloPickMeshPart(partData, depth))
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickNodeByPartData(this->getAllNodesByPartName(partData), depth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickAnyMeshByPart(std::shared_ptr<mMeshPartData> partData)
	{
		float depth = FLT_MAX;
		if (!isSoloPickMeshPart(partData, depth))
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshByPartData(this->getAllMeshsByPartName(partData), depth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshLineByPart(std::shared_ptr<mMeshPartData> partData)
	{
		float depth = FLT_MAX;
		if (!isSoloPickMeshPart(partData, depth))
		{
			return;
		}
		pickMutex.lock();
		//_pickData->setSoloPickMeshLineByPartData(this->getAllMeshLinesByPartName(partName), depth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshFaceByPart(std::shared_ptr<mMeshPartData> partData)
	{
		float depth = FLT_MAX;
		if (!isSoloPickMeshPart(partData, depth))
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshFaceByPartData(partData->getMeshFaces(), depth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickNodeByLine(std::shared_ptr<mMeshPartData> partData)
	{
		float depth = FLT_MAX;
		float mindepth = FLT_MAX;
		//MXGeoEdge *entity = nullptr;
		//QVector<MXGeoEdge*> geoEdges = MeshMessage::getInstance()->getGeoEdgeSamePart(partName);
		//for (auto geoEdge : geoEdges)
		//{
		//	if (isSoloPickGeoLine(geoEdge, depth) && depth < mindepth)
		//	{
		//		mindepth = depth;
		//		entity = geoEdge;
		//	}
		//}
		//if (entity == nullptr)
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setSoloPickNodeByPartData(this->getAllNodesByGeoEdge(entity), depth);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshByLine(std::shared_ptr<mMeshPartData> partData)
	{
		float depth = FLT_MAX;
		float mindepth = FLT_MAX;
		//MXGeoEdge *entity = nullptr;
		//QVector<MXGeoEdge*> geoEdges = MeshMessage::getInstance()->getGeoEdgeSamePart(partName);
		//for (auto geoEdge : geoEdges)
		//{
		//	if (isSoloPickGeoLine(geoEdge, depth) && depth < mindepth)
		//	{
		//		mindepth = depth;
		//		entity = geoEdge;
		//	}
		//}
		//if (entity == nullptr)
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setSoloPickMeshByPartData(this->getAllMeshsByGeoEdge(entity), depth);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshLineByLine(std::shared_ptr<mMeshPartData> partData)
	{
		float depth = FLT_MAX;
		float mindepth = FLT_MAX;
		//MXGeoEdge *entity = nullptr;
		//QVector<MXGeoEdge*> geoEdges = MeshMessage::getInstance()->getGeoEdgeSamePart(partName);
		//for (auto geoEdge : geoEdges)
		//{
		//	if (isSoloPickGeoLine(geoEdge, depth) && depth < mindepth)
		//	{
		//		mindepth = depth;
		//		entity = geoEdge;
		//	}
		//}
		//if (entity == nullptr)
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setSoloPickMeshLineByPartData(this->getAllMeshLinesByGeoEdge(entity), depth);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickNodeByFace(std::shared_ptr<mMeshPartData> partData)
	{
		float depth = FLT_MAX;
		float mindepth = FLT_MAX;
		//MXGeoFace *entity = nullptr;
		//QVector<MXGeoFace*> geoFaces = MeshMessage::getInstance()->getGeoFaceSamePart(partName);
		//for (auto geoFace : geoFaces)
		//{
		//	if (isSoloPickGeoFace(geoFace, depth) && depth < mindepth)
		//	{
		//		mindepth = depth;
		//		entity = geoFace;
		//	}
		//}
		//if (entity == nullptr)
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setSoloPickNodeByPartData(this->getAllNodesByGeoFace(entity), depth);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshByFace(std::shared_ptr<mMeshPartData> partData)
	{
		float depth = FLT_MAX;
		float mindepth = FLT_MAX;
		//MXGeoFace *entity = nullptr;
		//QVector<MXGeoFace*> geoFaces = MeshMessage::getInstance()->getGeoFaceSamePart(partName);
		//for (auto geoFace : geoFaces)
		//{
		//	if (isSoloPickGeoFace(geoFace, depth) && depth < mindepth)
		//	{
		//		mindepth = depth;
		//		entity = geoFace;
		//	}
		//}
		//if (entity == nullptr)
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setSoloPickMeshByPartData(this->getAllMeshsByGeoFace(entity), depth);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshFaceByFace(std::shared_ptr<mMeshPartData> partData)
	{
		float depth = FLT_MAX;
		float mindepth = FLT_MAX;
		//MXGeoFace *entity = nullptr;
		//QVector<MXGeoFace*> geoFaces = MeshMessage::getInstance()->getGeoFaceSamePart(partName);
		//for (auto geoFace : geoFaces)
		//{
		//	if (isSoloPickGeoFace(geoFace, depth) && depth < mindepth)
		//	{
		//		mindepth = depth;
		//		entity = geoFace;
		//	}
		//}
		//if (entity == nullptr)
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setSoloPickMeshFaceByPartData(this->getAllMeshFacesByGeoFace(entity), depth);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickNodeByLineAngle(std::shared_ptr<mMeshPartData> partData)
	{
		SoloPick1DMeshByAngle(partData);
		SoloPickMeshLineByAngle(partData);
	}
	void mPreMeshPickThread::SoloPickNodeByFaceAngle(std::shared_ptr<mMeshPartData> partData)
	{
		SoloPick2DMeshByAngle(partData);
		SoloPickMeshFaceByAngle(partData);
	}
	void mPreMeshPickThread::SoloPick1DMeshByAngle(std::shared_ptr<mMeshPartData> partData)
	{
		shared_ptr<mMeshData> _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;

		//获取一维网格
		QVector<shared_ptr<mMeshData>> meshs = partData->getMeshDatas1();
		for (auto mesh : meshs)
		{
			int num = 2;
			if(!mesh->getMeshVisual())
			{
				continue;
			}
			if (mesh->getMeshType() != MeshBeam)
			{
				continue;
			}
			QVector<QVector2D> tempQVector2D;
			std::set<float> depthlist;
			QVector<QVector3D> vertexs = mesh->getNodeVertex();
			WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
			if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
			{
				_meshdepth = *depthlist.begin();
				_pickMesh = mesh;
			}
		}


		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshDataByAngle(_pickMesh, partData->getPartName(), _meshdepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPick2DMeshByAngle(std::shared_ptr<mMeshPartData> partData)
	{
		shared_ptr<mMeshData> _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;
		float uv[2];
		float t;

		//获取二维网格
		QVector<shared_ptr<mMeshData>> meshs = partData->getMeshDatas2();
		for (auto mesh : meshs)
		{
			int num = 3;
			if(!mesh->getMeshVisual())
			{
				continue;
			}
			if (mesh->getMeshType() == MeshQuad)
			{
				num = 4;
			}
			QVector<QVector3D> vertexs = mesh->getNodeVertex();
			if (num == 3 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < _meshdepth)
				{
					_meshdepth = t;
					_pickMesh = mesh;
				}
			}
		}

		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshDataByAngle(_pickMesh, partData->getPartName(), _meshdepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshLineByAngle(std::shared_ptr<mMeshPartData> partData)
	{
		MEdge* _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;

		//获取边界线
		//set<MEdge*> medges = this->getAllMeshLinesByPartName(partName);
		//for (auto edge : medges)
		//{
		//	QVector<QVector2D> tempQVector2D;
		//	std::set<float> depthlist;
		//	QVector<QVector3D> vertexs = edge->getAllVertexs();
		//	WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
		//	if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < _meshdepth)
		//	{
		//		_meshdepth = *depthlist.begin();
		//		_pickMesh = edge;
		//	}
		//}
		//if (_pickMesh == nullptr)
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setSoloPickMeshLineDataByAngle(_pickMesh, partName, _meshdepth);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::SoloPickMeshFaceByAngle(std::shared_ptr<mMeshPartData> partData)
	{
		shared_ptr<mMeshFaceData> _pickMesh = nullptr;
		float _meshdepth = FLT_MAX;
		float depth = FLT_MAX;
		float uv[2];
		float t;

		//获取单元面
		QVector<shared_ptr<mMeshFaceData>> mfaces = partData->getMeshFaceData();
		for (auto mface : mfaces)
		{
			//获取所有节点
			QVector<QVector3D> vertexs = mface->getNodeVertex();
			if (mface->getMeshFaceIsTri() ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < _meshdepth)
				{
					_meshdepth = t;
					_pickMesh = mface;
				}
			}
		}
		if (_pickMesh == nullptr)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setSoloPickMeshFaceDataByAngle(_pickMesh, partData->getPartName(), _meshdepth);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickNode(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		set<shared_ptr<mMeshNodeData>> _picknodevertexs;
		if (isAllIn)
		{
			_picknodevertexs = this->getAllNodesByPartName(partData);
		}
		else
		{
			//获取网格
			set<shared_ptr<mMeshData>> meshs = this->getAllMeshsByPartName(partData);
			for (auto mesh : meshs)
			{
				if(!mesh->getMeshVisual())
				{
					continue;
				}
				auto vertexs = mesh->getNodeData();
				for (auto vertex : vertexs)
				{
					if (vertex == nullptr)
					{
						continue;
					}
					if (_pick->get2DAnd3DMeshCenterIsInPick(vertex->getNodeVertex()));
					{
						_picknodevertexs.insert(vertex);
					}
				}
			}
		}

		if (_picknodevertexs.size() == 0)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setMultiplyPickNodeData(_picknodevertexs);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickAnyMesh(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		set<shared_ptr<mMeshData>> pickmeshs;
		if (isAllIn)
		{
			pickmeshs = this->getAllMeshsByPartName(partData);
		}
		else
		{
			//获取网格
			set<shared_ptr<mMeshData>> meshs = this->getAllMeshsByPartName(partData);
			for (auto mesh : meshs)
			{
				QVector<QVector3D> vertexs = mesh->getNodeVertex();
				if(!mesh->getMeshVisual())
				{
					continue;
				}
				switch (mesh->getMeshType())
				{
				case MeshBeam:
				{
					if (_pick->get1DMeshIsInPick(vertexs))
					{
						pickmeshs.insert(mesh);
					}
					break;
				}
				default:
				{
					if (_pick->get2DAnd3DMeshCenterIsInPick(getCenter(vertexs)))
					{
						pickmeshs.insert(mesh);
					}
					break;
				}

				}
			}
		}

		if (pickmeshs.size() == 0)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setMultiplyPickMeshData(pickmeshs);
		pickMutex.unlock();

	}
	void mPreMeshPickThread::MultiplyPickMeshTypeFilter(std::shared_ptr<mMeshPartData> partData, QVector<MeshType> filters, bool isAllIn)
	{
		set<shared_ptr<mMeshData>> pickmeshs;
		if (isAllIn)
		{
			//获取网格
			set<shared_ptr<mMeshData>> meshs = this->getAllMeshsByPartName(partData);
			for (auto mesh : meshs)
			{
				if(!mesh->getMeshVisual())
				{
					continue;
				}
				if (!filters.contains(mesh->getMeshType()))
				{
					continue;
				}
				pickmeshs.insert(mesh);
			}
		}
		else
		{
			//获取网格
			set<shared_ptr<mMeshData>> meshs = this->getAllMeshsByPartName(partData);
			for (auto mesh : meshs)
			{
				if(!mesh->getMeshVisual())
				{
					continue;
				}
				if (!filters.contains(mesh->getMeshType()))
				{
					continue;
				}
				QVector<QVector3D> vertexs = mesh->getNodeVertex();
				switch (mesh->getMeshType())
				{
				case MeshBeam:
				{
					if (_pick->get1DMeshIsInPick(vertexs))
					{
						pickmeshs.insert(mesh);
					}
				}
				default:
				{
					if (_pick->get2DAnd3DMeshCenterIsInPick(getCenter(vertexs)))
					{
						pickmeshs.insert(mesh);
					}
				}
				break;
				}
			}
		}

		if (pickmeshs.size() == 0)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setMultiplyPickMeshData(pickmeshs);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshLine(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		set<MEdge*> pickmeshlines;
		//if (isAllIn)
		//{
		//	pickmeshlines = this->getAllMeshLinesByPartName(partName);
		//}
		//else
		//{
		//	//获取网格
		//	set<MEdge*> meshlines = this->getAllMeshLinesByPartName(partName);
		//	for (auto meshline : meshlines)
		//	{
		//		QVector<QVector3D> vertexs = meshline->getAllVertexs();
		//		if (_pick->get1DMeshIsInPick(vertexs))
		//		{
		//			pickmeshlines.insert(meshline);
		//		}
		//	}
		//}

		if (pickmeshlines.size() == 0)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setMultiplyPickMeshLineData(pickmeshlines);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshFace(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		set<shared_ptr<mMeshFaceData>> pickmeshfaces;
		if (isAllIn)
		{
			pickmeshfaces = this->getAllMeshFacesByPartName(partData);
		}
		else
		{
			//获取
			QVector<shared_ptr<mMeshFaceData>> mfaces = partData->getMeshFaceData();
			for (auto mface : mfaces)
			{
				if (mface == nullptr)
				{
					continue;
				}
				QVector<QVector3D> vertexs = mface->getNodeVertex();
				if (_pick->get2DAnd3DMeshCenterIsInPick(getCenter(vertexs)))
				{
					pickmeshfaces.insert(mface);
				}
			}

		}

		if (pickmeshfaces.size() == 0)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setMultiplyPickMeshFaceData(pickmeshfaces);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshPart(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		std::shared_ptr<mMeshPartData> _partData;
		if (isAllIn)
		{
			_partData = partData;
		}
		else
		{
			if (isMultiplyPickMeshPart(partData))
			{
				_partData = partData;
			}
		}
		if (!_partData)
		{
			return;
		}
		pickMutex.lock();
		_pickData->setMultiplyPickMeshPartData(_partData->getPartName());
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickNodeByPart(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		std::shared_ptr<mMeshPartData> _partData;
		if (isAllIn)
		{
			_partData = partData;
		}
		else
		{
			if (isMultiplyPickMeshPart(partData))
			{
				_partData = partData;
			}
		}
		if (!_partData)
		{
			return;
		}
		set<shared_ptr<mMeshNodeData>> picks = this->getAllNodesByPartName(_partData);
		pickMutex.lock();
		_pickData->setMultiplyPickNodeData(picks);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickAnyMeshByPart(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		std::shared_ptr<mMeshPartData> _partData;
		if (isAllIn)
		{
			_partData = partData;
		}
		else
		{
			if (isMultiplyPickMeshPart(partData))
			{
				_partData = partData;
			}
		}
		if (!_partData)
		{
			return;
		}
		set<shared_ptr<mMeshData>> picks = this->getAllMeshsByPartName(partData);
		pickMutex.lock();
		_pickData->setMultiplyPickMeshData(picks);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshLineByPart(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		//QString _partName;
		//if (isAllIn)
		//{
		//	_partName = partName;
		//}
		//else
		//{
		//	if (isMultiplyPickMeshPart(partName))
		//	{
		//		_partName = partName;
		//	}
		//}
		//if (_partName.isEmpty())
		//{
		//	return;
		//}
		//set<MEdge*> picks = this->getAllMeshLinesByPartName(partName);
		//pickMutex.lock();
		//_pickData->setMultiplyPickMeshLineData(picks);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshFaceByPart(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		std::shared_ptr<mMeshPartData> _partData;
		if (isAllIn)
		{
			_partData = partData;
		}
		else
		{
			if (isMultiplyPickMeshPart(partData))
			{
				_partData = partData;
			}
		}
		if (!_partData)
		{
			return;
		}
		set<shared_ptr<mMeshFaceData>> picks = this->getAllMeshFacesByPartName(_partData);
		pickMutex.lock();
		_pickData->setMultiplyPickMeshFaceData(picks);
		pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickNodeByLine(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		//set<shared_ptr<mMeshNodeData>> picks;
		//QVector<MXGeoEdge*> geoEdges = MeshMessage::getInstance()->getGeoEdgeSamePart(partName);
		//for (auto geoEdge : geoEdges)
		//{
		//	if (isAllIn || IsMultiplyPickGeoLine(geoEdge))
		//	{
		//		set<shared_ptr<mMeshNodeData>> datas = this->getAllNodesByGeoEdge(geoEdge);
		//		picks.insert(datas.begin(), datas.end());
		//	}
		//}

		//if (picks.empty())
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setMultiplyPickNodeData(picks);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshByLine(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		//set<shared_ptr<mMeshData>> picks;
		//QVector<MXGeoEdge*> geoEdges = MeshMessage::getInstance()->getGeoEdgeSamePart(partName);
		//for (auto geoEdge : geoEdges)
		//{
		//	if (isAllIn || IsMultiplyPickGeoLine(geoEdge))
		//	{
		//		set<shared_ptr<mMeshData>> datas = this->getAllMeshsByGeoEdge(geoEdge);
		//		picks.insert(datas.begin(), datas.end());
		//	}
		//}

		//if (picks.empty())
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setMultiplyPickMeshData(picks);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshLineByLine(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		//set<MEdge*> picks;
		//QVector<MXGeoEdge*> geoEdges = MeshMessage::getInstance()->getGeoEdgeSamePart(partName);
		//for (auto geoEdge : geoEdges)
		//{
		//	if (isAllIn || IsMultiplyPickGeoLine(geoEdge))
		//	{
		//		set<MEdge*> datas = this->getAllMeshLinesByGeoEdge(geoEdge);
		//		picks.insert(datas.begin(), datas.end());
		//	}
		//}

		//if (picks.empty())
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setMultiplyPickMeshLineData(picks);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickNodeByFace(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		set<shared_ptr<mMeshNodeData>> picks;
		//QVector<MXGeoFace*> geoFaces = MeshMessage::getInstance()->getGeoFaceSamePart(partName);
		//for (auto geoFace : geoFaces)
		//{
		//	if (isAllIn || IsMultiplyPickGeoFace(geoFace))
		//	{
		//		set<shared_ptr<mMeshNodeData>> datas = this->getAllNodesByGeoFace(geoFace);
		//		picks.insert(datas.begin(), datas.end());
		//	}
		//}

		//if (picks.empty())
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setMultiplyPickNodeData(picks);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshByFace(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		//set<shared_ptr<mMeshData>> picks;
		//QVector<MXGeoFace*> geoFaces = MeshMessage::getInstance()->getGeoFaceSamePart(partName);
		//for (auto geoFace : geoFaces)
		//{
		//	if (isAllIn || IsMultiplyPickGeoFace(geoFace))
		//	{
		//		set<shared_ptr<mMeshData>> datas = this->getAllMeshsByGeoFace(geoFace);
		//		picks.insert(datas.begin(), datas.end());
		//	}
		//}

		//if (picks.empty())
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setMultiplyPickMeshData(picks);
		//pickMutex.unlock();
	}
	void mPreMeshPickThread::MultiplyPickMeshFaceByFace(std::shared_ptr<mMeshPartData> partData, bool isAllIn)
	{
		//set<shared_ptr<mMeshFaceData>> picks;
		//QVector<MXGeoFace*> geoFaces = MeshMessage::getInstance()->getGeoFaceSamePart(partName);
		//for (auto geoFace : geoFaces)
		//{
		//	if (isAllIn || IsMultiplyPickGeoFace(geoFace))
		//	{
		//		set<shared_ptr<mMeshFaceData>> datas = this->getAllMeshFacesByGeoFace(geoFace);
		//		picks.insert(datas.begin(), datas.end());
		//	}
		//}

		//if (picks.empty())
		//{
		//	return;
		//}
		//pickMutex.lock();
		//_pickData->setMultiplyPickMeshFaceData(picks);
		//pickMutex.unlock();

	}
	void mPreMeshPickThread::SoloPickNodePath()
	{
		QVector<shared_ptr<mMeshNodeData>> nodeIDs = _pickData->getPickNodeIDsOrder();
		if (nodeIDs.size() < 2)
		{
			return;
		}

		QVector<shared_ptr<mMeshNodeData>> res;

		shared_ptr<mMeshNodeData> lastNodeID = nodeIDs.takeLast();
		shared_ptr<mMeshNodeData> firstNodeID = nodeIDs.last();

		QVector3D lastVertex = lastNodeID->getNodeVertex();

		QVector3D dirction = (lastVertex - firstNodeID->getNodeVertex()).normalized();
		//节点ID
		QQueue<shared_ptr<mMeshNodeData>> queue;
		//存储相邻单元的方向的队列
		QQueue<QVector3D> queueDirection;
		queue.enqueue(firstNodeID);
		queueDirection.enqueue(dirction);
		//判断维度
		QVector<shared_ptr<mMeshData>> test/* = firstNodeID->getNodeInMeshIDs()*/;
		if (!test.isEmpty())//二维
		{
			while (!queue.empty())
			{
				shared_ptr<mMeshNodeData> nodeData = queue.dequeue();
				QVector3D lastDirection = queueDirection.dequeue();
				QVector<shared_ptr<mMeshData>> meshIDs/* = nodeData->_linkElements_2D*/;
				float maxCosValue = 0.0;
				shared_ptr<mMeshNodeData> NodeIDData = nullptr;
				QVector3D dirc;
				for (auto meshData : meshIDs)
				{
					if (meshData == nullptr)
					{
						continue;
					}
					nodeIDs /*= meshData->lineNodes(nodeData)*/;
					for (auto node : nodeIDs)
					{
						if (node == lastNodeID)
						{
							NodeIDData = lastNodeID;
							break;
						}
						if (res.contains(node))
						{
							break;
						}
						dirction = (node->getNodeVertex() - nodeData->getNodeVertex()).normalized();
						//dirction = (lastVertex - node->getNodeVertex()).normalized();
						float cosValue = QVector3D::dotProduct(dirction, lastDirection);
						if (cosValue >= maxCosValue)
						{
							NodeIDData = node;
							maxCosValue = cosValue;
							dirc = (lastVertex - node->getNodeVertex()).normalized();
						}
					}
					if (NodeIDData == lastNodeID)
					{
						NodeIDData = nullptr;
						break;
					}
				}
				if (NodeIDData != nullptr)
				{
					queue.enqueue(NodeIDData);
					queueDirection.enqueue(dirc);
					res.append(NodeIDData);
				}
			}


		}
		else if (!firstNodeID/*->linkMFaces.isEmpty()*/)
		{
			while (!queue.empty())
			{
				shared_ptr<mMeshNodeData> nodeData = queue.dequeue();
				QVector3D lastDirection = queueDirection.dequeue();
				QVector<shared_ptr<mMeshFaceData>> meshFaceIDs /*= nodeData->linkMFaces*/;
				float maxCosValue = 0.0;
				shared_ptr<mMeshNodeData> NodeIDData = nullptr;
				QVector3D dirc;
				for (auto meshFaceData : meshFaceIDs)
				{
					if (meshFaceData == nullptr)
					{
						continue;
					}
					nodeIDs/* = meshFaceData->lineNodes(nodeData)*/;
					for (auto node : nodeIDs)
					{
						if (node == lastNodeID)
						{
							NodeIDData = lastNodeID;
							break;
						}
						if (res.contains(node))
						{
							break;
						}
						dirction = (node->getNodeVertex() - nodeData->getNodeVertex()).normalized();
						//dirction = (lastVertex - node->getNodeVertex()).normalized();
						float cosValue = QVector3D::dotProduct(dirction, lastDirection);
						if (cosValue >= maxCosValue)
						{
							NodeIDData = node;
							maxCosValue = cosValue;
							dirc = (lastVertex - node->getNodeVertex()).normalized();
						}
					}
					if (NodeIDData == lastNodeID)
					{
						NodeIDData = nullptr;
						break;
					}
				}
				if (NodeIDData != nullptr)
				{
					queue.enqueue(NodeIDData);
					queueDirection.enqueue(dirc);
					res.append(NodeIDData);
				}
			}

		}
		res.append(lastNodeID);
		_pickData->setMultiplyPickNodeData(res);

	}
	void mPreMeshPickThread::SoloPickNodeByLineAngle()
	{
		/*
		QPair<PickObjectType, QPair<QString, void*>> pickObjectID = _pickData->getSoloPickNodeDataByLineAngle();
		QString partName = pickObjectID.second.first;
		//拾取到的节点
		std::set<shared_ptr<mMeshNodeData>> pickNodeIDs;
		if (pickObjectID.first == PickObjectType::Mesh1D)
		{
			shared_ptr<mMeshData> meshData = static_cast<shared_ptr<mMeshData>>(pickObjectID.second.second);
			if (meshData == nullptr)
			{
				return;
			}
			//判断过的单元ID
			std::set<shared_ptr<mMeshData>> judgeMeshIDs;
			//存储相邻单元ID的队列
			QQueue<shared_ptr<mMeshData>> queue;
			//存储相邻单元的方向的队列
			QQueue<QVector3D> queueDirection;
			//方向
			QVector3D direction, lastDirection;
			//角度值
			float angleValue;

			if (meshData != nullptr)
			{
				QVector<QVector3D> vertexs = meshData->getNodeVertex();

				lastDirection = (vertexs.at(1) - vertexs.at(0)).normalized();
				//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
				{
					//lastDirection = -lastDirection;
				}

				queue.enqueue(meshData);
				queueDirection.enqueue(lastDirection);

			}

			while (!queue.isEmpty() && !queueDirection.isEmpty())
			{
				meshData = queue.dequeue();
				lastDirection = queueDirection.dequeue();
				if (!judgeMeshIDs.insert(meshData).second)
				{
					continue;
				}

				if (meshData != nullptr)
				{
					QVector<QVector3D> vertexs = meshData->getNodeVertex();
					if (vertexs.size() < 2)
					{
						return;
					}
					direction = (vertexs.at(1) - vertexs.at(0)).normalized();
					float x = QVector3D::dotProduct(direction, lastDirection);
					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(x) / 3.1415926);
					}
					if (_pickAngleValue > angleValue || _pickAngleValue > (180 - angleValue))
					{
						int num = meshData->getNumVertices();
						for (int i = 0; i < num; i++)
						{
							pickNodeIDs.insert(meshData->getVertex(i));
						}
						//单元表面
						set<shared_ptr<mMeshData>> adjacentMeshIDs = meshData->linkElements();
						for (auto mesh : adjacentMeshIDs)
						{
							queue.enqueue(mesh);
							queueDirection.enqueue(direction);
						}
					}
				}

			}
		}
		else
		{
			MEdge* meshLineData = static_cast<MEdge*>(pickObjectID.second.second);
			if (meshLineData == nullptr)
			{
				return;
			}

			//判断过的单元边ID
			std::set<MEdge*> judgeMeshLineIDs;
			//存储相邻单元边ID的队列
			QQueue<MEdge*> queue;
			//存储相邻单元边的方向的队列
			QQueue<QVector3D> queueDirection;
			//方向
			QVector3D direction, lastDirection;
			//角度值
			float angleValue;

			if (meshLineData != nullptr)
			{
				QVector<QVector3D> vertexs = meshLineData->getAllVertexs();

				lastDirection = (vertexs.at(1) - vertexs.at(0)).normalized();
				//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
				{
					//lastDirection = -lastDirection;
				}

				queue.enqueue(meshLineData);
				queueDirection.enqueue(lastDirection);

			}

			while (!queue.isEmpty() && !queueDirection.isEmpty())
			{
				meshLineData = queue.dequeue();
				lastDirection = queueDirection.dequeue();
				if (!judgeMeshLineIDs.insert(meshLineData).second)
				{
					continue;
				}

				if (meshLineData != nullptr)
				{
					QVector<QVector3D> vertexs = meshLineData->getAllVertexs();
					if (vertexs.size() < 2)
					{
						return;
					}
					direction = (vertexs.at(1) - vertexs.at(0)).normalized();
					float x = QVector3D::dotProduct(direction, lastDirection);
					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(x) / 3.1415926);
					}
					if (_pickAngleValue > angleValue || _pickAngleValue > (180 - angleValue))
					{
						for (int i = 0; i < 2; i++)
						{
							pickNodeIDs.insert(meshLineData->getVerOfMEdge(i));
						}
						//单元边
						set<MEdge*> adjacentMeshFaceIDs = meshLineData->linkMEdges();
						for (auto meshline : adjacentMeshFaceIDs)
						{
							queue.enqueue(meshline);
							queueDirection.enqueue(direction);
						}
					}

				}

			}
		}
		_pickData->setMultiplyPickNodeData(pickNodeIDs);
		*/

	}
	void mPreMeshPickThread::SoloPickNodeByFaceAngle()
	{
		/*
		QPair<PickObjectType, QPair<QString, shared_ptr<void>>> pickObjectID = _pickData->getSoloPickNodeDataByFaceAngle();
		QString partName = pickObjectID.second.first;
		//拾取到的节点
		std::set<shared_ptr<mMeshNodeData>> pickNodeIDs;
		if (pickObjectID.first == PickObjectType::Mesh2D)
		{
			shared_ptr<mMeshData> meshData = dynamic_cast<shared_ptr<mMeshData>>(pickObjectID.second.second);
			if (meshData == nullptr)
			{
				return;
			}
			//判断过的单元ID
			std::set<shared_ptr<mMeshData>> judgeMeshIDs;
			//存储相邻单元ID的队列
			QQueue<shared_ptr<mMeshData>> queue;
			//存储相邻单元的方向的队列
			QQueue<QVector3D> queueDirection;
			//方向
			QVector3D direction, lastDirection;
			//角度值
			float angleValue;

			if (meshData->getMeshType() == MeshQuad || meshData->getMeshType() == MeshTri)
			{
				QVector<QVector3D> vertexs = meshData->getNodeVertex();
				if (vertexs.size() < 3)
				{
					return;
				}
				lastDirection = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
				queue.enqueue(meshData);
				queueDirection.enqueue(lastDirection);
			}


			while (!queue.isEmpty() && !queueDirection.isEmpty())
			{
				meshData = queue.dequeue();
				lastDirection = queueDirection.dequeue();
				if (!judgeMeshIDs.insert(meshData).second)
				{
					continue;
				}

				if (meshData != nullptr && meshData->getMeshVisual())
				{
					QVector<QVector3D> vertexs = meshData->getNodeVertex();
					if (vertexs.size() < 3)
					{
						return;
					}
					direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
					float x = QVector3D::dotProduct(direction, lastDirection);
					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(x) / 3.1415926);
					}
					if (_pickAngleValue > angleValue)
					{
						int n = meshData->getNumVertices();
						for (int i = 0; i < n; i++)
						{
							pickNodeIDs.insert(meshData->getVertex(i));
						}

						set<shared_ptr<mMeshData>> adjacentMeshIDs = meshData->linkElements();
						for (auto mesh : adjacentMeshIDs)
						{
							queueDirection.enqueue(direction);
							queue.enqueue(mesh);
						}
					}
				}

			}
		}
		else
		{
			shared_ptr<mMeshFaceData> meshFaceData = static_cast<shared_ptr<mMeshFaceData>>(pickObjectID.second.second);
			if (meshFaceData == nullptr)
			{
				return;
			}

			//判断过的单元面ID
			std::set<shared_ptr<mMeshFaceData>> judgeMeshFaceIDs;
			//存储相邻单元面ID的队列
			QQueue<shared_ptr<mMeshFaceData>> queue;
			//存储相邻单元的方向的队列
			QQueue<QVector3D> queueDirection;
			//方向
			QVector3D direction, lastDirection;
			//角度值
			float angleValue;

			if (meshFaceData != nullptr)
			{
				QVector<QVector3D> vertexs = meshFaceData->getNodeVertex();

				lastDirection = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
				//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
				{
					//lastDirection = -lastDirection;
				}

				queue.enqueue(meshFaceData);
				queueDirection.enqueue(lastDirection);

			}

			while (!queue.isEmpty() && !queueDirection.isEmpty())
			{
				meshFaceData = queue.dequeue();
				lastDirection = queueDirection.dequeue();
				if (!judgeMeshFaceIDs.insert(meshFaceData).second)
				{
					continue;
				}

				if (meshFaceData != nullptr)
				{
					QVector<QVector3D> vertexs = meshFaceData->getNodeVertex();
					if (vertexs.size() < 3)
					{
						return;
					}
					direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
					float x = QVector3D::dotProduct(direction, lastDirection);
					if (x > 1.0)
					{
						angleValue = 0;
					}
					else
					{
						angleValue = fabs(180 * acos(x) / 3.1415926);
					}
					if (_pickAngleValue > angleValue)
					{
						int n = meshFaceData->type() == 1 ? 3 : 4;
						for (int i = 0; i < n; i++)
						{
							pickNodeIDs.insert(meshFaceData->getVertex(i));
						}
						set<shared_ptr<mMeshFaceData>> adjacentMeshFaceIDs = meshFaceData->linkMFaces();
						for (auto meshface : adjacentMeshFaceIDs)
						{
							queueDirection.enqueue(direction);
							queue.enqueue(meshface);
						}
					}

				}

			}
		}

		pickMutex.lock();
		_pickData->setMultiplyPickNodeData(pickNodeIDs);
		pickMutex.unlock();
		*/
	}
	void mPreMeshPickThread::SoloPick1DMeshByAngle()
	{
		/*
		QPair<QString, void*> partNameMesh = _pickData->getSoloPickMeshLineDataByAngle();
		shared_ptr<mMeshData> meshData = static_cast<shared_ptr<mMeshData>>(partNameMesh.second);
		if (meshData == 0)
		{
			return;
		}

		//拾取到的单元ID
		std::set<shared_ptr<mMeshData>> pickMeshIDs;
		//判断过的单元ID
		std::set<shared_ptr<mMeshData>> judgeMeshIDs;
		//存储相邻单元ID的队列
		QQueue<shared_ptr<mMeshData>> queue;
		//存储相邻单元的方向的队列
		QQueue<QVector3D> queueDirection;
		//方向
		QVector3D direction, lastDirection;
		//角度值
		float angleValue;

		if (meshData != nullptr)
		{
			QVector<QVector3D> vertexs = meshData->getNodeVertex();

			lastDirection = (vertexs.at(1) - vertexs.at(0)).normalized();
			//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
			{
				//lastDirection = -lastDirection;
			}

			queue.enqueue(meshData);
			queueDirection.enqueue(lastDirection);

		}

		while (!queue.isEmpty() && !queueDirection.isEmpty())
		{
			meshData = queue.dequeue();
			lastDirection = queueDirection.dequeue();
			if (!judgeMeshIDs.insert(meshData).second)
			{
				continue;
			}

			if (meshData != nullptr)
			{
				QVector<QVector3D> vertexs = meshData->getNodeVertex();
				if (vertexs.size() < 2)
				{
					return;
				}
				direction = (vertexs.at(1) - vertexs.at(0)).normalized();
				float x = QVector3D::dotProduct(direction, lastDirection);
				if (x > 1.0)
				{
					angleValue = 0;
				}
				else
				{
					angleValue = fabs(180 * acos(x) / 3.1415926);
				}
				if (_pickAngleValue > angleValue || _pickAngleValue > (180 - angleValue))
				{
					if (pickMeshIDs.insert(meshData).second)
					{
						//单元表面
						set<shared_ptr<mMeshData>> adjacentMeshIDs = meshData->linkElements();
						for (auto mesh : adjacentMeshIDs)
						{
							queue.enqueue(mesh);
							queueDirection.enqueue(direction);
						}
					}
				}

			}

		}


		_pickData->setMultiplyPickMeshData(pickMeshIDs);
		*/
	}
	void mPreMeshPickThread::SoloPickMeshLineByAngle()
	{
		/*
		QPair<QString, void*> partNameMeshLine = _pickData->getSoloPickMeshLineDataByAngle();
		MEdge* meshLineData = static_cast<MEdge*>(partNameMeshLine.second);
		if (meshLineData == 0)
		{
			return;
		}

		//拾取到的单元面ID
		std::set<MEdge*> pickMeshLineIDs;
		//判断过的单元面ID
		std::set<MEdge*> judgeMeshLineIDs;
		//存储相邻单元面ID的队列
		QQueue<MEdge*> queue;
		//存储相邻单元的方向的队列
		QQueue<QVector3D> queueDirection;
		//方向
		QVector3D direction, lastDirection;
		//角度值
		float angleValue;

		if (meshLineData != nullptr)
		{
			QVector<QVector3D> vertexs = meshLineData->getAllVertexs();

			lastDirection = (vertexs.at(1) - vertexs.at(0)).normalized();
			//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
			{
				//lastDirection = -lastDirection;
			}

			queue.enqueue(meshLineData);
			queueDirection.enqueue(lastDirection);

		}

		while (!queue.isEmpty() && !queueDirection.isEmpty())
		{
			meshLineData = queue.dequeue();
			lastDirection = queueDirection.dequeue();
			if (!judgeMeshLineIDs.insert(meshLineData).second)
			{
				continue;
			}

			if (meshLineData != nullptr)
			{
				QVector<QVector3D> vertexs = meshLineData->getAllVertexs();
				if (vertexs.size() < 2)
				{
					return;
				}
				direction = (vertexs.at(1) - vertexs.at(0)).normalized();
				float x = QVector3D::dotProduct(direction, lastDirection);
				if (x > 1.0)
				{
					angleValue = 0;
				}
				else
				{
					angleValue = fabs(180 * acos(x) / 3.1415926);
				}
				if (_pickAngleValue > angleValue || _pickAngleValue > (180 - angleValue))
				{
					if (pickMeshLineIDs.insert(meshLineData).second)
					{
						//单元表面
						set<MEdge*> adjacentMeshFaceIDs = meshLineData->linkMEdges();
						for (auto meshline : adjacentMeshFaceIDs)
						{
							queue.enqueue(meshline);
							queueDirection.enqueue(direction);
						}
					}
				}

			}

		}


		_pickData->setMultiplyPickMeshLineData(pickMeshLineIDs);
		*/
	}
	void mPreMeshPickThread::SoloPick2DMeshByAngle()
	{
		/*
		QPair<QString, void*> partNameMeshID = _pickData->getSoloPickMeshDataByAngle();
		shared_ptr<mMeshData> meshData = static_cast<shared_ptr<mMeshData>>(partNameMeshID.second);
		if (meshData == nullptr)
		{
			return;
		}

		//拾取到的单元ID
		std::set<shared_ptr<mMeshData>> pickMeshIDs;
		//判断过的单元ID
		std::set<shared_ptr<mMeshData>> judgeMeshIDs;
		//存储相邻单元ID的队列
		QQueue<shared_ptr<mMeshData>> queue;
		//存储相邻单元的方向的队列
		QQueue<QVector3D> queueDirection;
		//方向
		QVector3D direction, lastDirection;
		//角度值
		float angleValue;

		if (meshData->getMeshType() == MeshQuad || meshData->getMeshType() == MeshTri)
		{
			QVector<QVector3D> vertexs = meshData->getNodeVertex();
			if (vertexs.size() < 3)
			{
				return;
			}
			lastDirection = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
			queue.enqueue(meshData);
			queueDirection.enqueue(lastDirection);
		}


		while (!queue.isEmpty() && !queueDirection.isEmpty())
		{
			meshData = queue.dequeue();
			lastDirection = queueDirection.dequeue();
			if (!judgeMeshIDs.insert(meshData).second)
			{
				continue;
			}

			if (meshData != nullptr && meshData->getMeshVisual())
			{
				QVector<QVector3D> vertexs = meshData->getNodeVertex();
				if (vertexs.size() < 3)
				{
					return;
				}
				direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
				float x = QVector3D::dotProduct(direction, lastDirection);
				if (x > 1.0)
				{
					angleValue = 0;
				}
				else
				{
					angleValue = fabs(180 * acos(x) / 3.1415926);
				}
				if (_pickAngleValue > angleValue)
				{
					if (pickMeshIDs.insert(meshData).second)
					{
						set<shared_ptr<mMeshData>> adjacentMeshIDs = meshData->linkElements();
						for (auto mesh : adjacentMeshIDs)
						{
							queueDirection.enqueue(direction);
							queue.enqueue(mesh);
						}
					}
				}
			}

		}

		pickMutex.lock();
		_pickData->setMultiplyPickMeshData(pickMeshIDs);
		pickMutex.unlock();
		*/
	}
	void mPreMeshPickThread::SoloPickMeshFaceByAngle()
	{
		/*
		QPair<QString, void*> partNameMeshFace = _pickData->getSoloPickMeshFaceDataByAngle();
		shared_ptr<mMeshFaceData> meshFaceData = static_cast<shared_ptr<mMeshFaceData>>(partNameMeshFace.second);
		if (meshFaceData == 0)
		{
			return;
		}

		//拾取到的单元面ID
		std::set<shared_ptr<mMeshFaceData>> pickMeshFaceIDs;
		//判断过的单元面ID
		std::set<shared_ptr<mMeshFaceData>> judgeMeshFaceIDs;
		//存储相邻单元面ID的队列
		QQueue<shared_ptr<mMeshFaceData>> queue;
		//存储相邻单元的方向的队列
		QQueue<QVector3D> queueDirection;
		//方向
		QVector3D direction, lastDirection;
		//角度值
		float angleValue;

		if (meshFaceData != nullptr)
		{
			QVector<QVector3D> vertexs = meshFaceData->getNodeVertex();

			lastDirection = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
			//if (!_oneFrameData->getMeshDataByID(meshID)->getMeshVisual())
			{
				//lastDirection = -lastDirection;
			}

			queue.enqueue(meshFaceData);
			queueDirection.enqueue(lastDirection);

		}

		while (!queue.isEmpty() && !queueDirection.isEmpty())
		{
			meshFaceData = queue.dequeue();
			lastDirection = queueDirection.dequeue();
			if (!judgeMeshFaceIDs.insert(meshFaceData).second)
			{
				continue;
			}

			if (meshFaceData != nullptr)
			{
				QVector<QVector3D> vertexs = meshFaceData->getNodeVertex();
				if (vertexs.size() < 3)
				{
					return;
				}
				direction = QVector3D::crossProduct((vertexs.at(1) - vertexs.at(0)).normalized(), (vertexs.at(2) - vertexs.at(1)).normalized()).normalized();
				float x = QVector3D::dotProduct(direction, lastDirection);
				if (x > 1.0)
				{
					angleValue = 0;
				}
				else
				{
					angleValue = fabs(180 * acos(x) / 3.1415926);
				}
				if (_pickAngleValue > angleValue)
				{
					if (pickMeshFaceIDs.insert(meshFaceData).second)
					{
						set<shared_ptr<mMeshFaceData>> adjacentMeshFaceIDs = meshFaceData->linkMFaces();
						for (auto meshface : adjacentMeshFaceIDs)
						{
							queueDirection.enqueue(direction);
							queue.enqueue(meshface);
						}
					}
				}

			}

		}


		_pickData->setMultiplyPickMeshFaceData(pickMeshFaceIDs);
		*/
	}
	bool mPreMeshPickThread::isSoloPickMeshPart(std::shared_ptr<mMeshPartData> partData, float & depth)
	{
		float uv[2];
		float t;
		//获取单元面
		QVector<shared_ptr<mMeshFaceData>> mfaces = partData->getMeshFaceData();
		for (auto mface : mfaces)
		{
			if (mface == nullptr)
			{
				continue;
			}
			QVector<QVector3D> vertexs = mface->getNodeVertex();
			if (vertexs.size() == 3 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < depth)
				{
					depth = t;
				}
			}

		}

		//获取网格
		set<shared_ptr<mMeshData>> meshs = this->getAllMeshsByPartName(partData);
		for (auto mesh : meshs)
		{
			QVector<QVector3D> vertexs = mesh->getNodeVertex();
			switch (mesh->getMeshType())
			{
			case MeshBeam:
			{
				QVector<QVector2D> tempQVector2D;
				std::set<float> depthlist;
				WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
				if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < depth)
				{
					depth = *depthlist.begin();
				}
				break;
			}
			case MeshTri:
			case MeshQuad:
			{
				QVector<QVector3D> vertexs = mesh->getNodeVertex();
				if (vertexs.size() == 3 ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
				{
					if (t < depth)
					{
						depth = t;
					}
				}
				break;
			}
			case MeshPoint:
			{
				auto vertex = mesh->getNodeVertex();
				QVector2D ap1 = WorldvertexToScreenvertex(vertex.first(), t);
				if (fabs(ap1.x() - _pos.x()) <= 5 && fabs(ap1.y() - _pos.y()) <= 5 && t < depth)
				{
					depth = t;
				}
			}
			default:
				break;
			}

		}

		if (depth != FLT_MAX)
		{
			return true;
		}
		return false;
	}
	/*
	bool mPreMeshPickThread::isSoloPickGeoFace(MXGeoFace * geoFaceData, float & depth)
	{
		float uv[2];
		float t;
		//获取一个几何面上的单元面
		set<shared_ptr<mMeshFaceData>> mfaces = this->getAllMeshFacesByGeoFace(geoFaceData);
		for (auto mface : mfaces)
		{
			QVector<QVector3D> vertexs = mface->getNodeVertex();
			if (mface->getMeshFaceIsTri() ? mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t) : mPickToolClass::rayQuadIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < depth)
				{
					depth = t;
				}
			}
		}

		//获取二维网格
		for (auto mesh : geoFaceData->_mTriangles)
		{
			if(!mesh->getMeshVisual())
			{
				continue;
			}
			QVector<QVector3D> vertexs = mesh->getNodeVertex();
			if (mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < depth)
				{
					depth = t;
				}
			}
		}

		for (auto mesh : geoFaceData->_mQuadangles)
		{
			if(!mesh->getMeshVisual())
			{
				continue;
			}
			QVector<QVector3D> vertexs = mesh->getNodeVertex();
			if (mPickToolClass::rayTriangleIntersect(_origin, _dir, vertexs, uv, t))
			{
				if (t < depth)
				{
					depth = t;
				}
			}
		}
		if (depth != FLT_MAX)
		{
			return true;
		}
		return false;
	}
	bool mPreMeshPickThread::isSoloPickGeoLine(MXGeoEdge * geoEdgeData, float & depth)
	{
		//获取边界线
		set<MEdge*> medges = this->getAllMeshLinesByGeoEdge(geoEdgeData);
		for (auto edge : medges)
		{
			QVector<QVector2D> tempQVector2D;
			std::set<float> depthlist;
			QVector<QVector3D> vertexs = edge->getAllVertexs();
			WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
			if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < depth)
			{
				depth = *depthlist.begin();
			}
		}

		for (auto mesh : geoEdgeData->_mLines)
		{
			QVector<QVector3D> vertexs = mesh->getNodeVertex();
			QVector<QVector2D> tempQVector2D;
			std::set<float> depthlist;
			WorldvertexToScreenvertex(vertexs, tempQVector2D, depthlist);
			if (mPickToolClass::IsLineIntersectionWithQuad(tempQVector2D, soloQuad, MeshBeam) && *depthlist.begin() < depth)
			{
				depth = *depthlist.begin();
			}
		}
		if (depth != FLT_MAX)
		{
			return true;
		}
		return false;
	}
	bool mPreMeshPickThread::IsMultiplyPickGeoLine(MXGeoEdge * geoLineData)
	{
		for (auto mesh : geoLineData->_mLines)
		{
			QVector<QVector3D> vertexs = mesh->getNodeVertex();
			if (_pick->get1DMeshIsInPick(vertexs))
			{
				return true;
			}
		}
		return false;
	}
	bool mPreMeshPickThread::IsMultiplyPickGeoFace(MXGeoFace * geoFaceData)
	{
		for (auto mesh : geoFaceData->_mTriangles)
		{
			QVector<QVector3D> vertexs = mesh->getNodeVertex();
			if (_pick->get2DAnd3DMeshCenterIsInPick(this->getCenter(vertexs)))
			{
				return true;
			}
		}
		for (auto mesh : geoFaceData->_mQuadangles)
		{
			QVector<QVector3D> vertexs = mesh->getNodeVertex();
			if (_pick->get2DAnd3DMeshCenterIsInPick(this->getCenter(vertexs)))
			{
				return true;
			}
		}
		return false;
	}
	*/
}